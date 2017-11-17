
#include "mouvement_SSE2.h"



void initImageSSE(image_SSE* Image, int w, int h,int intensity)
{
	int i,j;
	Image->w = w;
	Image->h=h;
	Image->maxInt = intensity;

	int card;
  	int si0, si1; // scalar indices
    int vi0, vi1; // vector indices
    int mi0, mi1; // memory (bounded) indices

 	card = card_vuint8();

 	/*si0 = 0;
    si1 = Image->h -1;
    s2v1D(si0, si1, card, &vi0, &vi1);
    v2m1D(vi0, vi1, card, &mi0, &mi1);*/

    Image->data = vui8vectorArray(-1, Image->h+1);

    si0 = -1;
    si1 = Image->w+1;
    s2v1D(si0, si1, card, &vi0, &vi1);
    v2m1D(vi0, vi1, card, &mi0, &mi1);

    for(i=-1;i<Image->h+2;i++)
    {
    	Image->data[i] = vui8vector(vi0, vi1);
    }
}

int readPGM_SSE(char* NomFichier, image_SSE* ImgRead)
{
	FILE* fp;
	char motMagique[3]="";
	char chaine[1000] = "";
	int w,h,intensity;
	int i,j,k;
	char tabLect[16]="";
	vuint8 x;

	fp=fopen(NomFichier,"r");
	if(fp != NULL)
	{
			// Lecture du mot permettant de reconnaitre le PGM 
			fgets(chaine, 1000, fp);
			strncpy(motMagique,chaine,2);
		
			if(strcmp(motMagique,"P5") !=0)
			{
				printf("Ce n'est pas un fichier PGM\n");
				return -1;
			}

			//Suppression des lignes de commentaires
			fgets(chaine, 1000, fp);
			while(chaine[0] == '#')
			{
				fgets(chaine, 1000, fp);
			}

			fseek(fp, -(int)strlen(chaine), SEEK_CUR); // Retour en arrière car il y a eu saut de ligne dans le while
			fscanf(fp,"%d %d \n",&w,&h); // Récupération de Width hight 
			fscanf(fp,"%d \n",&intensity); // Récupération de intensity 


			//initialisation de l'image
			initImageSSE(ImgRead,w,h,intensity);

			//Lecture de chacune des valeurs 
			for(i=0;i<ImgRead->h;i++)
			{
				for(j=0;j<ImgRead->w;j+=16)
				{
					/*for(k=0;k<16;k++){
						tabLect[k] = fgetc(fp);
						
					}*/
					//fread(&x, sizeof(vuint8), 1, fp);
					x =_mm_set_epi8(fgetc(fp),fgetc(fp),fgetc(fp),fgetc(fp),fgetc(fp),
						fgetc(fp),fgetc(fp),fgetc(fp),fgetc(fp),fgetc(fp),fgetc(fp),fgetc(fp),
						fgetc(fp),fgetc(fp),fgetc(fp),fgetc(fp));
					_mm_store_si128(&ImgRead->data[i][j/16],x);
				
				}
			}

			fclose(fp);

			return 0;
		}
	else
	{
			//printf("Fichier impossible à ouvrir\n");
			return -1;
	}
	
	return -1;
}

void copyImage_t_to_Image_SSE(image_t* imaget, image_SSE* imageSSE)
{
	int i,j,k;
	vuint8 x;
	int c = card_vuint8();
	for(i=0;i<imaget->h;i++)
		{
			for(j=0;j<imaget->w;j+=c)
			{
						x =_mm_set_epi8(imaget->data[i][j+15],imaget->data[i][j+14],imaget->data[i][j+13],imaget->data[i][j+12],imaget->data[i][j+11],
							imaget->data[i][j+10],imaget->data[i][j+9],imaget->data[i][j+8],imaget->data[i][j+7],imaget->data[i][j+6],imaget->data[i][j+5],imaget->data[i][j+4],
							imaget->data[i][j+3],imaget->data[i][j+2],imaget->data[i][j+1],imaget->data[i][j]);
						_mm_store_si128(&imageSSE->data[i][j/c],x);
			}
		}

}

void copyImage_SSE_to_Image_t(image_SSE* imageSSE,image_t* imaget)
{
	int i,j,k;
	vuint8 T[1];
	uint8 *p = (uint8*) T;
	int c = card_vuint8();
	for(i=0;i<imaget->h;i++)
		{
			for(j=0;j<imaget->w;j+=c)
			{
				 _mm_store_si128(T,imageSSE->data[i][j/c]);
				 for(k=0;k<c;k++)
					imaget->data[i][j+k] = p[k];
			}
		}

}


