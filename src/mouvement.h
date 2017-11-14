#ifndef MOUVEMENT_H
#define MOUVEMENT_H

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



	#define TETA 21
		
	#define VMAX 22
	#define VMIN 10
	#define N 7

	static uint8_t imaxarg1,imaxarg2;
	#define MAX(a,b) (imaxarg1=(a),imaxarg2=(b),(imaxarg1) > (imaxarg2) ?\
	        (imaxarg1) : (imaxarg2))
	    
	static uint8_t iminarg1,iminarg2;
	#define MIN(a,b) (iminarg1=(a),iminarg2=(b),(iminarg1) < (iminarg2) ?\
	        (iminarg1) : (iminarg2))

	typedef struct image_t {
		int w;
		int h;
		int maxInt;
		uint8_t** data;
	}image_t;


	uint8_t convLogicToBin(uint8_t t);
	uint8_t convBinToLogic(uint8_t t);
	void initImage_t(image_t* Image, int w, int h,int intensity);
	void freeImage_t(image_t* Image);
	int readPGM(char* NomFichier, image_t* ImgRead);
	void FD_1_Step(image_t* ImgRead1, image_t* ImgRead, image_t* dif);

	void Conc(char* nom,int i,char* ret);
	void cpy_Image(image_t* Mt, image_t* ImgRead);
	void writePGM(image_t* dif, int k, char* dossier);
	void FD_Full_Step_NO_Morpho();
	void FD_Full_Step_Morpho3_3();
	void FD_Full_Step_Morpho5_5();

	void SD_1_step(image_t* ImgRead, image_t* Ot, image_t* Vt, image_t* Mt);
	void SD_Full_Step_NO_Morpho();
	void SD_Full_Step_Morpho3_3();
	void SD_Full_Step_Morpho5_5();

	void compareImage(image_t* image1, image_t* image2);

#endif