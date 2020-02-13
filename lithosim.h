
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "fftw3.h"

#include "lithosim.h"
#include "fragment.h"
#include "lut.h"
#include "png_write.h"

#include "agsBm.h"

#include "agsKrn.h"
#include "agsMem.h"

float WEIGHT_INTENSITY = 0.0;    /* Cost_Function = Weight_Intensity*(I - Itarget) + (1-Weight_Intensity)*exp(Islope) */
int MAX_FILE_NAME_LENGTH = 80;
float BACKGROUND_REAL = 0.0229;
float BACKGROUND_IMAG = -0.0217;
float MASK_REAL = 0.9771;
float MASK_IMAG = 0.0217;

char *fftwf_wisdom[2] = {
  //for macbook pro _cf2048x2048_cb2048x2048_ =
"(fftw-3.2.1 fftwf_wisdom\
  (fftwf_dft_nop_register 0 #x1040 #x1040 #x0 #x7f6a6432 #x2d6843ab #x2527ce25 #x368a637c)\
  (fftwf_rdft_rank0_register 4 #x1040 #x1040 #x0 #x80c09daa #x83ad25dd #x8d7a205b #xc5645b5f)\
  (fftwf_codelet_t1_32 0 #x40 #x40 #x0 #xf7704b04 #x5997cad8 #x86de22f7 #xb4be321f)\
  (fftwf_dft_vrank_geq1_register 0 #x40 #x40 #x0 #x2ab6aeb6 #xa3c5408d #x0ba6d5d5 #x2fcdec33)\
  (fftwf_dft_vrank_geq1_register 0 #x1040 #x1040 #x0 #x3c0aa804 #x09ff3a0e #x3970e0ad #x175c5ea9)\
  (fftwf_codelet_n1_4 0 #x40 #x40 #x0 #xca3a4d79 #xa349ccf8 #x0877a3a5 #x62edc76f)\
  (fftwf_dft_rank_geq2_register 0 #x1040 #x1040 #x0 #x6d0d3557 #x66c05dd2 #x08c72cca #x96573262)\
  (fftwf_codelet_n1_4 0 #x40 #x40 #x0 #x94178bf7 #xf50166b7 #xa507ebc4 #x8495b5ca)\
  (fftwf_codelet_t1_64 0 #x1040 #x1040 #x0 #x52c16d5c #x78672292 #xb152d1c2 #x86ccca81)\
  (fftwf_dft_buffered_register 1 #x1040 #x1040 #x0 #xef76f989 #xbd4f4561 #x0aeeb443 #xfc90f8c1)\
  (fftwf_dft_r2hc_register 0 #x1040 #x1040 #x0 #x4160825d #xd6712fce #x28e48976 #x429f2d9c)\
  (fftwf_codelet_t1_64 0 #x1040 #x1040 #x0 #xeab40192 #xd605d0ea #x33acdc5e #xd87ce2ac)\
  (fftwf_dft_r2hc_register 0 #x1040 #x1040 #x0 #x41aebf60 #x5f9a94de #xa1db3560 #xaaa35541)\
  (fftwf_codelet_t1_16 0 #x40 #x40 #x0 #xe90e5bd7 #x9b96231a #x5ae521e6 #xaf8d9e76)\
  (fftwf_codelet_t1_16 0 #x40 #x40 #x0 #xb8ffa816 #x392a7ef1 #xdcd466aa #x45c25d3d)\
  (fftwf_codelet_t1_32 0 #x40 #x40 #x0 #x421c52a8 #x16494b63 #xd410b8a8 #x5bdfc4cc)\
  (fftwf_dft_rank_geq2_register 0 #x1040 #x1040 #x0 #x499a8235 #x3dd05817 #x554cd266 #xb0cff2bb)\
  (fftwf_dft_buffered_register 1 #x1040 #x1040 #x0 #xab1cc9c2 #xeb3c33aa #x71ef6175 #xa0246365)\
  (fftwf_dft_vrank_geq1_register 0 #x1040 #x1040 #x0 #x0d9ff864 #xf8defb37 #xfc370118 #x920983f7)\
  (fftwf_dft_vrank_geq1_register 0 #x40 #x40 #x0 #xf621dcbf #x0cd08781 #xb1dba09f #xf9e76e38)\
  (fftwf_dft_vrank_geq1_register 0 #x40 #x40 #x0 #x3fb8754a #xf5931640 #x74d8ad32 #x7d796fa3)\
  (fftwf_dft_vrank_geq1_register 0 #x40 #x40 #x0 #xe4f64e90 #xf2c71347 #x96fbf0a7 #x8bb5233a)\
  (fftwf_dft_nop_register 0 #x1040 #x1040 #x0 #x58c24a54 #x34caee9b #x6da274de #x686c1b30)\
  (fftwf_codelet_n1_32 0 #x1040 #x1040 #x0 #xf15b330d #xbf11680d #xac2e6b40 #x74efe163)\
  (fftwf_codelet_n1_32 0 #x1040 #x1040 #x0 #x68879c73 #x58ad2bd9 #x96b1e709 #xd0eba397)\
)",
// for arlx111 ... arlx118 _cf2048x2048_cb2048x2048_
"(fftw-3.2.1 fftwf_wisdom\
  (fftwf_dft_r2hc_register 0 #x1040 #x1040 #x0 #x87d402be #x9edaf97e #x578bd92b #x9b94644d)\
  (fftwf_dft_vrank_geq1_register 0 #x1040 #x1040 #x0 #x2f525d92 #x354db20c #x5dc9c81c #xd5262385)\
  (fftwf_dft_buffered_register 1 #x1040 #x1040 #x0 #x0551392c #xb8a7f449 #x18f646c0 #xf9608e38)\
  (fftwf_codelet_t1_64 0 #x40 #x40 #x0 #x24a3cb08 #x596c2b8b #x72ff3abb #x0d42e591)\
  (fftwf_dft_nop_register 0 #x1040 #x1040 #x0 #x7927936d #x770c8ef7 #x7cda0f74 #x0ef3ed58)\
  (fftwf_dft_vrank_geq1_register 0 #x440 #x440 #x0 #xf80d33e6 #x7f21d9a0 #xf0163aa7 #x9336d2cf)\
  (fftwf_rdft_rank0_register 2 #x40 #x40 #x0 #xfce92b26 #xd61bc939 #x513238d4 #x71cc55f3)\
  (fftwf_codelet_t1_32 0 #x40 #x40 #x0 #x9b478eaf #x6b4eb1dd #x2181f1f4 #x5fd7ee9a)\
  (fftwf_codelet_n1_32 0 #x1040 #x1040 #x0 #x04ff6da1 #x20ae4760 #xb5314542 #x6e1be611)\
  (fftwf_dft_nop_register 0 #x1040 #x1040 #x0 #x9bc41ce1 #x0bcb22c5 #x9c236ad1 #xf034d07b)\
  (fftwf_dft_rank_geq2_register 0 #x1040 #x1040 #x0 #x78d97a71 #xecc36c1f #xb05dd254 #x78217136)\
  (fftwf_dft_r2hc_register 0 #x40 #x40 #x0 #xd683b9af #x6c47fd8b #x94b22112 #xcb95e9c6)\
  (fftwf_dft_buffered_register 1 #x1040 #x1040 #x0 #x44868a42 #x169fd24f #x7ce2cf5b #x399a18f5)\
  (fftwf_dft_vrank_geq1_register 0 #x1040 #x1040 #x0 #x9abe6f40 #xa299582f #x7eb66309 #x128ce6c4)\
  (fftwf_codelet_t1_64 0 #x1040 #x1040 #x0 #xa5ec3219 #xb6a587c3 #x59ea6ff1 #x344b8c2b)\
  (fftwf_dft_vrank_geq1_register 0 #x40 #x40 #x0 #xfa8b7e37 #x32d92aed #xd9a1bd77 #x61fe06a9)\
  (fftwf_dft_indirect_register 0 #x40 #x40 #x0 #xbaac3df1 #x08141485 #xeb919b5f #x06e839f3)\
  (fftwf_codelet_n1_32 0 #x440 #x440 #x0 #x162d3b85 #xae08a7f3 #x370cad45 #xa2396241)\
  (fftwf_dft_vrank_geq1_register 0 #x40 #x40 #x0 #x6a20f1a0 #x6c518403 #x9a36a6c9 #x06655191)\
  (fftwf_codelet_n1_32 0 #x1040 #x1040 #x0 #x1ed069aa #x4cdf0b81 #xaba11f5d #x9e76b43f)\
  (fftwf_rdft_rank0_register 4 #x1040 #x1040 #x0 #xb2990766 #xa2d3c102 #xce2cca98 #x96e02af4)\
  (fftwf_codelet_t1_64 0 #x1040 #x1040 #x0 #x3182f4fb #x0a5541af #xd1943d43 #x87c40628)\
  (fftwf_dft_r2hc_register 0 #x1040 #x1040 #x0 #x1ddea6cc #xe6e3df5f #x79956d3f #x21c77e34)\
  (fftwf_dft_vrank_geq1_register 0 #x1040 #x1040 #x0 #x3d4161a6 #x33cd4303 #xcfe62f13 #xea549dc6)\
  (fftwf_dft_rank_geq2_register 0 #x1040 #x1040 #x0 #x4ef51761 #x7671a8ae #x0344b1b0 #x75952958)\
  (fftwf_codelet_n1_64 1 #x40 #x40 #x0 #xd0abe487 #x0ecd413e #xbd08ae7a #x78ddf448)\
)"
};				/*  */