void FD_1_Step_SSE(image_SSE* ImageSSE1, image_SSE* ImageSSE2, image_SSE* Ot)
{
	int i,j;
	int c = card_vuint8();

	vuint8  it, it_1, ot, zero, valMax, teta, max, min, sl;
 	zero =_mm_set1_epi8(0);
 	valMax = _mm_set1_epi8(255);
	teta = _mm_set1_epi8(TETA);	// 20	// define

	for(i=0;i<ImageSSE1->h;i++){
		for(j=0;j<ImageSSE1->w/c;j++){

			// initialisation
			it = _mm_load_si128(&ImageSSE2->data[i][j]);
			it_1 = _mm_load_si128(&ImageSSE1->data[i][j]);
			
			// STEP 1
			max = _mm_max_epu8(it, it_1);
           	min = _mm_min_epu8(it, it_1);
            
            ot = _mm_sub_epi8_limit(max, min);

            // STEP 2
            sl = _mm_cmplt_epu8(ot,teta);
            ot = _mm_or_si128(_mm_andnot_si128(sl,valMax), zero);

            _mm_store_si128(&Ot->data[i][j],ot);
		}
	}
}


void FD_Full_Step_NO_Morpho_SSE()
{
	//printf("Demarage FD sans morpho\n");
	image_t ImgRead;
	image_t ImgRead1;
	int i;
	char nomFichier[50] ="";
	char nomFichier2[50] ="";

	image_SSE ImageSSE1,ImageSSE2, Ot;

	readPGM_SSE("car3/car_3000.pgm",&ImageSSE1);
	//readPGM("car3/car_3000.pgm",&ImgRead);

 	//initImageSSE(&ImageSSE1,ImgRead.w,ImgRead.h,ImgRead.maxInt);
	initImageSSE(&ImageSSE2,ImageSSE1.w,ImageSSE1.h,ImageSSE1.maxInt);
	initImageSSE(&Ot,ImageSSE1.w,ImageSSE1.h,ImageSSE1.maxInt);

	for(i=0;i<199;i++){
		Conc("car3/car_",3000+i,nomFichier);
		Conc("car3/car_",3000+i+1,nomFichier2);

		readPGM_SSE(nomFichier,&ImageSSE1);
			//printf("L'image a bien été lu\n ");
		readPGM_SSE(nomFichier2,&ImageSSE2);
			//printf("L'image a bien été lu\n ");

		FD_1_Step_SSE(&ImageSSE1,&ImageSSE2,&Ot);

		//copyImage_SSE_to_Image_t(&dif,&ImgRead);
		//writePGM(&ImgRead,i,"FDSSE/FDSSEcar_");
		writePGM_SSE(&Ot,i,"FDSSE/FDSSEcar_");
	}

	//printf("Fin FD sans morpho\n");
	//freeImage_t(&ImgRead);
	//freeImageSSE(&ImageSSE1);
	//freeImageSSE(&ImageSSE2);
	//freeImageSSE(&dif);

}

void FD_Full_Step_Morpho3_3_SSE()
{
	//printf("Demarage FD sans morpho\n");
	image_t ImgRead;
	image_t ImgRead1;
	int i;
	char nomFichier[50] ="";
	char nomFichier2[50] ="";

	image_SSE ImageSSE1,ImageSSE2, Ot, out, inter;

	readPGM_SSE("car3/car_3000.pgm",&ImageSSE1);
	//readPGM("car3/car_3000.pgm",&ImgRead);

 	//initImageSSE(&ImageSSE1,ImgRead.w,ImgRead.h,ImgRead.maxInt);
	initImageSSE(&ImageSSE2,ImageSSE1.w,ImageSSE1.h,ImageSSE1.maxInt);
	initImageSSE(&Ot,ImageSSE1.w,ImageSSE1.h,ImageSSE1.maxInt);
	initImageSSE(&out,ImageSSE1.w,ImageSSE1.h,ImageSSE1.maxInt);
	initImageSSE(&inter,ImageSSE1.w,ImageSSE1.h,ImageSSE1.maxInt);

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
	}

	//printf("Fin FD sans morpho\n");
	//freeImage_t(&ImgRead);
	//freeImageSSE(&ImageSSE1);
	//freeImageSSE(&ImageSSE2);
	//freeImageSSE(&dif);

}

