/*
**
*/
#include "jcm_ss.h"

/*
**
*/
cl_platform_id				createPlatform(void) {
   cl_platform_id			platform;
   cl_int				err;
   
   if((err=clGetPlatformIDs(1, &platform, NULL)) < 0) {
     perror("Couldn't identify a platform");
     exit(1);
   }

   return platform;
}

cl_device_id				*createDevice(cl_platform_id platform, cl_uint *numDevices) {
   int					err;
   cl_device_id				*devices;

   err=clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 0, NULL, numDevices);
   printf(">>INFO   : Found [%u] device(s)\n", *numDevices);
   devices=allocate(sizeof(*devices) * *numDevices);
   err=clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, *numDevices, devices, numDevices);
   if(err == CL_DEVICE_NOT_FOUND) {
     perror("Couldn't access GPU devices");
     exit(1);   
   }
   //*numDevices=1;
   
   return devices;
}

cl_program				buildProgram(cl_context context, cl_device_id *dev, const char* filename) {
  cl_program				program;
  char					*programBuffer;
  size_t				programSize;
  int					err;
  #define WD_MAX_SIZE			2048
  char					cwd[WD_MAX_SIZE];
  char					*iPath;

  programBuffer=getConfFile(filename);
  programSize=getFilesize(filename);
  if ((program=clCreateProgramWithSource(context, 1, (const char**)&programBuffer, &programSize, &err)) < 0) {
    perror("Couldn't create the program");
    exit(1);
  }
  bzero(cwd, WD_MAX_SIZE);
  if (getcwd(cwd, WD_MAX_SIZE) == NULL) {
    perror("can't get current working dir\n");
    exit(1);
  }
  asprintf(&iPath, "-I%s", cwd);
  if ((err=clBuildProgram(program, 0, NULL, iPath, NULL, NULL)) < 0) {
    char				*programLog;
    size_t				logSize;
    
    clGetProgramBuildInfo(program, *dev, CL_PROGRAM_BUILD_LOG, 0, NULL, &logSize);
    logSize += 1;
    programLog=allocate(logSize);
    clGetProgramBuildInfo(program, *dev, CL_PROGRAM_BUILD_LOG, logSize, programLog, NULL);
    printf("%s\n", programLog);
    exit(1);
  }
  free(iPath);

   return program;
}

cl_context				createContext(cl_device_id *devices, cl_uint numDevices) {
  cl_context				ret;
  cl_int				err;

  if ((ret=clCreateContext(NULL, numDevices, devices, NULL, NULL, &err)) < 0) {
    perror("Couldn't create a context");
    exit(1);   
  }

  return ret;
}

void					initGpu(t_gpu *execution, char *programFile, char *programName) {
  cl_uint				i;
  
  execution->platform=createPlatform();
  execution->devices=createDevice(execution->platform, &execution->numDevices);
  execution->context=createContext(execution->devices, execution->numDevices);
  execution->program=buildProgram(execution->context, execution->devices, programFile);
  execution->queue=allocate(sizeof(cl_command_queue) * execution->numDevices);
  for (i=0 ; i < execution->numDevices ; i++) {
    execution->queue[i]=createCommandQueue(execution->context, execution->devices[i]);
  }
  execution->kernel=createKernel(execution->program, programName);
}


void					releaseGpu(t_gpu *execution) {
  int					i;
  
  clReleaseKernel(execution->kernel);
  //  clReleaseMemObject(sum_buffer);
  //clReleaseMemObject(input_buffer);
  for (i=0 ; i <= execution->numDevices ; i++) {  
    clReleaseCommandQueue(execution->queue[i]);
  }
  clReleaseProgram(execution->program);
  clReleaseContext(execution->context);
}
