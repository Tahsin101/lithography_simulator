
#include "parse.h"

#include "glpParse.h"

/* int rctr = 0; */
/* int pctr = 0; */
/* int lctr = 0; */
/* int actr = 0; */

int findLR(int n, int *xy) {
  int i;
  int k = 0;
  int maxx = xy[k++];
  int miny = xy[k++];
  int m = 0;
  k=0;
  for (i=0; i<n; i++) {
    int x = xy[k++];
    int y = xy[k++];
    if (y < miny || (y == miny && x > maxx)) {
      m = i; maxx = x; miny = y;
    }
  }
  return m;
}
int isCCW(int n, int *xy, int m) {
  int i;
  int a[2], b[2], c[2];
  int mpre = (m + (n-1)) %  n;
  int msuc = (m+1) % n;
  for(i=0; i<2; i++) {
    a[i] = xy[2*mpre+i];
    b[i] = xy[2*m   +i];
    c[i] = xy[2*msuc+i];
  }
  int area2 =  
    a[0] * b[1] - a[1] * b[0] +
    a[1] * c[0] - a[0] * c[1] +
    b[0] * c[1] - c[0] * b[1];
  if (area2 > 0) return 1;
  else if (area2 < 0) return -1;
  else return 0;
}


void inslvl(int *lvl, int *cnt, int v) {
  int n = *cnt;
  int i;
  for (i = 0; i<n; i++) {
    if (lvl[i]==v) return;
    if (v < lvl[i]) {
      for(i=i; i<n; i++) {
	int t2 = lvl[i];
	lvl[i] = v;
	v=t2;
      }
    }
  }
  lvl[n] = v;
  *cnt = n+1;
}

#define X(i)  ((float) p[2*i])
#define Y(i)  ((float) p[2*i+1])
#define F(x)  ((float) (x))
int pointInPolygon(int n, int *p,  int x, int y) {

  int  i, j=n-1 ;
  int  oddNodes=0    ;
  for (i=0; i<n; i++) {
    if (Y(i)<F(y) && Y(j)>=F(y)
	||  Y(j)<F(y) && Y(i)>=F(y)) {
      if (X(i)+(F(y)-Y(i))/(Y(j)-Y(i))*(X(j)-X(i))<F(x)) {
        oddNodes=!oddNodes; }}
    j=i; }
  return oddNodes;
} 

#define MAXLVL 10000
int glp_ep(int (*er)(char *l,int x, int y, int w, int h), char *l,int n, int *xy){
  int i,j;
  int hcnt = 0;
  int vcnt = 0;
  int hlvl[MAXLVL];
  int vlvl[MAXLVL];
  int x1 = xy[2*(n-1)];
  int y1 = xy[2*(n-1)+1];
  for (i = 0; i<n; i++) {
    int x2  = xy[2*i];
    int y2  = xy[2*i+1];
    if (x1==x2) {
      //fprintf(stderr, "vins %d\n", x1);
      inslvl(vlvl, &vcnt, x1);
    } else if (y1==y2) {
      //fprintf(stderr, "hins %d\n", y1);
      inslvl(hlvl, &hcnt, y1);
    } else {
      fprintf(stderr, "WARNING: diagonal edge in polygon, layer = %s\n", l);
    }
    x1 = x2;
    y1 = y2;
  }
  //  fprintf(stderr, "polygon %d (n=%d) ccw = %d\n", pctr++, n, isCCW(n,xy,findLR(n,xy)));
  //  fprintf(stderr, "v = %d: ", vcnt);for(i=0; i<vcnt; i++) fprintf(stderr, "%d ", vlvl[i]); fprintf(stderr,"\n");
  //  fprintf(stderr, "h = %d: ", hcnt);for(i=0; i<hcnt; i++) fprintf(stderr, "%d ", hlvl[i]); fprintf(stderr,"\n");
  for(i=0; i<vcnt-1; i++) {
    for (j=0; j<hcnt-1; j++) {
      int xp1 = vlvl[i];
      int yp1 = hlvl[j];
      int xp2 = vlvl[i+1];
      int yp2 = hlvl[j+1];
      int x = (xp1 + xp2) / 2;
      int y = (yp1 + yp2) / 2;
      int p = pointInPolygon(n, xy, x, y);
      //      fprintf(stderr, "Testing %d, %d = %d\n", x, y, p);
      if (p==1) er(l, xp1, yp1, xp2 - xp1, yp2 - yp1);
    }
  }
  //  fprintf(stderr, "-----\n");
  return 1;
}
  
