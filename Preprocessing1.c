#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include "kiss_fft.h"
#include "kiss_fftnd.h"
#include "kernels.h"
#include "layoutrw.h"
int MAX_FILE_NAME_LENGTH = 80;
//float BACKGROUND_REAL = 0.0229;
//float BACKGROUND_IMAG = -0.0217;
float BACKGROUND_REAL = 0.0;
float BACKGROUND_IMAG = 0.0;
float MASK_REAL = 0.9771;
float MASK_IMAG = 0.0217;

float TARGET_INTENSITY = 0.225;
float MAX_DOSE = 1.02;
float MIN_DOSE = 0.98;
char INFILE[80]; //INPUT GLP FILE with specific step
char OUTFILE[80]; // OUTPUT LOOK UP TABLE
float *realPart,*imgPart;
float intensity;

//rect *lvldata[MAXLVL];
rect *d_lvldata[MAXLVL];

#include "write_png.h"

#define myMin(a,b) ((a)<=(b) ? (a) : (b))
#define myMax(a,b) ((a)<=(b) ? (b) : (a))

/*char *fftwf_wisdom[2] = {
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

//will it work for origin 10,10
/////////////////////////////////////
/*
void findIntensity(){
  int count=0;
  for(int i=0; i<2048; i++) {
    for(int j=0; j<2048; j++) {
      printf("intensity.real =%f\n ", realPart[j*2048+i]);
      count++;
    }
    printf("count %d",count);
  }
}
*/

void mySubBlock(unsigned char *bmsk, int nx, int ny, unsigned char *tiletemp, int tnx, int tny, int x, int y) {
  int i,  j;
  for(i=0; i<tnx; i++) {
    for(j=0; j<tny; j++) {
      if((y+j)<tny && (x+i)<tnx) {	tiletemp[j*tnx+i] = bmsk[(y+j)*ny+(x+i)]; }
      else { tiletemp[j*tnx+i] = 0; }
    }
  }
}

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
//the following creates a binary mask
int* myDevLayerDraw(unsigned char *imgpbo, int nx, int ny, int ox, int oy, int sc, rect r, int lvlcnt, int c) {
  //TI_TN;
    int i;
    int x;
    int y;
    static int key[2];
  	//lvlcnt is number of rectange and in devdata the rectangles are stored
    printf("lvlcnt = %d\n",lvlcnt);
    int count=0;
    int x0 = (r.x / sc - ox);
    int y0 = (r.y / sc - oy);
    
    int x1 = x0 + r.w / sc ;
    int y1 = y0 + r.h / sc ;
    key[0]=r.w;
    key[1]=r.h;
    printf("----------------------------------------------");
    fprintf(stderr,"nx=%d, ny=%d\n",nx,ny);
    fprintf(stderr,"Read rectangle (%d,%d) to (%d,%d)\n",x0,y0,x1,y1);
    printf("----------------------------------------------");
    int p = (x1 < 0) || (x0 >= nx) || (y1 < 0) || (y0 >= ny); // condition where the  rectangle will fail
    if (!p) { // so if not fail, means if it is a rectangle, p=1; if not p=0
        for( x = myMax(0,x0) ; x<myMin(nx-1,x1); x++) {//from x0 to x1
          for( y = myMax(0,y0); y<myMin(ny-1,y1); y++) {//from y0 to y1
            count=count+1;
            imgpbo[nx*y + x] = c;
          }
        }
      printf("\nnumber of pixel =%d\n",count);
      return key;
    }
    else { 
      printf("Rectangle is out of bound");
      return 0;
      }
  /*else { fprintf(stderr,"Error: Rectangle (%d, %d) to (%d,%d) out of bounds!\n",x0,y0,x1,y1); exit(0); }*/
}

int knlvl = 1;	// Only one simulation layer for now
agsKernel **krns;

float *d_scale;
float *d_kernels;

int nk, nkx, nky; //24,35,35

int imx = 2048;
int imy = 2048;
int imxy = 2048 * 2048;

unsigned char *bmsk, *bimg,*bmskout,*bmskout1;

kiss_fft_cpx *msk, *mskp, *mskpf, *mskf;
kiss_fft_cpx *km, *kmp, *imgpk, *imgkin,*imgkout,*imgkout1,*firsttile; 
float *img;
kiss_fft_cpx *midpointIntensity;


