#ifndef MOUVEMENT_SSE2_H
#define MOUVEMENT_SSE2_H

#include "mouvement.h"

	typedef struct image_SEE {
		int w;
		int h;
		int maxInt;
		vfloat32 **data;
	}image_SEE;

	void initImageSEE(image_SEE* Image, int w, int h,int intensity);
	void copyImage_t_to_Image_SEE(image_t* imaget, image_SEE* imageSEE);
	void copyImage_SEE_to_Image_t(image_SEE* imageSEE,image_t* imaget);



#endif