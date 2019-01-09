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
void			saveFilePovray(int ticNum, t_universe *universe) {
#define MAX_SIZE	1000
  char			buff[MAX_SIZE];
  int			size;
  FILE			*fp;
  char			filename[MAX_SIZE];
  int			i;

  sprintf(filename, "%s%d-%s.pov",universe->saveDir, ticNum, universe->saveFile);
  fp=fopen(filename, "w");
  if (fp == NULL) {
    printf("Can't open file [%s] for writing\n", filename);
    exit(1);
  }
  size=sprintf(buff, "#include \"colors.inc\"\n background { color Black } \n camera { location <%d, %d, %d>\nlook_at  <0, 0,  0>\n} \nlight_source { <%d, %d, %d> color White}\n", universe->radius * 2, universe->radius * 2, universe->radius * 2, universe->radius * 2, universe->radius * 2, universe->radius * 2);
  fwrite(buff, size, 1, fp);
  for (i=0 ; i < universe->numObj ; i++) {
    if (OBJ_MASS(universe, i)) {
      size=sprintf(buff, "sphere { <%f, %f, %f>, %d texture {pigment { %s } finish { reflection 1 }}}\n", OBJ_X(universe, i), OBJ_Y(universe, i), OBJ_Z(universe, i), (OBJ_MASS(universe, i) < 0 ? universe->objSize : 1), (OBJ_MASS(universe, i) < 0 ? "color red 1.0" : "BrightGold"));
      fwrite(buff, size, 1, fp);
    }
  }
  fclose(fp);
}

void			saveFileJson(int ticNum, t_universe *universe) {
#define MAX_SIZE	1000
  char			buff[MAX_SIZE];
  int			size;
  FILE			*fp;
  char			filename[MAX_SIZE];
  int			i;
  
  sprintf(filename, "%s%d-%s.json",universe->saveDir, ticNum, universe->saveFile);
  if ((fp=fopen(filename, "w")) == NULL) {
    printf("--ERROR  :Can't open file [%s] for writing\n", filename);
    exit(1);
  }
  size=sprintf(buff, "{\n\t\"%s\": \"%s\",\n\t\"%s\": \"%s\",\n\t\"%s\": %d,\n\t\"%s\": %d,\n\t\"%s\": \"%s\",\n\t\"%s\": %d,\n\t\"%s\": %d,\n\t\"%s\": %d,\n\t\"%s\":\n\t\t[\n", JCM_SS_SAVE_FORMAT, JCM_SS_JSON, JCM_SS_SAVE_FILE, universe->saveFile, JCM_SS_UNIVERSE_RADIUS, universe->radius, JCM_SS_NUM_TIC, universe->numTic, JCM_SS_SAVE_DIR, universe->saveDir, JCM_SS_TOO_CLOSE, universe->tooClose, JCM_SS_TOO_FAR, universe->tooFar, JCM_SS_TIC_NUM, ticNum, JCM_SS_INIT_OBJ);
  fwrite(buff, size, 1, fp);
  for (i=0 ; i < universe->numObj ; i++) {
    size=sprintf(buff, "\t\t{\"x\": %f, \"y\": %f, \"z\": %f, \"velx\": %f, \"vely\": %f, \"velz\": %f, \"Mass\": %f},\n", OBJ_X(universe, i), OBJ_Y(universe, i), OBJ_Z(universe, i), OBJ_VEL_X(universe, i), OBJ_VEL_Y(universe, i), OBJ_VEL_Z(universe, i), OBJ_MASS(universe, i));
    fwrite(buff, size, 1, fp);
  }
  size=sprintf(buff, "\t]\n}\n\n");
  fwrite(buff, size, 1, fp);
  fclose(fp);
}