kiss_fftnd_cfg fwdplan;
kiss_fftnd_cfg bwdplan;

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
void myClearComplex(kiss_fft_cpx *msk) {
  int i;
  for(i=0; i<imxy; i++) {
    msk[i].r = 0.0;
    msk[i].i = 0.0;
  }
}
void myMaskFloat(unsigned char *bmsk, kiss_fft_cpx *msk, float dose,int type) {
  int i, j;
  if (type){
    for(i=0; i<imxy; i++) {// 2048*2048
      msk[i].r = (((float)bmsk[i]) / 255.0 * MASK_REAL + BACKGROUND_REAL)*dose;   /* Enable these lines for Attenuated PSM */
      msk[i].i = (((float)bmsk[i]) / 255.0 * MASK_IMAG + BACKGROUND_IMAG)*dose;
    }
  }
  else{
    for(i=0; i<imxy; i++) {// 2048*2048
      msk[i].r = ((float)bmsk[i]) / 255.0 * dose;    /* Enable these lines for binary masks */
      msk[i].i = 0.0;
    }
  }

}




void myShift(kiss_fft_cpx *in, kiss_fft_cpx *out) { // mirror image is being generated here
  int i,j;
  int x, y;
  int imxh = imx/2;
  int imyh = imy/2;
 
 
  for (i=0; i<imx; i++) { //2048
    for(j=0; j<imy; j++) { //2048
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
      out[y*imx+x].r = in[j*imx+i].r;
      out[y*imx+x].i = in[j*imx+i].i;
      
       
    }
  }
  
}
/*
void linear_shift(kiss_fft_cpx *in, kiss_fft_cpx *out, int shift,bool right,bool down){
  int i,j;
  int x, y;
  memset(out, 0, imx*imy* sizeof(kiss_fft_cpx));
    for (i=0; i<imx; i++) { //2048
     for(j=0; j<imy; j++) { //2048
         if(right==0 && down==0){x=i;y=j;}
         if(right==1 && down==0){x=shift+i;y=j;}
         if(right==0 && down==1){x=i; y=shift+j;}
         if(x<imx && y<imy){
           out[y*imx+x].r = in[j*imx+i].r;
           out[y*imx+x].i = in[j*imx+i].i;
         }
     }
    }

}
*/

