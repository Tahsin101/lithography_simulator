#include <stdlib.h>

#ifndef __AGSKRN_H__
#define __AGSKRN_H__

typedef struct agsKernel {
  int nk;
  int nkx;
  int nky;
  float *scale;
  //krn i's (i=0..nk-1) start at krn[i*nkx*nky]
  float *krn;
} agsKernel;

#endif

void swap4(void *x) {
  unsigned char t;
  unsigned char *y = x;
  t = y[0];
  y[0] = y[3];
  y[3] = t;
  t = y[1];
  y[1] = y[2];
  y[2] = t;
}

agsKernel *agsReadKernel(char *lyr) {

  char fname[1024];
  int i;
  //READ SCALES FILE
  sprintf(fname, "Kernels/%s/scales.txt", lyr); //lyr = M1OPC, name of kernel file

  FILE *F;

  F = fopen(fname, "r");

  if (F==NULL) {
    fprintf(stderr, "Kernel %s not found\n", fname);
    return NULL;
  }

  agsKernel *k = malloc(sizeof(agsKernel)); // creating k struct for agskernel
  fscanf(F, "%d\n", &(k->nk));

  k->scale = malloc(k->nk * sizeof(float));
  float d;
// 24 kernels, k->nk is 24. So deals with 24 kernels;
  for(i = 0; i<k->nk; i++) {
    fscanf(F, "%f", &d);
    k->scale[i] = d;
    //fprintf(stderr,"%s(scale %d) = %f\n", lyr, i, k->scale[i]);
  }
  close (F);

  int idata[5];
// reads 24 kernels from Kernel folder fh0 to fh23.
  for(i=0; i<k->nk; i++) {
    sprintf(fname, "Kernels/%s/fh%d.bin", lyr, i);
    F = fopen(fname, "r");
    fread(idata, 5, sizeof(int), F);
    swap4(idata);
    swap4(idata+1);
    swap4(idata+2);
    swap4(idata+3);
    swap4(idata+4);
    fprintf(stderr, "%s (%d %d %d %d %d)\n", fname, idata[0], idata[1], idata[2], idata[3], idata[4]);
    if (idata[2] != 2) fprintf(stderr, "Should be type 2 kernel not %d\n", idata[2]);
    if (idata[3] != i) fprintf(stderr, "Kernel %d should be %d in file also\n", i, idata[3]);
    if (i==0) {
      k->nkx = idata[0]; //35
      k->nky = idata[1]; //35
      k->krn = malloc(k->nk * k->nkx * k->nky * 2 * sizeof(float)); //24*35*35*2
    } else {
      if(idata[0] != k->nkx || idata[1] != k->nky)
	fprintf(stderr, "Kernel size mismatch in kernel %d (%d,%d) is not (%d %d)\n", i, idata[0], idata[1], k->nkx, k->nky);
    }
    fread(k->krn + i * k->nkx * k->nky * 2, k->nkx * k->nky * 2, sizeof(float), F);// reads 2450 values of size float and stores it in krn
    //first i=0 ; first kenrel, then 2nd kernel starts at

    close(F);
  }

  //fprintf(stderr, "Layer %s - kernel data = %d floats\n", k->nk * k->nkx * k->nky * 2);
  //FILE *fp;
  //fp=fopen("./results/kernval.csv","w+");
  for(i = 0; i< k->nk * k->nkx * k->nky * 2; i++) {
    swap4(k->krn+i);
    //fprintf(stderr, "%f ", k->krn[i]);
    
  }
  //fprintf(stderr,"\n");
  //fprintf(fp,"\n");
  //fclose(fp);
  return k;
}

//no fft done, reads from kernel ,stpred in memory. 35*35 points