void mySubBlock(unsigned char *bmsk, int nx, int ny, unsigned char *tiletemp, int tnx, int tny, int x, int y) {
  int i,  j;
  for(i=0; i<tnx; i++) {
    for(j=0; j<tny; j++) {
      tiletemp[j*tnx+i] = bmsk[(y+j)*ny+(x+i)];
      printf(stderr,"tiletemp[%d]=%d",(j*tnx+i),tiletemp[j*tnx+i]);
    }
  }
}


#define MAXLVL 100 // ???
int lvlcnt[MAXLVL]; //???

typedef struct {
  int x,y,w,h;
} rect;

rect *lvldata[MAXLVL];
rect *d_lvldata[MAXLVL];
int nlvl = 0;

int *devdata[MAXLVL];

void mySubSample(unsigned char *tile, int tnx, int tny, int sc, unsigned char *out) {
  int i, j;
  int snx = tnx / sc;
  int sny = tny / sc;
  for(i=0; i<snx; i++) {
    for(j=0; j<sny; j++) {
      out[j*snx+i] = tile[j*sc*tnx + i*sc];
    }
  }
}

void myDevLayerDraw(unsigned char *imgpbo, int nx, int ny, int ox, int oy, int sc, rect *devdata, int lvlcnt, int c) {
  //TI_TN;
  int i;
  int x;
  int y;
  for( i = 0; i<lvlcnt; i++) {
    rect r = devdata[i];
    int x0 = (r.x / sc - ox);
    int y0 = (r.y / sc - oy);
    int x1 = x0 + r.w / sc + 1;
    int y1 = y0 + r.h / sc + 1;
    /*fprintf(stderr,"nx=%d, ny=%d\n",nx,ny);
    fprintf(stderr,"Read rectangle (%d,%d) to (%d,%d)\n",x0,y0,x1,y1);*/
    int p = (x1 < 0) || (x0 >= nx) || (y1 < 0) || (y0 >= ny);
    if (!p) {
      for( x = max(0,x0) ; x<min(nx-1,x1); x++) {
	for( y = max(0,y0); y<min(ny-1,y1); y++) {
	  imgpbo[nx*y + x] = c;
	}
      }
    }
    /*else { fprintf(stderr,"Error: Rectangle (%d, %d) to (%d,%d) out of bounds!\n",x0,y0,x1,y1); exit(0); }*/
  }
}

void write_mask_file_slave(unsigned char* wmsk, char* fname, int imx, int imy)
{
  ucharac4 *pngmask;
  pngmask = (ucharac4*)malloc(imx * imy * sizeof(ucharac4));
  int ii, jj, mskval;
  for(jj=0; jj<imy; jj++) {
      for(ii=0; ii<imx; ii++) {
           mskval = (int)wmsk[imx*jj + ii]/255;
	   pngmask[imx*jj + ii].g = 0*mskval;
	   pngmask[imx*jj + ii].r = 255*mskval;
	   pngmask[imx*jj + ii].b = 255*mskval;
	   pngmask[imx*jj + ii].a = 255;
      }
  }
  write_png_file(fname, imx, imy, pngmask);
  free(pngmask);
  /* End mask writing section */
}