/*
void mycircShift2(kiss_fft_cpx *in, kiss_fft_cpx *out, int shift,bool right,bool down) { // mirror image is being generated here
  int i,j;
  int x, y;
  int centerx = 1023, centery = 1023;
  memset(out, 0, imx*imy* sizeof(kiss_fft_cpx));

  if(right==0 && down==0){x=i;y=j;}
  if(right==1 && down==0){x=shift+i;y=j;}
  if(right==0 && down==1){x=i; y=shift+j;}
  if(x<imx && y<imy){
    out[y*imx+x].r = in[centery*imx+centerx].r;
    out[y*imx+x].i = in[centery*imx+centerx].i;
  }
  else if (x>=imx && y<imy){
    out[y*imx+x-(imx)].r = in[j*imx+i].r;
    out[y*imx+x-(imx)].i = in[j*imx+i].i;
  }
  else if (x<imx && y>=imy){
    out[(y-(imy))*imx+x].r = in[j*imx+i].r;
    out[(y-(imy))*imx+x].i = in[j*imx+i].i;
  }
  else if (x>=imx && y>=imy){
    out[(y-(imy-1))*imx+x-(imx-1)].r = in[j*imx+i].r;
    out[(y-(imy-1))*imx+x-(imx-1)].i = in[j*imx+i].i;
  }
 
  
}

void mycircShift(kiss_fft_cpx *in, kiss_fft_cpx *out, int shift,bool right,bool down) { // mirror image is being generated here
  int i,j;
  int x, y;
  
  memset(out, 0, imx*imy* sizeof(kiss_fft_cpx));

  for (i=0; i<imx; i++) { //2048
     for(j=0; j<imy; j++) { //2048
         if(right==0 && down==0){x=i;y=j;}
         if(right==1 && down==0){x=shift+i;y=j;}
         if(right==0 && down==1){x=i; y=shift+j;}
         if(x<imx && y<imy){
           out[y*imx+x].r = in[j*imx+i].r;
           out[y*imx+x].i = in[j*imx+i].i;
         }
         else if (x>=imx && y<imy){
           out[y*imx+x-(imx)].r = in[j*imx+i].r;
           out[y*imx+x-(imx)].i = in[j*imx+i].i;
         }
         else if (x<imx && y>=imy){
           out[(y-(imy))*imx+x].r = in[j*imx+i].r;
           out[(y-(imy))*imx+x].i = in[j*imx+i].i;
         }
         else if (x>=imx && y>=imy){
           out[(y-(imy-1))*imx+x-(imx-1)].r = in[j*imx+i].r;
           out[(y-(imy-1))*imx+x-(imx-1)].i = in[j*imx+i].i;
         }
     }
  }
 
  
}
  
void mycircmask(unsigned char *in, unsigned char *out, int shift,bool right,bool down) { // mirror image is being generated here
  int i,j;
  int x, y;
  
  for (i=0; i<imx; i++) { //2048
     for(j=0; j<imy; j++) { //2048
         if(right==0 && down==0){x=i;y=j;}
         if(right==1 && down==0){x=shift+i;y=j;}
         if(right==0 && down==1){x=i; y=shift+j;}
         if(x<imx && y<imy){
           out[y*imx+x] = in[j*imx+i];
         }
         else if (x>=imx && y<imy){
           out[y*imx+x-(imx-1)] = in[j*imx+i];
         }
         else if (x<imx && y>=imy){
           out[(y-(imy-1))*imx+x] = in[j*imx+i];
         }
         else if (x>=imx && y>=imy){
           out[(y-(imy-1))*imx+x-(imx-1)] = in[j*imx+i];
         }
     }
  }
 
}
*/
void mycomplexcopy(kiss_fft_cpx *in,kiss_fft_cpx *out){
  int i,j;
  myClearComplex(out);
  for (i=0; i<imx; i++) {
    for(j=0; j<imy; j++) { //2048
      out[j*imx+i].r = in[j*imx+i].r;
      out[j*imx+i].i = in[j*imx+i].i;
    }
  }

}
void myBytecopy(unsigned char *in,unsigned char *out){
  int i,j;
  myClearByte(out);
  for (i=0; i<imx; i++) {
    for(j=0; j<imy; j++) { //2048
      out[j*imx+i] = in[j*imx+i];
    }
  }
}
void mySumImg(kiss_fft_cpx *p, float *s, float scale) { // for one kernel only, 
// if n= 24. then this loop runs 24 times
  int i;
  for(i=0; i<imxy; i++) {//2048*2048
    float pr = p[i].r; // intensity real part
    float pi = p[i].i; // intensity imaginary part
    s[i] += scale * (pr*pr + pi*pi);
  }
}
/* int pi = 0; */
/* #define P0 fprintf(stderr,"pi @ %d %d %d\n", pi=0, i, j); */
/* #define PI fprintf(stderr,"pi @ %d %d %d\n", pi++, i, j); */
#define P0
#define PI

//after FFT          35      35      one kernel     msk FFT,output img FFT Here there is no kernel input
void myKernelMult(int knx, int kny, float *krn, kiss_fft_cpx *mskf, kiss_fft_cpx *km ,int kern_num,float scale) { // deals with only one kernel
  int i, j, k;
  k=0;
  int imxh = imx/2; // midway of imx
  int imyh = imy/2; // midway of imy
  int xoff = imxh - knx/2 ; // midway
  int yoff = imyh - kny/2 ;
  for(i=0; i<knx; i++) { //35
    for(j=0; j<kny; j++) { //35
      P0 float kr = krn[k++];
      //fprintf(stderr, "%f\n", krn);
      PI float ki = krn[k++];
      //fprintf(stderr, "krn(%d,%d) = %f+%fJ\n", i,j,kr,ki);
      PI int ind = (yoff+j)*imx+(xoff+i);
      PI float mr = mskf[ind].r;
      PI float mi = mskf[ind].i;
      PI km[ind].r = (mr*kr-mi*ki); // means it is done in Freq domain
      PI km[ind].i = (mi*kr+mr*ki);
    }
  }
}

