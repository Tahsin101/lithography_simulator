
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


void write_image_wo_th(float* image, char* fname, int sizex, int sizey, float dose) // write image without threshold
{
     /* This section dumps out the image */
     ucharac4 *pngimg;
     pngimg = (ucharac4*)malloc(sizex * sizey * sizeof(ucharac4));
     float imgfloat;
     int imgval, j, k;
     for(k=0; k<sizey; k++) {
        for(j=0; j<sizex; j++) {
         imgfloat = image[sizex*k + j];
	 if(1) { imgval = 252; }
	 else { imgval = 0; }
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
