#ifndef MOUVEMENT_SSE2_H
#define MOUVEMENT_SSE2_H

#include "mouvement.h"
#define _mm_cmpge_epu8(a, b) \
        _mm_cmpeq_epi8(_mm_max_epu8(a, b), a)

#define _mm_cmple_epu8(a, b) _mm_cmpge_epu8(b, a)

#define _mm_cmpgt_epu8(a, b) \
        _mm_xor_si128(_mm_cmple_epu8(a, b), _mm_set1_epi8(-1))

#define _mm_cmplt_epu8(a, b) _mm_cmpgt_epu8(b, a)

#define _mm_add_epi8_limit(a, b) \
    	_mm_max_epu8( _mm_or_si128(_mm_cmplt_epu8(_mm_add_epi8(a,b),a) , _mm_cmplt_epu8(_mm_add_epi8(a,b),b)) , _mm_add_epi8(a,b) )
    	// MAX[lt(add, a) OR lt(add,b)]    
#define _mm_sub_epi8_limit(a,b) \
    	_mm_min_epu8( _mm_or_si128(_mm_cmplt_epu8(_mm_sub_epi8(a,b),a) , _mm_cmplt_epu8(_mm_sub_epi8(a,b),b)) , _mm_sub_epi8(a,b) )
    	// MiIN[lt(sub, a) OR lt(sub,b)]




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
	void FD_Full_Step_Morpho5_5_SSE();
	

	void SD_1_Step_SSE(image_SSE* ImgRead, image_SSE* Ot, image_SSE* Vt, image_SSE* Mt);
	void SD_Full_Step_NO_Morpho_SSE();
	void SD_Full_Step_Morpho3_3_SSE();
	void SD_Full_Step_Morpho5_5_SSE();

	void freeImageSSE(image_SSE* image);



#endif