#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "csvparser.h"
#include "kiss_fftnd.h"
#include "layoutrw.h"
#include <assert.h>
#include <time.h>

int MAX_FILE_NAME_LENGTH = 80;
float BACKGROUND_REAL = 0.0229;
float BACKGROUND_IMAG = -0.0217;
float MASK_REAL = 0.9771;
float MASK_IMAG = 0.0217;
float TARGET_INTENSITY = 0.225;
//float TARGET_INTENSITY = 0.02;
float MAX_DOSE = 1.02;
float MIN_DOSE = 0.98;
char INFILE[80];
char INFILE1[80];
char OUTFILE[80];
int ox = 0, oy = 0, sc = 1;
float *realPart, *imgPart;
float intensity;

rect *lvldata[MAXLVL];
rect *d_lvldata[MAXLVL];

#include "write_png.h"

#define myMin(a, b) ((a) <= (b) ? (a) : (b))
#define myMax(a, b) ((a) <= (b) ? (b) : (a))

kiss_fft_cpx *lookup_table;
int lseg = 20;
int imx = 2048;
int imy = 2048;
int imxy = 2048 * 2048;

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

/////////////////////////////////////

void myClearByte(unsigned char *msk)
{
  int i;
  for (i = 0; i < imxy; i++)
  {
    msk[i] = 0.0;
  }
}

void myClearFloat(float *msk)
{
  int i;
  for (i = 0; i < imxy; i++)
  {
    msk[i] = 0.0;
  }
}
void myClearComplex(kiss_fft_cpx *msk)
{
  int i;
  for (i = 0; i < imxy; i++)
  {
    msk[i].r = 0.0;
    msk[i].i = 0.0;
  }
}

typedef struct
{
  int x;
  int y;
} point;

rect shifting_module(point p, rect r)
{
  point shift;
  shift.x = 1024 - p.x;
  shift.y = 1024 - p.y;
  r.x = r.x + shift.x;
  r.y = r.y + shift.y;
  return r;
}

point *tap_point_x, *tap_point_y;

