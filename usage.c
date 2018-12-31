/*
**
*/
#include <stdlib.h>
#include "jcm_ss.h"

/*
**
*/
void                    usage() {
  printf("jcm_ss [-h] -p <nbr positive part> -n <nbr negative part> -r <radius> -m <max mass> -u <universe radius> -v <velocity> [-a <[rand]>] [-s <filename>] \n");
  exit(1);
}
