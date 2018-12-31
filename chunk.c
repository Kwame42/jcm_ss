/*
**
*/
#include "jcm_ss.h"

/*
**
*/
t_object		**getChunkAddress(t_object *objList, int numDevices, int deviceRange) {
  t_object		**objChunk;
  int			i;

  objChunk=(t_object **)allocate(sizeof(t_object *) * numDevices);
  for (i=0 ; i < numDevices ; i++) {
    objChunk[i]=&objList[deviceRange * i];
  }
  
  return objChunk;
}
