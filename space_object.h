/*
**
*/
#define LO_MEM_BUF_POS		(sizeof(unsigned int))
#define G			(0.667)

/*
**
*/
typedef struct			s_const_data {
  unsigned long			localMemSize;
  unsigned int			numObj;
  unsigned int			radius;
  unsigned int			workerRange;
  unsigned int			tooClose;
  unsigned int			tooFar;
  unsigned int			deviceRange;
}				t_const_data;

typedef struct			s_pos {
  float				x;
  float				y;
  float				z;
}				t_pos;

typedef struct			s_object {
  t_pos				pos;
  t_pos				velocity;
  float				mass;
  //this is to align for OpenCL code (t_pos size is 3 - float - and mass 1 so 2x3+1=7 and need jump to float8!)
  float				padding; 
}				t_object;
