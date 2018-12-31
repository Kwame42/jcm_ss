/*
**
*/
#include "jcm_ss.h"

/*
**
*/
size_t					getDevicePrefWorkGroupSizeMul(cl_kernel kernel, cl_device_id device) {
  size_t				size;
  size_t				copied;
  
  if (clGetKernelWorkGroupInfo (kernel, device, CL_KERNEL_PREFERRED_WORK_GROUP_SIZE_MULTIPLE, (size_t) sizeof(size_t), &size, &copied) != CL_SUCCESS)
    {
      perror("Can't get CL_KERNEL_PREFERRED_WORK_GROUP_SIZE_MULTIPLE\n");
      exit(0);
    }

  return size;
}

size_t					getDeviceMaxWorkItemPerWorkGroup(cl_device_id device) {
  size_t				size;
  
  if (clGetDeviceInfo(device, CL_DEVICE_MAX_WORK_GROUP_SIZE, sizeof(size_t), &size, NULL) != CL_SUCCESS)
    {
      perror("Can't get CL_DEVICE_MAX_WORK_GROUP_SIZE\n");
      exit(0);
    }

  return size;
}

unsigned long				getDeviceLocalMemSize(cl_device_id device) {
  unsigned long				memSize;

  if (clGetDeviceInfo(device, CL_DEVICE_LOCAL_MEM_SIZE, sizeof(cl_ulong), &memSize, NULL) != CL_SUCCESS) {
    perror("Can't get local mem size info from device");
    exit(1);
  }
  printf(">>INFO   : local memory size per device [%lu] bit\n", memSize);
  
  return memSize;
}

unsigned long				getDeviceGlobalMemSize(cl_device_id device) {
  unsigned long				memSize;
  
  if (clGetDeviceInfo(device, CL_DEVICE_GLOBAL_MEM_SIZE, sizeof(cl_ulong), &memSize, NULL) != CL_SUCCESS) {
    perror("Can't get global mem size info from device\n");
    exit(1);
  }

  return memSize;
}

int					getDeviceNumCore(cl_device_id device) {
  cl_ulong					maxWorkItems;
  
  if (clGetDeviceInfo(device, CL_DEVICE_MAX_COMPUTE_UNITS, sizeof(cl_uint), &maxWorkItems, NULL) != CL_SUCCESS) {
    perror("Can't get info from device");
    exit(1);
  }

  return maxWorkItems;
}
int					getMultiple(size_t core, size_t pref) {
  int					i;
  int					j;

  for (i = 1 ; i <= core ; i++) {
    for (j = 1 ; j <= pref ; j++) {
      if ( i * core == j * pref) {
	return (i * core);
      }
    }
  }

  return core * pref;
}


int					getMultipleMax(size_t core, size_t pref, size_t max) {
  int					m;

  m=getMultiple(core, pref);
  if (m > max) {
    return (max / 2);
  }

  return m;
}
