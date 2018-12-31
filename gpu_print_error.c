/*
**
*/
#include "jcm_ss.h"

/*
**
*/
void			printError(cl_int err, t_ocl_err *tmp) {
    for (; tmp->err ; tmp++) {
      if (tmp->err == err) {
	printf("%s\n", tmp->info);
      }
    }
}
