/*
**
*/
#include "jcm_ss.h"

/*
**
*/
void					enqueueReadBuffer(cl_command_queue queue, cl_mem buffName, void *buff, int size) {
  cl_int				err;
  
  if ((err=clEnqueueReadBuffer(queue, buffName, CL_TRUE, 0, size, buff, 0, NULL, NULL)) < 0) {
    perror("Couldn't read the buffer");
    exit(1);
  }
}
