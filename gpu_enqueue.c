/*
**
*/
#include "jcm_ss.h"

/*
**
*/
void					enqueueKernel(int deviceNum, t_gpu *execution) {
  cl_int				err;

  execution->numWorker=execution->uintData.deviceRange;
  printf(">>INFO   : number of workgroup [%lu]\n", execution->numWorker /  execution->numWorkerPerWorkgroup);
  if((err=clEnqueueNDRangeKernel(execution->queue[deviceNum], execution->kernel[deviceNum], 1, NULL, &execution->numWorker, &execution->numWorkerPerWorkgroup, 0, NULL, NULL)) < 0) {
    t_ocl_err				err_list[]={
						  {CL_INVALID_PROGRAM_EXECUTABLE, "there is no successfully built program executable available for device associated with command_queue."},
						  {CL_INVALID_COMMAND_QUEUE, "command_queue is not a valid host command-queue."},
						  {CL_INVALID_KERNEL, "kernel is not a valid kernel object."},
						  {CL_INVALID_CONTEXT, "context associated with command_queue and kernel is not the same or if the context associated with command_queue and events in event_wait_list are not the same."},
						  {CL_INVALID_KERNEL_ARGS, "the kernel argument values have not been specified or if a kernel argument declared to be a pointer to a type does not point to a named address space."},
						  {CL_INVALID_WORK_DIMENSION, "work_dim is not a valid value (i.e. a value between 1 and 3)."},
						  {CL_INVALID_GLOBAL_WORK_SIZE, "global_work_size is NULL, or if any of the values specified in global_work_size[0], ...global_work_size [work_dim - 1] are 0 or exceed the range given by the sizeof(size_t) for the device on which the kernel execution will be enqueued."},
						  {CL_INVALID_GLOBAL_OFFSET, "the value specified in global_work_size + the corresponding values in global_work_offset for any dimensions is greater than the sizeof(size_t) for the device on which the kernel execution will be enqueued."},
						  {CL_INVALID_WORK_GROUP_SIZE, "local_work_size is specified and does not match the work-group size for kernel in the program source given by the __attribute__ ((reqd_work_group_size(X, Y, Z))) qualifier."},
						  {CL_INVALID_WORK_GROUP_SIZE, "local_work_size is specified and the total number of work-items in the work-group computed as local_work_size[0] * … local_work_size[work_dim – 1] is greater than the value specified by CL_DEVICE_MAX_WORK_GROUP_SIZE in the table of OpenCL Device Queries for clGetDeviceInfo."},
						  {CL_INVALID_WORK_GROUP_SIZE, "local_work_size is NULL and the __attribute__ ((reqd_work_group_size(X, Y, Z))) qualifier is used to declare the work-group size for kernel in the program source."},
						  {CL_INVALID_WORK_ITEM_SIZE, "the number of work-items specified in any of local_work_size[0], ... local_work_size[work_dim - 1] is greater than the corresponding values specified by CL_DEVICE_MAX_WORK_ITEM_SIZES[0], .... CL_DEVICE_MAX_WORK_ITEM_SIZES[work_dim - 1]."},
						  {CL_INVALID_WORK_GROUP_SIZE, "the program was compiled with –cl-uniform-work-group-size and the number of work-items specified by global_work_size is not evenly divisible by size of work-group given by local_work_size."},
						  {CL_INVALID_WORK_ITEM_SIZE, "the number of work-items specified in any of local_work_size[0], ... local_work_size[work_dim – 1] is greater than the corresponding values specified by CL_DEVICE_MAX_WORK_ITEM_SIZES[0], ... CL_DEVICE_MAX_WORK_ITEM_SIZES[work_dim – 1]."},
						  {CL_MISALIGNED_SUB_BUFFER_OFFSET, "a sub-buffer object is specified as the value for an argument that is a buffer object and the offset specified when the sub-buffer object is created is not aligned to CL_DEVICE_MEM_BASE_ADDR_ALIGN value for device associated with queue."},
						  {CL_INVALID_IMAGE_SIZE, "an image object is specified as an argument value and the image dimensions (image width, height, specified or compute row and/or slice pitch) are not supported by device associated with queue."},
						  {CL_IMAGE_FORMAT_NOT_SUPPORTED, "an image object is specified as an argument value and the image format (image channel order and data type) is not supported by device associated with queue."},
						  {CL_OUT_OF_RESOURCES, "there is a failure to queue the execution instance of kernel on the command-queue because of insufficient resources needed to execute the kernel. For example, the explicitly specified local_work_size causes a failure to execute the kernel because of insufficient resources such as registers or local memory. Another example would be the number of read-only image args used in kernel exceed the CL_DEVICE_MAX_READ_IMAGE_ARGS value for device or the number of write-only image args used in kernel exceed the CL_DEVICE_MAX_READ_WRITE_IMAGE_ARGS value for device or the number of samplers used in kernel exceed CL_DEVICE_MAX_SAMPLERS for device."},
						  {CL_MEM_OBJECT_ALLOCATION_FAILURE, "there is a failure to allocate memory for data store associated with image or buffer objects specified as arguments to kernel."},
						  {CL_INVALID_EVENT_WAIT_LIST, "event_wait_list is NULL and num_events_in_wait_list > 0, or event_wait_list is not NULL and num_events_in_wait_list is 0, or if event objects in event_wait_list are not valid events."},
						  {CL_INVALID_OPERATION, "SVM pointers are passed as arguments to a kernel and the device does not support SVM or if system pointers are passed as arguments to a kernel and/or stored inside SVM allocations passed as kernel arguments and the device does not support fine grain system SVM allocations."},
						  {CL_OUT_OF_RESOURCES, "there is a failure to allocate resources required by the OpenCL implementation on the device."},
						  {CL_OUT_OF_HOST_MEMORY, "there is a failure to allocate resources required by the OpenCL implementation on the host."},
						  {0}
    };
    printError(err, err_list);
    perror("Couldn't enqueue the kernel");
    exit(1);
  }
  clFlush(execution->queue[deviceNum]);
}
