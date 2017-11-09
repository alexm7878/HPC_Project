#include "mouvement_SSE2.h"


/* NOTES A MOI-MEME

	add => pb si addition dépasse 255; retour au debut
		ex : 255 + 1 = 0 / 255 + 2 = 1   pas de bit n3?  0xff + 0x01  = 0x00 et non 0x100

	finir tableaux drive/oracle code

	faire fonction comparaison
*/


int comparaion_verif(__m128i val_comp, char **oracle){

}

int main(){


// INIT
	vuint8 resul_test[16];
	int i;
	__m128i test_cmplt;
	char *val_test_hexa;
/*
 Rappel :
 	0 => 0x00
 	1 => 0x01
 	127 => 0x7F
 	128 => 0x80
 	254 => 0xFE
 	255 => 0xFF	
*/

// Oracle
	char oracle[6][36] = {
							{0,0,0,0,0,0, 255,0,0,0,0,0, 255,255,0,0,0,0, 255,255,255,0,0,0, 255,255,255,255,0,0, 255,255,255,255,255,0},
							{}

						 };

	__m128i val = _mm_setr_epi8(0, 1, 127, 128, 254, 255,0,0,0,0,0,0,0,0,0,0);

	__m128i _0= _mm_setr_epi8(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
	__m128i _1= _mm_setr_epi8(1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1);
	__m128i _127= _mm_setr_epi8(127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127);
	__m128i _128= _mm_setr_epi8(128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128);
	__m128i _254= _mm_setr_epi8(254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254);
	__m128i _255= _mm_setr_epi8(255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255);


//_mm_cmplt_epu8 // 0xFF si a < b ; 0 sinon
	 /*
	 test_cmplt =  _mm_cmplt_epu8(val, _127);
	 //comparaion_verif(test_cmplt,oracle);
	 test_cmplt =  _mm_cmplt_epu8(val, _1); 
	 test_cmplt =  _mm_cmplt_epu8(val, _127); 
	 test_cmplt =  _mm_cmplt_epu8(val, _128); 
	 test_cmplt =  _mm_cmplt_epu8(val, _254); 
	 test_cmplt =  _mm_cmplt_epu8(val, _255); 

*/

	 test_cmplt =  _mm_add_epi8(val, _255); 
	// Affichage
	val_test_hexa = (char *) &test_cmplt;
/*
	if(val_test_hexa[2] == 127)
		printf("ok\n");
	else printf("non ok\n" );
*/
	for(i = 0; i < 6; i++) 
		printf("%04hhx ", val_test_hexa[i]);

	printf("\n");


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
	
