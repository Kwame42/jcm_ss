/*
**
*/
#include "jcm_ss.h"

/*
**
*/
void			*saveFileUniverse(void *arg) {
  t_tInfo		*tInfo;
  
  tInfo=(t_tInfo *)arg;
  while (tInfo->cont == TINFO_CONTINUE) {
    pthread_mutex_lock(&tInfo->mutex);
    pthread_cond_wait(&tInfo->cond, &tInfo->mutex);
    tInfo->universe->saveFunc(tInfo->ticNum, tInfo->universe);
    pthread_mutex_unlock(&tInfo->mutex);
  }

  return NULL;
}

t_tInfo				*initThreadInfo(t_universe *universe) {
  t_tInfo			*tInfo;

  tInfo=(t_tInfo *)allocate(sizeof(t_tInfo));
  if (pthread_mutex_init(&tInfo->mutex, NULL) != 0) {
    perror("Thread mutex init error");
    exit(1);
  }
  if (pthread_cond_init(&tInfo->cond, NULL) != 0) {
    perror("Thread cond init error");
    exit(1);
  }
  if (pthread_attr_init(&tInfo->attr) != 0) {
    perror("Thread attr error");
    exit(1);
  }
  tInfo->universe=universe;
  tInfo->cont=TINFO_CONTINUE;
  if (pthread_create(&(tInfo->tId), NULL, &saveFileUniverse, tInfo) != 0) {
    perror("Thread create error");
    exit(1);
  }
  
  return tInfo;

}

void				waitThread(t_tInfo *tInfo) {
  tInfo->cont=TINFO_STOP;
  pthread_mutex_lock(&tInfo->mutex);
  pthread_cond_signal(&tInfo->cond);
  pthread_mutex_unlock(&tInfo->mutex);
  pthread_join(tInfo->tId, NULL);
}


void			saveUniverse(t_tInfo *tInfo, int ticNum) {
  printf("saving file %d\n", ticNum);
  tInfo->ticNum=ticNum;
  pthread_mutex_lock(&tInfo->mutex);
  pthread_cond_signal(&tInfo->cond);
  pthread_mutex_unlock(&tInfo->mutex);
}

