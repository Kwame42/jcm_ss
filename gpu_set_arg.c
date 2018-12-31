/*
**
*/
#include "jcm_ss.h"

/*
**
*/
int					setKernelArg(cl_kernel kernel, int *nextArg, int size, void *arg) {
  cl_int				err;

  if ((err=clSetKernelArg(kernel, *nextArg, size, arg)) < 0) {
    t_ocl_err				err_list[]={
						    {CL_INVALID_KERNEL, "if kernel is not a valid kernel object."},
						    {CL_INVALID_ARG_INDEX, "if arg_index is not a valid argument index."},
						    {CL_INVALID_ARG_VALUE, "if arg_value specified is NULL for an argument that is not declared with the __local qualifier or vice-versa."},
						    {CL_INVALID_MEM_OBJECT, "for an argument declared to be a memory object when the specified arg_value is not a valid memory object."},
						    {CL_INVALID_SAMPLER,"for an argument declared to be of type sampler_t when the specified arg_value is not a valid sampler object."},
						    {CL_INVALID_ARG_SIZE, "if arg_size does not match the size of the data type for an argument that is not a memory object or if the argument is a memory object and arg_size != sizeof(cl_mem) or if arg_size is zero and the argument is declared with the __local qualifier or if the argument is a sampler and arg_size != sizeof(cl_sampler)."},
						    {0},
    };
    printError(err, err_list);
    perror("Couldn't enqueue the kernel");
    exit(1);
  }
  (*nextArg)+=1;
  
  return *nextArg;
}

int					setGlobalKernelArg(t_gpu *execution, t_universe *universe) {
  int					nextArg;
  unsigned int				deviceNum;
  
  nextArg=0;

  /*
  ** 1 - Local Mem for all particules (recuring buffer) __local (RW)
  */ 
  setKernelArg(execution->kernel, &nextArg, execution->uintData.localMemSize, NULL);

  /*
  ** 2 - Recuring buffer position __local (RW)
  */
  setKernelArg(execution->kernel, &nextArg, sizeof(unsigned int), NULL);
  
  /*
  ** 3 - Local Mem for temp particules __local (RW)
  */
  setKernelArg(execution->kernel, &nextArg, execution->numWorkerPerWorkgroup * sizeof(t_object), NULL);

  /*
  ** 4 - List of particules __global (RO)
  */
  execution->clObjectList=createBuffer(execution->context,
				       CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
				       sizeof (t_object) * universe->numObj,
				       universe->objectList);
  setKernelArg(execution->kernel, &nextArg, sizeof(cl_mem), &execution->clObjectList);

  /*
  ** 5 - Constant __constant (RO)
  */
  execution->uintData.numObj=universe->numObj;
  execution->uintData.radius=universe->radius;
  execution->uintData.tooClose=universe->tooClose;
  execution->uintData.tooFar=universe->tooFar;
  execution->clUintData=createBuffer(execution->context,
                                     CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
                                     sizeof (t_const_data),
                                     &execution->uintData);
  setKernelArg(execution->kernel, &nextArg, sizeof(cl_mem), &execution->clUintData);

  /*
  ** 6 - Device index __constant device number (RO)
  */
  deviceNum=0;
  execution->clDeviceIndex0=createBuffer(execution->context,
					 CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
					 sizeof(cl_uint),
					 &deviceNum);
  setKernelArg(execution->kernel, &nextArg, sizeof(cl_mem), &execution->clDeviceIndex0);
  
  /*
  ** 7 - Universe object affected by others... (RW)
  */
  execution->clObjChunk=createBuffer(execution->context,
					CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR,
					sizeof(t_object) * execution->uintData.deviceRange,
					execution->objChunk[0]);
  setKernelArg(execution->kernel, &nextArg, sizeof(cl_mem), &(execution->clObjChunk));
  
  return nextArg;
}
