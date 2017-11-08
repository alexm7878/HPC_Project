
#include "mouvement_SSE2.h"


int main(){

// Tests SIMD

vuint8 test_max, val_min, val_max, val127, val128;
int val_test;

val_min =_mm_set1_epi8(0);
val_max = _mm_set1_epi8(255);
val127 = _mm_set1_epi8(127);
val128 = _mm_set1_epi8(128);


test_max = _mm_max_epu8(val_min, val_max);
val_test = _mm_store_si128(val_test,test_max);

if(val_test == val_max)
	printf("test val max basique --- OK");
else
	printf("test val max basique --- ERROR");






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
	
