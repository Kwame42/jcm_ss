/*
**
*/
#include <json-c/json.h>

/*
**
*/
#include "jcm_ss.h"

/*
**
*/
void			setInitialCond(t_universe *universe) {
  int i;

  universe->numObj=json_object_array_length(universe->initCond);
  universe->objectList=allocateObjectList(universe->numObj);
  for (i=0 ; i < universe->numObj ; i++) {
    OBJ_X(universe, i)=json_object_get_double(json_object_object_get(json_object_array_get_idx(universe->initCond, i), "x"));
    OBJ_Y(universe, i)=json_object_get_double(json_object_object_get(json_object_array_get_idx(universe->initCond, i), "y"));
    OBJ_Z(universe, i)=json_object_get_double(json_object_object_get(json_object_array_get_idx(universe->initCond, i), "z"));
    OBJ_VEL_X(universe, i)=json_object_get_double(json_object_object_get(json_object_array_get_idx(universe->initCond, i), "vx"));
    OBJ_VEL_Y(universe, i)=json_object_get_double(json_object_object_get(json_object_array_get_idx(universe->initCond, i), "vy"));
    OBJ_VEL_Z(universe, i)=json_object_get_double(json_object_object_get(json_object_array_get_idx(universe->initCond, i), "vz"));
    OBJ_MASS(universe, i)=json_object_get_double(json_object_object_get(json_object_array_get_idx(universe->initCond, i), "mass"));
  }
}