void myBlurTile(unsigned char *tile, int tnx, int tny, int w, unsigned char *out) {
  int i, j, a, b;
  int den = 2*w + 1;
  int den2 = den * den;
  for (i=w; i<tnx-w; i++) {
    for(j=w; j<tny-w; j++) {
      int sum = 0;
      for(a=-w; a<=w; a++) {
	for(b=-w; b<=w; b++) {
	  sum += tile[(j+b)*tnx+(i+a)];
	}
      }
      out[j*tnx+i] = sum / den2;
    }
  }
}

int knlvl;
agsKernel **krns;

float *d_scale;
float *d_kernels;

int nk, nkx, nky;

int imx = 2048;
int imy = 2048;
int imxy = 2048 * 2048;

unsigned char *bmsk, *bimg;

fftwf_complex *msk, *mskp, *mskpf, *mskf;
fftwf_complex *km, *kmp, *imgpk, *imgk;
float *img;

fftwf_plan fwdplan;
fftwf_plan bwdplan;

float *d_work;
unsigned char  *d_tile;
/* unsigned char *d_bmsk; */

void myClearByte(unsigned char *msk) {
  int i;
  for(i=0; i<imxy; i++) {
    msk[i] = 0.0;
  }
}

void myClearFloat(float *msk) {
  int i;
  for(i=0; i<imxy; i++) {
    msk[i] = 0.0;
  }
}
void myClearComplex(fftwf_complex *msk) {
  int i;
  for(i=0; i<imxy; i++) {
    msk[i][0] = 0.0;
    msk[i][1] = 0.0;
  }
}
void myMaskFloat(unsigned char *bmsk, fftwf_complex *msk, float dose) {
  int i, j;
  for(i=0; i<imxy; i++) {
    msk[i][0] = (((float)bmsk[i]) / 255.0 * MASK_REAL + BACKGROUND_REAL)*dose;   /* Enable these lines for Attenuated PSM */
    msk[i][1] = (((float)bmsk[i]) / 255.0 * MASK_IMAG + BACKGROUND_IMAG)*dose;
    //msk[i][0] = ((float)bmsk[i]) / 255.0 * dose;    /* Enable these lines for binary masks */
    //msk[i][1] = 0.0;
  }
}

void myShift(fftwf_complex *in, fftwf_complex *out) {
  int i,j;
  int x, y;
  int imxh = imx/2;
  int imyh = imy/2;
  for (i=0; i<imx; i++) {
    for(j=0; j<imy; j++) {
      if(i<imxh) {
	x = i+imxh;
      } else {
	x = i-imxh;
      }
      if(j<imyh) {
	y = j+imyh;
      } else {
	y = j-imyh;
      }
      out[y*imx+x][0] = in[j*imx+i][0];
      out[y*imx+x][1] = in[j*imx+i][1];
    }
  }
}
void mySumImg(fftwf_complex *p, float *s, float scale) {
  int i;
  for(i=0; i<imxy; i++) {
    float pr = p[i][0];
    float pi = p[i][1];
    s[i] += scale * (pr*pr + pi*pi);
    /*if(i%100000==0) fprintf(stderr,"sum img %d scale %f real %f imag %f is %f\n", i, scale, pr, pi, s[i]);*/
  }
}
/* int pi = 0; */
/* #define P0 fprintf(stderr,"pi @ %d %d %d\n", pi=0, i, j); */
/* #define PI fprintf(stderr,"pi @ %d %d %d\n", pi++, i, j); */
#define P0
#define PI
void myKernelMult(int knx, int kny, float *krn, fftwf_complex *mskf, fftwf_complex *km ) {
  int i, j, k;
  k=0;
  int imxh = imx/2;
  int imyh = imy/2;
  int xoff = imxh - knx/2 ;
  int yoff = imyh - kny/2 ;

  for(i=0; i<knx; i++) {
    for(j=0; j<kny; j++) {
      P0 float kr = krn[k++];
      PI float ki = krn[k++];
      //fprintf(stderr, "krn(%d,%d) = %f+%fJ\n", i,j,kr,ki);
      PI int ind = (yoff+j)*imx+(xoff+i);
      PI float mr = mskf[ind][0];
      PI float mi = mskf[ind][1];
      PI km[ind][0] = (mr*kr-mi*ki);
      PI km[ind][1] = (mi*kr+mr*ki);
      //fprintf(stderr, "km(%d,%d) = %f+%fJ\n", i,j,km[ind][0], km[ind][1]);
    }
  }
}
#define myMin(a,b) ((a)<=(b) ? (a) : (b))
#define myMax(a,b) ((a)<=(b) ? (b) : (a))

void myThreshhold(float *img, unsigned char *bimg) {
  int i,j;

#if 1
  //  for (i=0; i<imxy; i++) bimg[i] = img[i] * 255;
  int cnt = 0;
  for(i=0; i<imx; i++) {
    for(j=0; j<imy; j++) {
      if ( img[j*imx+i] >= TARGET_INTENSITY)  {
	bimg[(j)*imx+(i)] =  255;
	cnt++;
      } else {
	bimg[(j)*imx+( i)] =  0;
      }
    }
  }
  fprintf(stderr, "(%d)\n", cnt	 );
#else
  float mn =  1.0e20;
  float mx = -1.0e20;
  for(i=0; i<imxy; i++) {
    float v = img[i];
    //if(i%100000==0) fprintf(stderr,"threshhold %d is %f (%f,%f)\n", i, v, mn, mx);
    mn = myMin(mn,v);
    mx = myMax(mx,v);
  }
  fprintf(stderr, "image min/maxy = %f/%f\n", mn, mx);
  float sc = mx - mn;
  for(i=0; i<imx; i++) {
    for(j=0; j<imy; j++) {
      float v = (img[j*imx+i]-mn)/sc;
      bimg[((imy-1)-j)*imx+((imx-1)-i)] = 255.0 * v;
    }
  }
#endif
}

void myVmul(int n, float *v, float sc) {
  int i;
  for(i=0; i<n; i++) v[i] *= sc;
}