void myThreshhold(float *img, unsigned char *bimg) {
  int i,j;
  //  for (i=0; i<imxy; i++) bimg[i] = img[i] * 255;
  int cnt = 0;
  float max_intensity = 0.0;
  for(i=0; i<imx; i++) {// 2048
    for(j=0; j<imy; j++) { //2048
		  if( img[j*imx+i] > max_intensity) { max_intensity = img[j*imx+i]; }
      
		  if ( img[j*imx+i] >= TARGET_INTENSITY)  {
			   bimg[(j)*imx+(i)] =  255;
			   cnt++;
		  } else { 
        bimg[(j)*imx+( i)] =  0; }
    }
    
  }
  fprintf(stderr, "%d Pixels, Max. Intensity = %f\n",cnt,max_intensity);
}

void myVmul(int n, float *v, float sc) {
  int i;
  for(i=0; i<n; i++) v[i] *= sc;
}

int is_in_tile(int workx,int worky,int x,int y)
{
   if(x>=workx && x<=workx+1024 && y>=worky && y<=worky+1024) return 1;
   return 0;
}

void myComputeImage(agsKernel *krn, int workx, int worky, float dose,int width,int height,int index,int type) { //kernel input is here
  int i,j,k;
  //Image simulation
  myMaskFloat(bmsk, msk, dose,type);//converts binary msk to normal intensity mask
  myShift(msk, mskp);
  //fftwf_execute(fwdplan);
  kiss_fftnd(fwdplan, mskp, mskpf); //fft of mask
  myVmul(2*imxy, (float*)mskpf, 1.0 / ((float)imxy)); // scaled mask,mskpf 2048*2 size because fft is done
  myShift(mskpf, mskf);
  myClearComplex(km);
  int  nk = 1; // number of kernel

  for(k=0; k<nk; k++) { // loopos nk times

    /* Convolve with each kernel */
    fprintf(stderr, "Now convolving with kernel %d weight %f\n", k, krn->scale[k]);
    //printf("Size of mask %lu",sizeof(mskf));
    float scale=krn ->scale[k];
    myKernelMult(krn->nkx, krn->nky, krn->krn + k*krn->nkx*krn->nky, mskf, km,k,scale);//kernel,msk mult, all values are float and in frequency domain, mskf is input , km is output km = fft(msk) x fft (kernel)
    myShift(km, kmp); //2048*2048
    //fftwf_execute(bwdplan);
    kiss_fftnd(bwdplan, kmp, imgpk); //2048 IFFT
    myShift(imgpk, imgkin);// freq domain,2048,2028
  }
}



/*
char* make_key(int x,int y){
  //int sizex=(int)((ceil(log10(x))+1)*sizeof(char));
  //int sizey=(int)((ceil(log10(y))+1)*sizeof(char));
  //printf("sizex %d sizey %d ",sizex,sizey);
  static char *strx,*stry;
  strx=(char*) malloc( 10*sizeof(char) );
  stry=(char*) malloc( 10*sizeof(char) );

  //printf("x , y = %d %d\n",x,y);
  sprintf(strx, "%d", x);
  sprintf(stry, "%d", y);
  strcat(strx,"_");
  strcat(strx,stry);
  return strx;

}
*/

