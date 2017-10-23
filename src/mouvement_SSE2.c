
#include "mouvement_SSE2.h"

void initImageSEE(image_SEE* Image, int w, int h,int intensity)
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

    Image->data = vui8vectorArray(0, Image->h-1);

    si0 = 0;
    si1 = Image->w-1;
    s2v1D(si0, si1, card, &vi0, &vi1);
    v2m1D(vi0, vi1, card, &mi0, &mi1);

    for(i=0;i<Image->h;i++)
    {
    	Image->data[i] = vui8vector(vi0, vi1);

    }
}

int readPGM_SEE(char* NomFichier, image_SEE* ImgRead)
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
			initImageSEE(ImgRead,w,h,intensity);

			//Lecture de chacune des valeurs 
			for(i=0;i<ImgRead->h;i++)
			{
				for(j=0;j<ImgRead->w;j+=16)
				{
					/*for(k=0;k<16;k++){
						tabLect[k] = fgetc(fp);
						
					}*/
					
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

void copyImage_t_to_Image_SEE(image_t* imaget, image_SEE* imageSEE)
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
						_mm_store_si128(&imageSEE->data[i][j/c],x);
			}
		}

}

void copyImage_SEE_to_Image_t(image_SEE* imageSEE,image_t* imaget)
{
	int i,j,k;
	vuint8 T[1];
	uint8 *p = (uint8*) T;
	int c = card_vuint8();
	for(i=0;i<imaget->h;i++)
		{
			for(j=0;j<imaget->w;j+=c)
			{
				 _mm_store_si128(T,imageSEE->data[i][j/c]);
				 for(k=0;k<c;k++)
					imaget->data[i][j+k] = p[k];
			}
		}

}


void FD_1_Step_SEE(image_SEE* ImageSEE1, image_SEE* ImageSEE2, image_SEE* dif)
{
	int i,j;
	int c =card_vuint8();

	vuint8  x,y,z, zero, valMax,teta ,cmp,neg,un;
 	zero =_mm_set1_epi8(0);
 	valMax = _mm_set1_epi8(255);
	teta = _mm_set1_epi8(TETA);
	neg = _mm_set1_epi8(-1);
	un =_mm_set1_epi8(1);

	for(i=0;i<ImageSEE1->h;i++)
	{
		for(j=0;j<ImageSEE1->w/c;j++)
			{
			x = _mm_load_si128(&ImageSEE2->data[i][j]);
			y = _mm_load_si128(&ImageSEE1->data[i][j]);
			z = _mm_subs_epu8(x,y);
			

			//display_vuint8(z, "%d ", "z2 "); puts("");

			cmp =_mm_cmplt_epi8(z ,teta);
			x =_mm_or_si128(_mm_and_si128(cmp,zero),_mm_andnot_si128(cmp,valMax));

			_mm_store_si128(&dif->data[i][j],x);
		}
	}
}


void FD_Full_Step_NO_Morpho_SEE()
{
	//printf("Demarage FD sans morpho\n");
	image_t ImgRead;
	image_t ImgRead1;
	int i;
	char nomFichier[50] ="";
	char nomFichier2[50] ="";

	image_SEE ImageSEE1,ImageSEE2, dif;

	readPGM_SEE("car3/car_3000.pgm",&ImageSEE1);
	//readPGM("car3/car_3000.pgm",&ImgRead);

 	//initImageSEE(&ImageSEE1,ImgRead.w,ImgRead.h,ImgRead.maxInt);
	initImageSEE(&ImageSEE2,ImageSEE1.w,ImageSEE1.h,ImageSEE1.maxInt);
	initImageSEE(&dif,ImageSEE1.w,ImageSEE1.h,ImageSEE1.maxInt);

	for(i=0;i<199;i++){
		Conc("car3/car_",3000+i,nomFichier);
		Conc("car3/car_",3000+i+1,nomFichier2);

		readPGM_SEE(nomFichier,&ImageSEE1);
			//printf("L'image a bien été lu\n ");
		readPGM_SEE(nomFichier2,&ImageSEE2);
			//printf("L'image a bien été lu\n ");

		FD_1_Step_SEE(&ImageSEE1,&ImageSEE2,&dif);

		//copyImage_SEE_to_Image_t(&dif,&ImgRead);
		//writePGM(&ImgRead,i,"FDSEE/FDSEEcar_");
		writePGM_SEE(&dif,i,"FDSEE/FDSEEcar_");
	}

	//printf("Fin FD sans morpho\n");
	//freeImage_t(&ImgRead);
	//freeImageSEE(&ImageSEE1);
	//freeImageSEE(&ImageSEE2);
	//freeImageSEE(&dif);

}

