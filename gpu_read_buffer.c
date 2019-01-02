/*
**
*/
#include "jcm_ss.h"

/*
**
*/
void					enqueueReadBuffer(cl_command_queue queue, cl_mem buffName, void *buff, int size) {
  cl_int				err;
  cl_event				e;
  
  if ((err=clEnqueueReadBuffer(queue, buffName, CL_TRUE, 0, size, buff, 0, NULL, &e)) < 0) {
    perror("Couldn't read the buffer");
    exit(1);
  }
  clWaitForEvents(1, &e);
}
