/*
**
*/
#include "jcm_ss.h"

/*
**
*/
int			getNumWorkItemPerWorkGroup(t_universe *universe, t_gpu *execution) {
  size_t		prefWgSizeMul, maxWorkItem;
  int			numCore;
  int			num;

  numCore=getDeviceNumCore(execution->devices[0]);
  prefWgSizeMul=getDevicePrefWorkGroupSizeMul(execution->kernel[0], execution->devices[0]);
  maxWorkItem=getDeviceMaxWorkItemPerWorkGroup(execution->devices[0]);
  num=getMultipleMax(numCore, prefWgSizeMul, maxWorkItem);
  printf(">>INFO   : We consider [%d] work-item per workgroup\n", num);

  return num;
}

size_t			alignLocalMem(size_t memsize, int numObj) {
  int			obj;
  
  memsize=memsize - LO_MEM_BUF_POS;
  obj=memsize / sizeof(t_object) - numObj;
  if (obj % numObj) {
    obj=obj - obj % numObj;
  }
  printf(">>INFO   : We can have [%d] objects in local mem\n", obj);
  
  return obj * sizeof(t_object);
  
}

unsigned int		alignUniverseNumObj(t_universe *universe, t_gpu *execution) {
  unsigned int		numObj;

  numObj=execution->uintData.localMemSize / sizeof(t_object) * execution->numDevices;
  if (universe->numObj < numObj) {
    printf("--ERROR  : There is not enough part [%d] in your universe considering the amount of GPU. Rise it at least to %lu\n", universe->numObj, execution->uintData.localMemSize / sizeof(t_object) * execution->numDevices);
    exit(1);
  }
  if (universe->numObj % numObj) {
    universe->numObj=universe->numObj - universe->numObj % numObj;
    printf(">>INFO   : Universe number parts has been aligned to [%d]\n", universe->numObj);
  }

  return universe->numObj;
}

int			getObjChunkPerDevice(t_gpu *execution) {
  return (execution->uintData.deviceRange * sizeof(t_object) / execution->uintData.localMemSize > 0 ? execution->uintData.deviceRange * sizeof(t_object) / execution->uintData.localMemSize : 1);
}

void			getRange(t_universe *universe, t_gpu *execution) {
  size_t		localMemSize;

  execution->numWorkerPerWorkgroup=getNumWorkItemPerWorkGroup(universe, execution);
  localMemSize=getDeviceLocalMemSize(execution->devices[0]);
  execution->uintData.localMemSize=alignLocalMem(localMemSize, execution->numWorkerPerWorkgroup);
  universe->numObj=alignUniverseNumObj(universe, execution);
  execution->uintData.workerRange=execution->uintData.localMemSize / sizeof(t_object) / execution->numWorkerPerWorkgroup;
  printf(">>INFO   : Work-item range is [%d]\n", execution->uintData.workerRange);
  execution->uintData.deviceRange=universe->numObj /execution->numDevices;
  printf(">>INFO   : Device range is [%d]\n", execution->uintData.deviceRange);
  printf(">>INFO   : Object local mem chunk per device is [%d]\n", getObjChunkPerDevice(execution));
  execution->objChunk=getChunkAddress(universe->objectList, execution->numDevices, execution->uintData.deviceRange);
  execution->deviceIndex=(unsigned int *)allocate(sizeof(unsigned int) * execution->numDevices);
}