int er_none(int (*er)(char *l,int x, int y, int w, int h), char *l, int x1, int y1, int x2, int y2, int w) {
  int ret = 1;
  if (x1==x2) /* vertical */ {
    if (y1 < y2)                                { er(l, x1-w/2, y1, w, y2-y1); } 
    else                                        { er(l, x1-w/2, y2, w, y1-y2); }
  } else if (y1==y2) /* horizontal */ {
    if (x1 < x2)                                { er(l, x1, y1 - w/2, x2-x1, w); } 
    else                                        { er(l, x2, y1 - w/2, x1-x2, w); }
  } else {fprintf(stderr,"WARNING er_none: degenerate line with a diagonal on %s %d,%d -> %d,%d\n",l,x1,y1,x2,y2);ret=0;}
  return ret;
}
//TBD: fix endpoint adjust
int er_ext1(int (*er)(char *l,int x, int y, int w, int h), char *l, int x1, int y1, int x2, int y2, int w) {
  int ret = 1;
  if (x1==x2) /* vertical */ {
    if (y1 < y2)                                { er(l, x1-w/2, y1, w, y2-y1); } 
    else                                        { er(l, x1-w/2, y2, w, y1-y2); }
  } else if (y1==y2) /* horizontal */ {
    if (x1 < x2)                                { er(l, x1, y1 - w/2, x2-x1, w); } 
    else                                        { er(l, x2, y1 - w/2, x1-x2, w); }
  } else {fprintf(stderr,"WARNING er_ext1: degenerate line with a diagonal on %s %d,%d -> %d,%d\n",l,x1,y1,x2,y2);ret=0;}
  return ret;
}
//TBD: fix endpoint adjust
int er_ext2(int (*er)(char *l,int x, int y, int w, int h), char *l, int x1, int y1, int x2, int y2, int w) {
  int ret = 1;
  if (x1==x2) /* vertical */ {
    if (y1 < y2)                                { er(l, x1-w/2, y1, w, y2-y1); } 
    else                                        { er(l, x1-w/2, y2, w, y1-y2); }
  } else if (y1==y2) /* horizontal */ {
    if (x1 < x2)                                { er(l, x1, y1 - w/2, x2-x1, w); } 
    else                                        { er(l, x2, y1 - w/2, x1-x2, w); }
  } else {fprintf(stderr,"WARNING er_ext2: degenerate line with a diagonal on %s %d,%d -> %d,%d\n",l,x1,y1,x2,y2);ret=0;}
  return ret;
}
//TBD: fix endpoint adjust
int er_both(int (*er)(char *l,int x, int y, int w, int h), char *l, int x1, int y1, int x2, int y2, int w) {
  int ret = 1;
  if (x1==x2) /* vertical */ {
    if (y1 < y2)                                { er(l, x1-w/2, y1, w, y2-y1); } 
    else                                        { er(l, x1-w/2, y2, w, y1-y2); }
  } else if (y1==y2) /* horizontal */ {
    if (x1 < x2)                                { er(l, x1, y1 - w/2, x2-x1, w); } 
    else                                        { er(l, x2, y1 - w/2, x1-x2, w); }
  } else {fprintf(stderr,"WARNING er_both: degenerate line with a diagonal on %s %d,%d -> %d,%d\n",l,x1,y1,x2,y2);ret=0;}
  return ret;
}
void line_err_prt(char *l,int w, int n, int* xy) {
  int i;
  fprintf(stderr, "LINE %s %d %d - ", l, w, n);
  for (i = 0; i<n; i++) {
    fprintf(stderr, "%d %d, ", xy[2*i], xy[2*i+1]);
  }
  fprintf(stderr,"\n");
}

int glp_el(int (*er)(char *l,int x, int y, int w, int h), char *l,int w, int n, int* xy){
  int i;
  if (n<2) {
    fprintf(stderr,"WARNING: degenerate line with less than 2 points\n");
    return 1;
  }
  int k = 0;
  int x1 = xy[k++];
  int y1 = xy[k++];
  int x2 = xy[k++];
  int y2 = xy[k++];
  if (n==2) /* single line segemnt */ {
    if(er_none(er, l,x1,y1,x2,y2,w)==0) line_err_prt(l,w,n,xy);
  } else /* multiple line segments */ {
    /* first segement */
    if(er_ext2(er, l,x1,y1,x2,y2,w)==0) line_err_prt(l,w,n,xy);
    /* middle segments */
    x1 = x2;
    y1 = y2;
    for(i=2; i<n-1; i++) {
      x2 = xy[k++];
      y2 = xy[k++];
      if(er_both(er, l,x1,y1,x2,y2,w)==0) line_err_prt(l,w,n,xy);
      x1=x2;
      y1=y2;
    }
    /* final segment */
    x2=xy[k++];
    y2=xy[k++];
    if(er_ext1(er, l,x1,y1,x2,y2,w)==0) line_err_prt(l,w,n,xy);
  }
  //  fprintf(stderr, "line %d\n", lctr++);
  return 1;
}

