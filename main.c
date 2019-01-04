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
  processUniverse(universe);
  printf("...xxXXX ENJOY YOUR RENDERING XXXxx...\n");
  exit(0);
}
