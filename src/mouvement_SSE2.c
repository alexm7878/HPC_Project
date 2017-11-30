
#include "mouvement_SSE2.h"





void FD_1_Step_SSE(image_SSE* ImageSSE1, image_SSE* ImageSSE2, image_SSE* Ot)
{
	int i,j;
	int c = card_vuint8();

	vuint8  it, it_1, ot, zero, valMax, teta, max, min, sl;
 	zero =_mm_set1_epi8(0);
 	valMax = _mm_set1_epi8(255);
	teta = _mm_set1_epi8(TETA);	

	for(i=0;i<H;i++){
		for(j=0;j<W/c;j++){

			// initialisation
			it = _mm_load_si128(&ImageSSE2->data[i][j]);
			it_1 = _mm_load_si128(&ImageSSE1->data[i][j]);
			
			// STEP 1
			max = _mm_max_epu8(it, it_1);
           	min = _mm_min_epu8(it, it_1);
            
            ot = _mm_sub_epi8_limit(max, min);

            // STEP 2
            ot = _mm_cmpgt_epu8(ot,teta);
            //sl = _mm_cmplt_epu8(ot,teta);
            //ot = _mm_or_si128(_mm_andnot_si128(sl,valMax), zero);

            _mm_store_si128(&Ot->data[i][j],ot);
		}
	}
}


void FD_Full_Step_NO_Morpho_SSE()
{
	
	image_t ImgRead;
	image_t ImgRead1;
	int i;
	char nomFichier[50] ="";
	char nomFichier2[50] ="";

	image_SSE ImageSSE1,ImageSSE2, Ot;


	initImageSSE(&Ot);

	for(i=0;i<199;i++){
		Conc("car3/car_",3000+i,nomFichier);
		Conc("car3/car_",3000+i+1,nomFichier2);

		readPGM_SSE(nomFichier,&ImageSSE1);
		readPGM_SSE(nomFichier2,&ImageSSE2);

		FD_1_Step_SSE(&ImageSSE1,&ImageSSE2,&Ot);

		writePGM_SSE(&Ot,i,"FDSSE/FDSSEcar_");

		freeImageSSE(&ImageSSE1);
		freeImageSSE(&ImageSSE2);

	}

	freeImageSSE(&Ot);
	//printf("Fin FD sans morpho\n");
	//freeImage_t(&ImgRead);
	//freeImageSSE(&ImageSSE1);
	//freeImageSSE(&ImageSSE2);
	//freeImageSSE(&dif);

}

void FD_Full_Step_Morpho3_3_SSE()
{
	
	image_t ImgRead;
	image_t ImgRead1;
	int i;
	char nomFichier[50] ="";
	char nomFichier2[50] ="";

	image_SSE ImageSSE1,ImageSSE2, Ot, out, inter;

	initImageSSE(&Ot);
	initImageSSE(&out);
	initImageSSE(&inter);

	for(i=0;i<199;i++){
		Conc("car3/car_",3000+i,nomFichier);
		Conc("car3/car_",3000+i+1,nomFichier2);

		readPGM_SSE(nomFichier,&ImageSSE1);
			//printf("L'image a bien été lu\n ");
		readPGM_SSE(nomFichier2,&ImageSSE2);
			//printf("L'image a bien été lu\n ");

		FD_1_Step_SSE(&ImageSSE1,&ImageSSE2,&Ot);
 
		//fermeture_SSE3_3(&Ot,&inter,&out);
		//ouverture_SSE3_3(&out,&inter,&Ot);
		morpho_SSE_Erosion3_3(&Ot,&out);
		writePGM_SSE(&out,i,"FDSSE_Morpho3_3/FDSSEcar_");

		freeImageSSE(&ImageSSE1);
		freeImageSSE(&ImageSSE2);
	}

	freeImageSSE(&Ot);
	freeImageSSE(&out);
	freeImageSSE(&inter);

	//printf("Fin FD sans morpho\n");
	//freeImage_t(&ImgRead);
	//freeImageSSE(&ImageSSE1);
	//freeImageSSE(&ImageSSE2);
	//freeImageSSE(&dif);

}

