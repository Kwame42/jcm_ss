/*
**
*/
#include "jcm_ss.h"

/*
**
*/
int			main(int ac, char **av) {
  t_universe		*universe;

  universe = getConf(ac, av);
  setInitialCond(universe);
  //  showUniverse(universe);
  processUniverse(universe);
  //  printf(">>INFO  : Total duration %f\n", (double) ((end.tv_usec - start.tv_usec) / 1000000) + (double)(end.tv_sec - start.tv_sec));
  exit(0);
}
