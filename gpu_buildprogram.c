/*
**
*/
#include "jcm_ss.h"

/*
**
*/
cl_program				*buildProgram(cl_context *context, cl_device_id *dev, cl_uint numDevices, const char* filename) {
  cl_program				*program;
  char					*programBuffer;
  size_t				programSize;
  int					err;
  #define WD_MAX_SIZE			2048
  char					cwd[WD_MAX_SIZE];
  char					*iPath;
  int					i;
  
  bzero(cwd, WD_MAX_SIZE);
  if (getcwd(cwd, WD_MAX_SIZE) == NULL) {
    perror("can't get current working dir\n");
    exit(1);
  }
  asprintf(&iPath, "-I%s", cwd);
  programBuffer=getConfFile(filename);
  programSize=getFilesize(filename);
  program=(cl_program *)allocate(sizeof(cl_program) * numDevices);
  for (i = 0 ; i < numDevices ; i++) {
    program[i]=clCreateProgramWithSource(context[i], 1, (const char**)&programBuffer, &programSize, &err);
    if (err < 0) {
      t_ocl_err				err_list[]={
	{CL_INVALID_CONTEXT, "if context is not a valid context."},
	{CL_INVALID_VALUE, "if count is zero or if strings or any entry in strings is NULL."},
	{CL_OUT_OF_HOST_MEMORY, "if there is a failure to allocate resources required by the OpenCL implementation on the host."},
	{0}
      };
      printError(err, err_list);
      perror("Couldn't create the program");
      exit(1);
    }
    if ((err=clBuildProgram(program[i], 0, NULL, iPath, NULL, NULL)) < 0) {
      char				*programLog;
      size_t				logSize;
      t_ocl_err				err_list[]={
	{CL_INVALID_PROGRAM, "if program is not a valid program object."},
	{CL_INVALID_VALUE, "if device_list is NULL and num_devices is greater than zero, or if device_list is not NULL and num_devices is zero."},
	{CL_INVALID_VALUE, "if pfn_notify is NULL but user_data is not NULL."},
	{CL_INVALID_DEVICE, "if OpenCL devices listed in device_list are not in the list of devices associated with program."},
	{CL_INVALID_BINARY, "if program is created with clCreateWithProgramWithBinary and devices listed in device_list do not have a valid program binary loaded."},
	{CL_INVALID_BUILD_OPTIONS, "if the build options specified by options are invalid."},
	{CL_INVALID_OPERATION, "if the build of a program executable for any of the devices listed in device_list by a previous call to clBuildProgram for program has not completed."},
	{CL_COMPILER_NOT_AVAILABLE, "if program is created with clCreateProgramWithSource and a compiler is not available i.e. CL_DEVICE_COMPILER_AVAILABLE specified in the table of OpenCL Device Queries for clGetDeviceInfo is set to CL_FALSE."},
	{CL_BUILD_PROGRAM_FAILURE, "if there is a failure to build the program executable. This error will be returned if clBuildProgram does not return until the build has completed."},
	{CL_INVALID_OPERATION, "if there are kernel objects attached to program."},
	{CL_OUT_OF_HOST_MEMORY, "if there is a failure to allocate resources required by the OpenCL implementation on the host."},
	{0}
      };
      perror("can't compile");
      printError(err, err_list);
      clGetProgramBuildInfo(program[i], dev[i], CL_PROGRAM_BUILD_LOG, 0, NULL, &logSize);
      logSize += 1;
      programLog=(char *)allocate(logSize);
      clGetProgramBuildInfo(program[i], dev[i], CL_PROGRAM_BUILD_LOG, logSize, programLog, NULL);
      printf("Error [%s]\n", programLog);
      exit(1);
    }
  }
  free(iPath);
  
  return program;
}