void read_Efield(int x, int y, fftwf_complex* E)
{
   int index = y*imx + x;
   (*E)[0] = imgk[index][0];
   (*E)[1] = imgk[index][1];
}

void read_Eslope(int x, int y, int dfom, int is_horiz_frag, fftwf_complex* E)
{
   int index1 = y*imx + x;
   int index2;
   if(is_horiz_frag)  {  index2 = (y+dfom*1)*imx + x; }
   else               {  index2 = y*imx + x + dfom*1; }
   (*E)[0] = imgk[index2][0]-imgk[index1][0];
   (*E)[1] = imgk[index2][1]-imgk[index1][1];
}

int is_in_tile(int workx,int worky,int x,int y)
{
   if(x>=workx && x<=workx+1024 && y>=worky && y<=worky+1024) return 1;
   return 0;
}

void myLithoLocalSearch(fragment** f, int shapes, int kernel_num, float scale, float* cost_function_intensity, float* cost_function_slope,
			int iteration_num, int workx, int worky, int ollx, int olly)
{
    int fragnum = 0;
    int i;
    int eval_target_move = (iteration_num>=START_RETARGET_ITER && iteration_num<STOP_RETARGET_ITER)? EVAL_TARGET_MOVEMENT[iteration_num - START_RETARGET_ITER]:0;
    int eval_mask_move = (iteration_num>=START_RETARGET_ITER && iteration_num<STOP_RETARGET_ITER)? EVAL_MASK_MOVEMENT[iteration_num - START_RETARGET_ITER]:0;
    char lut_int_vert_file[MAX_FILE_NAME_LENGTH];
    char lut_int_horiz_file[MAX_FILE_NAME_LENGTH];
    char lut_slope_vert_file[MAX_FILE_NAME_LENGTH];
    char lut_slope_horiz_file[MAX_FILE_NAME_LENGTH];
    sprintf(lut_int_vert_file,"Lookup_Tables/delta_E%d_delta_m_vert.txt",kernel_num);
    sprintf(lut_int_horiz_file,"Lookup_Tables/delta_E%d_delta_m_horiz.txt",kernel_num);
    sprintf(lut_slope_vert_file,"Lookup_Tables/delta_Eslope%d_delta_m_vert.txt",kernel_num);
    sprintf(lut_slope_horiz_file,"Lookup_Tables/delta_Eslope%d_delta_m_horiz.txt",kernel_num);
    int size_lut_int_vert = read_num_entries(lut_int_vert_file);
    int size_lut_int_horiz = read_num_entries(lut_int_horiz_file);
    int size_lut_slope_vert = read_num_entries(lut_slope_vert_file);
    int size_lut_slope_horiz = read_num_entries(lut_slope_horiz_file);
    lut* vertical_intensity_lut = (lut*)malloc(size_lut_int_vert*sizeof(lut));
    lut* horizontal_intensity_lut = (lut*)malloc(size_lut_int_horiz*sizeof(lut));
    lut* vertical_slope_lut = (lut*)malloc(size_lut_slope_vert*sizeof(lut));
    lut* horizontal_slope_lut = (lut*)malloc(size_lut_slope_horiz*sizeof(lut));
    lut* intensity_lut;
    lut* slope_lut;
    int size_int_lut, size_slope_lut;
    lut_load(vertical_intensity_lut, lut_int_vert_file);
    lut_load(horizontal_intensity_lut, lut_int_horiz_file);
    lut_load(vertical_slope_lut, lut_slope_vert_file);
    lut_load(horizontal_slope_lut, lut_slope_horiz_file);

    /* Check fragment midpoints and evaluate local cost function */
    for(i=0; i<shapes; i++) {
       fragment* head = f[i];
       while(head!=NULL) {
           int x,y;
	   int md,nh,nl;
	   fftwf_complex H0, H1, H2, H3, H4, H5, H6, H7, H8;
	   fftwf_complex Hslope0, Hslope1, Hslope2, Hslope3, Hslope4, Hslope5, Hslope6, Hslope7, Hslope8;
	   fftwf_complex E0, E1, E2;
	   fftwf_complex Eslope0, Eslope1, Eslope2;
	   get_fragment_midpoint(head,&x,&y); //fprintf(stderr,"Fragment midpoint at %d %d\nOrigin at %d %d\n", x,y,ollx,olly);
	   x += 512-ollx; y += 512-olly;
	   int tempwx = workx+512; int tempwy = worky + 512;
	   if(!is_in_tile(tempwx,tempwy,x,y)) { head = head->next; fragnum++; continue; }
	   md = head->movement;
	   nl = get_fragment_length(head);
	   nh = (int)nl/2;
	   x-=workx; y-=worky;
	   read_Efield(x,y, &E1); 	   //fprintf(stderr,"Getting EField at %d %d\n",x,y); exit(0);
	   read_Eslope(x,y,head->dfom,is_horizontal_fragment(head), &Eslope1);
	   if(is_horizontal_fragment(head)) {
	      intensity_lut = horizontal_intensity_lut; size_int_lut = size_lut_int_horiz;
	      slope_lut = horizontal_slope_lut; size_slope_lut = size_lut_slope_horiz;
	      read_Efield(x,y-eval_target_move*head->dfom, &E0);
	      read_Efield(x,y+eval_target_move*head->dfom, &E2);
	      read_Eslope(x,y-eval_target_move*head->dfom,head->dfom,1, &Eslope0);
	      read_Eslope(x,y+eval_target_move*head->dfom,head->dfom,1, &Eslope2);
	   }
	   else  {
	      intensity_lut = vertical_intensity_lut; size_int_lut = size_lut_int_vert;
	      slope_lut = vertical_slope_lut; size_slope_lut = size_lut_slope_vert;
	      read_Efield(x-eval_target_move*head->dfom,y, &E0);
	      read_Efield(x+eval_target_move*head->dfom,y, &E2);
	      read_Eslope(x-eval_target_move*head->dfom,y,head->dfom,0, &Eslope0);
	      read_Eslope(x+eval_target_move*head->dfom,y,head->dfom,0, &Eslope2);
	   }
	   read_lookup_table(intensity_lut,size_int_lut,md+eval_target_move,nh,nl,-eval_mask_move, &H0);
	   read_lookup_table(intensity_lut,size_int_lut,md+eval_target_move,nh,nl,0, &H1);
	   read_lookup_table(intensity_lut,size_int_lut,md+eval_target_move,nh,nl,eval_mask_move, &H2);
	   read_lookup_table(intensity_lut,size_int_lut,md,nh,nl,-eval_mask_move, &H3);
	   read_lookup_table(intensity_lut,size_int_lut,md,nh,nl,0, &H4);
	   read_lookup_table(intensity_lut,size_int_lut,md,nh,nl,eval_mask_move, &H5);
	   read_lookup_table(intensity_lut,size_int_lut,md-eval_target_move,nh,nl,-eval_mask_move, &H6);
	   read_lookup_table(intensity_lut,size_int_lut,md-eval_target_move,nh,nl,0, &H7);
	   read_lookup_table(intensity_lut,size_int_lut,md-eval_target_move,nh,nl,eval_mask_move, &H8);
	   read_lookup_table(slope_lut,size_slope_lut,md+eval_target_move,nh,nl,-eval_mask_move, &Hslope0);
	   read_lookup_table(slope_lut,size_slope_lut,md+eval_target_move,nh,nl,0, &Hslope1);
	   read_lookup_table(slope_lut,size_slope_lut,md+eval_target_move,nh,nl,eval_mask_move, &Hslope2);
	   read_lookup_table(slope_lut,size_slope_lut,md,nh,nl,-eval_mask_move, &Hslope3);
	   read_lookup_table(slope_lut,size_slope_lut,md,nh,nl,0, &Hslope4);
	   read_lookup_table(slope_lut,size_slope_lut,md,nh,nl,eval_mask_move, &Hslope5);
	   read_lookup_table(slope_lut,size_slope_lut,md-eval_target_move,nh,nl,-eval_mask_move, &Hslope6);
	   read_lookup_table(slope_lut,size_slope_lut,md-eval_target_move,nh,nl,0, &Hslope7);
	   read_lookup_table(slope_lut,size_slope_lut,md-eval_target_move,nh,nl,eval_mask_move, &Hslope8);
	   cost_function_intensity[fragnum*NUM_EVALS_PER_SITE] += scale*(E0[0]*E0[0]+E0[1]*E0[1] + 2*(H0[0]*E0[0]+H0[1]*E0[1]));
	   cost_function_intensity[fragnum*NUM_EVALS_PER_SITE+1] += scale*(E0[0]*E0[0]+E0[1]*E0[1] + 2*(H1[0]*E0[0]+H1[1]*E0[1]));
	   cost_function_intensity[fragnum*NUM_EVALS_PER_SITE+2] += scale*(E0[0]*E0[0]+E0[1]*E0[1] + 2*(H2[0]*E0[0]+H2[1]*E0[1]));
	   cost_function_intensity[fragnum*NUM_EVALS_PER_SITE+3] += scale*(E1[0]*E1[0]+E1[1]*E1[1] + 2*(H3[0]*E1[0]+H3[1]*E1[1]));
	   cost_function_intensity[fragnum*NUM_EVALS_PER_SITE+4] += scale*(E1[0]*E1[0]+E1[1]*E1[1] + 2*(H4[0]*E1[0]+H4[1]*E1[1]));
	   cost_function_intensity[fragnum*NUM_EVALS_PER_SITE+5] += scale*(E1[0]*E1[0]+E1[1]*E1[1] + 2*(H5[0]*E1[0]+H5[1]*E1[1]));
	   cost_function_intensity[fragnum*NUM_EVALS_PER_SITE+6] += scale*(E2[0]*E2[0]+E2[1]*E2[1] + 2*(H6[0]*E2[0]+H6[1]*E2[1]));
	   cost_function_intensity[fragnum*NUM_EVALS_PER_SITE+7] += scale*(E2[0]*E2[0]+E2[1]*E2[1] + 2*(H7[0]*E2[0]+H7[1]*E2[1]));
	   cost_function_intensity[fragnum*NUM_EVALS_PER_SITE+8] += scale*(E2[0]*E2[0]+E2[1]*E2[1] + 2*(H8[0]*E2[0]+H8[1]*E2[1]));
	   cost_function_slope[fragnum*NUM_EVALS_PER_SITE] += (2*scale*(E0[0]*Eslope0[0]+E0[1]*Eslope0[1]))+(2*scale*(Hslope0[0]*E0[0]+Hslope0[1]*E0[1]+H0[0]*Eslope0[0]+H0[1]*Eslope0[1]));
	   cost_function_slope[fragnum*NUM_EVALS_PER_SITE+1] += (2*scale*(E0[0]*Eslope0[0]+E0[1]*Eslope0[1]))+(2*scale*(Hslope1[0]*E0[0]+Hslope1[1]*E0[1]+H1[0]*Eslope0[0]+H1[1]*Eslope0[1]));
	   cost_function_slope[fragnum*NUM_EVALS_PER_SITE+2] += (2*scale*(E0[0]*Eslope0[0]+E0[1]*Eslope0[1]))+(2*scale*(Hslope2[0]*E0[0]+Hslope2[1]*E0[1]+H2[0]*Eslope0[0]+H2[1]*Eslope0[1]));
	   cost_function_slope[fragnum*NUM_EVALS_PER_SITE+3] += (2*scale*(E1[0]*Eslope1[0]+E1[1]*Eslope1[1]))+(2*scale*(Hslope3[0]*E1[0]+Hslope3[1]*E1[1]+H3[0]*Eslope1[0]+H3[1]*Eslope1[1]));
	   cost_function_slope[fragnum*NUM_EVALS_PER_SITE+4] += (2*scale*(E1[0]*Eslope1[0]+E1[1]*Eslope1[1]))+(2*scale*(Hslope4[0]*E1[0]+Hslope4[1]*E1[1]+H4[0]*Eslope1[0]+H4[1]*Eslope1[1]));
	   cost_function_slope[fragnum*NUM_EVALS_PER_SITE+5] += (2*scale*(E1[0]*Eslope1[0]+E1[1]*Eslope1[1]))+(2*scale*(Hslope5[0]*E1[0]+Hslope5[1]*E1[1]+H5[0]*Eslope1[0]+H5[1]*Eslope1[1]));
	   cost_function_slope[fragnum*NUM_EVALS_PER_SITE+6] += (2*scale*(E2[0]*Eslope2[0]+E2[1]*Eslope2[1]))+(2*scale*(Hslope6[0]*E2[0]+Hslope6[1]*E2[1]+H6[0]*Eslope2[0]+H6[1]*Eslope2[1]));
	   cost_function_slope[fragnum*NUM_EVALS_PER_SITE+7] += (2*scale*(E2[0]*Eslope2[0]+E2[1]*Eslope2[1]))+(2*scale*(Hslope7[0]*E2[0]+Hslope7[1]*E2[1]+H7[0]*Eslope2[0]+H7[1]*Eslope2[1]));
	   cost_function_slope[fragnum*NUM_EVALS_PER_SITE+8] += (2*scale*(E2[0]*Eslope2[0]+E2[1]*Eslope2[1]))+(2*scale*(Hslope8[0]*E2[0]+Hslope8[1]*E2[1]+H8[0]*Eslope2[0]+H8[1]*Eslope2[1]));
	   head = head->next;
	   fragnum++;
        }
    }
    free(vertical_intensity_lut); free(horizontal_intensity_lut); free(vertical_slope_lut); free(horizontal_slope_lut);
}


