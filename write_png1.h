
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
void write_mask_file(unsigned char* wmsk, char* fname, int imx, int imy,
		     int nx, int ny, int ox, int oy, int sc, 
		     rect  bedata, int becnt )
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
  // ========== target image ================ //
  int i, x, y;
    rect r = bedata;
    //    fprintf(stderr,"Read line (%d,%d) w=%d, h=%d\n",r.x,r.y,r.w,r.h);
    int x0 = (r.x / sc - ox);
    int y0 = (r.y / sc - oy);
    int x1 = x0 + r.w / sc;
    int y1 = y0 + r.h / sc;
    if( (!r.w && !r.h) || r.w && r.h ) {
      //      fprintf(stderr,"Error: Edge (%d, %d) to (%d,%d) is not HV\n",x0,y0,x1,y1);
      exit(0);
    }
    int p = ( (x0>x1?x0:x1) < 0) || ((x0<x1?x0:x1) >= nx) ||
            ( (y0>y1?y0:y1) < 0) || ((y0<y1?y0:y1) >= ny);
    if (!p) {
      //      fprintf(stderr,"Write line (%d,%d) w=%d, h=%d\n",x0,y0,x1,y1);
      if      ( r.w > 0 )
	{ for( x = (x0 < 0 ? 0: x0); x<= (x1 > nx ? nx : x1); x++) pngmask[nx*y0 + x].g = 255;}
      else if ( r.w < 0 )
	{ for( x = (x1 < 0 ? 0: x1); x<= (x0 > nx ? nx : x0); x++) pngmask[nx*y0 + x].g = 255;}
      else if ( r.h > 0 )
	{ for( y = (y0 < 0 ? 0: y0); y<= (y1 > nx ? nx : y1); y++) pngmask[nx*y + x0].g = 255;}
      else
	{ for( y = (y1 < 0 ? 0: y1); y<= (y0 > nx ? nx : y0); y++) pngmask[nx*y + x0].g = 255;}
    }
  /*Encode the image*/
  //  unsigned error = lodepng_encode32_file(fname, pngmask, imx, imy);
  unsigned error = lodepng_encode32_file(fname, (unsigned char*) pngmask, imx, imy);

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
	 if(imgfloat>=TARGET_INTENSITY) { imgval = 252; }
	 else { imgval = 0; }
	 if(dose==1.0) { pngimg[sizex*k + j].r = 0; pngimg[sizex*k + j].g = pngimg[sizex*k + j].b =  imgval; }
	 else if(dose==MAX_DOSE) { pngimg[sizex*k + j].r = 0; pngimg[sizex*k + j].g = pngimg[sizex*k + j].b =  imgval/2; }
	 else if(dose==MIN_DOSE) { pngimg[sizex*k + j].g = 0; pngimg[sizex*k + j].r = pngimg[sizex*k + j].b =  imgval/2; }
	 pngimg[sizex*k + j].a = 127;
       }
     }
  /*Encode the image*/
     //  unsigned error = lodepng_encode32_file(fname, pngimg, sizex, sizey);
  unsigned error = lodepng_encode32_file(fname, (unsigned char*) pngimg, sizex, sizey);

  /*if there's an error, display it*/
  if(error) fprintf(stderr, "*Error* %u: %s\n", error, lodepng_error_text(error));

  free(pngimg);
}

