#include "mouvement_SSE2.h"


void affichage(__m128i val){
	int i;
	uint8_t *val_test = (uint8_t *) &val;
	
	for(i = 0; i < 6; i++){ 
		printf("%d ", val_test[i]);
	}
}


void test_mm_cmplt_epu8(__m128i val,__m128i _0,__m128i _1,__m128i _127,__m128i _128,__m128i _254,__m128i _255){
	__m128i val_test;

	printf("===== Test _mm_cmplt_epu8 =====\n");
	 val_test =  _mm_cmplt_epu8(val, _0);
	 	printf("_mm_cmplt_epu8(val, _0) ");
	 	affichage(val_test);
	 	printf("\n");

	 val_test =  _mm_cmplt_epu8(val, _1); 
	 	printf("_mm_cmplt_epu8(val, _1) ");
	 	affichage(val_test);
	 	printf("\n");

	 val_test =  _mm_cmplt_epu8(val, _127); 
	 	printf("_mm_cmplt_epu8(val, _127) ");
	 	affichage(val_test);
	 	printf("\n");

	 val_test =  _mm_cmplt_epu8(val, _128); 
		printf("_mm_cmplt_epu8(val, _128) ");
	 	affichage(val_test);
	 	printf("\n");

	 val_test =  _mm_cmplt_epu8(val, _254); 
	 	printf("_mm_cmplt_epu8(val, _254) ");
	 	affichage(val_test);
	 	printf("\n");

	 val_test =  _mm_cmplt_epu8(val, _255); 	 	
	 	printf("_mm_cmplt_epu8(val, _255) ");
	 	affichage(val_test);
	 	printf("\n");

 	printf("\n");	
}

void test_mm_cmpgt_epu8(__m128i val,__m128i _0,__m128i _1,__m128i _127,__m128i _128,__m128i _254,__m128i _255){
	__m128i val_test;

	printf("===== Test _mm_cmpgt_epu8 =====\n");
	 val_test =  _mm_cmpgt_epu8(val, _0);
	 	printf("_mm_cmpgt_epu8(val, _0) ");
	 	affichage(val_test);
	 	printf("\n");

	 val_test =  _mm_cmpgt_epu8(val, _1); 
	 	printf("_mm_cmpgt_epu8(val, _1) ");
	 	affichage(val_test);
	 	printf("\n");

	 val_test =  _mm_cmpgt_epu8(val, _127); 
	 	printf("_mm_cmpgt_epu8(val, _127) ");
	 	affichage(val_test);
	 	printf("\n");

	 val_test =  _mm_cmpgt_epu8(val, _128); 
		printf("_mm_cmpgt_epu8(val, _128) ");
	 	affichage(val_test);
	 	printf("\n");

	 val_test =  _mm_cmpgt_epu8(val, _254); 
	 	printf("_mm_cmpgt_epu8(val, _254) ");
	 	affichage(val_test);
	 	printf("\n");

	 val_test =  _mm_cmpgt_epu8(val, _255); 	 	
	 	printf("_mm_cmpgt_epu8(val, _255) ");
	 	affichage(val_test);
	 	printf("\n");
	 printf("\n");
}

void test_mm_add_epi8_limit(__m128i val,__m128i _0,__m128i _1,__m128i _127,__m128i _128,__m128i _254,__m128i _255){
	__m128i val_test;

	printf("===== Test _mm_add_epi8_limit =====\n");
	 val_test =  _mm_add_epi8_limit(val, _0);
	 	printf("_mm_add_epi8_limit(val, _0) ");
	 	affichage(val_test);
	 	printf("\n");

	 val_test =  _mm_add_epi8_limit(val, _1); 
	 	printf("_mm_add_epi8_limit(val, _1) ");
	 	affichage(val_test);
	 	printf("\n");

	 val_test =  _mm_add_epi8_limit(val, _127); 
	 	printf("_mm_add_epi8_limit(val, _127) ");
	 	affichage(val_test);
	 	printf("\n");

	 val_test =  _mm_add_epi8_limit(val, _128); 
		printf("_mm_add_epi8_limit(val, _128) ");
	 	affichage(val_test);
	 	printf("\n");

	 val_test =  _mm_add_epi8_limit(val, _254); 
	 	printf("_mm_add_epi8_limit(val, _254) ");
	 	affichage(val_test);
	 	printf("\n");

	 val_test =  _mm_add_epi8_limit(val, _255); 	 	
	 	printf("_mm_add_epi8_limit(val, _255) ");
	 	affichage(val_test);
	 	printf("\n");

	printf("\n");
}

void test_mm_sub_epi8_limit(__m128i val,__m128i _0,__m128i _1,__m128i _127,__m128i _128,__m128i _254,__m128i _255){
	__m128i val_test;

	printf("===== Test _mm_sub_epi8_limit =====\n");
	 val_test =  _mm_sub_epi8_limit(val, _0);
	 	printf("_mm_sub_epi8_limit(val, _0) ");
	 	affichage(val_test);
	 	printf("\n");

	 val_test =  _mm_sub_epi8_limit(val, _1); 
	 	printf("_mm_sub_epi8_limit(val, _1) ");
	 	affichage(val_test);
	 	printf("\n");

	 val_test =  _mm_sub_epi8_limit(val, _127); 
	 	printf("_mm_sub_epi8_limit(val, _127) ");
	 	affichage(val_test);
	 	printf("\n");

	 val_test =  _mm_sub_epi8_limit(val, _128); 
		printf("_mm_sub_epi8_limit(val, _128) ");
	 	affichage(val_test);
	 	printf("\n");

	 val_test =  _mm_sub_epi8_limit(val, _254); 
	 	printf("_mm_sub_epi8_limit(val, _254) ");
	 	affichage(val_test);
	 	printf("\n");

	 val_test =  _mm_sub_epi8_limit(val, _255); 	 	
	 	printf("_mm_sub_epi8_limit(val, _255) ");
	 	affichage(val_test);
	 	printf("\n");

	printf("\n");
}

