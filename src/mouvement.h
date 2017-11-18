#ifndef MOUVEMENT_H
#define MOUVEMENT_H

	




	
	#include "util.h"

	#define H 240
	#define W 320
	#define INTENSITY 255

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

	
	

	void FD_1_Step(image_t* ImgRead1, image_t* ImgRead, image_t* dif);

	
	void FD_Full_Step_NO_Morpho();
	void FD_Full_Step_Morpho3_3();
	void FD_Full_Step_Morpho5_5();

	void SD_1_step(image_t* ImgRead, image_t* Ot, image_t* Vt, image_t* Mt);
	void SD_Full_Step_NO_Morpho();
	void SD_Full_Step_Morpho3_3();
	void SD_Full_Step_Morpho5_5();



#endif