void FD_Full_Step_Morpho5_5_SSE()
{

	image_t ImgRead;
	image_t ImgRead1;
	int i;
	char nomFichier[50] ="";
	char nomFichier2[50] ="";

	image_SSE ImageSSE1,ImageSSE2, Ot, out, inter;

	
	initImageSSE(&Ot);
	initImageSSE(&out);
	initImageSSE(&inter);

	for(i=0;i<199;i++){
		Conc("car3/car_",3000+i,nomFichier);
		Conc("car3/car_",3000+i+1,nomFichier2);

		readPGM_SSE(nomFichier,&ImageSSE1);
		readPGM_SSE(nomFichier2,&ImageSSE2);

		FD_1_Step_SSE(&ImageSSE1,&ImageSSE2,&Ot);

		morpho_SSE_Dilatation5_5_reducColumn(&Ot,&out);
		writePGM_SSE(&out,i,"FDSSE_Morpho5_5/FDSSEcar_");

		freeImageSSE(&ImageSSE1);
		freeImageSSE(&ImageSSE2);
	}

	freeImageSSE(&Ot);
	freeImageSSE(&out);
	freeImageSSE(&inter);
	//freeImage_t(&ImgRead);
	//freeImageSSE(&ImageSSE1);
	//freeImageSSE(&ImageSSE2);
	//freeImageSSE(&dif);
}



void SD_1_Step_SSE(image_SSE* ImgRead, image_SSE* Ot, image_SSE* Vt, image_SSE* Mt) // manque la condition ==  dans le premier et deuxieme if
{

    int i,j;
    int c =card_vuint8();
    vuint8  mt,img_read,vt, sl,sg,ot, ot_bis, max,min;

    vuint8 zero =_mm_set1_epi8(0);
    vuint8 un =_mm_set1_epi8(1);
    vuint8 valMax = _mm_set1_epi8(255);
    vuint8 vmax = _mm_set1_epi8(VMAX);
    vuint8 vmin = _mm_set1_epi8(VMIN);

    for(i=0;i<H;i++){
        for(j=0;j<W/c;j++){

           // Initialisation    
            mt = _mm_load_si128(&Mt->data[i][j]);
            vt = _mm_load_si128(&Vt->data[i][j]);
            img_read = _mm_load_si128(&ImgRead->data[i][j]);


           // STEP 1   
            sl = _mm_cmplt_epu8(mt, img_read);  
            sg = _mm_cmpgt_epu8(mt, img_read);  
			
			mt = _mm_add_epi8_limit(mt, conv_simd_logic_bin(_mm_andnot_si128(sg, un)));    
            mt = _mm_sub_epi8_limit(mt, conv_simd_logic_bin(_mm_andnot_si128(sl, un)));
            //mt = _mm_add_epi8_limit(mt, conv_simd_logic_bin(_mm_or_si128(_mm_and_si128(sl,valMax), zero)));    
            //mt = _mm_sub_epi8_limit(mt, conv_simd_logic_bin(_mm_or_si128(_mm_and_si128(sg,valMax), zero)));  


           // STEP 2 // abs
            max = _mm_max_epu8(mt, img_read);
            min = _mm_min_epu8(mt, img_read);

            ot = _mm_sub_epi8_limit(max, min);

            ot_bis = _mm_add_epi8_limit(ot, ot);
            ot_bis = _mm_add_epi8_limit(ot_bis, ot);
            
           // STEP 3 
            sl = _mm_cmplt_epu8(vt, ot_bis);    
            sg = _mm_cmpgt_epu8(vt, ot_bis);   

            vt = _mm_add_epi8_limit(vt, conv_simd_logic_bin(_mm_andnot_si128(sg, un)));    
            vt = _mm_sub_epi8_limit(vt, conv_simd_logic_bin(_mm_andnot_si128(sl, un)));
            //vt = _mm_add_epi8_limit(vt, conv_simd_logic_bin(_mm_or_si128(_mm_and_si128(sg,valMax), zero)));    
            //vt = _mm_sub_epi8_limit(vt, conv_simd_logic_bin(_mm_or_si128(_mm_and_si128(sl,valMax), zero)));

            vt = _mm_max_epu8(_mm_min_epu8(vt,vmax), vmin);

           // STEP 4 
            ot = _mm_cmpgt_epu8(ot,vt);
            //sl = _mm_cmplt_epu8(ot,vt);
            //ot = _mm_or_si128(_mm_andnot_si128(sl,valMax), zero);


            _mm_store_si128(&Ot->data[i][j],ot);
            _mm_store_si128(&Mt->data[i][j],mt);
            _mm_store_si128(&Vt->data[i][j],vt);
        }
    }
}


