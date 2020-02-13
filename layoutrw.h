#include "glpParse.h"
#include "agsUtil.h"

#define MAXLVL 1
int lvlcnt[MAXLVL];

typedef struct {
  int x,y,w,h;
} rect;

rect *lvldata[MAXLVL];
rect *d_lvldata[MAXLVL];
int nlvl = 0;

int *devdata[MAXLVL];
int llx =  1000000000;
int lly =  1000000000;
int urx = -1000000000;
int ury = -1000000000;

int rctr = 0;
int ef_xoff = 0;
int ef_yoff = 0;

int min(int a, int b) {
	return ((a<b)?a:b);
}

int max(int a, int b) {
	return ((a>b)?a:b);
}

int ercnt(char *l,int x, int y, int w, int h) {
  int i = agsIntern(l);
  lvlcnt[i]++;
  llx = min(x,llx);
  lly = min(y,lly);
  urx = max(x+w,urx);
  ury = max(y+h,ury);
  rctr++;
  return 1;
}

int erfill(char *l,int x, int y, int w, int h) {
  int i = agsIntern(l);
  int j = lvlcnt[i];
  rect *r = lvldata[i]+j;
  r->x = x + ef_xoff;
  r->y = y + ef_yoff;
  r->w = w;
  r->h = h;
  llx = min(r->x,llx);
  lly = min(r->y,lly);
  urx = max(r->x+w,urx);
  ury = max(r->y+h,ury);
  lvlcnt[i]++;
  return 1;
}

void clear_design_space()
{
  int i;
  for(i=0;i<nlvl;i++)  { free(lvldata[i]); lvlcnt[i]=0; }   /* Clear up the design space */
  llx = lly =  10000000;
  urx = ury = -10000000;
}

// This function reads in a file in GULP form and stores it as rectangles
void read_layout(char* fname)
{
     fprintf(stderr, "First pass counting\n"); glpParseFileRect(fname, ercnt); nlvl = agsInternCnt();
     fprintf(stderr,"%d layers parsed\n", nlvl);
     int i;
     for(i=0;i<MAXLVL;i++) {
       lvldata[i]=NULL;
       if (lvlcnt[i]>0) { fprintf(stderr, "Level %s (%d) count = %d\n", agsUnintern(i),i,lvlcnt[i]); lvldata[i] = malloc(lvlcnt[i] * sizeof(rect)); lvlcnt[i] = 0;}
     }
     //fprintf(stderr, "Second pass filling\n"); ef_xoff = -llx; ef_yoff = -lly;
     fprintf(stderr, "Second pass filling\n"); ef_xoff = 0; ef_yoff = 0;
     // offset is set to 0
     llx = lly =  10000000; urx = ury = -10000000;
     glpParseFileRect(fname, erfill);
     fprintf(stderr,"Adjusted bounding box	(%5d,%d) -> (%d,%d)\n", llx, lly, urx, ury);
}
