#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <json-c/json.h>

/*
**
*/
#include "jcm_ss.h"

/*
**
*/
void			checkUniverse(t_universe *universe) {
  if (universe->saveDir == NULL) {
    printf("Option %s not set\n", JCM_SS_SAVE_DIR);
    exit(1);
  }
  if (universe->saveFile == NULL) {
    printf("Option %s not set\n", JCM_SS_SAVE_FILE);
    exit(1);
  }
  if (universe->radius == 0) {
    printf("Option %s not set\n", JCM_SS_UNIVERSE_RADIUS);
    exit(1);
  }
  if (universe->numTic == 0) {
    printf("Option %s not set\n", JCM_SS_NUM_TIC);
    exit(1);
  }
  if (universe->tooClose == 0) {
    printf("Option %s not set\n", JCM_SS_TOO_CLOSE);
    exit(1);
  }
  if (universe->tooFar == 0) {
    printf("Option %s not set\n", JCM_SS_TOO_FAR);
    exit(1);
  }
  if (universe->initCond == 0) {
    printf("Option %s not set\n", JCM_SS_INIT_OBJ);
    exit(1);
  }

}

char			*normalizedString(const char *str) {
  int			size;
  char			*ret;
  int			i;

  size=strlen(str);
  if ((ret=malloc(size)) == NULL) {
    printf ("can't allocate memory\n");
    exit(1);
  }
  i=0;
  while (str && *str) {
    if (*str != DOUBLE_QUOTE && *str != BACK_SLASH) {
      ret[i++]=*str;
    }
    str++;
  }
  ret[i]=0;

  return (ret);
}

int			getFilesize(const char* file) {
    struct stat		st;
    
    stat(file, &st);
    
    return st.st_size;
}

char			*getConfFile(const char *file) {
  char			*buff;
  int			fd;
  int			size;

  if ((fd = open(file, O_RDONLY)) < 0 ) {
    printf("Can't open file %s\n", file);
    exit(1);
  }
  size=getFilesize(file);
  if ((buff=mmap(0, size, PROT_READ, MAP_PRIVATE, fd, 0)) == MAP_FAILED) {
    printf("can't map region...\n");
    exit(1);
  }
  close(fd);

  return buff;
}

t_universe			*getConf(int ac, char **av) {
  t_universe			*universe=NULL;
  char				*confFile;
  json_object			*jobj;

  if (ac < 2)
    usage();
  universe=allocateUniverse();
  confFile=getConfFile(av[1]);
  jobj=json_tokener_parse(confFile);
  json_object_object_foreach(jobj, key, val) {
    if (strcmp(key, JCM_SS_SAVE_FILE) == 0) {
      universe->saveFile=json_object_get_string(val);
    }
    if (strcmp(key, JCM_SS_SAVE_DIR) == 0) {
      universe->saveDir=json_object_get_string(val);
    }
    if (strcmp(key, JCM_SS_UNIVERSE_RADIUS) == 0) {
      universe->radius=json_object_get_int(val);
    }
    if (strcmp(key, JCM_SS_NUM_TIC) == 0) {
      universe->numTic=json_object_get_int(val);
    }
    if (strcmp(key, JCM_SS_TOO_CLOSE) == 0) {
      universe->tooClose=json_object_get_int(val);
    }
    if (strcmp(key, JCM_SS_TOO_FAR) == 0) {
      universe->tooFar=json_object_get_int(val);
    }
    if (strcmp(key, JCM_SS_INIT_OBJ) == 0) {
      universe->initCond=val;
    }
  }
 checkUniverse(universe);
  
  return universe;
}
