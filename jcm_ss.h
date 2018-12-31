/*
**
*/
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/queue.h>
#include <sys/time.h>
#include <json-c/json.h>
#include <string.h>
#include <math.h>
#ifdef MAC
#  include <OpenCL/cl.h>
#else
#  include <CL/cl.h>
#endif

/*
**
*/
#include "space_object.h"
/*
**
*/
#define VERSION			0
#define JCM_SS_SAVE_FILE	"Save File"
#define JCM_SS_UNIVERSE_RADIUS	"Universe Radius"
#define JCM_SS_NUM_TIC		"Num Tic"
#define JCM_SS_SAVE_DIR		"Save Dir"
#define JCM_SS_TOO_CLOSE	"Too Close"
#define JCM_SS_TOO_FAR		"Too Far"
#define JCM_SS_PRECISION	"Precision"
#define JCM_SS_OBJ_SIZE		"Object Size"
#define JCM_SS_INIT_OBJ		"Initial Space Objects"
#define DOUBLE_QUOTE		34
#define BACK_SLASH		92
#define OCL_PROGRAM_FILE	"process_velocity.cl"
#define OCL_KERNEL_FUNC		"processVelocity"

/*
**
*/
#define degreesToRadians(angleDegrees)				(angleDegrees * M_PI / 180.0)
#define radiansToDegrees(angleRadians)				(angleRadians * 180.0 / M_PI)
#define	objectCopy(dst, src)					memcpy((dst), (src), sizeof(t_object));
#define OBJ_X(universe, i)					((universe)->objectList[i].pos.x)
#define OBJ_Y(universe, i)					((universe)->objectList[i].pos.y)
#define OBJ_Z(universe, i)					((universe)->objectList[i].pos.z)
#define OBJ_POS(universe, i)					((universe)->objectList[i].pos)
#define OBJ_MASS(universe, i)					((universe)->objectList[i].mass)
#define OBJ_VEL_X(universe, i)					((universe)->objectList[i].velocity.x)
#define OBJ_VEL_Y(universe, i)					((universe)->objectList[i].velocity.y)
#define OBJ_VEL_Z(universe, i)					((universe)->objectList[i].velocity.z)
#define OBJ_VEL(universe, i)					((universe)->objectList[i].velocity)
#define SET_OBJ(universe, i, objX, objY, objZ, objM, objVX, objVY, objVZ)	{OBJ_X(universe, i) = (objX); OBJ_Y(universe, i) = (objY); OBJ_Y(universe, i) = (objY); OBJ_MASS(universe, i) = (objM); OBJ_VEL_X(universe, i) = (objVX); OBJ_VEL_Y(universe, i) = (objVY); OBJ_VEL_Z(universe, i) = (objVZ);}
#define TMP_X(universe, i)					((universe)->tmpObjectList[i].pos.x)
#define TMP_Y(universe, i)					((universe)->tmpObjectList[i].pos.y)
#define TMP_Z(universe, i)					((universe)->tmpObjectList[i].pos.z)
#define TMP_MASS(universe, i)					((universe)->tmpObjectList[i].mass)
#define TMP_VEL_X(universe, i)					((universe)->tmpObjectList[i].velocity.x)
#define TMP_VEL_Y(universe, i)					((universe)->tmpObjectList[i].velocity.y)
#define TMP_VEL_Z(universe, i)					((universe)->tmpObjectList[i].velocity.z)
#define SET_TMP(universe, i, objX, objY, objZ, objM, objVX, objVY, objVZ)	{TMP_X(universe, i) = (objX); TMP_Y(universe, i) = (objY); TMP_Y(universe, Z) = (objZ); TMP_MASS(universe, i) = (objM); TMP_VEL_X(universe, i) = (objVX); TMP_VEL_Y(universe, i) = (objVY); TMP_VEL_Z(universe, i) = (objVZ);}
#define SHOW_OBJ(universe, i)					printf("fin %5d:[x:%4.4f] [y:%4.4f] [z:%4.4f] [m: %4f] {a: %4.4f, b: %4.4f c: %4.4f} \n", i, OBJ_X(universe,i), OBJ_Y(universe, i), OBJ_Z(universe, i), OBJ_MASS(universe, i), OBJ_VEL_X(universe, i), OBJ_VEL_Y(universe, i), OBJ_VEL_Z(universe, i));
#define SHOW_TMP(universe, i)					printf("tmp %5d:[x:%4.4f] [y:%4.4f] [m: %4f] {a: %4.4f, m: %4.4f} \n", i, TMP_X(universe,i), TMP_Y(universe, i), TMP_MASS(universe, i), TMP_VEL_X(universe, i), TMP_VEL_Y(universe, i));
#define DEL_OBJ(universe, i)					{bzero(&((universe)->objectList[i]), sizeof(t_object)), bzero(&((universe)->tmpObjectList[i]), sizeof(t_object));}

