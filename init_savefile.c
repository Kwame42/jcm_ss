/*
**
*/
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <json-c/json.h>

/*
**
*/
#include "jcm_ss.h"

/*
**
*/
void			saveFileUniverse(int i, t_universe *universe) {
#define MAX_SIZE	1000
  char			buff[MAX_SIZE];
  int			size;
  FILE			*fp;
  char			filename[MAX_SIZE];

  sprintf(filename, "%s%d-%s",universe->saveDir, i, universe->saveFile);
  fp=fopen(filename, "w");
  if (fp == NULL) {
    printf("Can't open file [%s] for writing\n", filename);
    exit(1);
  }
  for (i=0 ; i < universe->numObj ; i++) {
    if (OBJ_MASS(universe, i)) {
      size=sprintf(buff, "sphere { <%f, %f, 0>, %f texture {pigment { %s } finish { reflection 1 }}}\n", OBJ_X(universe, i), OBJ_Y(universe, i), universe->objSize * sqrt(fabs(OBJ_MASS(universe, i))), (OBJ_MASS(universe, i) < 0 ? "color red 1.0" : "BrightGold"));
      fwrite(buff, size, 1, fp);
    }
  }
  fclose(fp);
}


void			saveUniverse(int ticNum, t_universe *universe) {
#define MAX_SIZE	1000
  char			buff[MAX_SIZE];
  int			size;
  FILE			*fp;
  char			filename[MAX_SIZE];
  int			i;
  
  sprintf(filename, "%s%d-%s",universe->saveDir, ticNum, universe->saveFile);
  if ((fp=fopen(filename, "w")) == NULL) {
    printf("Can't open file [%s] for writing\n", filename);
    exit(1);
  }
  size=sprintf(buff, "{\n\t\"Tic\": %d,\n\t\"Universe Radius\": %d,\n\t\"Space Object\": \[\n", ticNum, universe->radius);
  fwrite(buff, size, 1, fp);
  for (i=0 ; i < universe->numObj ; i++) {
    size=sprintf(buff, "\t\t{\"x\": %f, \"y\": %f, \"Size\": %d, \"Sign\": \"%s\"},\n", OBJ_X(universe, i), OBJ_Y(universe, i), universe->objSize, (OBJ_MASS(universe, i) <0 ? "M+" : "M-"));
    fwrite(buff, size, 1, fp);
  }
  size=sprintf(buff, "\t]\n}\n\n");
  fwrite(buff, size, 1, fp);
  fclose(fp);
}