void SD_Full_Step_NO_Morpho_SSE()
{
	image_t ImgRead;
	image_t ImgRead1;
	int i;
	char nomFichier[50] ="";
	char nomFichier2[50] ="";

	image_SSE ImageSSE1, Ot, Vt,  Mt;

	readPGM_SSE("car3/car_3001.pgm",&Mt);


	initImageSSE(&Vt);
	initImageSSE(&Ot);

	setVal_image_SSE(&Vt, VMIN);

	for(i=0;i<199;i++){

		Conc("car3/car_",3000+i,nomFichier);
		//Conc("car3/car_",3000+i+1,nomFichier2);

		readPGM_SSE(nomFichier,&ImageSSE1);
		//readPGM_SSE(nomFichier2,&ImageSSE2);

		SD_1_Step_SSE(&ImageSSE1, &Ot, &Vt,&Mt);

		//copyImage_SSE_to_Image_t(&dif,&ImgRead);
		//writePGM(&ImgRead,i,"FDSSE/FDSSEcar_");
		writePGM_SSE(&Ot,i,"SDSSE/SDSSEcar_");

		freeImageSSE(&ImageSSE1);
	}

	freeImageSSE(&Mt);
	freeImageSSE(&Vt);
	freeImageSSE(&Ot);


}

void SD_Full_Step_Morpho3_3_SSE()
{
	image_t ImgRead;
	image_t ImgRead1;
	int i;
	char nomFichier[50] ="";
	char nomFichier2[50] ="";

	image_SSE ImageSSE1, Ot, Vt,  Mt,out;

	readPGM_SSE("car3/car_3001.pgm",&Mt);
	


	initImageSSE(&Vt);
	initImageSSE(&Ot);
	initImageSSE(&out);

	for(i=0;i<199;i++){

		Conc("car3/car_",3000+i,nomFichier);
		//Conc("car3/car_",3000+i+1,nomFichier2);

		readPGM_SSE(nomFichier,&ImageSSE1);
		//readPGM_SSE(nomFichier2,&ImageSSE2);

		SD_1_Step_SSE(&ImageSSE1, &Ot, &Vt,&Mt);

		//copyImage_SSE_to_Image_t(&dif,&ImgRead);
		//writePGM(&ImgRead,i,"FDSSE/FDSSEcar_");
		morpho_SSE_Erosion3_3(&Ot, &out);
		writePGM_SSE(&out,i,"SDSSE_Morpho3_3/SDSSEcar_");

		freeImageSSE(&ImageSSE1);
	}

	freeImageSSE(&Mt);
	freeImageSSE(&Vt);
	freeImageSSE(&Ot);
	freeImageSSE(&out);
	//freeImage_t(&ImgRead);
	//freeImageSSE(&ImageSSE1);
	//freeImageSSE(&ImageSSE2);
	//freeImageSSE(&dif);

}

void SD_Full_Step_Morpho5_5_SSE()
{
	image_t ImgRead;
	image_t ImgRead1;
	int i;
	char nomFichier[50] ="";
	char nomFichier2[50] ="";

	image_SSE ImageSSE1, Ot, Vt,  Mt,out;

	readPGM_SSE("car3/car_3001.pgm",&Mt);
	
	initImageSSE(&Vt);
	initImageSSE(&Ot);
	initImageSSE(&out);

	for(i=0;i<199;i++){
		Conc("car3/car_",3000+i,nomFichier);
		//Conc("car3/car_",3000+i+1,nomFichier2);

		readPGM_SSE(nomFichier,&ImageSSE1);
		//readPGM_SSE(nomFichier2,&ImageSSE2);

		SD_1_Step_SSE(&ImageSSE1, &Ot, &Vt,&Mt);

		//copyImage_SSE_to_Image_t(&dif,&ImgRead);
		//writePGM(&ImgRead,i,"FDSSE/FDSSEcar_");
		morpho_SSE_Erosion5_5(&Ot, &out);
		writePGM_SSE(&out,i,"SDSSE_Morpho5_5/SDSSEcar_");
		freeImageSSE(&ImageSSE1);
	}

	freeImageSSE(&Mt);
	freeImageSSE(&Vt);
	freeImageSSE(&Ot);
	freeImageSSE(&out);

	//freeImage_t(&ImgRead);
	//freeImageSSE(&ImageSSE1);
	//freeImageSSE(&ImageSSE2);
	//freeImageSSE(&dif);

}