#define LOOK(s,p) fprintf(stderr, "%s = %e %e %e %e %e\n", s, ((float*)p)[0],((float*)p)[1],((float*)p)[2],((float*)p)[3],((float*)p)[4])
#define DUMPC(fn,m) fprintf(stderr, "dumpc %s\n", fn); agsWriteFloatsHst(fn, (float*)(m), 2 * imxy)
#define DUMPF(fn,m) fprintf(stderr, "dumpf %s\n", fn);agsWriteFloatsHst(fn, (float*)(m), imxy)
#define DUMPN(fn,m,n) fprintf(stderr, "dumpn %s\n", fn);agsWriteFloatsHst(fn, (float*)(m), n)
void myComputeImage(agsKernel *krn, fragment** f, float* cost_function, int shapes, int ollx, int olly,
			int workx, int worky, int iteration_num, float dose, int flag_retarget_yes) {
  int i,k;
  int nk = krn->nk;
  int numfrags = get_total_frags(f,shapes);
  float* cost_function_intensity = (float*)malloc(NUM_EVALS_PER_SITE*numfrags*sizeof(float));
  float* cost_function_slope = (float*)malloc(NUM_EVALS_PER_SITE*numfrags*sizeof(float));
  for(i=0; i<(NUM_EVALS_PER_SITE*numfrags); i++)   cost_function_slope[i] = 0;   /* Initialization */
  for(i=0; i<(NUM_EVALS_PER_SITE*numfrags); i++)   cost_function_intensity[i] = 0;   /* Initialization */

  myMaskFloat(bmsk, msk, dose);

  myShift(msk, mskp);

  fftwf_execute(fwdplan);

  myVmul(2*imxy, (float*)mskpf, 1.0 / ((float)imxy));

  myShift(mskpf, mskf);
  myClearFloat(img);
  myClearComplex(km);

  for(k=0; k<nk; k++) {

    fprintf(stderr, "Now convolving with kernel %d weight %f\n", k, krn->scale[k]);

    myKernelMult(krn->nkx, krn->nky, krn->krn + k*krn->nkx*krn->nky, mskf, km);

    myShift(km, kmp);

    fftwf_execute(bwdplan);

    myShift(imgpk, imgk);

    /* Local search during retargeting */
    if(iteration_num>=START_RETARGET_ITER && iteration_num<=STOP_RETARGET_ITER) {
       if(flag_retarget_yes)
          myLithoLocalSearch(f, shapes, k, krn->scale[k], cost_function_intensity, cost_function_slope, iteration_num, workx, worky, ollx, olly);
    }
    /* End added functions for retargeting */

    mySumImg(imgk, img, krn->scale[k]);

  }

  for(i=0; i<(NUM_EVALS_PER_SITE*numfrags); i++) {
      if(cost_function_slope[i]==0) { cost_function[i] += 0; }
      else {
      //cost_function[i] += WEIGHT_INTENSITY*1000*fabs(TARGET_INTENSITY - cost_function_intensity[i]) + (1-WEIGHT_INTENSITY)*cost_function_intensity[i]/fabs(cost_function_slope[i]);
      //cost_function[i] = cost_function_slope[i]/cost_function_intensity[i];   /* Move on basis of normalized slope */
      cost_function[i] = cost_function_slope[i];	/* Move only on basis of slope */
      }
   }

  /* Debug section */
  FILE* fdebug = fopen("dump/dump_cost_function.dat","w");
  int costcnt = 0;
  for(i=0; i<shapes; i++) {
      fragment* head = f[i];
      while(head!=NULL) {
          fprintf(fdebug,"Fragment (%d,%d)-(%d,%d) ",(head->startx-ollx),(head->starty-olly),(head->endx-ollx),(head->endy-olly));
	  int ii;
	  for(ii=0; ii<NUM_EVALS_PER_SITE; ii++) fprintf(fdebug,"CF%d: %f ",ii,cost_function[costcnt++]);
	  fprintf(fdebug,"\n");
          head = head->next;
      }
  } fclose(fdebug);
  /* End Debug section */

  myThreshhold(img, bimg);
  return;
}
int gpuid  = -1;
int hostid = -1;

