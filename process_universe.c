/*
**
*/
#include "jcm_ss.h"

/*
**
*/

void			setObjNewPositionGPU(t_universe *universe, t_gpu *execution, int numArg) {
  unsigned int		i;

  for (i=0 ; i < execution->numDevices ; i++) {
    printf(">>INFO   : Start Job [%d]\n", i);
    printf("         : Writing [%lu] bit in device\n", sizeof(t_object) * execution->uintData.deviceRange);
    enqueueWriteBuffer(execution->queue[i], execution->clObjChunk[i], execution->objChunk[i], sizeof(t_object) * execution->uintData.deviceRange);
    enqueueKernel(i, execution);
    printf(">>INFO  : flushing data\n");
    clFlush(execution->queue[i]);
  }
  for (i=0 ; i < execution->numDevices ; i++) {
    cl_ulong		start, end;
    int			err;
    
    enqueueReadBuffer(execution->queue[i], execution->clObjChunk[i], execution->objChunk[i], sizeof(t_object) * execution->uintData.deviceRange);
    err=clWaitForEvents(1, &execution->queueEvent[i]);
    err=clGetEventProfilingInfo(execution->queueEvent[i], CL_PROFILING_COMMAND_START, sizeof(cl_ulong), &start, NULL);
    err|=clGetEventProfilingInfo(execution->queueEvent[i], CL_PROFILING_COMMAND_SUBMIT, sizeof(cl_ulong), &end, NULL);
    printf(">>INFO   : Kernel [%d] duration time [%f] ms\n", i, (start - end) * 0.000001f);
  }
}

void			processUniverse(t_universe *universe) {
  int			i;
  t_gpu			*execution;
  int			numArg;

  execution=allocateGpu();
  saveUniverse(0, universe);
  initGpu(execution, "process_universe_chunk.cl", "process_universe_chunk"); 
  getRange(universe, execution);
  //  showUniverse(universe);
  numArg=setGlobalKernelArg(execution, universe);
  for (i=0 ; i < universe->numTic ; i++) {
    setObjNewPositionGPU(universe, execution, numArg);
    memcpy(universe->objectList, universe->tmpObjectList, universe->numObj * sizeof(t_object));
    //    showUniverse(universe);
  }
}
