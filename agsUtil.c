  
#include "agsUtil.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int divCeil(int x, int n) {
  int ret;
  if (x % n == 0) {
    ret = x/n;
  } else {
    ret = x / n + 1;
  }
  fprintf(stderr, "divCeil(%d, %d) = %d(%d)\n", x , n, x/n, ret);
  return ret;
}



int agsVersion() {
  return 1;
}

int tsec[100];
int tusec[100];


void agsTimerStart(int i) {
  struct timeval t;
  gettimeofday(&t,0);
  tsec[i] = t.tv_sec;
  tusec[i] = t.tv_usec;
}

double agsTimerStop(int i) {
  struct timeval t;
  gettimeofday(&t,0);
  int sec = t.tv_sec;
  int usec = t.tv_usec;
  double dt;
  if (usec >= tusec[i]) {
    dt = 1000000.0 * (double)(sec - tsec[i]) + (double)(usec - tusec[i]);
  } else {
    dt = 1000000.0 * (double)(sec - (tsec[i]-1)) + (double)(1000000 + usec - tusec[i]);
  }
  return dt;
}


#define MAXINTERN 1000
int internCnt = 0;
int internAlloc = 0;
char **internStr = NULL;

int internBase = 0;

int agsInternCnt() { return internCnt; }

int agsIntern(char *s) {
  int i;
  if(internStr == NULL) {
    internAlloc = 1000;
    internStr = malloc(internAlloc * sizeof(char *));
  }
  for(i=0; i<internCnt; i++) {
    if (strcmp(s,internStr[(i+internBase) % internCnt])==0) {
      internBase = (i+internBase) % internCnt;
      return internBase;
    }
  }
  if(internCnt >= internAlloc) {
    internAlloc += 1000;
    internStr = realloc(internStr, internAlloc * sizeof(char *));
  }
  internBase = internCnt;
  internStr[internCnt++] = strdup(s);
  return internBase;
}

char *agsUnintern(int i) {
  if (i<internCnt) {
    return internStr[i];
  } else {
    return "";
  }
}

