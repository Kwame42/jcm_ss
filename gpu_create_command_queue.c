/*
**
*/
#include "jcm_ss.h"

/*
**
*/
cl_command_queue			*createCommandQueue(cl_context *context, cl_device_id *device, unsigned int numDevices) {
  cl_command_queue			*ret;
  cl_int				err;
  int					i;

  ret=allocate(sizeof(cl_command_queue) * numDevices);
  for (i=0 ; i < numDevices ; i++) {
    ret[i]=clCreateCommandQueue(context[i], device[i], 0, &err);
    if(err < 0) {
      t_ocl_err				err_list[]={
	{CL_INVALID_CONTEXT, "if context is not a valid context."},
	{CL_INVALID_DEVICE, "if device is not a valid device or is not associated with context."},
	{CL_INVALID_VALUE, "if values specified in properties are not valid."},
	{CL_INVALID_QUEUE_PROPERTIES, "if values specified in properties are valid but are not supported by the device."},
	{CL_OUT_OF_HOST_MEMORY, "if there is a failure to allocate resources required by the OpenCL implementation on the host."}
      };
      
      printError(err, err_list);
      perror("Couldn't creste command queue for device");
      exit(1);
    };
  }
  
  return ret;
}
