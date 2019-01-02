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
cl_ulong				*getTimerRes(cl_device_id *devices, int numDevices) {
  cl_ulong				*res;
  int					i;
  
  res=(cl_ulong *)allocate(sizeof(cl_ulong) * numDevices);
  for (i=0 ; i < numDevices ; i++) {
    res[i]=getDeviceTimerResolution(devices[i]);
  }

  return res;
}

void					initGpu(t_gpu *execution, char *programFile, char *programName) {
  execution->platform=createPlatform();
  execution->devices=createDevice(execution->platform, &execution->numDevices);
  execution->context=createContext(execution->devices, execution->numDevices);
  execution->queueEvent=allocateEvent(execution->numDevices);
  execution->timerRes=getTimerRes(execution->devices, execution->numDevices);
  execution->program=buildProgram(execution->context, execution->devices, execution->numDevices, programFile);
  execution->kernel=createKernel(execution->program, programName, execution->numDevices);
  execution->queue=createCommandQueue(execution->context, execution->devices, execution->numDevices);
}

/*
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
*/
