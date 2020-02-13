 
#ifndef __AGSUTIL_H__
#define __AGSUTIL_H__


#include <limits.h>
#include <stddef.h>
#include <sys/time.h>

#ifdef __cplusplus
extern "C" {
#endif

  int divCeil(int x, int n);

  int agsVersion();

  int agsIntern(char *s);
  char *agsUnintern(int i);
  int agsInternCnt();

  void agsTimerStart(int i);
  double agsTimerStop(int i);

#ifdef __cplusplus
}
#endif
#endif