int segmentation_x(rect r)
{
  int seg = 0, numseg;
  int x0 = (r.x / sc - ox);
  int y0 = (r.y / sc - oy);

  int x1 = (x0 + r.w) / sc;
  int y1 = (y0 + r.h) / sc;

  int v_distance, h_distance;

  v_distance = abs(x0 - x1);
  h_distance = abs(y0 - y1);
  if (v_distance % lseg == 0)
  {
    seg = v_distance / lseg;
    numseg = seg * 2;
    tap_point_x = (point *)malloc(numseg * sizeof(point));
    int index = 0;
    int start_x, end_x, mid_x;
    for (int i = x0; i <= x1 - lseg; i = i + lseg)
    {
      start_x = i;
      end_x = i + lseg;
      mid_x = round((start_x + end_x) / 2);
      //printf("HELLO\n");
      tap_point_x[index].x = mid_x;
      tap_point_x[index].y = y0;
      index++;
      tap_point_x[index].x = mid_x;
      tap_point_x[index].y = y1;
      index++;
      //printf("tap point: %d %d index: %d\n",tap_point_x[index].x,tap_point_x[index].y,index);
    }
  }
  else
  {
    seg = v_distance / lseg;
    numseg = seg * 2;
    tap_point_x = (point *)malloc(numseg * sizeof(point));
    int num_loop = 0;
    if (seg % 2 != 0)
    {
      num_loop = seg / 2 + 1;
    }
    else
    {
      num_loop = seg / 2;
    }

    int front_start_x, front_end_x, front_mid_x;
    int back_start_x, back_end_x, back_mid_x;
    int front_index = 0, back_index = numseg - 1;
    for (int i = 0; i < num_loop; i++)
    {

      if (i == num_loop - 1 && seg % 2 != 0)
      {

        front_start_x = x0 + i * lseg;
        front_end_x = x0 + (i + 1) * lseg;
        front_mid_x = (front_start_x + front_end_x) / 2;
        tap_point_x[front_index].x = front_mid_x;
        tap_point_x[front_index].y = y0;
        front_index++;
        tap_point_x[front_index].x = front_mid_x;
        tap_point_x[front_index].y = y1;
        front_index++;
        //printf("front mid: %d %d\n", front_mid_x, y0);
        //printf("front mid: %d %d\n", front_mid_x, y0);
        if (abs(front_end_x - back_end_x) < lseg)
        {
          int temp = round(abs(front_end_x - back_end_x) / 2);
          front_end_x = front_end_x + temp;
          back_end_x = back_end_x - temp;
          front_mid_x = (front_start_x + front_end_x) / 2;
          back_mid_x = (back_start_x + back_end_x) / 2;
          tap_point_x[front_index - 1].x = front_mid_x;
          tap_point_x[front_index - 1].y = y0;
          tap_point_x[back_index + 1].x = back_mid_x;
          tap_point_x[back_index + 1].y = y0;
          front_index++;
          back_index--;
          tap_point_x[front_index - 1].x = front_mid_x;
          tap_point_x[front_index - 1].y = y1;
          tap_point_x[back_index + 1].x = back_mid_x;
          tap_point_x[back_index + 1].y = y1;
          front_index++;
          back_index--;
          //printf("front mid: %d %d\n", front_mid_x, y0);
          //printf("front mid: %d %d\n", front_mid_x, y0);
          //printf("back mid: %d %d\n", back_mid_x, y1);
          //printf("back mid: %d %d\n", back_mid_x, y1);
          //printf("%d\n", i);
        }
      }
      else
      {
        front_start_x = x0 + i * lseg;
        front_end_x = x0 + (i + 1) * lseg;
        front_mid_x = (front_start_x + front_end_x) / 2;
        back_start_x = x1 - i * lseg;
        back_end_x = x1 - (i + 1) * lseg;
        back_mid_x = (back_start_x + back_end_x) / 2;
        tap_point_x[front_index].x = front_mid_x;
        tap_point_x[front_index].y = y0;
        front_index++;
        tap_point_x[back_index].x = back_mid_x;
        tap_point_x[back_index].y = y0;
        back_index--;
        tap_point_x[front_index].x = front_mid_x;
        tap_point_x[front_index].y = y1;
        front_index++;
        tap_point_x[back_index].x = back_mid_x;
        tap_point_x[back_index].y = y1;
        back_index--;
        //printf("front mid: %d %d\n", front_mid_x, y0);
        //printf("front mid: %d %d\n", front_mid_x, y0);
        //printf("back mid: %d %d\n", back_mid_x, y1);
        //printf("back start %d , back end %d\n", back_start_x, back_end_x);
        //printf("back mid: %d %d\n", back_mid_x, y1);
        //printf("%d\n", i);
        ///////////////////////////////////////
        if (abs(front_end_x - back_end_x) < lseg)
        {

          int temp = round(abs(front_end_x - back_end_x) / 2);
          front_end_x = front_end_x + temp;
          back_end_x = back_end_x - temp;
          front_mid_x = (front_start_x + front_end_x) / 2;
          printf("start %d end %d mid %d\n", front_start_x, front_end_x, front_mid_x);
          back_mid_x = (back_start_x + back_end_x) / 2;
          printf("start %d end %d mid %d\n", back_start_x, back_end_x, back_mid_x);
          tap_point_x[front_index - 1].x = front_mid_x;
          tap_point_x[front_index - 1].y = y0;
          tap_point_x[back_index + 1].x = back_mid_x;
          tap_point_x[back_index + 1].y = y0;
          front_index++;
          back_index--;
          tap_point_x[front_index - 1].x = front_mid_x;
          tap_point_x[front_index - 1].y = y1;
          tap_point_x[back_index + 1].x = back_mid_x;
          tap_point_x[back_index + 1].y = y1;
          front_index++;
          back_index--;
          //printf("front mid: %d %d\n", front_mid_x, y0);
          //printf("front mid: %d %d\n", front_mid_x, y0);
          //printf("back mid: %d %d\n", back_mid_x, y1);
          //printf("back mid: %d %d\n", back_mid_x, y1);
          //printf("back start %d , back end %d\n", back_start_x, back_end_x);
          //printf("inside loop %d\n", i);
        }
      }
    }
  }

  return numseg;
}

