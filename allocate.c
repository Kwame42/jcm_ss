/*
**
*/
#include "jcm_ss.h"

/*
**
*/
void				*allocate(int size) {
  void				*ret;

  if ((ret=malloc(size)) != NULL) {
    bzero(ret, size);
  } else {
    perror("Can't allocate memory");
    exit(1);
  }

  return ret;
}

t_universe			*allocateUniverse(void) {
  return (t_universe *)allocate(sizeof (t_universe));
}

t_object			*allocateObjectList(int nbrObject) {
  return (t_object *)allocate(sizeof (t_object) * (nbrObject + 1));
}

t_gpu				*allocateGpu(void) {
  return (t_gpu *)allocate(sizeof(t_gpu));
}

cl_event			*allocateEvent(int num) {
  return (cl_event *)allocate(sizeof (cl_event) * num);
}
