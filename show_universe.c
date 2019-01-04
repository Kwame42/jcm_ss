/*
**
*/
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <stdio.h>
#include <strings.h>
#include "jcm_ss.h"

/*
**
*/
void			showUniverse(t_universe *universe) {
  int			i;
  
  printf(" numObj %d\n saveDir %s\n saveFile %s\n radius %d\n numTic %d\n precision %d\n tooClose %ud\n tooFar %ud\n objSize %d\n",\
	 universe->numObj,\
	 universe->saveDir,\
	 universe->saveFile,\
	 universe->radius,\
	 universe->numTic,\
	 universe->precision,\
	 universe->tooClose,\
	 universe->tooFar,\
	 universe->objSize\
	 );
  for (i=0 ; i < universe->numObj ; i++)
    SHOW_OBJ(universe, i);
  printf("---\n");
}