void FD_Full_Step_Morpho5_5_SSE()
{

//printf("Demarage FD sans morpho\n");
	image_t ImgRead;
	image_t ImgRead1;
	int i;
	char nomFichier[50] ="";
	char nomFichier2[50] ="";

	image_SSE ImageSSE1,ImageSSE2, Ot, out, inter;

	readPGM_SSE("car3/car_3000.pgm",&ImageSSE1);
	//readPGM("car3/car_3000.pgm",&ImgRead);

 	//initImageSSE(&ImageSSE1,ImgRead.w,ImgRead.h,ImgRead.maxInt);
	initImageSSE(&ImageSSE2,ImageSSE1.w,ImageSSE1.h,ImageSSE1.maxInt);
	initImageSSE(&Ot,ImageSSE1.w,ImageSSE1.h,ImageSSE1.maxInt);
	initImageSSE(&out,ImageSSE1.w,ImageSSE1.h,ImageSSE1.maxInt);
	initImageSSE(&inter,ImageSSE1.w,ImageSSE1.h,ImageSSE1.maxInt);

	for(i=0;i<199;i++){
		Conc("car3/car_",3000+i,nomFichier);
		Conc("car3/car_",3000+i+1,nomFichier2);

		readPGM_SSE(nomFichier,&ImageSSE1);
			//printf("L'image a bien été lu\n ");
		readPGM_SSE(nomFichier2,&ImageSSE2);
			//printf("L'image a bien été lu\n ");

		FD_1_Step_SSE(&ImageSSE1,&ImageSSE2,&Ot);

		morpho_SSE_Dilatation5_5(&Ot,&out);
		writePGM_SSE(&out,i,"FDSSE_Morpho5_5/FDSSEcar_");
	}

	//printf("Fin FD sans morpho\n");
	//freeImage_t(&ImgRead);
	//freeImageSSE(&ImageSSE1);
	//freeImageSSE(&ImageSSE2);
	//freeImageSSE(&dif);
}

void cpySSE(image_SSE* in, image_SSE* out)
{

	int i,j;
	for(i=0;i<in->h;i++)
	{
		for(j=0;j<in->w/16;j++)
		{
			_mm_store_si128(&out->data[i][j],in->data[i][j]);
		}
	}
}



vuint8 conv_simd_logic_bin(vuint8 t)
{

    vuint8 _1 =_mm_set1_epi8(1);
    t = (vuint8)_mm_min_epu8(t,_1);

}

void SD_1_Step_SSE(image_SSE* ImgRead, image_SSE* Ot, image_SSE* Vt, image_SSE* Mt) // manque la condition ==  dans le premier et deuxieme if
{

    int i,j;
    int c =card_vuint8();
    vuint8  mt,img_read,vt, sl,sg,ot, ot_bis, max,min;

    vuint8 zero =_mm_set1_epi8(0);
    vuint8 valMax = _mm_set1_epi8(255);
    vuint8 vmax = _mm_set1_epi8(VMAX);
    vuint8 vmin = _mm_set1_epi8(VMIN);

    for(i=0;i<ImgRead->h;i++){
        for(j=0;j<ImgRead->w/c;j++){

            //RAPPEL 
            // Vmax = 50    // define
            // Vmin = 30    // define
            // N = 3        // par defaut, define ne modifira pas celui-ci

           // initialisation    
            mt = _mm_load_si128(&Mt->data[i][j]);
            vt = _mm_load_si128(&Vt->data[i][j]);
            img_read = _mm_load_si128(&ImgRead->data[i][j]);

            //display_vuint8(mt, "%d ", "mt0 "); puts("");
            //display_vuint8(img_read, "%d ", "ig0 "); puts("");
            //display_vuint8(vt, "%d ", "vt0 "); puts("");

            // STEP 1   
            sl = _mm_cmplt_epu8(mt, img_read);  // 0xFF si a < b ; 0 si a > b
            sg = _mm_cmpgt_epu8(mt, img_read);  // 0 si a < b; 0xFF si a > b
			
            // convert 255 en 1 logique
            mt = _mm_add_epi8_limit(mt, conv_simd_logic_bin(_mm_or_si128(_mm_and_si128(sl,valMax), zero)));     // mt ADD ( (Sg ET 255) OR 0 ) 
            mt = _mm_sub_epi8_limit(mt, conv_simd_logic_bin(_mm_or_si128(_mm_and_si128(sg,valMax), zero)));     // mt SUB ( (Sl ET 255) OR 0 ) 


           // STEP 2 // abs
            max = _mm_max_epu8(mt, img_read);
            min = _mm_min_epu8(mt, img_read);

            ot = _mm_sub_epi8_limit(max, min);

            // <=> N = 3  fonction _mm_mul = error
            ot_bis = _mm_add_epi8_limit(ot, ot);
            ot_bis = _mm_add_epi8_limit(ot_bis, ot);
            
           // STEP 3 

            sl = _mm_cmplt_epu8(vt, ot_bis);    //_mm_mul_epu32(n,ot)); // 0xFF si a < b ; 0 si a > b
            sg = _mm_cmpgt_epu8(vt, ot_bis);    //_mm_mul_epu32(n,ot)); // 0 si a < b; 0xFF si a > b

            vt = _mm_add_epi8_limit(vt, conv_simd_logic_bin(_mm_or_si128(_mm_and_si128(sg,valMax), zero)));     // mt ADD ( (Sl ET 255) OR 0 ) 
            vt = _mm_sub_epi8_limit(vt, conv_simd_logic_bin(_mm_or_si128(_mm_and_si128(sl,valMax), zero))); // mt SUB ( (Sl ET 255) OR 0 ) 

            vt = _mm_max_epu8(_mm_min_epu8(vt,vmax), vmin);

           // STEP 4 
            sl = _mm_cmplt_epu8(ot,vt);
            ot = _mm_or_si128(_mm_andnot_si128(sl,valMax), zero);

            _mm_store_si128(&Ot->data[i][j],ot);
            _mm_store_si128(&Mt->data[i][j],mt);
            _mm_store_si128(&Vt->data[i][j],vt);
        }
    }
}





