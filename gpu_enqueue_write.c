/*
**
*/
#include "jcm_ss.h"

/*
**
*/
void			enqueueWriteBuffer(cl_command_queue queue, cl_mem buffer, const void *ptr, size_t size) {
  int			err;

  err=clEnqueueWriteBuffer(queue, buffer, CL_TRUE, 0, size, ptr, 0, NULL, NULL);
  if(err < 0) {
    t_ocl_err				err_list[]={
      {CL_INVALID_COMMAND_QUEUE, "if command_queue is not a valid command-queue."},
      {CL_INVALID_CONTEXT, "if the context associated with command_queue and buffer are not the same or if the context associated with command_queue and events in event_wait_list are not the same."},
      {CL_INVALID_MEM_OBJECT, "if buffer is not a valid buffer object."},
      {CL_INVALID_VALUE, "if the region being written specified by (offset, size) is out of bounds or if ptr is a NULL value or if size is 0."},
      {CL_INVALID_EVENT_WAIT_LIST, "if event_wait_list is NULL and num_events_in_wait_list greater than 0, or event_wait_list is not NULL and num_events_in_wait_list is 0, or if event objects in event_wait_list are not valid events."},
      {CL_MISALIGNED_SUB_BUFFER_OFFSET, "if buffer is a sub-buffer object and offset specified when the sub-buffer object is created is not aligned to CL_DEVICE_MEM_BASE_ADDR_ALIGN value for device associated with queue."},
      {CL_EXEC_STATUS_ERROR_FOR_EVENTS_IN_WAIT_LIST, "if the read and write operations are blocking and the execution status of any of the events in event_wait_list is a negative integer value."},
      {CL_MEM_OBJECT_ALLOCATION_FAILURE, "if there is a failure to allocate memory for data store associated with buffer."},
      {CL_INVALID_OPERATION, "if clEnqueueWriteBuffer is called on buffer which has been created with CL_MEM_HOST_READ_ONLY or CL_MEM_HOST_NO_ACCESS."},
      {CL_OUT_OF_RESOURCES, "if there is a failure to allocate resources required by the OpenCL implementation on the device."},
      {CL_OUT_OF_HOST_MEMORY, "if there is a failure to allocate resources required by the OpenCL implementation on the host."},
      {0}
    };
    
    printError(err, err_list);
    perror("Couldn't write mem to device");
    exit(1);
  };
}
