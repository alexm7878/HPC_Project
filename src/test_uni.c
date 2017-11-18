#include "mouvement_SSE2.h"





void affichage(__m128i val){
	int i;
	uint8_t *val_test = (uint8_t *) &val;
	
	for(i = 0; i < 6; i++){ 
		printf("%d ", val_test[i]);
	}
}


void test_unitaire_main()
{

// INIT
	vuint8 resul_test[16];
	int i;
	__m128i val_test,val_test2;
	char *val_test_hexa, *val_test_hexa2;
	/*
	 Rappel :
	 	0 => 0x00
 	1 => 0x01
 	127 => 0x7F
 	128 => 0x80
 	254 => 0xFE
 	255 => 0xFF	
	*/
	/*
	// Oracle
		 _mm_cmplt_epu8
		 _mm_cmpgt_epu8
		 _mm_add_epi8_limit
		 _mm_sub_epi8_limit
		 _mm_max_epu8
		 _mm_min_epu8

	*/
	 /*
	char oracle[6][36] = 
		{
			{0,0,0,0,0,0, 255,0,0,0,0,0, 255,255,0,0,0,0, 255,255,255,0,0,0, 255,255,255,255,0,0, 255,255,255,255,255,0},
			{0,255,255,255,255,255, 0,0,255,255,255,255, 0,0,0,255,255,255, 0,0,0,0,255,255, 0,0,0,0,0,255, 0,0,0,0,0,0},
			{0,1,127,128,254,255, 1,2,128,129,255,255, 127,128,254,255,255,255, 128,129,255,255,255,255, 254,255,255,255,255,255, 255,255,255,255,255,255},
			{0,1,127,128,254,255, 0,0,126,127,253,254, 0,0,0,1,127,128, 0,0,0,0,126,127, 0,0,0,0,0,1, 0,0,0,0,0,0},
			{0,1,127,128,254,255, 1,1,127,128,254,255, 127,127,127,128,254,255, 128,128,128,128,254,255, 254,254,254,254,254,255, 255,255,255,255,255,255},
			{0,0,0,0,0,0, 0,1,1,1,1,1, 0,1,127,127,127,127, 0,1,127,128,128,128, 0,1,127,128,254,254, 0,1,127,128,254,255}
		 };

	*/

	__m128i val = _mm_setr_epi8(0, 1, 127, 128, 254, 255,0,0,0,0,0,0,0,0,0,0);

	__m128i _0= _mm_setr_epi8(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
	__m128i _1= _mm_setr_epi8(1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1);
	__m128i _127= _mm_setr_epi8(127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127);
	__m128i _128= _mm_setr_epi8(128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128);
	__m128i _254= _mm_setr_epi8(254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254);
	__m128i _255= _mm_setr_epi8(255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255);


	printf("val = 0, 1, 127, 128, 254, 255\n\n");

	//_mm_cmplt_epu8 
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


	//_mm_cmpgt_epu8 
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

	//_mm_add_epi8_limit
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

// ......... suivant ..........
// ..............
// ............

}

int main(){



//test_unitaire_main();


	//chrono_Difference_FD();
	//chrono_Difference_morpho3_3_dilatation();

	//chrono_Difference_morpho3_3_erosion();
	//chrono_Difference_morpho5_5_erosion();
	//chrono_Difference_morpho5_5_dilatation();
	//chrono_Difference_SD();




	SD_Full_Step_NO_Morpho_SSE();
	/*chrono_Difference_SD();
	SD_Full_Step_NO_Morpho_SSE();
	SD_Full_Step_NO_Morpho();
	//FD_Full_Step_Morpho5_5();
	//chrono_Difference_morpho5_5_dilatation();

int MROC[2][2]={0,0,0,0};
image_t image,verite,image2;

readPGM("SDSSE/SDSSEcar_99.pgm",&image);
readPGM("SD/SDcar_99.pgm",&image2);
readPGM("Verite/car_100.pgm",&verite);*/
//	roc(&image,&verite,MROC);
//	displayROC(MROC);
	


	//FD_Full_Step_Morpho5_5();

	//compareImage(&image,&image2);
	return 0;
}



/*
	image_t ImgRead;
	image_t ImgRead1;
	image_t dif;
	int i;
	char nomFichier[50] ="";
	char nomFichier2[50] ="";

	readPGM("car3/car_3000.pgm",&ImgRead);
		
	initImage_t(&dif,ImgRead.w,ImgRead.h,ImgRead.maxInt);



	for(i=0;i<199;i++){
		Conc("car3/car_",3000+i,nomFichier);
		Conc("car3/car_",3000+i+1,nomFichier2);

		readPGM(nomFichier,&ImgRead);
			//printf("L'image a bien été lu\n ");
		readPGM(nomFichier2,&ImgRead1);
			//printf("L'image a bien été lu\n ");

		FD_1_Step(&ImgRead1,&ImgRead,&dif);
		writePGM(&dif,i,"FD/FDcar_");
	}
*/






/*image_SSE in,out;
readPGM_SSE("car3/car_3100.pgm",&in);
	morpho_SSE_Erosion3_3( &in,  &out);*/
	
	//FD_Full_Step_Morpho3_3_SSE();
	//chrono(FD_Full_Step_Morpho3_3_SSE);
	
	//FD_Full_Step_NO_Morpho_SSE();


//FD SANS OPTI

//FD_Full_Step_NO_Morpho();

//FD_Full_Step_Morpho3_3();

//FD_Full_Step_Morpho5_5();


//SD SANS OPTI

//SD_Full_Step_NO_Morpho();

//SD_Full_Step_Morpho3_3();

//SD_Full_Step_Morpho5_5();


//FD_OPTI

//FD_Full_Step_NO_Morpho_SSE();
//SD_Full_Step_NO_Morpho_SSE();
//FD_Full_Step_Morpho3_3_SSE();
//SD_Full_Step_Morpho3_3_SSE();

//SD OPTI
//SD_Full_Step_NO_Morpho_SSE();


  //main_Bench_FD();
  
  //main_Bench_SD();


// BENCH MARQUE DE CHAQUE FONCTION 





//chrono(SD_Full_Step_NO_Morpho_SSE);
//chrono(FD_Full_Step_Morpho3_3_SSE);
/*
printf("\n FD\n");
chrono(FD_Full_Step_NO_Morpho);
chrono(FD_Full_Step_Morpho3_3);
chrono(FD_Full_Step_Morpho5_5);
//chrono(FD_Full_Step_NO_Morpho_SSE);
//chrono(FD_Full_Step_Morpho3_3_SSE);
//chrono(FD_Full_Step_Morpho5_5_SSE);
printf("\n SD\n");
chrono(SD_Full_Step_NO_Morpho);
chrono(SD_Full_Step_Morpho3_3);
chrono(SD_Full_Step_Morpho5_5);
//chrono(SD_Full_Step_NO_Morpho);
//chrono(SD_Full_Step_Morpho3_3_SSE);
//chrono(SD_Full_Step_Morpho5_5_SSE);
*/


/*
int MROC[2][2]={0,0,0,0};
image_t image,verite;

readPGM("FDSSE_Morpho3_3/FDSSEcar_99.pgm",&image);
readPGM("Verite/car_100.pgm",&verite);
	roc(&image,&verite,MROC);
	displayROC(MROC);	
*/
//chrono(FD_Full_Step_Morpho3_3_SSE);
	