void make_dictionary_v2(int loopx,int loopy,int step,int numshift){
  int originx=0,originy=0,x,y;
  kiss_fft_cpx *rect,*sum;
  rect	= (kiss_fft_cpx*) malloc( loopx* loopy* sizeof(kiss_fft_cpx) );
  int numx,numy,precision = 10e7;;
  FILE *fp =fopen(OUTFILE,"ab");
  for (int i=1;i<=loopx;i++){
    x=originx+i*step;
    if(x>imx){x=imx;}
    for(int j=1;j<=loopy;j++){
      y=originy+j*step;
      if(y>imy){y=imy;}
      numx=x/step;
      numy=y/step;
      if(numx==1 && numy==1){

        rect[(i-1)*loopx+(j-1)].r = midpointIntensity[(j-1)*loopy+(i-1)].r;
        rect[(i-1)*loopx+(j-1)].i = midpointIntensity[(j-1)*loopy+(i-1)].i;
        //printf("--------------------------------------\n");
        //printf("index= %d j=%d,i=%d, a.r = %lf a.i=%lf\n",(j-1)*loopy+(i-1),j,i,midpointIntensity[(j-1)*loopx+(i-1)].r,midpointIntensity[(j-1)*loopx+(i-1)].i);
        //printf("width=%d,height=%d sum.r = %lf sum.i=%lf\n",x,y,rect[(i-1)*loopx+(j-1)].r,rect[(i-1)*loopx+(j-1)].i);
        //printf("--------------------------------------\n");
      }
      else if (numy==1 && numx>1){
          
        rect[(i-1)*loopx+(j-1)].r= (midpointIntensity[(j-1)*loopx+(i-1)].r + rect[(i-2)*loopx+(j-1)].r);
        rect[(i-1)*loopx+(j-1)].i =(midpointIntensity[(j-1)*loopx+(i-1)].i + rect[(i-2)*loopx+(j-1)].i);
        //printf("--------------------------------------\n");
        //printf("index= %d j=%d,i=%d, a.r = %lf a.i=%lf\n",(j-1)*loopy+(i-1),j,i,midpointIntensity[(j-1)*loopx+(i-1)].r,midpointIntensity[(j-1)*loopx+(i-1)].i);
        //printf("b.r=%lf, b.i=%lf\n", rect[(i-2)*loopx+(j-1)].r, rect[(i-2)*loopx+(j-1)].i);
        //printf("width=%d,height=%d sum.r = %lf sum.i=%lf\n",x,y,rect[(i-1)*loopx+(j-1)].r,rect[(i-1)*loopx+(j-1)].i);
        //printf("--------------------------------------\n");
      }
      else if (numy>1 && numx==1){
        rect[(i-1)*loopx+(j-1)].r= (midpointIntensity[(j-1)*loopy+(i-1)].r + rect[(i-1)*loopx+(j-2)].r);
        rect[(i-1)*loopx+(j-1)].i =(midpointIntensity[(j-1)*loopy+(i-1)].i + rect[(i-1)*loopx+(j-2)].i);
        //printf("--------------------------------------\n");
        //printf("index= %d j=%d,i=%d, a.r = %lf a.i=%lf\n",(j-1)*loopy+(i-1),j,i,midpointIntensity[(j-1)*loopx+(i-1)].r,midpointIntensity[(j-1)*loopx+(i-1)].i);
        //printf("d.r=%lf, d.i=%lf\n", rect[(i-1)*loopx+(j-2)].r, rect[(i-1)*loopx+(j-2)].i);
        //printf("width=%d,height=%d sum.r = %lf sum.i=%lf\n",x,y,rect[(i-1)*loopx+(j-1)].r,rect[(i-1)*loopx+(j-1)].i);
        //printf("--------------------------------------\n");
      }

      else {
        rect[(i-1)*loopx+(j-1)].r= (midpointIntensity[(j-1)*loopx+(i-1)].r  + rect[(i-2)*loopx+(j-1)].r + rect[(i-1)*loopx+(j-2)].r - rect[(i-2)*loopx+(j-2)].r);
        rect[(i-1)*loopx+(j-1)].i= (midpointIntensity[(j-1)*loopx+(i-1)].i  + rect[(i-2)*loopx+(j-1)].i + rect[(i-1)*loopx+(j-2)].i - rect[(i-2)*loopx+(j-2)].i);            
        //printf("--------------------------------------\n");
        //printf("index= %d j=%d,i=%d, a.r = %lf a.i=%lf\n",(j-1)*loopy+(i-1),j,i,midpointIntensity[(j-1)*loopx+(i-1)].r,midpointIntensity[(j-1)*loopx+(i-1)].i);
        //printf("b.r=%lf, b.i=%lf\n", rect[(i-2)*loopx+(j-1)].r, rect[(i-2)*loopx+(j-1)].i);
        //printf("c.r=%lf, c.i=%lf\n", rect[(i-2)*loopx+(j-2)].r, rect[(i-2)*loopx+(j-2)].i);
        //printf("d.r=%lf, d.i=%lf\n", rect[(i-1)*loopx+(j-2)].r, rect[(i-1)*loopx+(j-2)].i);
        //printf("width=%d,height=%d sum.r = %lf sum.i=%lf\n",x,y,rect[(i-1)*loopx+(j-1)].r,rect[(i-1)*loopx+(j-1)].i);
        //printf("--------------------------------------\n");
      }
      printf("width=%d,height=%d sum.r = %lf sum.i=%lf\n",x,y,rect[(i-1)*loopx+(j-1)].r,rect[(i-1)*loopx+(j-1)].i);
        
      fwrite(&rect[(i-1)*loopx+(j-1)].r,sizeof(float),1,fp);
      fwrite(&rect[(i-1)*loopx+(j-1)].i,sizeof(float),1,fp);

    }
  }
  fclose(fp);
  
}

