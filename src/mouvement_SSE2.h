#ifndef MOUVEMENT_SSE2_H
#define MOUVEMENT_SSE2_H

#include "mouvement.h"

	typedef struct image_SSE {
		int w;
		int h;
		int maxInt;
		vuint8 **data;
	}image_SSE;

	void initImageSSE(image_SSE* Image, int w, int h,int intensity);
	void copyImage_t_to_Image_SSE(image_t* imaget, image_SSE* imageSSE);
	void copyImage_SSE_to_Image_t(image_SSE* imageSSE,image_t* imaget);
	int readPGM_SSE(char* NomFichier, image_SSE* ImgRead);
	void writePGM_SSE(image_SSE* dif, int k, char* dossier);

	void FD_1_Step_SSE(image_SSE* ImageSSE1, image_SSE* ImageSSE2, image_SSE* dif);
	void FD_Full_Step_NO_Morpho_SSE();
	void FD_Full_Step_Morpho3_3_SSE();

	void SD_1_Step_SSE(image_SSE* ImgRead, image_SSE* Ot, image_SSE* Vt, image_SSE* Mt);
	void SD_Full_Step_NO_Morpho_SSE();
	void SD_Full_Step_Morpho3_3_SSE();
	void SD_Full_Step_Morpho5_5_SSE();

	void freeImageSSE(image_SSE* image);



#endif