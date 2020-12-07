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
        printf("no match ,so fail\n");
    kiss_fft_cpx *fail;
    fail	= (kiss_fft_cpx*) malloc( 1* 1* sizeof(kiss_fft_cpx) );
    fail->r=-1; fail->i=-1;
    return fail; // char pointer
}
void delItem(dict_t **dict, char *key) {
    dict_t *ptr, *prev;
    for (ptr = *dict, prev = NULL; ptr != NULL; prev = ptr, ptr = ptr->next) {
        if (strcmp(ptr->key, key) == 0) {
            if (ptr->next != NULL) {
                if (prev == NULL) {
                    *dict = ptr->next;
                } else {
                    prev->next = ptr->next;
                }
            } else if (prev != NULL) {
                prev->next = NULL;
            } else {
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
    for(int i=0; i<imxy; i++) {
        d->imgk[i].r = imgk[i].r; // intensity real part
        d->imgk[i].i = imgk[i].i; // intensity imaginary part
    
    }
    d->next = *dict;
    *dict = d;
}


