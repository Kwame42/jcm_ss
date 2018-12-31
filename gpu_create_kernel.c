/*
**
*/
#include "jcm_ss.h"

/*
**
*/
cl_kernel				*createKernel(cl_program *program, char *funcName, unsigned int numDevices) {
  cl_kernel				*ret;
  cl_int				err;
  int					i;
  
  ret=(cl_kernel *)allocate(sizeof(cl_kernel) * numDevices);
  for (i=0 ; i < numDevices ; i++) {
    ret[i]=clCreateKernel(program[i], funcName, &err);
    if(err < 0) {
      t_ocl_err                           err_list[]={
	{CL_INVALID_PROGRAM, "if program is not a valid program object."},
	{CL_INVALID_PROGRAM_EXECUTABLE, "if there is no successfully built executable for program."},
	{CL_INVALID_KERNEL_NAME, "if kernel_name is not found in program."},
	{CL_INVALID_KERNEL_DEFINITION, "if the function definition for __kernel function given by kernel_name such as the number of arguments, the argument types are not the same for all devices for which the program executable has been built."},
	{CL_INVALID_VALUE, "if kernel_name is NULL."},
	{CL_OUT_OF_HOST_MEMORY, "if there is a failure to allocate resources required by the OpenCL implementation on the host."},
	{0}
      };
      printError(err, err_list);
      perror("Couldn't create kernel");
      exit(1);
    }
  }
  
  return ret;
}