void print_tap_point(point *tap_point, int seg)
{

  for (int i = 0; i < seg; i++)
  {
    printf("index %d x = %d y = %d\n", i, tap_point[i].x, tap_point[i].y);
  }
}
int segmentation_y(rect r)
{
  int seg = 0, numseg;
  int x0 = (r.x / sc - ox);
  int y0 = (r.y / sc - oy);

  int x1 = (x0 + r.w) / sc;
  int y1 = (y0 + r.h) / sc;

  int v_distance, h_distance;

  v_distance = abs(x0 - x1);
  h_distance = abs(y0 - y1);
  if (h_distance % lseg == 0)
  {
    seg = h_distance / lseg;
    numseg = seg * 2;
    tap_point_y = (point *)malloc(numseg * sizeof(point));
    int index = 0;
    int start_y, end_y, mid_y;
    for (int i = y0; i <= y1 - lseg; i = i + lseg)
    {
      start_y = i;
      end_y = i + lseg;
      mid_y = round((start_y + end_y) / 2);
      //printf("HELLO\n");
      tap_point_y[index].y = mid_y;
      tap_point_y[index].x = x0;
      index++;
      tap_point_y[index].y = mid_y;
      tap_point_y[index].x = x1;
      index++;
      //printf("tap point: %d %d index: %d\n",tap_point_y[index].x,tap_point_y[index].y,index);
    }
  }
  else
  {
    seg = h_distance / lseg;
    numseg = seg * 2;
    tap_point_y = (point *)malloc(numseg * sizeof(point));
    int num_loop = 0;
    if (seg % 2 != 0)
    {
      num_loop = seg / 2 + 1;
    }
    else
    {
      num_loop = seg / 2 + 1;
    }

    int front_start_y, front_end_y, front_mid_y;
    int back_start_y, back_end_y, back_mid_y;
    int front_index = 0, back_index = numseg - 1;
    for (int i = 0; i < num_loop; i++)
    {

      if (i == num_loop - 1 && seg % 2 != 0)
      {
        front_start_y = y0 + i * lseg;
        front_end_y = y0 + (i + 1) * lseg;
        front_mid_y = (front_start_y + front_end_y) / 2;
        tap_point_y[front_index].y = front_mid_y;
        tap_point_y[front_index].x = x0;
        front_index++;
        tap_point_y[front_index].y = front_mid_y;
        tap_point_y[front_index].x = x1;
        front_index++;
        //printf("front mid: %d %d\n", front_mid_y, x0);
        //printf("front mid: %d %d\n", front_mid_y, x1);
        if (abs(front_end_y - back_end_y) < lseg)
        {
          int temp = round(abs(front_end_y - back_end_y) / 2);
          front_end_y = front_end_y + temp;
          back_end_y = back_end_y - temp;
          front_mid_y = (front_start_y + front_end_y) / 2;
          back_mid_y = (back_start_y + back_end_y) / 2;
          tap_point_y[front_index - 1].y = front_mid_y;
          tap_point_y[front_index - 1].x = x0;
          tap_point_y[back_index + 1].y = back_mid_y;
          tap_point_y[back_index + 1].x = x0;
          front_index++;
          back_index--;
          tap_point_y[front_index - 1].y = front_mid_y;
          tap_point_y[front_index - 1].x = x1;
          tap_point_y[back_index + 1].y = back_mid_y;
          tap_point_y[back_index + 1].x = x1;
          front_index++;
          back_index--;
          //printf("front mid: %d %d\n", front_mid_y, x0);
          //printf("front mid: %d %d\n", front_mid_y, x0);
          //printf("back mid: %d %d\n", back_mid_y, x1);
          //printf("back mid: %d %d\n", back_mid_y, x1);
          //printf("%d\n", i);
        }
      }
      else
      {
        front_start_y = y0 + i * lseg;
        front_end_y = y0 + (i + 1) * lseg;
        front_mid_y = (front_start_y + front_end_y) / 2;
        back_start_y = y1 - i * lseg;
        back_end_y = y1 - (i + 1) * lseg;
        back_mid_y = (back_start_y + back_end_y) / 2;
        tap_point_y[front_index].y = front_mid_y;
        tap_point_y[front_index].x = x0;
        front_index++;
        tap_point_y[back_index].y = back_mid_y;
        tap_point_y[back_index].x = x0;
        back_index--;
        tap_point_y[front_index].y = front_mid_y;
        tap_point_y[front_index].x = x1;
        front_index++;
        tap_point_y[back_index].y = back_mid_y;
        tap_point_y[back_index].x = x1;
        back_index--;
        //printf("front mid: %d %d\n", front_mid_y, x0);
        //printf("front mid: %d %d\n", front_mid_y, x0);
        //printf("back mid: %d %d\n", back_mid_y, x1);
        //printf("back mid: %d %d\n", back_mid_y, x1);
        //printf("%d\n", i);
        if (abs(front_end_y - back_end_y) < lseg)
        {
          int temp = round(abs(front_end_y - back_end_y) / 2);
          front_end_y = front_end_y + temp;
          back_end_y = back_end_y - temp;
          front_mid_y = (front_start_y + front_end_y) / 2;
          back_mid_y = (back_start_y + back_end_y) / 2;
          tap_point_y[front_index - 1].y = front_mid_y;
          tap_point_y[front_index - 1].x = x0;
          tap_point_y[back_index + 1].y = back_mid_y;
          tap_point_y[back_index + 1].x = x0;
          front_index++;
          back_index--;
          tap_point_y[front_index - 1].y = front_mid_y;
          tap_point_y[front_index - 1].x = x1;
          tap_point_y[back_index + 1].y = back_mid_y;
          tap_point_y[back_index + 1].x = x1;
          front_index++;
          back_index--;
          //printf("front mid: %d %d\n", front_mid_y, x0);
          //printf("front mid: %d %d\n", front_mid_y, x0);
          //printf("back mid: %d %d\n", back_mid_y, x1);
          //printf("back mid: %d %d\n", back_mid_y, x1);
          //printf("%d\n", i);
        }
      }
    }
  }

  return numseg;
}