void writePGM_SSE(image_SSE* dif, int k, char* dossier)
{

	FILE* fp;
	int i,j;
	char nomFichier[50] = "";

	Conc(dossier,k,nomFichier);
	vuint8  x;

	fp = fopen(nomFichier,"w");
	if(fp!=NULL)
	{
		fprintf(fp,"P5\n");
		fprintf(fp,"%d %d\n",dif->w,dif->h);
		fprintf(fp,"%d\n",dif->maxInt);

		for(i=0;i<dif->h;i++)
		{
			for(j=0;j<dif->w/16;j++)
			{
				//x = _mm_load_si128(&dif->data[i][j]);
				 fwrite(&(dif->data[i][j]), sizeof(vuint8), 1, fp);
				
				//fprintf(fp,"%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c",(char)x[0],(char)x[1],(char)x[2],(char)x[3],(char)x[4],
				//	(char)x[5],(char)x[6],(char)x[7],(char)x[8],(char)x[9],
				//	(char)x[10],(char)x[11],(char)x[12],(char)x[13],(char)x[14],(char)x[15]);
			}
		}	
		fclose(fp);
	}
	
}

void SD_Full_Step_NO_Morpho_SSE()
{
	//printf("Demarage FD sans morpho\n");
	image_t ImgRead;
	image_t ImgRead1;
	int i;
	char nomFichier[50] ="";
	char nomFichier2[50] ="";

	image_SSE ImageSSE1, Ot, Vt,  Mt;

	readPGM_SSE("car3/car_3000.pgm",&ImageSSE1);
	readPGM_SSE("car3/car_3001.pgm",&Mt);
	//readPGM_SSE("car3/car_3000.pgm",&Vt);
	


	//initImageSSE(&ImageSSE2,ImageSSE1.w,ImageSSE1.h,ImageSSE1.maxInt);
	initImageSSE(&Vt,ImageSSE1.w,ImageSSE1.h,ImageSSE1.maxInt);
	initImageSSE(&Ot,ImageSSE1.w,ImageSSE1.h,ImageSSE1.maxInt);


	for(i=0;i<199;i++){//199
		//i=0;
		//printf("============== i = %d ====================\n\n\n\n\n",i);
		Conc("car3/car_",3000+i,nomFichier);
		//Conc("car3/car_",3000+i+1,nomFichier2);

		readPGM_SSE(nomFichier,&ImageSSE1);
			//printf("L'image a bien été lu\n ");
		//readPGM_SSE(nomFichier2,&ImageSSE2);
			//printf("L'image a bien été lu\n ");

		SD_1_Step_SSE(&ImageSSE1, &Ot, &Vt,&Mt);

		//copyImage_SSE_to_Image_t(&dif,&ImgRead);
		//writePGM(&ImgRead,i,"FDSSE/FDSSEcar_");
		writePGM_SSE(&Ot,i,"SDSSE/SDSSEcar_");
	}

	//printf("Fin FD sans morpho\n");
	//freeImage_t(&ImgRead);
	//freeImageSSE(&ImageSSE1);
	//freeImageSSE(&ImageSSE2);
	//freeImageSSE(&dif);

}

