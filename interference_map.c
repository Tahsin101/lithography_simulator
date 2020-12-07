#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "kiss_fft.h"
#include "kiss_fftnd.h"
#include "kernels.h"
#include "layoutrw.h"

int MAX_FILE_NAME_LENGTH = 80;
//float BACKGROUND_REAL = 0.0229;
//float BACKGROUND_IMAG = -0.0217;
float BACKGROUND_REAL = 0;
float BACKGROUND_IMAG = 0;
float MASK_REAL = 0.9771;
float MASK_IMAG = 0.0217;
//float MASK_REAL = 1;
//float MASK_IMAG = 0;
float TARGET_INTENSITY = 0.225;
//float TARGET_INTENSITY = 0.02;
float MAX_DOSE = 1.02;
float MIN_DOSE = 0.98;

rect *lvldata[MAXLVL];
rect *d_lvldata[MAXLVL];


#include "write_png.h"

#define myMin(a,b) ((a)<=(b) ? (a) : (b))
#define myMax(a,b) ((a)<=(b) ? (b) : (a))
char *testfile="results/binarywithnoise.csv";
void writetestfile(){
 FILE *fp;
 fp=fopen(testfile,"a");
 fprintf(fp,"ox,oy,lrx,lry,pr,pi,intensity\n");
 fclose(fp);
}
void writeline(int flag,double val){
  if(flag==1){
    FILE *fp;
    fp=fopen(testfile,"a");
    fprintf(fp,"%f,",val);
    fclose(fp);
  }
  else if(flag==2){
    FILE *fp;
    fp=fopen(testfile,"a");
    fprintf(fp,"%d,",(int)val);
    fclose(fp);
  }
  else{
    FILE *fp;
    fp=fopen(testfile,"a");
    fprintf(fp,"%f\n",val);
    fclose(fp);
  }

}

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



int flagthreshold =0;
int flagdraw = 0;
int last = 0 ;

/////////////////////////////////////

void writelogfile(float a,FILE *fl){

  if(flagdraw) {
    fprintf(fl,"%d,",(int) a);
  }
  else if (flagthreshold) {
    fprintf(fl,"%d,",(int)a);
    if ( last ) {fprintf(fl,"%f",a);}
  }
}
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
void myDevLayerDraw(unsigned char *imgpbo, int nx, int ny, int ox, int oy, int sc, rect *devdata, int lvlcnt, int c) {
  //TI_TN;
  int i;
  int x;
  int y;
  	
printf("lvlcnt = %d\n",lvlcnt);
  for( i = 0; i<lvlcnt; i++) {
    int width,height,pixels;
    rect r = devdata[i];
		int count=0;
    int x0 = (r.x / sc - ox);
    int y0 = (r.y / sc - oy);
    int x1 = x0 + r.w / sc + 1;
    int y1 = y0 + r.h / sc + 1;
    width = r.w;
    height = r.h;
    pixels=width*height;
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
    }
printf("for level %d , count=%d\n",i,count);
    /*else { fprintf(stderr,"Error: Rectangle (%d, %d) to (%d,%d) out of bounds!\n",x0,y0,x1,y1); exit(0); }*/
  }
}

int knlvl = 1;	// Only one simulation layer for now
agsKernel **krns;

float *d_scale;
float *d_kernels;

int nk, nkx, nky; //24,35,35

int imx = 2048;
int imy = 2048;
int imxy = 2048 * 2048;

unsigned char *bmsk, *bimg;

kiss_fft_cpx *msk, *mskp, *mskpf, *mskf;
kiss_fft_cpx *km, *kmp, *imgpk, *imgk; // fft config
float *img;

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
void myMaskFloat(unsigned char *bmsk, kiss_fft_cpx *msk, float dose) {
  int i, j;
	FILE *fp;
	fp = fopen("results/flt_msk.csv","w");
 
  for(i=0; i<imxy; i++) {// 2048*2048
    fprintf(fp,"%f\n",msk[i].r);
    msk[i].r = (((float)bmsk[i]) / 255.0 * MASK_REAL+BACKGROUND_REAL)*dose;   /* Enable these lines for Attenuated PSM */
    msk[i].i = (((float)bmsk[i]) / 255.0 * MASK_IMAG+BACKGROUND_IMAG )*dose;
    //msk[i].r = ((((float)bmsk[i])/ 255.0)*MASK_REAL+BACKGROUND_REAL) * dose;    /* Enable these lines for binary masks */
    //msk[i].r = ((((float)bmsk[i])/ 255.0)) * dose;    /* Enable these lines for binary masks */
    //msk[i].i = (((float)bmsk[i]) / 255.0 )*dose;
    
    //printf("bmsk[%d]= %d ,msk[%d].r = %f\n",i,bmsk[i],i,msk[i].r);
	//fprintf(stderr,"binary: %f ,msk real:  %f , msk img: %f\n",bmsk[i],msk[i].r,msk[i].i);
	
   //msk[i].i = 0.0;
}
fclose(fp);
}