//float *look_up_table;
int load_table1()
{
  ////////////// START TILE INFORMATION//////////////////
  int start_tile, step;
  FILE *fp;
  fp = fopen(INFILE1, "rb");
  fread(&start_tile, 1, sizeof(start_tile), fp); // first information
  step = start_tile;
  printf("start tile,step = %d, %d\n", start_tile, step);
  int size = (2048 / start_tile);
  printf("size is %d\n", size * size);

  float *array;
  array= (float *)malloc(size * size * 2*sizeof(float));
  //look_up_table =(float*)malloc(size*2*sizeof(float));

  /************ LOOK UP TABLE FORMATION **********/
  fread(array, size * size * 2, sizeof(float), fp);
  fclose(fp);
  int index = 0;
  lookup_table = (kiss_fft_cpx *)malloc(size * size * sizeof(kiss_fft_cpx));
 //memset(lookup_table, 0, size * size * sizeof(kiss_fft_cpx));
  for (int j = 0; j < size; j = j + 1)
  { // j is X and i is Y
    for (int i = 0; i < size; i = i + 1)
    {
      lookup_table[j * size + i].r = array[index];
      index++;
      lookup_table[j * size + i].i = array[index];
      index++;
      //printf("index= %d,%d real,imag  : %lf,%lf\n", j, i, lookup_table[j * size + i].r, lookup_table[j * size + i].i);
    }
  }
  printf("Loading Done\n");
  free(array);
  return start_tile;
}

kiss_fft_cpx calc_intensity(rect r, point p, int start_tile)
{
  int count = 0;
  point shift;
  int factor = 0; // may b 5
  r = shifting_module(p, r);
  int step, numx0, numy0, numx1, numy1,numstep;
  step = start_tile;

  int x0 = (r.x) / (sc - ox);
  int y0 = (r.y) / (sc - oy);

  if (x0 > 2048)
  {
    x0 = 2048;
  }
  if (y0 > 2048)
  {
    y0 = 2048;
  }

  int x1 = (x0 + r.w) / sc;
  int y1 = (y0 + r.h) / sc;

  if (x1 > 2048)
  {
    x1 = 2048;
  }
  if (y1 > 2048)
  {
    y1 = 2048;
  }
  //printf("(%d,%d)-->(%d,%d)\n", x0, y0, x1, y1);

  numx0 = x0 / step;
  numy0 = y0 / step;
  numx1 = x1 / step;
  numy1 = y1 / step;
  numstep=2048/step;
  
  kiss_fft_cpx a;
  kiss_fft_cpx b;
  kiss_fft_cpx c;
  kiss_fft_cpx d;
  kiss_fft_cpx this_rect;

  //printf("index  is %d\n", (numx0-1)*numstep + (numy0-1));
  //printf("x0 = %d,y0=%d\n", numx0-1, numy0-1);
  //printf("index is %d\n", (numx1 - 1)*numstep + (numy0-1));
  //printf("x0 = %d,y1=%d\n", numx0-1, numy1-1);
  //printf("index is %d\n", (numx0-1)*numstep + (numy1 - 1));
  //printf("x1 = %d,y0=%d\n", numx1-1, numy0-1);
  //printf("index is %d\n", (numx1 - 1) * numstep + (numy1 - 1));
  //printf("x1 = %d,y1=%d\n", numx1-1, numy1-1);
  a.r = lookup_table[(numx0 - 1) * numstep + (numy0 - 1)].r;
  b.r = lookup_table[(numx0 - 1) * numstep + (numy1 - 1)].r;
  c.r = lookup_table[(numx1 - 1) * numstep + (numy0 - 1)].r;
  d.r = lookup_table[(numx1 - 1) * numstep + (numy1 - 1)].r;
  a.i = lookup_table[(numx0 - 1) * numstep + (numy0 - 1)].i;
  b.i = lookup_table[(numx0 - 1) * numstep + (numy1 - 1)].i;
  c.i = lookup_table[(numx1 - 1) * numstep + (numy0 - 1)].i;
  d.i = lookup_table[(numx1 - 1) * numstep + (numy1 - 1)].i;
  this_rect.r = (d.r - b.r - c.r + a.r);
  this_rect.i = (d.i - b.i - c.i + a.i);
  double total = 86.943428 * (this_rect.r * this_rect.r + this_rect.i * this_rect.i);
  //FILE * fp=fopen(OUTFILE,"a");
  //printf("a= %lf,%lf \nb = %lf,%lf\nc = %lf,%lf \nd = %lf,%lf\nsum = %lf,%lf \nIntensity= %lf\n", a.r, a.i, b.r, b.i, c.r, c.i, d.r, d.i, this_rect.r, this_rect.i, total);
  //fprintf(fp,"%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf\n",a.r,a.i,b.r,b.i,c.r,c.i,d.r,d.i,this_rect.r,this_rect.i,total);
  //fclose(fp);
  return this_rect;
}

