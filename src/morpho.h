
#ifndef MORPHO_H
#define MORPHO_H

	void morpho_Erosion3_3(image_t* in, image_t* out);
	void morpho_Dilatation3_3(image_t* in, image_t* out);
	void morpho_Erosion5_5(image_t* in,image_t* out);
	void morpho_Dilatation5_5(image_t* in,image_t* out);

	void fermeture3_3(image_t* in, image_t* inter, image_t* out);
	void ouverture3_3(image_t* in, image_t* inter, image_t* out);
	void fermeture5_5(image_t* in, image_t* inter, image_t* out);
	void ouverture5_5(image_t* in, image_t* inter, image_t* out);

#endif