void write_pvband(float* inner_img, float* outer_img, char* fname, int sizex, int sizey,
		  int nx, int ny, int ox, int oy, int sc, 
		  rect * mskedata, int mskecnt,
		  rect * bedata, int becnt )
{
  /* This section dumps out the PV Band */
  ucharac4 *pngimg;
  pngimg = (ucharac4*)malloc(sizex * sizey * sizeof(ucharac4));
  // ========== image ================ //
  float in_img, out_img;
  int j, k;
  for(k=0; k<sizey; k++) {
    for(j=0; j<sizex; j++) {
      in_img  = inner_img[sizex*k + j];
      out_img = outer_img[sizex*k + j];
      pngimg[sizex*k + j].a = 255;
      pngimg[sizex*k + j].g = 0;
      // ========== pvband image ================ //
      pngimg[sizex*k + j].b =  0;
      if(out_img>=TARGET_INTENSITY && in_img<TARGET_INTENSITY)
	pngimg[sizex*k + j].b =  255;
      // ========== intensity image ================ //
      double iscore;
      if( out_img >= TARGET_INTENSITY)
	iscore = 220 + 10 * (int) ( 8 * log10( (double) (out_img / TARGET_INTENSITY) ) );
      else
	iscore = 180 + 10 * (int) ( 4 * log10( (double) (out_img / TARGET_INTENSITY) ) );
      pngimg[sizex*k + j].r = (iscore > 255 ? 255 : (iscore < 0 ? 0 : iscore ) );
    }
  }
  // ========== target image ================ //
  int i, x, y;
  for( i = 0; i<becnt; i++) {
    rect r = bedata[i];
    //    fprintf(stderr,"Read line (%d,%d) w=%d, h=%d\n",r.x,r.y,r.w,r.h);
    int x0 = (r.x / sc - ox);
    int y0 = (r.y / sc - oy);
    int x1 = x0 + r.w / sc;
    int y1 = y0 + r.h / sc;
    if( (!r.w && !r.h) || r.w && r.h ) {
      //      fprintf(stderr,"Error: Edge (%d, %d) to (%d,%d) is not HV\n",x0,y0,x1,y1);
      exit(0);
    }
    int p = ( (x0>x1?x0:x1) < 0) || ((x0<x1?x0:x1) >= nx) ||
            ( (y0>y1?y0:y1) < 0) || ((y0<y1?y0:y1) >= ny);
    if (!p) {
      //      fprintf(stderr,"Write line (%d,%d) w=%d, h=%d\n",x0,y0,x1,y1);
      if      ( r.w > 0 )
	{ for( x = (x0 < 0 ? 0: x0); x<= (x1 > nx ? nx : x1); x++) pngimg[nx*y0 + x].g = 255;}
      else if ( r.w < 0 )
	{ for( x = (x1 < 0 ? 0: x1); x<= (x0 > nx ? nx : x0); x++) pngimg[nx*y0 + x].g = 255;}
      else if ( r.h > 0 )
	{ for( y = (y0 < 0 ? 0: y0); y<= (y1 > nx ? nx : y1); y++) pngimg[nx*y + x0].g = 255;}
      else
	{ for( y = (y1 < 0 ? 0: y1); y<= (y0 > nx ? nx : y0); y++) pngimg[nx*y + x0].g = 255;}
    }
  }
  // ========== mask image ================ //
  for( i = 0; i<mskecnt; i++) {
    rect r = mskedata[i];
    //    fprintf(stderr,"Read line (%d,%d) w=%d, h=%d\n",r.x,r.y,r.w,r.h);
    int x0 = (r.x / sc - ox);
    int y0 = (r.y / sc - oy);
    int x1 = x0 + r.w / sc;
    int y1 = y0 + r.h / sc;
    if( (!r.w && !r.h) || r.w && r.h ) {
      //      fprintf(stderr,"Error: Edge (%d, %d) to (%d,%d) is not HV\n",x0,y0,x1,y1);
      exit(0);
    }
    int p = ( (x0>x1?x0:x1) < 0) || ((x0<x1?x0:x1) >= nx) ||
            ( (y0>y1?y0:y1) < 0) || ((y0<y1?y0:y1) >= ny);
    if (!p) {
      //      fprintf(stderr,"Write line (%d,%d) w=%d, h=%d\n",x0,y0,x1,y1);
      if      ( r.w > 0 )
	{ for( x = (x0 < 0 ? 0: x0); x<= (x1 > nx ? nx : x1); x++) pngimg[nx*y0 + x].b = 255;}
      else if ( r.w < 0 )
	{ for( x = (x1 < 0 ? 0: x1); x<= (x0 > nx ? nx : x0); x++) pngimg[nx*y0 + x].b = 255;}
      else if ( r.h > 0 )
	{ for( y = (y0 < 0 ? 0: y0); y<= (y1 > nx ? nx : y1); y++) pngimg[nx*y + x0].b = 255;}
      else
	{ for( y = (y1 < 0 ? 0: y1); y<= (y0 > nx ? nx : y0); y++) pngimg[nx*y + x0].b = 255;}
    }
  }
  /*Encode the image*/
  //	unsigned error = lodepng_encode32_file(fname, pngimg, sizex, sizey);
  unsigned error = lodepng_encode32_file(fname, (unsigned char*) pngimg, sizex, sizey);
  /*if there's an error, display it*/
  if(error) fprintf(stderr, "*Error* %u: %s\n", error, lodepng_error_text(error));
  free(pngimg);
}