void SD_Full_Step_Morpho3_3_SSE()
{
	//printf("Demarage FD sans morpho\n");
	image_t ImgRead;
	image_t ImgRead1;
	int i;
	char nomFichier[50] ="";
	char nomFichier2[50] ="";

	image_SSE ImageSSE1, Ot, Vt,  Mt,out;

	readPGM_SSE("car3/car_3000.pgm",&ImageSSE1);
	readPGM_SSE("car3/car_3001.pgm",&Mt);
	


	//initImageSSE(&ImageSSE2,ImageSSE1.w,ImageSSE1.h,ImageSSE1.maxInt);
	initImageSSE(&Vt,ImageSSE1.w,ImageSSE1.h,ImageSSE1.maxInt);
	initImageSSE(&Ot,ImageSSE1.w,ImageSSE1.h,ImageSSE1.maxInt);
	initImageSSE(&out,ImageSSE1.w,ImageSSE1.h,ImageSSE1.maxInt);

	for(i=0;i<199;i++){//199
		//i=0;
		//printf("============== i = %d ====================\n\n\n\n\n",i);
		Conc("car3/car_",3000+i,nomFichier);
		//Conc("car3/car_",3000+i+1,nomFichier2);

		readPGM_SSE(nomFichier,&ImageSSE1);
			//printf("L'image a bien été lu\n ");
		//readPGM_SSE(nomFichier2,&ImageSSE2);
			//printf("L'image a bien été lu\n ");

		SD_1_Step_SSE(&ImageSSE1, &Ot, &Vt,&Mt);

		//copyImage_SSE_to_Image_t(&dif,&ImgRead);
		//writePGM(&ImgRead,i,"FDSSE/FDSSEcar_");
		morpho_SSE_Erosion3_3(&Ot, &out);
		writePGM_SSE(&out,i,"SDSSE_Morpho3_3/SDSSEcar_");
	}

	//printf("Fin FD sans morpho\n");
	//freeImage_t(&ImgRead);
	//freeImageSSE(&ImageSSE1);
	//freeImageSSE(&ImageSSE2);
	//freeImageSSE(&dif);

}

void SD_Full_Step_Morpho5_5_SSE()
{
	//printf("Demarage FD sans morpho\n");
	image_t ImgRead;
	image_t ImgRead1;
	int i;
	char nomFichier[50] ="";
	char nomFichier2[50] ="";

	image_SSE ImageSSE1, Ot, Vt,  Mt,out;

	readPGM_SSE("car3/car_3000.pgm",&ImageSSE1);
	readPGM_SSE("car3/car_3001.pgm",&Mt);
	


	//initImageSSE(&ImageSSE2,ImageSSE1.w,ImageSSE1.h,ImageSSE1.maxInt);
	initImageSSE(&Vt,ImageSSE1.w,ImageSSE1.h,ImageSSE1.maxInt);
	initImageSSE(&Ot,ImageSSE1.w,ImageSSE1.h,ImageSSE1.maxInt);
	initImageSSE(&out,ImageSSE1.w,ImageSSE1.h,ImageSSE1.maxInt);

	for(i=0;i<199;i++){//199
		//i=0;
		//printf("============== i = %d ====================\n\n\n\n\n",i);
		Conc("car3/car_",3000+i,nomFichier);
		//Conc("car3/car_",3000+i+1,nomFichier2);

		readPGM_SSE(nomFichier,&ImageSSE1);
			//printf("L'image a bien été lu\n ");
		//readPGM_SSE(nomFichier2,&ImageSSE2);
			//printf("L'image a bien été lu\n ");

		SD_1_Step_SSE(&ImageSSE1, &Ot, &Vt,&Mt);

		//copyImage_SSE_to_Image_t(&dif,&ImgRead);
		//writePGM(&ImgRead,i,"FDSSE/FDSSEcar_");
		morpho_SSE_Erosion5_5(&Ot, &out);
		writePGM_SSE(&out,i,"SDSSE_Morpho5_5/SDSSEcar_");
	}

	//printf("Fin FD sans morpho\n");
	//freeImage_t(&ImgRead);
	//freeImageSSE(&ImageSSE1);
	//freeImageSSE(&ImageSSE2);
	//freeImageSSE(&dif);

}

void freeImageSSE(image_SSE* image)
{

	int i,j;

	for(i=0;i<image->h;i++)
	{
		free_vui8vector(image->data[i], 0, 19);
	}

	free_vui8vector(image->data, 0, 239);
}