/*
**
*/

typedef struct			s_universe {
  unsigned int			numObj;
  const char			*saveDir;
  const char			*saveFile;
  unsigned int			radius;
  int				numTic;
  int				precision;
  unsigned int			tooClose;
  unsigned int			tooFar;
  int				objSize;
  json_object			*initCond;
  t_object			*objectList;
  t_object			*tmpObjectList;
}				t_universe;

typedef struct			s_gpu {
  cl_platform_id		platform;
  cl_device_id			*devices;
  t_const_data			uintData;
  cl_uint			numDevices;
  t_object			**objChunk;
  cl_context			context;
  cl_command_queue		*queue;
  cl_program			program;
  cl_kernel			kernel;
  size_t			numWorker;
  size_t			numWorkerPerWorkgroup;
  size_t			numWorkGroup;
  float				duration;
  cl_mem			clDuration;
  cl_mem			clUintData;
  cl_mem			clObjChunk;
  cl_mem			clObjectList;
  cl_mem			clTmpObjectList;
  cl_mem			clLocalObjectList;
  cl_mem			clDeviceIndex0;
}				t_gpu;

typedef struct				s_ocl_err {
  int					err;
  char					*info;
}					t_ocl_err;
  
/*
**
*/
void				usage(void);
t_universe			*getConf(int, char **);
char				*getConfFile(const char *);
int				getFilesize(const char*);
t_universe			*init_universe(int, int);
void				*allocate(int);
t_universe			*allocateUniverse();
t_object			*allocateObjectList(int);
void				saveUniverse(int, t_universe *);
void				calulNewTmpVelocity(t_universe *);
void				calculNewVelocity(t_universe *, int, int);
void				processUniverse(t_universe *);
void				setInitialCond(t_universe *);
void				showUniverse(t_universe *universe);
void				showTmpUniverse(t_universe *universe);
void				check_opt(t_universe *);
struct timeval			*allocateTimer();
void				setTimer(struct timeval *);
void				showTime(struct timeval *);
float				getTime(struct timeval *);
void				setTimer(struct timeval *);
void				initGpu(t_gpu *, char *, char *);
int				setGlobalKernelArg(t_gpu *, t_universe *);
int				setKernelArg(cl_kernel, int *, int, void *);
void				enqueueKernel(int, t_gpu *);
void				releaseGpu(t_gpu *);
void				enqueueReadBuffer(cl_command_queue, cl_mem, void *, int);
cl_kernel			createKernel(cl_program, char *);
t_gpu				*allocateGpu(void);
cl_mem				createBuffer(cl_context, unsigned int, size_t, void *);
void				printError(cl_int, t_ocl_err *);
void				getRange(t_universe *, t_gpu *);
int				getDeviceNumCore(cl_device_id);
cl_command_queue		createCommandQueue(cl_context, cl_device_id);
void				enqueueReadBuffer(cl_command_queue, cl_mem, void *, int);
t_object			**getChunkAddress(t_object *, int, int);
unsigned long			getDeviceLocalMemSize(cl_device_id);
unsigned long			getDeviceGlobalMemSize(cl_device_id);
size_t				getDeviceMaxWorkItemPerWorkGroup(cl_device_id);
size_t				getDevicePrefWorkGroupSizeMul(cl_kernel, cl_device_id);
int				getMultipleMax(size_t, size_t, size_t);
