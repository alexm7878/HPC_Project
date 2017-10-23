/* ----------------- */
/* --- ppm2pgm.c --- */
/* ----------------- */

#include <math.h>

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>

#include "nrdef.h"
#include "nrutil.h"
#include "ppm2pgm.h"

// seuillage d'image de verite-terrain
// input format: ppm (24 bits) avec zone a garder en vert(R=0, G=255, B=0)
// output format: pgm (8 bits) avec zone a garder en blanc (255) et le reste en noir (0)

// -----------------------------------
int green_thresold(char *filename_ppm)
// -----------------------------------
{
    rgb8 **X, x; // input
    uint8 **Y, y; // output
    int i,j;
    long i0, i1, j0, j1;
    int height, width;
    int t0, t1; // low and high threshold;
    
    char* filename_pgm;
    
    // input and malloc
    printf("opening %s\n", filename_ppm);
    X =  LoadPPM_rgb8matrix(filename_ppm, &i0, &i1, &j0, &j1);
    height = i1 - i0 + 1;
    width  = j1 - j0 + 1;
    //printf("[%d %d]x[%d %d]\n", i0, i1, j0, j1);
    printf("size = %d x %d\n", width, height);
    Y = ui8matrix(i0, i1, j0, j1);
     
    // set param
    t0 = 10;
    t1 = 255 - t0;
    
    // threshold (t0, t1)
    for(i = i0; i <= i1; i++) {
        for(j = j0; j <= j1; j++) {
            //printf("i = %d j = %d\n", i, j);
            if((X[i][j].r < t0) && (X[i][j].g > t1) && (X[i][j].b < t0)) {
                y = 255;
            } else {
                y = 0;
            }
            Y[i][j] = y;
        }
    }
    
    // save to disk with default name
    int len = strlen(filename_ppm);
    //printf("len = %d\n", len);
    filename_pgm = (char*) malloc((len+1) * sizeof(char));
    if(!filename_pgm) {
        printf("cannot allocate mem to copy %s\n", filename_ppm);
        exit(-1);
    }
    
    // replace .ppm by pgm by inserting 'g' at the right place (not foolproof at all)
    strcpy(filename_pgm, filename_ppm);
    filename_pgm[len] = 0; // NULL-terminated
    filename_pgm[len-2] = 'g';
    printf("filename_pgm = %s\n", filename_pgm);
    
    SavePGM_ui8matrix(Y, i0, i1, j0, j1, filename_pgm);
    //SavePGM_ui8matrix(Y, i0, i1, j0, j1, "verif_pgm");
    
    // free
    free_ui8matrix (Y, i0, i1, j0, j1);
    free_rgb8matrix(X, i0, i1, j0, j1);
    
    return 0;
}