/*
void make_dictionary(int loopx,int loopy,int step,int numshift,dict_t **dict){
  int originx=0,originy=0,x,y;
  kiss_fft_cpx *rect;
  rect	= (kiss_fft_cpx*) malloc( 1* 1* sizeof(kiss_fft_cpx) );
  
  for (int i=1;i<=loopx;i++){
  
    x=originx+i*step;
    if(x>imx){x=imx;}
    for(int j=1;j<=loopy;j++){
      y=originy+j*step;
      if(y>imy){y=imy;}
      char *strx=make_key(x,y);
     //rect=calculateMidPointRect(x,y,step,numshift);
      rect=CALC_MIDPOINT(x,y,step,numshift,dict);

      kiss_fft_cpx *check;
      check	= (kiss_fft_cpx*) malloc( 1* 1* sizeof(kiss_fft_cpx) );
      check=(kiss_fft_cpx *)getItem(*dict,strx);

      if((i-4)>0){
       char *str_temp=make_key(i-4,j);
       delItem(dict, str_temp); 
      }
      
      if(check->r== -1 && check->i== -1){
        addItem(dict,strx,rect,1,1); 
        FILE *fp;
        fp =fopen(OUTFILE,"ab");
        fwrite(&rect->r,sizeof(rect->r),1,fp);
        fwrite(&rect->i,sizeof(rect->i),1,fp);
        fclose(fp);
        //fwrite(&imag,sizeof(double),1,fp);
        //fprintf(fp,"%s,%lf,%lf\n",strx,rect->r,rect->i);
        //fclose(fp);
        } // means it is not inside the dictionary, means it is a new item. so add it.
      else {
        delItem(dict,strx);
        addItem(dict,strx,rect,1,1);
        FILE *fp;
        fp =fopen(OUTFILE,"ab");
        fwrite(&rect->r,sizeof(rect->r),1,fp);
        fwrite(&rect->i,sizeof(rect->i),1,fp);
        fclose(fp);
        //FILE *fp;
        //fp=fopen(OUTFILE,"ab");
        //printf("\nOUTPUT %s,real = %lf,%lf\n",strx,real,rect->i);
        //fwrite(&imag,sizeof(double),1,fp);
        //fwrite(rect,sizeof(rect),1,fp);
        //fprintf(fp,"%s,%lf,%lf\n",strx,rect->r,rect->i);
        //fclose(fp);
      
      }

    }
  }
}
*/
void findmidpoint_v2(int index,kiss_fft_cpx *imgk,kiss_fft_cpx *midpointIntensity,int shifty,int shiftx){
      int i,j;
      int centerx = 1024;
      int centery = 1024;
      int indexx, indexy;
      if(shiftx < imx/2 && shifty< imy/2){ indexx = centerx-shiftx; indexy = centery - shifty; }
      else if(shiftx < imx/2 && shifty>= imy/2) {indexx = centerx-shiftx; indexy = centery - shifty+imy;}
      else if(shiftx >= imx/2 && shifty < imy/2) {indexx = centerx-shiftx+imx; indexy = centery - shifty;}
      else if(shiftx >=imx/2 && shifty >=imy/2) { indexx = centerx-shiftx + imx; indexy = centery - shifty + imy ; }
      midpointIntensity[index].r=imgk[(indexy-1)*imx+(indexx-1)].r;
      midpointIntensity[index].i=imgk[(indexy-1)*imx+(indexx-1)].i;

      //printf("pos:%d %d val: %f %f mp_intensity_index= %d imgk_index: %d\n",i,j,midpointIntensity[index].r,midpointIntensity[index].i,index,j*imx+i);
      //FILE *fp;
      //fp=fopen("test3.csv","a");
      //fprintf(fp,"%f,%f,%d\n",midpointIntensity[index].r,midpointIntensity[index].i,index);
      //fclose(fp);

}
/*
void writeimgfile(float *img,int x,int y){
  FILE *fp;
  char file[80];
  sprintf(file,"image%d_%d.csv",x,y);
  fp=fopen(file,"w");
  fprintf(fp,"Intensity\n");
  for(int i=0;i<imxy;i++){
    fprintf(fp,"%f\n",img[i]);
  }
  fclose(fp);
}
*/


