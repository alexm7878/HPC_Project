#ifndef MOUVEMENT_SSE2_H
#define MOUVEMENT_SSE2_H

#include "mouvement.h"

	typedef struct image_SEE {
		int w;
		int h;
		int maxInt;
		vuint8 **data;
	}image_SEE;

	void initImageSEE(image_SEE* Image, int w, int h,int intensity);
	void copyImage_t_to_Image_SEE(image_t* imaget, image_SEE* imageSEE);
	void copyImage_SEE_to_Image_t(image_SEE* imageSEE,image_t* imaget);
	int readPGM_SEE(char* NomFichier, image_SEE* ImgRead);
	void writePGM_SEE(image_SEE* dif, int k, char* dossier);

	void FD_1_Step_SEE(image_SEE* ImageSEE1, image_SEE* ImageSEE2, image_SEE* dif);
	void FD_Full_Step_NO_Morpho_SEE();

	void freeImageSEE(image_SEE* image);



#endif