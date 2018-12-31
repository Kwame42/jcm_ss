/*
**
*/
#include "jcm_ss.h"

/*
**
*/
void			calulNewTmpVelocity(t_universe *universe) {
  int			i, j;
  
  for (i=0 ; i < universe->numObj ; i++)
    for (j=0 ; j < universe->numObj ; j++)
      if (i != j && OBJ_MASS(universe, i) && OBJ_MASS(universe, j))
	calculNewVelocity(universe, i, j);
}
void qprint(t_object *objList, int size) {
  int j;
  
  for (j=0 ; j < size; j++) {
    printf("[%f|%f|%f] ", objList[j].pos.x, objList[j].pos.y, objList[j].pos.z);
  }
  printf("\n");
}

void			setObjNewPositionGPU(t_universe *universe, t_gpu *execution, int numArg) {
  int			i;

  for (i=0 ; i < execution->numDevices ; i++) {
    enqueueKernel(i, execution);    
  }
  for (i=0 ; i < execution->numDevices ; i++) {
    enqueueReadBuffer(execution->queue[i], execution->clObjChunk, execution->objChunk[i], sizeof(t_object) * execution->uintData.deviceRange);
  }
  //  qprint(universe->tmpObjectList, universe->numObj);
}

/*
** UNUSED: should be use as a reference code for OpenCL

void			setObjNewPositionCPU(t_universe *universe) { 
  int			i;
  float			Dx, Dy;
  
  for (i=0 ; i < universe->numObj ; i++) {
    Dx=TMP_VEL_X(universe, i) - TMP_X(universe, i);
    Dy=TMP_VEL_Y(universe, i) - TMP_Y(universe, i);
    TMP_X(universe, i)+=Dx / universe->precision / fabs(TMP_MASS(universe, i));
    TMP_Y(universe, i)+=Dy / universe->precision / TMP_MASS(universe, i);
    if (fabs(TMP_X(universe, i)) > universe->radius || fabs(TMP_Y(universe, i)) > universe->radius)
      TMP_MASS(universe, i)=0;
    TMP_VEL_X(universe, i)=Dx + TMP_X(universe, i);
    TMP_VEL_Y(universe, i)=Dy + TMP_Y(universe, i);
  }
}
*/

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
