/*
**
*/
#include "jcm_ss.h"

/*
**
*/
int					setKernelArg(cl_kernel kernel, int nextArg, int size, void *arg) {
  cl_int				err;

  if ((err=clSetKernelArg(kernel, nextArg, size, arg)) < 0) {
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
  
  return nextArg;
}

int					setGlobalKernelArg(t_gpu *execution, t_universe *universe) {
  int					i;
  
  execution->clDeviceIndex0=(cl_mem *)allocate(sizeof(cl_mem) * execution->numDevices);
  execution->clObjChunk=(cl_mem *)allocate(sizeof(cl_mem) * execution->numDevices);
  for (i=0 ; i < execution->numDevices ; i++) {
    /*
    ** 1 - Local Mem for all particules (recuring buffer) __local (RW)
    */ 
    setKernelArg(execution->kernel[i], 0, execution->uintData.localMemSize, NULL);
    
    /*
    ** 2 - Recuring buffer position __local (RW)
    */
    setKernelArg(execution->kernel[i], 1, sizeof(unsigned int), NULL);
    
    /*
    ** 3 - Local Mem for temp particules __local (RW)
    */
    setKernelArg(execution->kernel[i], 2, execution->numWorkerPerWorkgroup * sizeof(t_object), NULL);
    
    /*
    ** 4 - List of particules __global (RO)
    */
    execution->clObjectList=createBuffer(execution->context[i],
					 CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
					 sizeof (t_object) * universe->numObj,
					 universe->objectList);
    setKernelArg(execution->kernel[i], 3, sizeof(cl_mem), &execution->clObjectList);
    
    /*
    ** 5 - Constant __constant (RO)
    */
    execution->uintData.numObj=universe->numObj;
    execution->uintData.radius=universe->radius;
    execution->uintData.tooClose=universe->tooClose;
    execution->uintData.tooFar=universe->tooFar;
    execution->clUintData=createBuffer(execution->context[i],
				       CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
				       sizeof (t_const_data),
				       &execution->uintData);
    setKernelArg(execution->kernel[i], 4, sizeof(cl_mem), &execution->clUintData);
    
    /*
    ** 6 - Device index __constant device number (RO)
    */
    execution->deviceIndex[i]=i;
    execution->clDeviceIndex0[i]=createBuffer(execution->context[i],
					  CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR,
					  sizeof(unsigned int),
					  &execution->deviceIndex[i]);
    setKernelArg(execution->kernel[i], 5, sizeof(cl_mem), &execution->clDeviceIndex0[i]);
    
    /*
    ** 7 - Universe object affected by others... (RW)
    */
    execution->clObjChunk[i]=createBuffer(execution->context[i],
					  CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR,
					  sizeof(t_object) * execution->uintData.deviceRange,
					  execution->objChunk[i]);
    setKernelArg(execution->kernel[i], 6, sizeof(cl_mem), &execution->clObjChunk[i]);
  }
  
  return 6;
}