int main(int argc, char	 **argv) {
  int i,j,k;
  gpuid = atoi(argv[1]);
  hostid = atoi(argv[2]);

  agsInitSlave(argc, argv);
  int my = agsMyId();

  fprintf(stderr, "Slave %d: Started client using gpu %d\n", my, gpuid);

  fprintf(stderr,"Slave %d: my hostid is %d\n", my, hostid);

  //char *w1 = fftwfg_export_wisdom_to_string();
  fftwf_import_wisdom_from_string(fftwf_wisdom[hostid]);
  //char *w2 = fftwf_export_wisdom_to_string();
  //fprintf(stderr, "fft wisdom before:\n%s\n and after:\n%s\n\n", w1, w2);

  fprintf(stderr, "Size of fftw_complex = %d\n", sizeof(fftwf_complex));

  bmsk	= malloc(2048 * 2048);
  bimg	= malloc(2048 * 2048);

  msk	= fftwf_malloc(2048 * 2048 * sizeof(fftwf_complex));
  mskp	= fftwf_malloc(2048 * 2048 * sizeof(fftwf_complex));
  mskpf = fftwf_malloc(2048 * 2048 * sizeof(fftwf_complex));
  mskf	= fftwf_malloc(2048 * 2048 * sizeof(fftwf_complex));

  km	= fftwf_malloc(2048 * 2048 * sizeof(fftwf_complex));
  kmp	= fftwf_malloc(2048 * 2048 * sizeof(fftwf_complex));
  imgpk = fftwf_malloc(2048 * 2048 * sizeof(fftwf_complex));
  imgk	= fftwf_malloc(2048 * 2048 * sizeof(fftwf_complex));

  img	= malloc(2048 * 2048 * sizeof(float));

  memset(msk, 0, 2048 * 2048 * sizeof(fftwf_complex));
  memset(mskp, 0, 2048 * 2048 * sizeof(fftwf_complex));
  memset(mskpf, 0, 2048 * 2048 * sizeof(fftwf_complex));
  memset(mskf, 0, 2048 * 2048 * sizeof(fftwf_complex));

  memset(km, 0, 2048 * 2048 * sizeof(fftwf_complex));
  memset(kmp, 0, 2048 * 2048 * sizeof(fftwf_complex));
  memset(imgpk, 0, 2048 * 2048 * sizeof(fftwf_complex));
  memset(imgk, 0, 2048 * 2048 * sizeof(fftwf_complex));


  fwdplan = fftwf_plan_dft_2d(2048, 2048, mskp, mskpf, -1, FFTW_WISDOM_ONLY);
  bwdplan = fftwf_plan_dft_2d(2048, 2048, kmp, imgpk,	1, FFTW_WISDOM_ONLY);
  fprintf(stderr, "FFTW plans %d and %d\n", fwdplan, bwdplan);
  double sp_add, sp_mul, sp_fma;
  //fftwf_flops(fwdplan, &sp_add, &sp_mul, &sp_fma);
  //printf(stderr, "fwd fft plan flop count add = %d, mul = %d, fma = %d\n", sp_add, sp_mul, sp_fma);
  //fftwf_fprint_plan(fwdplan, stderr);
  //fprintf(stderr, "\n");
  //fftwf_fprint_plan(bwdplan, stderr);
  //fprintf(stderr, "\n");

  if (0 <= gpuid) {
    agsSetDev(gpuid);
  }



  int cmd;

  int workx[10000];
  int worky[10000];
  int workcnt = 0;

  int lyr;
  int tnx;
  int tny;
  int sc;

  int size;
  int scsize;

  int cv;

  int num_target_shapes;
  int iteration_num;
  fragment* frags[MAX_M1_SHAPES];
  fragment* head;
  fragment* cur;
  float* cost_function;
  int num_total_frags;
  int ollx, olly;
  float dose;
  int flag_retarget;

  unsigned char *tile = NULL;
  unsigned char *tiletemp = NULL;

  int done = 0;
  while (!done) {
    fprintf(stderr, "Slave %d: waiting on command\n", my);
    agsRecv(0, &cmd, sizeof(int));
    switch (cmd) {
    case CMD_DESIGN_LOAD:
      agsRecv(0,&nlvl,sizeof(int));
      for(j=0; j<nlvl; j++) {
	agsRecv(0, lvlcnt+j, sizeof(int));
	lvldata[j] = malloc(lvlcnt[j] * sizeof(rect));
	agsRecv(0, lvldata[j], lvlcnt[j] * sizeof(rect));
	fprintf(stderr, "Slave %d: Recvd level %d data, count = %d\n", my, j, lvlcnt[j]);
      }
      if(0 <= gpuid) {
        fprintf(stderr,"Shouldn't be in GPU Mode!\n"); exit(0);
	for(j=0; j<nlvl; j++) {
	  d_lvldata[j] = agsAlloc("lvldata", lvlcnt[j] * sizeof(rect));
	  agsPut(lvldata[j], d_lvldata[j], lvlcnt[j] * sizeof(rect));
	}
	fprintf(stderr, "Slave %d: Device level %d data, count = %d\n", my, j, lvlcnt[j]);
      }
      break;

    case CMD_KERNEL_LOAD:
      agsRecv(0, &knlvl, sizeof(int));
      krns = malloc(knlvl * sizeof(agsKernel *));
      for(i=0; i<knlvl; i++) {
	krns[i] = malloc(sizeof(agsKernel));
	agsRecv(0, krns[i], sizeof(agsKernel));
	nk = krns[i]->nk;
	nkx = krns[i]->nkx;
	nky = krns[i]->nky;
	krns[i]->scale	  = malloc(krns[i]->nk *				   sizeof(float));
	krns[i]->krn	  = malloc(krns[i]->nk * krns[i]->nkx * krns[i]->nky * 2 * sizeof(float));
	agsRecv(0, krns[i]->scale, krns[i]->nk *				   sizeof(float));
	agsRecv(0, krns[i]->krn,   krns[i]->nk * krns[i]->nkx * krns[i]->nky * 2 * sizeof(float));
      }
      fprintf(stderr, "Slave %d: received kernel data\n", my);
      if(0 <= gpuid) {
	d_scale = agsAlloc("scales", knlvl * nk * sizeof(float));
	d_kernels = agsAlloc("kernels", knlvl * nk * nkx * nky * 2 * sizeof(float));
	for(i=0; i<knlvl; i++) {
	  agsPut(krns[i]->scale, d_scale + i * nk , nk * sizeof(float));
	  agsPut(krns[i]->krn, d_kernels + i * nk * 2 * nkx * nky, nk * nkx * nky * 2 * sizeof(float));
	}
      }
      break;

    case CMD_PROCESS_TILE:
      agsRecv(0,&lyr, sizeof(int));
      agsRecv(0,&tnx, sizeof(int));
      agsRecv(0,&tny, sizeof(int));
      agsRecv(0,&sc,  sizeof(int));
      agsRecv(0,&cv, sizeof(int));
      agsRecv(0,&ollx, sizeof(int));
      agsRecv(0,&olly, sizeof(int));
      agsRecv(0,&dose, sizeof(float));
      agsRecv(0,&flag_retarget, sizeof(int));

      fprintf(stderr, "Slave: got cmd process tile\n");

      size = tnx * tny;
      scsize = size / (sc * sc);

      if (tile == NULL) {
	tile = malloc(size);
	tiletemp = malloc(size);
	if (0 <= gpuid) {
	  d_work  = agsAlloc("work", 4 * imx * imy * 2 * sizeof(float));
	}
      }

      fprintf(stderr, "Slave %d: --- processing window (%d, %d), on layer %d at scale %d(cmd=%d)\n", my, tnx, tny, lyr, sc, cmd);
      workcnt = 0;
      cmd=CMD_PROCESS_TILE;
      while (cmd==CMD_PROCESS_TILE) {
	agsRecv(0, &cmd, sizeof(int));
	fprintf(stderr, "Slave %d: workload cmd = %d\n", my, cmd);
	if (cmd==CMD_PROCESS_TILE) {
	  agsRecv(0, workx+workcnt, sizeof(int));
	  agsRecv(0, worky+workcnt, sizeof(int));
	  fprintf(stderr, "Slave %d: received work at %d %d (workload = %d)\n", my, workx[workcnt], worky[workcnt], workcnt);
	  workcnt++;
	}
      }
      fprintf(stderr, "Slave %d: Working on %d tiles\n", my, workcnt);
      for(i=0; i<workcnt; i++) {
	for (j=0; j<size; j++) tile[j] = 0;
	int wx = workx[i];
	int wy = worky[i];

	if (gpuid==-1) {
#if 0
	  // this is fake imaging
	  myDevLayerDraw(tile, tnx, tny, workx[i], worky[i], 1, lvldata[lyr], lvlcnt[lyr], 255);
	  myBlurTile(tile, tnx, tny, cv, tiletemp);
	  mySubSample(tiletemp, tnx, tny, sc, tile);
#else
	  // this is real assuming 1024x1024 tile is being asked for (i know! i know! i'll fix it later)
	  myClearByte(bmsk);
	  myDevLayerDraw(bmsk, imx, imy, workx[i]-512, worky[i]-512, 1, lvldata[lyr], lvlcnt[lyr], 255);
	  /*char maskfile[80]; sprintf(maskfile,"dump/mask_tile%d.png",i);
	  write_mask_file_slave(bmsk, maskfile, 2048, 2048);*/
	  myComputeImage(krns[lyr], frags, cost_function, num_target_shapes, ollx, olly, workx[i], worky[i], iteration_num, dose, flag_retarget);
	  mySubBlock(bimg, imx, imy, tiletemp, tnx, tny, 512, 512);
	  mySubSample(tiletemp, tnx, tny, sc, tile);
#endif
	} else {


      ///////////////////////////////////////////////////////////////////////////////////////////////////////
      ///////////////////////////////////////////////////////////////////////////////////////////////////////
      ///////////////////////////////////////////////////////////////////////////////////////////////////////

#define D_WORK(i) (d_work + i * (2 * imxy))
	  float *d_msk	= D_WORK(0);
	  float *d_img	= D_WORK(1);
	  float *d_tmp1	 = D_WORK(2);
	  float *d_tmp2	 = D_WORK(3);

	  unsigned char *d_bimg	  = (unsigned char *)D_WORK(1);
	  unsigned char *d_tileall= (unsigned char *)D_WORK(2);
	  unsigned char *d_tile	  = (unsigned char *)D_WORK(0);

	  agsZero(d_msk, imxy * 2 * sizeof(float));
	  agsDevLayerDrawComplex(d_msk, imx, imy, wx-512, wy-512, 1, d_lvldata[lyr], lvlcnt[lyr]);
	  agsComputeImage(nk, nkx, nky,
			  d_scale + lyr * nk,
			  d_kernels + lyr * nk * 2 * nkx * nky,
			  imx, imy, d_msk, d_img, d_tmp1, d_tmp2);

	  agsScaleToChar(imxy, d_img, NULL, NULL, 255, (unsigned char *)d_bimg);
	  agsBymGetBlock(d_bimg, imx, imy, d_tileall, tnx, tny, 512, 512);
	  agsSubSample(d_tileall, tnx, tny, sc, d_tile);
	  agsGet(d_tile, tile, scsize);
	}

      ///////////////////////////////////////////////////////////////////////////////////////////////////////
      ///////////////////////////////////////////////////////////////////////////////////////////////////////
      ///////////////////////////////////////////////////////////////////////////////////////////////////////

	fprintf(stderr, "Slave %d: sending back %d, %d\n", my, workx[i], worky[i]);
	agsSend(0, &my,	    sizeof(int));
	agsSend(0, workx+i, sizeof(int));
	agsSend(0, worky+i, sizeof(int));
	agsSend(0, img, 4*scsize*sizeof(float));
	agsSend(0, cost_function, num_total_frags*NUM_EVALS_PER_SITE*sizeof(float));
	fprintf(stderr, "Slave %d: sent back %d, %d with %d pixels\n", my, workx[i], worky[i], 4*scsize);
      }
      break;

    case CMD_FRAGMENT_LOAD:
       agsRecv(0, &iteration_num, sizeof(int));
       agsRecv(0, &num_target_shapes, sizeof(int));
       fprintf(stderr,"Slave: Received %d shapes\n",num_target_shapes);
       for(i=0; i<num_target_shapes; i++) {
           head = (fragment*)malloc(sizeof(fragment));
	   agsRecv(0, head, sizeof(fragment));
	   frags[i] = head;
	   while(head->next!=NULL) {
	       cur = (fragment*)malloc(sizeof(fragment));
	       agsRecv(0, cur, sizeof(fragment));
	       head->next = cur; head = cur;
	   }
       }
       num_total_frags = get_total_frags(frags,num_target_shapes);
       cost_function = (float*)malloc(NUM_EVALS_PER_SITE*num_total_frags*sizeof(float));
       for(i=0; i<NUM_EVALS_PER_SITE*num_total_frags; i++) cost_function[i] = 0;
       break;

    case CMD_CLEAN:
      for(i=0; i<knlvl; i++)  free(krns[i]);
      for(j=0; j<nlvl; j++)  { free(lvldata[j]); lvlcnt[j]=0; }
      fprintf(stderr,"Slave: Removed kernel and design data\n");
      break;

    case CMD_QUIT:
      done = 1;
      break;

    default:
      break;
    }
  }
  fprintf(stderr, "Slave %d: %d exiting\n", my, gpuid);
  exit(0);
}

