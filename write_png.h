#include <stdio.h>
//#include "png.h"
#include "lodepng.h"

typedef struct {
  unsigned char r;
  unsigned char g;
  unsigned char b;
  unsigned char a;
} ucharac4;

/* This function writes the mask as a PNG file */
void write_mask_file(unsigned char* wmsk, char* fname, int imx, int imy)
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
  /*Encode the image*/
  unsigned error = lodepng_encode32_file(fname, pngmask, imx, imy);

  /*if there's an error, display it*/
  if(error) printf("error %u: %s\n", error, lodepng_error_text(error));

  free(pngmask);
}

void write_image_file(float* image, char* fname, int sizex, int sizey, float dose)
{
     /* This section dumps out the image */
     ucharac4 *pngimg;
     pngimg = (ucharac4*)malloc(sizex * sizey * sizeof(ucharac4));
     float imgfloat;
     int imgval, j, k;
     for(k=0; k<sizey; k++) {
        for(j=0; j<sizex; j++) {
         imgfloat = image[sizex*k + j];
	      if(imgfloat>=TARGET_INTENSITY ) { imgval = 252; }
	      else { imgval = 0; }
        //imgval=imgfloat*255;
	      if(dose==1.0) { pngimg[sizex*k + j].r = 0; pngimg[sizex*k + j].g = pngimg[sizex*k + j].b =  imgval; }
	      else if(dose==MAX_DOSE) { pngimg[sizex*k + j].r = 0; pngimg[sizex*k + j].g = pngimg[sizex*k + j].b =  imgval/2; }
	      else if(dose==MIN_DOSE) { pngimg[sizex*k + j].g = 0; pngimg[sizex*k + j].r = pngimg[sizex*k + j].b =  imgval/2; }
	      pngimg[sizex*k + j].a = 127;
       }
     }


  /*Encode the image*/
  unsigned error = lodepng_encode32_file(fname, pngimg, sizex, sizey);

  /*if there's an error, display it*/
  if(error) fprintf(stderr, "*Error* %u: %s\n", error, lodepng_error_text(error));

  free(pngimg);
}
void write_intensity_file(float* image, char* fname, int sizex, int sizey, float dose)
{
     /* This section dumps out the image */
  float MAX_Intensity=1.02;
  float MIN_Intensity=0;
  float segment=1000;
  float step=(MAX_Intensity-MIN_Intensity)/segment;
  int colorstep=((255-0)/segment);
  ucharac4 *pngimg;
  pngimg = (ucharac4*)malloc(sizex * sizey * sizeof(ucharac4));
  float imgfloat;
  int imgval=0, j, k;
  FILE *fp;
  for(k=0; k<sizey; k++) {
    for(j=0; j<sizex; j++) {
      imgfloat = image[sizex*k + j];
      if(imgfloat>TARGET_INTENSITY){
        imgval=220+10*(int)8*log10(imgfloat/TARGET_INTENSITY);
       // if(TARGET_INTENSITY<imgfloat<.3){imgval=50;}
       // else if(.3<imgfloat<.4) {imgval=100;}
       // else if(.4<imgfloat<.5) {imgval=150;}
       // else if(.5<imgfloat<.6) {imgval=200;}
       // else if(.6<imgfloat<.7) {imgval=220;}
       // else if(.7<imgfloat<.8) {imgval=230;}
       // else if(.8<imgfloat<.9) {imgval=235;}
       // else if(.9<imgfloat<1) {imgval=245;}
       // else if(imgfloat>1) {imgval=255;}
        pngimg[sizex*k + j].r =imgval;
        pngimg[sizex*k + j].g =0;
        pngimg[sizex*k + j].b =imgval;
        pngimg[sizex*k + j].a = 220;
        }
      else {
        if(0<imgfloat<10e-9) {imgval=00;}
        else if(10e-9<imgfloat<10e-8) {imgval=50;}
        else if(10e-8<imgfloat<10e-7) {imgval=100;}
        else if(10e-7<imgfloat<10e-6) {imgval=150;}
        else if(10e-6<imgfloat<10e-5) {imgval=200;}
        else if(10e-5<imgfloat<10-4) {imgval=220;}
        else if(10e-4<imgfloat<10-3) {imgval=230;}
        else if(10e-3<imgfloat<10-2) {imgval=235;}
        else if(10e-2<imgfloat<10-1) {imgval=245;}
        else if (10e-1<imgfloat<TARGET_INTENSITY){imgval=255;}
        pngimg[sizex*k + j].b = imgval;
        pngimg[sizex*k + j].g = imgval;
        pngimg[sizex*k + j].r =0;
        pngimg[sizex*k + j].a = 200;
        }
      

      
      //if(imgfloat>=TARGET_INTENSITY ) { imgval = 252; }
      //else { imgval = 0; }
      //if(dose==1.0) { pngimg[sizex*k + j].r = imgval; pngimg[sizex*k + j].g =0; pngimg[sizex*k + j].b =0; }
      //else if(dose==MAX_DOSE) { pngimg[sizex*k + j].r = imgval/2; pngimg[sizex*k + j].g =0; pngimg[sizex*k + j].b = 0; }
      //else if(dose==MIN_DOSE) { pngimg[sizex*k + j].g = imgval/2; pngimg[sizex*k + j].r =0; pngimg[sizex*k + j].b = 0; }
     
    }
  }

  /*Encode the image*/
  unsigned error = lodepng_encode32_file(fname, pngimg, sizex, sizey);

  /*if there's an error, display it*/
  if(error) fprintf(stderr, "*Error* %u: %s\n", error, lodepng_error_text(error));

  free(pngimg);
}