void test_mm_max_epu8(__m128i val,__m128i _0,__m128i _1,__m128i _127,__m128i _128,__m128i _254,__m128i _255){
	__m128i val_test;

	printf("===== Test _mm_max_epu8 =====\n");
	 val_test =  _mm_max_epu8(val, _0);
	 	printf("_mm_max_epu8(val, _0) ");
	 	affichage(val_test);
	 	printf("\n");

	 val_test =  _mm_max_epu8(val, _1); 
	 	printf("_mm_max_epu8(val, _1) ");
	 	affichage(val_test);
	 	printf("\n");

	 val_test =  _mm_max_epu8(val, _127); 
	 	printf("_mm_max_epu8(val, _127) ");
	 	affichage(val_test);
	 	printf("\n");

	 val_test =  _mm_max_epu8(val, _128); 
		printf("_mm_max_epu8(val, _128) ");
	 	affichage(val_test);
	 	printf("\n");

	 val_test =  _mm_max_epu8(val, _254); 
	 	printf("_mm_max_epu8(val, _254) ");
	 	affichage(val_test);
	 	printf("\n");

	 val_test =  _mm_max_epu8(val, _255); 	 	
	 	printf("_mm_max_epu8(val, _255) ");
	 	affichage(val_test);
	 	printf("\n");

	printf("\n");
}

void test_mm_min_epu8(__m128i val,__m128i _0,__m128i _1,__m128i _127,__m128i _128,__m128i _254,__m128i _255){
	__m128i val_test;

	printf("===== Test _mm_min_epu8 =====\n");
	 val_test =  _mm_min_epu8(val, _0);
	 	printf("_mm_min_epu8(val, _0) ");
	 	affichage(val_test);
	 	printf("\n");

	 val_test =  _mm_min_epu8(val, _1); 
	 	printf("_mm_min_epu8(val, _1) ");
	 	affichage(val_test);
	 	printf("\n");

	 val_test =  _mm_min_epu8(val, _127); 
	 	printf("_mm_min_epu8(val, _127) ");
	 	affichage(val_test);
	 	printf("\n");

	 val_test =  _mm_min_epu8(val, _128); 
		printf("_mm_min_epu8(val, _128) ");
	 	affichage(val_test);
	 	printf("\n");

	 val_test =  _mm_min_epu8(val, _254); 
	 	printf("_mm_min_epu8(val, _254) ");
	 	affichage(val_test);
	 	printf("\n");

	 val_test =  _mm_min_epu8(val, _255); 	 	
	 	printf("_mm_min_epu8(val, _255) ");
	 	affichage(val_test);
	 	printf("\n");

	printf("\n");
}


int main(){

	__m128i val = _mm_setr_epi8(0, 1, 127, 128, 254, 255,0,0,0,0,0,0,0,0,0,0);

	__m128i _0= _mm_setr_epi8(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
	__m128i _1= _mm_setr_epi8(1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1);
	__m128i _127= _mm_setr_epi8(127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127);
	__m128i _128= _mm_setr_epi8(128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128);
	__m128i _254= _mm_setr_epi8(254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254);
	__m128i _255= _mm_setr_epi8(255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255);

	
	//printf("val = 0, 1, 127, 128, 254, 255\n\n");

	//test_mm_cmplt_epu8(val, _0, _1, _127, _128, _254, _255);
	//test_mm_cmpgt_epu8(val, _0, _1, _127, _128, _254, _255);
	//test_mm_add_epi8_limit(val, _0, _1, _127, _128, _254, _255);
	//test_mm_sub_epi8_limit(val, _0, _1, _127, _128, _254, _255);
	//test_mm_max_epu8(val, _0, _1, _127, _128, _254, _255);
	//test_mm_min_epu8(val, _0, _1, _127, _128, _254, _255);


	//chrono_Difference_FD();
	//chrono_Difference_morpho3_3_dilatation();

	//chrono_Difference_morpho3_3_erosion();
	//chrono_Difference_morpho5_5_erosion();
	//chrono_Difference_morpho5_5_dilatation();
	//chrono_Difference_SD();

	//FD_Full_Step_NO_Morpho();
	//FD_Full_Step_Morpho3_3();
	//FD_Full_Step_Morpho5_5();

	//SD_Full_Step_NO_Morpho();
	//SD_Full_Step_Morpho3_3();
	//SD_Full_Step_Morpho5_5();

	FD_Full_Step_NO_Morpho_SSE();
	//FD_Full_Step_Morpho3_3_SSE();
	//FD_Full_Step_Morpho5_5_SSE();

	//SD_Full_Step_NO_Morpho_SSE();
	//SD_Full_Step_Morpho3_3_SSE();
	//SD_Full_Step_Morpho5_5_SSE();


	/*
	int MROC[2][2]={0,0,0,0};
	image_t verite, image, image2;

	readPGM("SDSSE/SDSSEcar_99.pgm",&image);
	readPGM("SD/SDcar_99.pgm",&image2);
	//readPGM("Verite/car_100.pgm",&verite);
	//roc(&image,&verite,MROC);
	//displayROC(MROC);

	compareImage(&image,&image2);
	*/


	return 0;
}