int main(int argc, char	 **argv) {
	struct timeval start, end;
	gettimeofday(&start, NULL);
	int i,j,k;
	int dims[] = {2048, 2048};
	int ndims = 2;
	int isinverse = 1;
  int type=1; // type=1 for psm and 0 for binary
  if(argc<2) {
    printf("*Error*: Must specify input GULP file name\n");
    exit(1);
  } 
  else {
    strcpy(INFILE, argv[1]); 
      if(argc<3) { printf("*Error*: Must specify output CSV file name\n"); exit(1); }
      strcpy(OUTFILE, argv[2]);
  }
	
	bmsk	= malloc(2048 * 2048);
	bimg	= malloc(2048 * 2048);
 // Config for input mask
	msk	= (kiss_fft_cpx*) malloc( dims[0] * dims[1] * sizeof(kiss_fft_cpx) );
	mskp	= (kiss_fft_cpx*) malloc( dims[0] * dims[1] * sizeof(kiss_fft_cpx) );
	mskpf = (kiss_fft_cpx*) malloc( dims[0] * dims[1] * sizeof(kiss_fft_cpx) );
	mskf	= (kiss_fft_cpx*) malloc( dims[0] * dims[1] * sizeof(kiss_fft_cpx) );
	//intermediate	= (kiss_fft_cpx*) malloc( dims[0] * dims[1] * sizeof(kiss_fft_cpx) );
 // Config for output image
	km	= (kiss_fft_cpx*) malloc( dims[0] * dims[1] * sizeof(kiss_fft_cpx) );
	kmp	= (kiss_fft_cpx*) malloc( dims[0] * dims[1] * sizeof(kiss_fft_cpx) );
	imgpk = (kiss_fft_cpx*) malloc( dims[0] * dims[1] * sizeof(kiss_fft_cpx) );
	imgkin	= (kiss_fft_cpx*) malloc( dims[0] * dims[1] * sizeof(kiss_fft_cpx) );
	img	= malloc(2048 * 2048 * sizeof(float));

	fwdplan = kiss_fftnd_alloc (dims, ndims, 0, 0, 0);//(2048, 2048, mskp, mskpf, -1, FFTW_WISDOM_ONLY);
	bwdplan = kiss_fftnd_alloc (dims, ndims, isinverse, 0, 0); //fftwf_plan_dft_2d(2048, 2048, kmp, imgpk,	1, FFTW_WISDOM_ONLY);
  //ndims_2
	int workx[100];	// No more than 100 tiles. Start with 1
	int worky[100];
	int workcnt = 0;

	int lyr = 0;   // For single layer, it is always 0
	int tnx = 2048;
	int tny = 2048;
	int sc = 1;	// What does this variable do?

	int size;
	int scsize;
  int step,loop_x,loop_y,shift,numshift;
	//int num_target_shapes;
	float dose = 1.0;

	// Parse the input file and assign layer name
  
	clear_design_space();
  read_layout(INFILE);
	char layername[] = "M1OPC";

  //case CMD_KERNEL_LOAD:

  /// Only this part deals with KERNEL  ///////

  krns = malloc(knlvl * sizeof(agsKernel *));// knlvl= 1
  for(i=0; i<knlvl; i++) {
		krns[i] = agsReadKernel(layername);
		nk = krns[i]->nk;
		nkx = krns[i]->nkx;
		nky = krns[i]->nky;
  }
  printf("*Message*: received kernel data\n");
  ////////////////////////////////////////////////////////////////

  // Process the tile with litho simultion
  printf("*Message*: Running simulation on tile\n");
  size = tnx * tny; //2048*2048
  scsize = size / (sc * sc);  //sc=1

  workx[0] = 0;	// Let's start with just one tile
  worky[0] = 0;
  int *key;
  //***********************************
  int start_tile=2; // change it for every case
  FILE *file = fopen(OUTFILE,"wb"); //csv file mode
  if(file) {
    fwrite(&start_tile,1,sizeof(start_tile),file);
  }
  fclose(file);
 //**************************************
  rect* devdata = lvldata[lyr];
  int num_rect=lvlcnt[lyr];
  for (int i=0; i<num_rect;i++){

    memset(msk, 0, dims[0] * dims[1] * sizeof(kiss_fft_cpx));
    memset(mskp, 0, dims[0] * dims[1] * sizeof(kiss_fft_cpx));
    memset(mskpf, 0, dims[0] * dims[1] * sizeof(kiss_fft_cpx));
    memset(mskf, 0, dims[0] * dims[1] * sizeof(kiss_fft_cpx));
    myClearByte(bmsk);
  
    memset(km, 0, dims[0] * dims[1] * sizeof(kiss_fft_cpx));
    memset(kmp, 0, dims[0] * dims[1] * sizeof(kiss_fft_cpx));
    memset(imgpk, 0, dims[0] * dims[1] * sizeof(kiss_fft_cpx));
    memset(imgkin, 0, dims[0] * dims[1] * sizeof(kiss_fft_cpx));
    
    key=myDevLayerDraw(bmsk, imx, imy, workx[0], worky[0], 1,devdata[i],1, 255); // stores a layout to print
    rect r=devdata[i];
    shift = r.w;

    numshift=imx/shift;
    midpointIntensity	= (kiss_fft_cpx*) malloc( numshift* numshift* sizeof(kiss_fft_cpx) );
    memset(midpointIntensity, 0, numshift* numshift * sizeof(kiss_fft_cpx));

    myMaskFloat(bmsk, msk, dose,type);//converts binary msk to normal intensity mask
    myComputeImage(krns[lyr], workx[0], worky[0], dose,key[0],key[1],0,type); //kernel , the tile as input,everythong is float number in thise module
    float scale=86.9432;

    int shiftx=0,shifty=0;
    int rightshiftend=0;
    int downshiftend=0;
    for(int j=0;j<numshift;j++){
        shifty=j*shift;
        
        for(int i=0;i<numshift;i++){

          shiftx=i*shift;

          //char maskfile[80]; sprintf(maskfile,"Debug/mask_tile%d_%d_%d_%d%d.png",key[0],key[1],i*shift,i,j);
          //write_mask_file(bmskout1, maskfile, 2048, 2048);//converts 2 image
          //findmidpoint(j*numshift+i,imgkout,midpointIntensity,shifty,shiftx);
          findmidpoint_v2(j*numshift+i,imgkin,midpointIntensity,shifty,shiftx);
          //mySumImg(imgkout1, img, scale); // mul with scale, time domain,2048*2048
          //char imagefile[80]; sprintf(imagefile,"Debug/image_tile%d_%d_%d.png",key[0],key[1],i*shift);
          //write_image_file(img, imagefile, 2048, 2048, dose);
          //char intensityfile[80]; sprintf(intensityfile,"Debug/intensity_tile%d_%d_%d.png",key[0],key[1],i*shift);
          //write_intensity_file(img, intensityfile, 2048, 2048, dose);
          //myClearFloat(img);
       }     
      //mycomplexcopy(imgkout,imgkin);
      //myBytecopy(bmskout,bmsk);
     }
     step=shift;
     loop_x=numshift;
     loop_y=numshift;
     //make_dictionary(loop_x,loop_y,step,numshift,dict);
     make_dictionary_v2(loop_x,loop_y,step,numshift);
     // ********* Write the image output to a PNG file ************//
     //write_mask_file(bimg, imagefile, 2048, 2048);
     // ********************************************************* //
     printf("*Message*: Done computing image\n");
  // ********* Write the mask output to a PNG file ************//
  //Clean up
  }
  for(i=0; i<knlvl; i++)  free(krns[i]);
  for(j=0; j<nlvl; j++)  { free(lvldata[j]); lvlcnt[j]=0; }
  free(midpointIntensity);
  printf("*Message*: Removed kernel and design data\n");
 	gettimeofday(&end, NULL); 
  long seconds = (end.tv_sec - start.tv_sec);
	long micros = ((seconds * 1000000) + end.tv_usec) - (start.tv_usec); 
  printf("\nsimulator took %ld seconds %ld micro seconds to execute \n",seconds,micros); 
  exit(0);
}