void cpySEE(image_SEE* in, image_SEE* out)
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


void SD_1_Step_SEE(image_SEE* ImgRead, image_SEE* Ot, image_SEE* Vt, image_SEE* Mt) // manque la condition ==  dans le premier et deuxieme if
{
	int i,j;
	int c =card_vuint8();

	vuint8  x,y,z,zn, a, zero, valMax,teta ,cmp,neg,un,n,vmax,vmin;
 	zero =_mm_set1_epi8(0);
 	valMax = _mm_set1_epi8(255);
	n = _mm_set1_epi8(N);
	vmax = _mm_set1_epi8(VMAX);
	vmin = _mm_set1_epi8(VMIN);
	neg = _mm_set1_epi8(-1);
	un =_mm_set1_epi8(1);

	for(i=0;i<ImgRead->h;i++)
	{
		for(j=0;j<ImgRead->w/c;j++)
			{
			x = _mm_load_si128(&ImgRead->data[i][j]);
			y = _mm_load_si128(&Mt->data[i][j]);
			a = _mm_load_si128(&Vt->data[i][j]);
			
			cmp =_mm_cmplt_epi8(y ,x);
			x =_mm_or_si128(_mm_and_si128(cmp,un),_mm_andnot_si128(cmp,neg));
			y = _mm_add_epi8(x,y);

			z = _mm_subs_epu8(y,x);

			zn = _mm_mul_epu32(z,n);

			cmp =_mm_cmplt_epi8(a ,zn);
			x =_mm_or_si128(_mm_and_si128(cmp,un),_mm_andnot_si128(cmp,neg));
			a = _mm_add_epi8(a,x);

			a = _mm_max_epu8(_mm_min_epu8(a,vmax),vmin);

			cmp =_mm_cmplt_epi8(z ,a);
			x =_mm_or_si128(_mm_and_si128(cmp,zero),_mm_andnot_si128(cmp,valMax));

			_mm_store_si128(&Ot->data[i][j],x);

		}
	}
}




void writePGM_SEE(image_SEE* dif, int k, char* dossier)
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
		//fclose(fp);
	}
	
}

void SD_Full_Step_NO_Morpho_SEE()
{
	//printf("Demarage FD sans morpho\n");
	image_t ImgRead;
	image_t ImgRead1;
	int i;
	char nomFichier[50] ="";
	char nomFichier2[50] ="";

	image_SEE ImageSEE1, Ot, Vt,  Mt;

	readPGM_SEE("car3/car_3000.pgm",&ImageSEE1);
	readPGM_SEE("car3/car_3001.pgm",&Mt);
	


	//initImageSEE(&ImageSEE2,ImageSEE1.w,ImageSEE1.h,ImageSEE1.maxInt);
	initImageSEE(&Vt,ImageSEE1.w,ImageSEE1.h,ImageSEE1.maxInt);
	initImageSEE(&Ot,ImageSEE1.w,ImageSEE1.h,ImageSEE1.maxInt);

	for(i=0;i<199;i++){
		Conc("car3/car_",3000+i,nomFichier);
		//Conc("car3/car_",3000+i+1,nomFichier2);

		readPGM_SEE(nomFichier,&ImageSEE1);
			//printf("L'image a bien été lu\n ");
		//readPGM_SEE(nomFichier2,&ImageSEE2);
			//printf("L'image a bien été lu\n ");

		SD_1_Step_SEE(&ImageSEE1, &Ot, &Vt,&Mt);

		//copyImage_SEE_to_Image_t(&dif,&ImgRead);
		//writePGM(&ImgRead,i,"FDSEE/FDSEEcar_");
		writePGM_SEE(&Ot,i,"SDSEE/SDSEEcar_");
	}

	//printf("Fin FD sans morpho\n");
	//freeImage_t(&ImgRead);
	//freeImageSEE(&ImageSEE1);
	//freeImageSEE(&ImageSEE2);
	//freeImageSEE(&dif);

}

void freeImageSEE(image_SEE* image)
{

	int i,j;

	for(i=0;i<image->h;i++)
	{
		free_vui8vector(image->data[i], 0, 19);
	}

	free_vui8vector(image->data, 0, 239);
}