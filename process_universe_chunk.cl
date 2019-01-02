/*
**
*/
#include "./space_object.h"
#include "./proto.h"

/*
**
*/
#define SIZE_FLOAT8 (sizeof(float8))

/*
**
*/
uint		memcpy_gl_lo(__local float8 *dst, uint size_dst, __global float8 *src, uint size_src, __local uint *pos) {
  uint		size;
  event_t	e;

  if (*pos + size_dst < size_src) {
    size = size_dst;
  } else {
    size = size_src - *pos;
  }
  e = async_work_group_copy(dst, &src[*pos], size, e);
  if (get_local_id(0) == 0) {
    *pos += size;
  }
  wait_group_events(1, &e);
  
  return size;
}

/*
**
*/
__kernel void	process_universe_chunk(__local float8		*lo_obj_list,
				       __local uint		*lo_pos,
				       __local float8		*lo_data,
				       __global float8		*GL_obj_list,
				       __constant t_const_data	*GL_uint_data,
				       __global uint		*GL_device_num,
				       __global float8		*GL_data
				       ) {
  uint					num_work_group, local_addr, group_addr, num_worker, global_addr;
  uint					local_mem_size, num_obj, radius, worker_range, too_close, too_far, device_range;
  uint					device_num;
  uint					i, j, k;
  float8				part;
  float3				obj, vel;
  float					mass;
  float3				res;
  uint					size;
  uint					num_lo_cpy;
  event_t				e;

  /*
  **
  */
  num_work_group = get_num_groups(0);
  local_addr = get_local_id(0);
  group_addr = get_group_id(0);
  global_addr = get_global_id(0);
  num_worker = get_global_size(0);
  
  /*
  **
  */
  local_mem_size = GL_uint_data->localMemSize;
  num_obj = GL_uint_data->numObj;
  radius = GL_uint_data->radius;
  worker_range = GL_uint_data->workerRange;
  too_close = GL_uint_data->tooClose;
  too_far = GL_uint_data->tooFar;
  device_range = GL_uint_data->deviceRange;
  device_num = *GL_device_num;
  num_lo_cpy = num_obj * SIZE_FLOAT8 / local_mem_size;
  if (local_addr == 0) {
    *lo_pos = 0;
  }
  barrier(CLK_LOCAL_MEM_FENCE);
  size = 0;
  e = async_work_group_copy(lo_data, &GL_data[group_addr * device_range / num_work_group], device_range / num_work_group, e);
  wait_group_events(1, &e);
  for (i = 0 ; i < num_lo_cpy ; i++) {
    size=memcpy_gl_lo(lo_obj_list, local_mem_size / SIZE_FLOAT8, GL_obj_list, num_obj, lo_pos);
    for (j = 0 ; j < size ; j++) {
      float3			d;
      float			distance;
      float			force;
      float3			grav_vel;

      if (lo_data[local_addr].s6 != 0 && lo_obj_list[j].s6 != 0) { //if there is mass for both objects
	if ((device_num * device_range + global_addr) != i * size + k) { //don't affect an object by itself
	  d = lo_data[local_addr].xyz - lo_obj_list[j].xyz;
	  distance = sqrt(d.x * d.x + d.y * d.y + d.z * d.z);
	  if (distance < too_close) { // fusion objects
	    lo_data[j].s6 += lo_obj_list[j].s6;
	    GL_obj_list[i * size + j].s6 == 0; // affect global memory can cause issue in the model... todo !
	  } else if (distance < too_far) { //calculate new velocity
	    force = G * part.s6 * lo_obj_list[j].s6 / distance / distance;
	    grav_vel = d * force / distance;
	    lo_data[local_addr].s345 += grav_vel;
	  }
	}
      }
      barrier(CLK_LOCAL_MEM_FENCE);
    }
    if (lo_data[local_addr].s6 != 0) {
      lo_data[local_addr].s012 = lo_data[local_addr].s345 / lo_data[local_addr].s6; //new object position
    }
    e = async_work_group_copy(&GL_data[group_addr * device_range / num_work_group], lo_data, device_range / num_work_group, e);
    wait_group_events(1, &e);
  }
}

