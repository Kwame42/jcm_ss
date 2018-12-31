/*
**
*/
#include "jcm_ss.h"

/*
**
*/
cl_context				*createContext(cl_device_id *devices, cl_uint numDevices) {
  cl_context				*ret;
  cl_int				err;
  int					i;
  
  ret=(cl_context *)allocate(sizeof(cl_context) * numDevices);
  for (i=0 ; i < numDevices ; i++) {
    ret[i]=clCreateContext(NULL, 1, &devices[i], NULL, NULL, &err);
    if (err < 0) {
      t_ocl_err				err_list[]={
	{CL_INVALID_PLATFORM, "if properties is NULL and no platform could be selected or if platform value specified in properties is not a valid platform."},
	{CL_INVALID_VALUE, "if context property name in properties is not a supported property name; if devices is NULL; if num_devices is equal to zero; or if pfn_notify is NULL but user_data is not NULL."},
	{CL_INVALID_DEVICE, "if devices contains an invalid device or are not associated with the specified platform."},
	{CL_DEVICE_NOT_AVAILABLE, "if a device in devices is currently not available even though the device was returned by clGetDeviceIDs."},
	{CL_OUT_OF_HOST_MEMORY, "if there is a failure to allocate resources required by the OpenCL implementation on the host."},
	{0}
      };
      printError(err, err_list);
      perror("Couldn't create the program");
      exit(1);
    }
  }
  
  return ret;
}

