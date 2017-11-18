#ifndef UTIL_H
#define UTIL_H

	#include <stdio.h>
	#include <stdlib.h>

	#include <string.h>
	#include <stdint.h>
	#include <math.h>
	#include <ctype.h> 
	#include <time.h>

	#include "nrdef.h"
	#include "vnrdef.h"
	#include "nrutil.h"


typedef struct image_t {
		int w;
		int h;
		int maxInt;
		uint8_t** data;
	}image_t;


typedef struct image_SSE {
		int w;
		int h;
		int maxInt;
		vuint8 **data;
	}image_SSE;

	#include "mouvement.h"
	#include "mouvement_SSE2.h"

	

	uint8_t convLogicToBin(uint8_t t);
	uint8_t convBinToLogic(uint8_t t);
	void initImage_t(image_t* Image);
	void freeImage_t(image_t* Image);
	int readPGM(char* NomFichier, image_t* ImgRead);

	void Conc(char* nom,int i,char* ret);
	void cpy_Image(image_t* Mt, image_t* ImgRead);
	void writePGM(image_t* dif, int k, char* dossier);
	void ConcPPM(char* nom,int i,char* ret);
	void writePPM(image_t* dif, int k, char* dossier);

	void setVal_image(image_t* img, int val);
	void compareImage(image_t* image1, image_t* image2);

	void initImageSSE(image_SSE* Image);
	void freeImageSSE(image_SSE* image);
	
	void copyImage_t_to_Image_SSE(image_t* imaget, image_SSE* imageSSE);
	void copyImage_SSE_to_Image_t(image_SSE* imageSSE,image_t* imaget);
	int readPGM_SSE(char* NomFichier, image_SSE* ImgRead);
	vuint8 conv_simd_logic_bin(vuint8 t);
	void cpySSE(image_SSE* in, image_SSE* out);
	void writePGM_SSE(image_SSE* dif, int k, char* dossier);
	void setVal_image_SSE(image_SSE* img, int val);

#endif