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
void			showTmpUniverse(t_universe *universe) {
  int			i;

  printf("==> Tmp\n");
  for (i=0 ; i < universe->numObj ; i++)
    SHOW_TMP(universe, i);
  printf("---\n");
	   //    printf("%5d: x:%4d y:%4d m:%4d {a:%4.2f m:%4.2f}\n", i, POS_X(universe,i), POS_Y(, tmp->mass, tmp->velocity.angle, tmp->velocity.magnitude);
}

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