void myShift(kiss_fft_cpx *in, kiss_fft_cpx *out,int kern_num) { // mirror image is being generated here
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
  
void mySumImg(kiss_fft_cpx *p, float *s, float scale,int kern_num) { // for one kernel only, 
// if n= 24. then this loop runs 24 times
  int i;
  FILE *fp;
	fp = fopen("results/imgk.csv","w");
  for(i=0; i<imxy; i++) {//2048*2048
    float pr = p[i].r;
    float pi = p[i].i;
    fprintf(fp,"%f,%f\n",pr,pi);

    if(i==(imx/2-1)*(imx)+(imx/2-1)) {
      fprintf(stderr,"[VECTOR] %d,%f,%f\n",i,pr,pi);
      writeline(1,pr);
      writeline(1,pi);
      }
    //fprintf(stderr, " img_r = %f, img_i = %f,scale %f\n",p[i].r,p[i].i, scale);
    s[i] += scale * (pr*pr + pi*pi);
    if(i==(imx/2)*(imx)+(imx/2)) {
      fprintf(stderr,"[INTENSITY] %f\n",s[i]);
      writeline(0,s[i]);
      }
   
    /*if(i%100000==0) fprintf(stderr,"sum img %d scale %f real %f imag %f is %f\n", i, scale, pr, pi, s[i]);*/
    //fprintf(stderr,"sum img %d scale %f real %f imag %f is %f\n", i, scale, pr, pi, s[i]);
  }
  fclose(fp);
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
  int count =0;
  FILE *fp,*fq;
  char buff[64],buff1[64];
  int temp;

  sprintf(buff, "results/kernel_%d.csv", kern_num);
  //sprintf(buff1, "results/temp_%d.csv", kern_num);
  fp = fopen(buff,"w");
  fq = fopen("results/km.csv","w");
  //fq = fopen(buff1,"w");
  //fq=fopen("t.txt","w");
  fprintf(fp,"real_val,imag_val\n");
  for(i=0; i<knx; i++) { //35
    for(j=0; j<kny; j++) { //35
      P0 float kr = krn[k++];
      count=count+1;
      //fprintf(stderr, "%f\n", krn);
      PI float ki = krn[k++];
      //fprintf(stderr, "krn(%d,%d) = %f+%fJ\n", i,j,kr,ki);
      PI int ind = (yoff+j)*imx+(xoff+i);
      PI float mr = mskf[ind].r;
      PI float mi = mskf[ind].i;
      //fprintf(fq, "indice = %d kernel %f +%fj ; mask %f + %fj \n", ind,kr,ki,mr,mi); // So kernel must be FFT
      fprintf(fp, "%f,%f\n", kr,ki); // So kernel must be FFT
	
      //fprintf(stderr, "mask %f + %fj \n", ); // FFT of main msk
      PI km[ind].r = (mr*kr-mi*ki); // means it is done in Freq domain
      PI km[ind].i = (mi*kr+mr*ki);
      fprintf(fq, "%f,%f\n", km[ind].r,km[ind].r);
      //fprintf(stderr,"indice = %d, val=%f count=%d\n",ind,km[ind].r,count);
      //fprintf(fq, "%f,%f\n", km[ind].r,km[ind].i);
      temp=ind;
      //fprintf(stderr, "km(%d,%d) = %f+%fJ\n", i,j,km[ind].r, km[ind].i);
    }
  }
      //printf("The indice number %d\n",temp);
  fclose(fp);
  fclose(fq);
  //fclose(fq);
  //fclose(fq);
  fprintf(stderr, "%d   kernel= %d\n", count,kern_num);
}
float midpoint=0;
void myThreshhold(float *img, unsigned char *bimg) {
  int i,j;
  //  for (i=0; i<imxy; i++) bimg[i] = img[i] * 255;
  int cnt = 0;
  float max_intensity = 0.0;
  FILE *fp;

  fp = fopen("results/float_img.csv","w");
  for(i=0; i<imx; i++) {// 2048
    for(j=0; j<imy; j++) { //2048
	   	fprintf(fp,"%f\n",img[j*imx+i]); //copying the output mask values
		  if( img[j*imx+i] > max_intensity) { max_intensity = img[j*imx+i]; }
      if (i==imx/2 && j==imy/2) { midpoint  = img[j*imx+i]; printf("\n\n[INTENSITY]mid point intensity %f\n\n",midpoint); }
		  if ( img[j*imx+i] >= TARGET_INTENSITY)  {
         //fprintf(fp,"%f\n",img[j*imx+i]); //copying the output mask values
			   bimg[(j)*imx+(i)] =  255;
			   cnt++;
		  } else { 
        //fprintf(fp,"%f\n",img[j*imx+i]);
        bimg[(j)*imx+( i)] =  0; }
    }
    
  }
  fprintf(stderr, "%d Pixels, Max. Intensity = %f\n",cnt,max_intensity);
  fclose(fp);
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


void myComputeImage(agsKernel *krn, int workx, int worky, float dose) { //kernel input is here
  int i,k;
 // int nk = krn->nk;
  //Image simulation
  myMaskFloat(bmsk, msk, dose);//converts binary msk to normal intensity mask
  myShift(msk, mskp,k);
  //fftwf_execute(fwdplan);
  kiss_fftnd(fwdplan, mskp, mskpf); //fft of mask
  myVmul(2*imxy, (float*)mskpf, 1.0 / ((float)imxy)); // scaled mask,mskpf 2048*2 size because fft is done
  myShift(mskpf, mskf,k);
  myClearFloat(img);
  myClearComplex(km);

  int  nk = 1; // number of kernel

  for(k=0; k<nk; k++) { // loopos nk times

    /* Convolve with each kernel */
    fprintf(stderr, "Now convolving with kernel %d weight %f\n", k, krn->scale[k]);
    //printf("Size of mask %lu",sizeof(mskf));
    float scale=krn ->scale[k];
    myKernelMult(krn->nkx, krn->nky, krn->krn + k*krn->nkx*krn->nky, mskf, km,k,scale);//kernel,msk mult, all values are float and in frequency domain, mskf is input , km is output km = fft(msk) x fft (kernel)
    //fprintf(stderr,"Size of mask %lu, Size of km %lu",sizeof(mskf),sizeof(km));
    myShift(km, kmp,k); //2048*2048
    //printf("Size of kmp %lu",sizeof(kmp));
    //fftwf_execute(bwdplan);
    kiss_fftnd(bwdplan, kmp, imgpk); //2048 IFFT
    myShift(imgpk, imgk,k);// freq domain,2048,2028
    mySumImg(imgk, img, krn->scale[k],k); // mul with scale, time domain,2048*2048
    //printf("Size of img %lu",sizeof(img));

  }
//after IFFT
  myThreshhold(img, bimg); //converts floating intensity to binary.
    //printf("Size of bimg %lu",sizeof(bimg)); //2048*2028
  return;
}



int main(int argc, char	 **argv) {
	int i,j,k;
	int dims[] = {2048, 2048};
	int ndims = 2;
	int isinverse = 1;
	char INFILE[80];
	char OUTFILE[80];

	// Read in input and output file names
	if(argc<2) {
		fprintf(stderr,"*Error*: Must specify input GULP file name\n");
		exit(1);
	} else {
		strcpy(INFILE, argv[1]);
		if(argc<3) { fprintf(stderr,"*Error*: Must specify output GULP file name\n"); exit(1); }
		strcpy(OUTFILE, argv[2]);
	}
	bmsk	= malloc(2048 * 2048);
	bimg	= malloc(2048 * 2048);
 // Config for input mask
	msk	= (kiss_fft_cpx*) malloc( dims[0] * dims[1] * sizeof(kiss_fft_cpx) );
	mskp	= (kiss_fft_cpx*) malloc( dims[0] * dims[1] * sizeof(kiss_fft_cpx) );
	mskpf = (kiss_fft_cpx*) malloc( dims[0] * dims[1] * sizeof(kiss_fft_cpx) );
	mskf	= (kiss_fft_cpx*) malloc( dims[0] * dims[1] * sizeof(kiss_fft_cpx) );
 // Config for output image

	km	= (kiss_fft_cpx*) malloc( dims[0] * dims[1] * sizeof(kiss_fft_cpx) );
	kmp	= (kiss_fft_cpx*) malloc( dims[0] * dims[1] * sizeof(kiss_fft_cpx) );
	imgpk = (kiss_fft_cpx*) malloc( dims[0] * dims[1] * sizeof(kiss_fft_cpx) );
	imgk	= (kiss_fft_cpx*) malloc( dims[0] * dims[1] * sizeof(kiss_fft_cpx) );

	img	= malloc(2048 * 2048 * sizeof(float));

	memset(msk, 0, dims[0] * dims[1] * sizeof(kiss_fft_cpx));
	memset(mskp, 0, dims[0] * dims[1] * sizeof(kiss_fft_cpx));
	memset(mskpf, 0, dims[0] * dims[1] * sizeof(kiss_fft_cpx));
	memset(mskf, 0, dims[0] * dims[1] * sizeof(kiss_fft_cpx));

	memset(km, 0, dims[0] * dims[1] * sizeof(kiss_fft_cpx));
	memset(kmp, 0, dims[0] * dims[1] * sizeof(kiss_fft_cpx));
	memset(imgpk, 0, dims[0] * dims[1] * sizeof(kiss_fft_cpx));
	memset(imgk, 0, dims[0] * dims[1] * sizeof(kiss_fft_cpx));


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

	int num_target_shapes;
	float dose = 1.0;

	unsigned char *tile = NULL;
	unsigned char *tiletemp = NULL;

	//case CMD_DESIGN_LOAD:
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
      fprintf(stderr, "nk = %d  nkx=%d  nky=%d\n", nk,nkx,nky);
      }
      fprintf(stderr, "*Message*: received kernel data\n");
////////////////////////////////////////////////////////////////

  // Process the tile with litho simultion
  fprintf(stderr, "*Message*: Running simulation on tile\n");
  size = tnx * tny; //2048*2048
  scsize = size / (sc * sc);  //sc=1

  if (tile == NULL) {
	tile = malloc(size*sizeof(unsigned char));//2048x2048xchar
	tiletemp = malloc(size*sizeof(unsigned char));
  }
	// this is real assuming 1024x1024 tile is being asked for (i know! i know! i'll fix it later)
workx[0] = 0;	// Let's start with just one tile
worky[0] = 0;
myClearByte(bmsk);
myDevLayerDraw(bmsk, imx, imy, workx[0], worky[0], 1, lvldata[lyr], lvlcnt[lyr], 255);


  	// ********* Write the mask output to a PNG file ************//
char maskfile[80]; sprintf(maskfile,"Debug/mask_tile%d.png",i);
write_mask_file(bmsk, maskfile, 2048, 2048);//converts 2 image
myMaskFloat(bmsk, msk, dose);//converts binary msk to normal intensity mask
printf("\n\n**********After myMaskFloat***********\n\n");
     // *********************************************************//
 //

 	myComputeImage(krns[lyr], workx[0], worky[0], dose); //kernel , the tile as input,everythong is float number in thise module
 	mySubBlock(bimg, imx, imy, tiletemp, tnx, tny, 512, 512);
 	mySubSample(tiletemp, tnx, tny, sc, tile);

	// ********* Write the image output to a PNG file ************//
	char imagefile[80]; sprintf(imagefile,"Debug/image_tile%d.png",i);
	write_image_file(img, imagefile, 2048, 2048, dose);
	char imagefile1[80]; sprintf(imagefile1,"Debug/intensity_tile%d.png",i);
	write_intensity_file(img, imagefile1, 2048, 2048, dose);
    // ********************************************************* //

	fprintf(stderr,"*Message*: Done computing image\n");

	// Dump to output file
	FILE* fout = fopen(OUTFILE,"w");
	for(j=0; j<dims[1];j++) {
		 for(i=0; i<dims[0]; i++) {
			 if(bimg[tnx*j + i] ==0 ) { fprintf(fout,"0 "); }
			 else { fprintf(fout,"1 "); }
		}
		fprintf(fout,"\n");
	}

    //Clean up
    for(i=0; i<knlvl; i++)  free(krns[i]);
    for(j=0; j<nlvl; j++)  { free(lvldata[j]); lvlcnt[j]=0; }
    fprintf(stderr,"*Message*: Removed kernel and design data\n");
    exit(0);
}

