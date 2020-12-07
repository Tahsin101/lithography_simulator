/* A key/value dict system in C */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "kiss_fft.h"
#include "kiss_fftnd.h"

#define TEST TRUE /* Comment this line out to compile without a main function (used when including into another application). */



typedef struct dict_t_struct {
    char *key;
    kiss_fft_cpx *imgk;
    struct dict_t_struct *next;
} dict_t;

dict_t **dictAlloc(int dimx,int dimy ) {

    return malloc( dimx * dimy * sizeof(kiss_fft_cpx) );
}

void dictDealloc(dict_t **dict) {
    free(dict);
}

kiss_fft_cpx *getItem(dict_t *dict, char *key) { // get item gets the key
    dict_t *ptr;
    for (ptr = dict; ptr != NULL; ptr = ptr->next) {
        if (strcmp(ptr->key, key) == 0) {
            return ptr->imgk; //kiss fft cpt pointer
        }
    }
    kiss_fft_cpx *fail;
    fail->r=-1; fail->i=-1;
    return fail; // char pointer
}

void delItem(dict_t **dict, char *key) {
    dict_t *ptr, *prev;
                printf("inside del item\n");
    for (ptr = *dict, prev = NULL; ptr != NULL; prev = ptr, ptr = ptr->next) {
                printf("1st hello\n");
        if (strcmp(ptr->key, key) == 0) {
                printf("2nd hello\n");
            if (ptr->next != NULL) {
                printf("4\n");
                if (prev == NULL) {
                printf("1\n");
                    *dict = ptr->next;
                } else {
                printf("2\n");
                    prev->next = ptr->next;
                }
            } else if (prev != NULL) {
                printf("3\n");
                prev->next = NULL;
            } else {
                printf("4\n");
                *dict = NULL;
            }
            
            free(ptr->key);
            free(ptr->imgk);
            free(ptr);
            
            return;
        }
    }
}

void addItem(dict_t **dict, char *key, kiss_fft_cpx *imgk,int dimx,int dimy) {
    delItem(dict, key); /* If we already have a item with this key, delete it. */
    dict_t *d = malloc(sizeof(struct dict_t_struct));
    //printf("size of struct *d %zu byte",sizeof(struct dict_t_struct));
    d->key = malloc(strlen(key)+1);
    d->imgk=(kiss_fft_cpx*) malloc( dimx*dimy* sizeof(kiss_fft_cpx) );
    strcpy(d->key, key);
    int imxy =dimx*dimy;
    for(int i=0; i<imxy; i++) {//2048*2048
        d->imgk[i].r = imgk[i].r; // intensity real part
        d->imgk[i].i = imgk[i].i; // intensity imaginary part
    }
    d->next = *dict;
    *dict = d;
}

#ifdef TEST


kiss_fft_cpx *a,*b,*c,*aa,*bb,*cc;
int main(int argc, char **argv) {
    /* Create a dict */
    
    dict_t **dict = dictAlloc(1,3);//how many  fftcpx are there
    a	= (kiss_fft_cpx*) malloc( 1 * 1 * sizeof(kiss_fft_cpx) );
    b	= (kiss_fft_cpx*) malloc( 1 * 1 * sizeof(kiss_fft_cpx) );
    c	= (kiss_fft_cpx*) malloc( 1 * 1 * sizeof(kiss_fft_cpx) );
    /* lets add foo, and bar to the dict */
    a[0].r=10.5;
    a[0].i=.003;
    a[1].r=8.1;
    //a[1].i=.02;
    b[0].r=.998;
    b[0].i=11.9;
    //b[1].r=7001;
    //b[1].i=101;
    c[0].r=5.002;
    c[0].i=.078;
    //c[1].r=901;
    //c[1].i=.0045;
    //addItem(dict,"[10,10]",a,1,2); // first one is key ,next is val, this is built for one rectangle 
    addItem(dict,"[10,30]",c,1,2); // first one is key ,next is val, this is built for one rectangle 
    //addItem(dict, "bar", "foo");
      //delItem(dict, "[10,10]");
    /* and print their values */
    aa=(kiss_fft_cpx *)getItem(*dict, "[10,30]"); //get item takes key
    bb=(kiss_fft_cpx *)getItem(*dict, "[10,10]"); //get item takes key
    //cc=(kiss_fft_cpx *)getItem(*dict, "[10,10]"); //get item takes key
    printf("\nThis is C %f %f\n",aa[0].r,aa[0].i);
    printf("\nThis is B %f %f\n",bb->r,bb->i);
    //printf("\nThis is B %f %f",bb[0].r,bb[0].i);
    //printf("\nThis is B %f %f",bb[1].r,bb[1].i);
    //rintf("\nThis is A %f %f",cc[0].r,cc[0].i);
    //printf("\nThis is A %f %f",cc[1].r,cc[1].i);
    
    /* lets delete them */
  
    //delItem(dict, "bar");
    /* see, their gone, there NULL */

  
    
    
    ///* add them again to proof it works */
    //addItem(dict, "foo", "bar");
    //addItem(dict, "bar", "foo");
    //addItem(dict, "bar", "pan");
    
    ///* see, here */
    //printf("%s %s\n", (char *)getItem(*dict, "foo"), (char *)getItem(*dict, "bar"));
    
    //delItem(dict, "foo");
    //delItem(dict, "bar");
    
    dictDealloc(dict);
    
    return 0;
}

#endif