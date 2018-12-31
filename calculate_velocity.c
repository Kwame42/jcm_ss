/*
**
*/
#include "jcm_ss.h"

/*
**
*/
void		calculNewVelocity(t_universe *universe, int A, int B) {
  float		a, b, c;
  float		Fa, Fb, Fc;
  
  a=OBJ_Y(universe, B) - TMP_Y(universe, A);
  b=OBJ_X(universe, B) - TMP_X(universe, A);
  c=sqrt(a * a + b * b);
  if (c > universe->tooClose || TMP_MASS(universe, A) * OBJ_MASS(universe, B) < 0) {
    if (c < universe->tooFar) {
      Fc=G * TMP_MASS(universe, A) * OBJ_MASS(universe, B) / c / c;
      //    printf("  (%c) Fc %f\n", (TMP_MASS(universe, A) < 0 ? '-' : '+'), Fc);
      if (Fc < 0) {
	float	I, J, Nc, FNc;
	float	tmp, ia, ib;
	
	ia=a/b;
	ib=TMP_Y(universe, A) - a * TMP_X(universe, A);
	I=ia * universe->radius + ib;
	J=ia * -1 * universe->radius + ib;
	tmp=sqrt((J - I) * (J - I) + 4 * universe->radius * universe->radius);
	Nc=tmp - c;
	//      printf("tmp %f c %f\n", tmp, c);
	FNc=G * TMP_MASS(universe, A) * OBJ_MASS(universe, B) / Nc / Nc;
	Fc=Fc - FNc;
	//      printf("ia %f ib %f I %f J %f Nc %f FNc %f Fc %f\n", ia, ib, I, J, Nc, FNc, Fc);
      }
      Fa=a * Fc / c;
      Fb=b * Fc / c;
      TMP_VEL_Y(universe, A)+=Fa;
      TMP_VEL_X(universe, A)+=Fb;
    }
  } else {
    OBJ_MASS(universe, B)+=TMP_MASS(universe, A);
    TMP_MASS(universe, A)=0;
  }
}
