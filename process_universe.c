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
    enqueueWriteBuffer(execution->queue[i], execution->clDeviceIndex0[i], &i, sizeof(unsigned int));
    enqueueWriteBuffer(execution->queue[i], execution->clObjChunk[i], execution->objChunk[i], sizeof(t_object) * execution->uintData.deviceRange);
    enqueueKernel(i, execution);
    printf("Start job [%d]\n", i);
  }
  for (i=0 ; i < execution->numDevices ; i++) {
    enqueueReadBuffer(execution->queue[i], execution->clObjChunk[i], execution->objChunk[i], sizeof(t_object) * execution->uintData.deviceRange);
    printf("Jobs done [%d]\n", i);
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