int main(int argc, char **argv)
{
  int i, j, k;
  int dims[] = {2048, 2048};
  int ndims = 2;
  int isinverse = 1;
  int type = 1; //1 for psm and 0 for binary
  // Read in input and output file names
  if (argc < 2)
  {
    printf("*Error*: Must specify input GULP file name\n");
    exit(1);
  }
  else
  {
    strcpy(INFILE, argv[1]);
    if (argc < 3)
    {
      printf("*Error*: Must specify input BIN/CSV file name\n");
      exit(1);
    }
    strcpy(INFILE1, argv[2]);
    if (argc < 4)
    {
      printf("*Error*: Must specify output CSV file name\n");
      exit(1);
    }
    strcpy(OUTFILE, argv[3]);
  }
  int lyr = 0; // For single layer, it is always 0
  float dose = 1.0;
  // Parse the input file and assign layer name
  clear_design_space();
  read_layout(INFILE);
  char layername[] = "M1OPC";
  int numrect = lvlcnt[lyr];
  rect *devdata = lvldata[lyr];

  ///////////// Load LUT  /////////////////////
  struct timeval start, end;
  gettimeofday(&start, NULL);

  int step = load_table1();

  gettimeofday(&end, NULL);
  long seconds = (end.tv_sec - start.tv_sec);
  long micros = ((seconds * 1000000) + end.tv_usec) - (start.tv_usec);

  printf("load table took %ld seconds %ld micro seconds to execute \n", seconds, micros);
  /////////////////////////////////////////////
  //FILE *fp;
  //fp = fopen(OUTFILE, "w");
  //fprintf(fp, "a.r,a.i,b.r,b.i,c.r,c.i,d.r,d.i,mp real,mp imag,mpintensity\n");
  //fclose(fp);
  double total = 0, real = 0, imag = 0;

  gettimeofday(&start, NULL);
  
  int num_seg_x,num_seg_y;

  for(int i = 0; i < numrect; i++){
    rect r = devdata[i];
    num_seg_y=segmentation_y(r);
    print_tap_point(tap_point_y,num_seg_y);
    num_seg_x=segmentation_x(r);
    print_tap_point(tap_point_x,num_seg_x);
    printf("X tap point = %d Y tap point = %d\n",num_seg_x,num_seg_y);
  }
  
  kiss_fft_cpx *tap_point_y_amp,*tap_point_x_amp;
  double *tap_point_y_I,*tap_point_x_I;
  tap_point_y_amp = (kiss_fft_cpx*)malloc(num_seg_y*num_seg_y*sizeof(kiss_fft_cpx));
  tap_point_x_amp = (kiss_fft_cpx*)malloc(num_seg_x*num_seg_x*sizeof(kiss_fft_cpx));
  tap_point_y_I = (double*)malloc(num_seg_y*sizeof (double));
  tap_point_x_I = (double*)malloc(num_seg_x*sizeof(double));
  kiss_fft_cpx sum_rect;
  
 
  for(int j=0;j<num_seg_y;j++){
    sum_rect.r = 0.0;
    sum_rect.i = 0.0;
    for (int i = 0; i < numrect; i++){
      //printf("x=%d,y=%d j = %d \n\n",tap_point_y[j].x,tap_point_y[j].y,j);
      //printf("numrect= %d i=%d \n",numrect,i);
      rect r = devdata[i];
      tap_point_y_amp[j]=calc_intensity(r, tap_point_y[j], step);
      sum_rect.r = sum_rect.r + tap_point_y_amp[j].r;
      sum_rect.i = sum_rect.i + tap_point_y_amp[j].i;
    }
    tap_point_y_I[j] = 86.943428 * (sum_rect.r * sum_rect.r + sum_rect.i * sum_rect.i);
    printf("x=%d,y=%d real=%lf imag=%lf intensity=%lf\n",tap_point_y[j].x,tap_point_y[j].y,sum_rect.r,sum_rect.i,tap_point_y_I[j]);

  }
  printf("Y tap point done\n");

    for(int j=0;j<num_seg_x;j++){
    sum_rect.r = 0.0;
    sum_rect.i = 0.0;
    for (int i = 0; i < numrect; i++){
      //printf("Hello numrect= %d i=%d \n",numrect,i);
      rect r = devdata[i];
      tap_point_x_amp[j]=calc_intensity(r, tap_point_x[j], step);
      sum_rect.r = sum_rect.r + tap_point_x_amp[j].r;
      sum_rect.i = sum_rect.i + tap_point_x_amp[j].i;
    }
    tap_point_x_I[j] = 86.943428 * (sum_rect.r * sum_rect.r + sum_rect.i * sum_rect.i);
    printf("x=%d , y=%d  real=%lf  imag=%lf  intensity=%lf \n",tap_point_x[j].x,tap_point_x[j].y,sum_rect.r,sum_rect.i,tap_point_x_I[j]);

  }
  printf("X tap point done\n");
  free(tap_point_y_I);
  free(tap_point_x_I);
  free(tap_point_y_amp);
  free(tap_point_x_amp);
  //free(tap_point_y_I);
  //free(tap_point_x_I);
  

  // for center position intensity check
  /*
  kiss_fft_cpx sum_rect1;
  sum_rect1.r = 0.0;
  sum_rect1.i = 0.0;
  for (int i = 0; i < numrect; i++)
  {
    rect r = devdata[i];
    point p = {1024, 1024};
    kiss_fft_cpx this_rect = calc_intensity(r, p, step);
    sum_rect1.r = sum_rect1.r + this_rect.r;
    sum_rect1.i = sum_rect1.i + this_rect.i;

    // kiss_fft_cpx noise;
    //   noise.r=-0.0027;
    //   noise.i=-0.001869;
    //   int noise1=0;
    //   if(type){ //psm
    //     real=real + this_rect.r; //assumed formula
    //     imag=imag + this_rect.i;
    //     if(noise1){real=real+noise.r; imag=imag+noise.i;}
    //     else {real=real; imag=imag;}
    //     total=86.943428*(real*real+imag*imag);
    //     printf("PSM\n");
    //   }
    //   else{ //binary
    //     real=real + this_rect.r; //assumed formula
    //     imag=imag + this_rect.i;
    //     total=86.943428*(real*real+imag*imag);
    //     printf("BINARY\n");
    //   }
    //   if(i == numrect-1) {
    //   printf("\nthe intensity is %lf %lf %lf\n",real,imag,total);
    //   fp=fopen(OUTFILE,"a");
    //   fprintf(fp,",,,,,,,,%lf,%lf,%lf\n",real,imag,total);
    //   //fprintf(fp,"%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf\n",a[0].r,a[0].i,a[1].r,a[1].i,a[2].r,a[2].i,a[3].r,a[3].i,real,imag,total);
    //   fclose(fp); }
  }*/
  //double intensity = 0;
  //intensity = 86.943428 * (sum_rect1.r * sum_rect1.r + sum_rect1.i * sum_rect1.i);
  //printf("Intensity: %.9f real = %.9f Imag= %.9f\n", intensity, sum_rect1.r, sum_rect1.i);
  gettimeofday(&end, NULL);
  seconds = (end.tv_sec - start.tv_sec);
  micros = ((seconds * 1000000) + end.tv_usec) - (start.tv_usec);
  printf("simulator took %ld seconds %ld micro seconds to execute \n", seconds, micros);
  free(lookup_table);

}