int glp_ea(char *l,int x, int y, int w, int h, char *v) {
  //  fprintf(stderr, "alpha %d\n", actr++);
  return 1;
}

int glpParseFileRect(char *fname, int (*er)(char *l,int x, int y, int w, int h)) {
  glpParseFile(fname, er, glp_ep, glp_el, glp_ea);
  //fprintf(stderr,"[GLP PARSE RECT] Rect %d %d %d %d\n",er(x,y,w,h));

}

int glpParseFile(char *fname,
	      int (*er)( char *l,int x, int y, int w, int h), 
	      int (*ep)( int (*er)(char *l,int x, int y, int w, int h), char *l,int n, int *xy), 
	      int (*el)( int (*er)(char *l,int x, int y, int w, int h), char *l,int w, int n, int* xy), 
	      int (*ea)( char *l,int x, int y, int w, int h, char *v)

	      ) {
  // places to store things the parser found
  int ignore;
  char str[300];
  char lyr[300];
  int x, y, w, h;
  //  int x0, y0, x1, y1;
  int n;
  int xy[5000];
  int micron_scale;
  //the parser object
  parse *p = parseCreateFileName(fname);
  //printf("Starting parsing %08X\n",(unsigned int)p);
  // the parser
  return (KWD("BEGIN")
	  && maybe (many (((KWD("EQUIV") && NAT(ignore) &&  NAT(micron_scale) && assign(micron_scale /= 1000)
			    && KWD("MICRON") &&  KWD("+X,+Y") )
			   || (KWD("HEADER") && ID(str) && STR(str) && maybe(many( KWD("+") && STR(str))))
			   || (KWD("LEVEL") && ID(str) && fprintf(stderr, "Found level %s\n", str) )
			   || (KWD("CNAME") && ID(str))
			   || (KWD("ATTR") && ID(str) && STR(str))
			   || (KWD("NOOP")))))
	  && KWD("CELL") && ID(str) && KWD("PRIME")
	  /* && printf("Found cell %s\n", str) */
	  && maybe (many ((KWD("ATTR") && ID(str) && ((KWD("X") && STR(str)) || STR(str)))
			  || (KWD("RECT") && KWD("N") && ID(lyr) && INT(x) && INT(y) && INT(w) && INT(h)
			      			     // && fprintf(stderr,"[GLP PARSE] Rect %s %d %d %d %d\n",str,x,y,w,h)
			      && (*er)(lyr, x/micron_scale, y/micron_scale, w/micron_scale, h/micron_scale) )
			  || (KWD("PGON") && ID(str) && ID(lyr)
			      && assign(n=0)
			      && many(INT(x) && maybe(KWD("+")) && INT(y) && maybe(KWD("+")) &&
				      assign(xy[n++]=x/micron_scale) &&
				      assign(xy[n++]=y/micron_scale))
			      //			      && printf("Pgon %s\n", str)
			      && (*ep)(er, lyr, n/2,xy))
			  || (KWD("LINE") && KWD("N") && (KWD("U") || KWD("E"))&& ID(lyr)
			      && assign(n=0) 
			      && INT(w) 
			      && many(INT(x) && maybe(KWD("+")) && INT(y) && maybe(KWD("+")) 
				      && assign(xy[n++]=x/micron_scale) 
				      && assign(xy[n++]=y/micron_scale))
			      && (*el)(er, lyr, w/micron_scale, n/2, xy))
			  || (KWD("ALPHA") && KWD("N") && ID(lyr)
			      && INT(x) && INT(y) && INT(w) && INT(h) && INT(n)
			      && STR(str)
			      && (*ea)(lyr, x/micron_scale, y/micron_scale, 
				    w/micron_scale, h/micron_scale, 
				    str))
			  ))
	  && KWD("ENDMSG")
	  && END);
  //fprintf(stderr,"[GLP PARSE BELOW] Rect %s %d %d %d %d\n",str,x,y,w,h);
}
 
