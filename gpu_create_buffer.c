/*
**
*/
#include "jcm_ss.h"

/*
**
*/
cl_mem					createBuffer(cl_context context, unsigned int flags, size_t size, void *buff) {
  cl_mem				ret;
  cl_int				err;

  ret=clCreateBuffer(context, flags, size, buff, &err);
  if(err < 0) {
    t_ocl_err				err_list[]={
						     {CL_INVALID_CONTEXT,"context is not a valid context."},
						     {CL_INVALID_VALUE,"values specified in flags are not valid as defined in the table above."},
						     {CL_INVALID_BUFFER_SIZE,"size is 0. Implementations may return CL_INVALID_BUFFER_SIZE if size is greater than the CL_DEVICE_MAX_MEM_ALLOC_SIZE value specified in the table of allowed values for param_name for clGetDeviceInfo for all devices in context."},
						     {CL_INVALID_HOST_PTR,"host_ptr is NULL and CL_MEM_USE_HOST_PTR or CL_MEM_COPY_HOST_PTR are set in flags or if host_ptr is not NULL but CL_MEM_COPY_HOST_PTR or CL_MEM_USE_HOST_PTR are not set in flags."},
						     {CL_MEM_OBJECT_ALLOCATION_FAILURE,"there is a failure to allocate memory for buffer object."},
						     {CL_OUT_OF_RESOURCES,"there is a failure to allocate resources required by the OpenCL implementation on the device."},
						     {CL_OUT_OF_HOST_MEMORY,"there is a failure to allocate resources required by the OpenCL implementation on the host."},
						     {0}
     };
    printError(err, err_list);
    perror("Couldn't create a buffer");
    exit(1);   
  };
  
  return ret;
}
