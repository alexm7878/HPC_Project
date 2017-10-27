
#ifndef MORPHO_H
#define MORPHO_H
#include "mouvement_SSE2.h"
#include "mouvement.h"

	void morpho_Erosion3_3(image_t* in, image_t* out);
	void morpho_Dilatation3_3(image_t* in, image_t* out);
	void morpho_Erosion5_5(image_t* in,image_t* out);
	void morpho_Dilatation5_5(image_t* in,image_t* out);

	void fermeture3_3(image_t* in, image_t* inter, image_t* out);
	void ouverture3_3(image_t* in, image_t* inter, image_t* out);
	void fermeture5_5(image_t* in, image_t* inter, image_t* out);
	void ouverture5_5(image_t* in, image_t* inter, image_t* out);


	//VERSION SDE

	void morpho_SSE_Erosion3_3(image_SEE* in, image_SEE* out);
	void morpho_SSE_Dilatation3_3(image_SEE* in, image_SEE* out);
	void fermeture_SSE3_3(image_SEE* in, image_SEE* inter, image_SEE* out);
	void ouverture_SSE3_3(image_SEE* in, image_SEE* inter, image_SEE* out);
	
	void morpho_SSE_Erosion5_5(image_SEE* in, image_SEE* out);
	void morpho_SSE_Dilatation5_5(image_SEE* in, image_SEE* out);
#endif
