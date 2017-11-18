#include "util.h"





uint8_t convLogicToBin(uint8_t t)
{
	if(t == 255)
		return 1;
	else
		return 0;	
}

uint8_t convBinToLogic(uint8_t t)
{
	if(t == 1)
		return 255;
	else
		return 0;	
}

//Initialisation d'une variable image_t
void initImage_t(image_t* Image)
{
	int i,j;
			//image_t Image0;
			//Image0 = (image_t*) malloc(sizeof(image_t));
			Image->w = W;
			Image->h=H;
			Image->maxInt = INTENSITY;

			//Allocation de la matrice 
			Image->data =ui8matrix(-2,H+2,-2,W+2);

			for(i=-2;i<H+3;i++)
			{
				for(j=-2; j<W+3;j++)
				{
					Image->data[i][j] = 0;
				}
			}
}

void freeImage_t(image_t* Image)
{
		free_ui8matrix(Image->data,-2,Image->h+2,-2,Image->w+2);
		//free(Image);
}


//Fonction permettant de remplir une variable de type image_t en lisant une image .PGM
int readPGM(char* NomFichier, image_t* ImgRead)
{
	FILE* fp;
	char motMagique[3]="";
	char chaine[1000] = "";
	int w,h,intensity;
	int i,j;

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
			initImage_t(ImgRead);

			//Lecture de chacune des valeurs 
			for(i=0;i<H;i++)
			{
				for(j=0;j<W;j++)
					ImgRead->data[i][j] =fgetc(fp);
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


// concatene le nom avec le nombre i pour créer un nom du type  car_3000.pgm
void Conc(char* nom,int i,char* ret)
{
	char a[10];
	strcpy(ret,nom);
	sprintf(a,"%d",i);
	strcat(ret,a);
	strcat(ret,".pgm");
}

void ConcPPM(char* nom,int i,char* ret)
{
	char a[10];
	strcpy(ret,nom);
	sprintf(a,"%d",i);
	strcat(ret,a);
	strcat(ret,".ppm");
}

void cpy_Image(image_t* Mt, image_t* ImgRead)//********************* AJOUT
{
	int i,j;

	for(i=0;i<H;i++)
	{
		for(j=0;j<W;j++)
		{
			Mt->data[i][j] = ImgRead->data[i][j];
		}
	}
}

// écrit le contenu de l'image_t dans un fichier au format .PGM 
void writePGM(image_t* dif, int k, char* dossier)
{

	FILE* fp;
	int i,j;
	char nomFichier[50] = "";

	Conc(dossier,k,nomFichier);

	fp = fopen(nomFichier,"w");
	if(fp!=NULL)
	{
		fprintf(fp,"P5\n");
		fprintf(fp,"%d %d\n",dif->w,dif->h);
		fprintf(fp,"%d\n",dif->maxInt);

		for(i=0;i<H;i++)
		{
			for(j=0;j<W;j++)
			{
				fprintf(fp,"%c",dif->data[i][j]);
			}
		}	
		fclose(fp);
	}
	

}

void writePPM(image_t* dif, int k, char* dossier)
{

	FILE* fp;
	int i,j;
	char nomFichier[50] = "";

	Conc(dossier,k,nomFichier);
	printf("%s\n",nomFichier);

	fp = fopen(nomFichier,"w");
	if(fp!=NULL)
	{
		fprintf(fp,"P6\n");
		fprintf(fp,"%d %d\n",dif->w,dif->h);
		fprintf(fp,"%d\n",dif->maxInt);

		for(i=0;i<H;i++)
		{
			for(j=0;j<W;j++)
			{
				fprintf(fp,"%c",dif->data[i][j]);
				fprintf(fp,"%c",dif->data[i][j]);
				fprintf(fp,"%c",dif->data[i][j]);
			}
		}	
		fclose(fp);
	}
	

}

void setVal_image(image_t* img, int val)
{

	int i,j;
	for(i=0;i<H;i++)
	{
		for(j=0;j<W;j++)
		{
			img->data[i][j] = val;
		}
	}
}

void compareImage(image_t* image1, image_t* image2)
{
    int i,j;
    int cpt=0;

    for(i=0;i<H;i++)
    {
        for(j=0;j<W;j++)
        {
            if(image1->data[i][j] == image2->data[i][j])
            {
                cpt++;
            }else
            {
                printf("ici erreur i = %d , j= %d\n",i,j);
            }
        }
    }

    printf("il a %d pixel corect sur %d, soit %f pourcent\n",cpt,W*H, cpt*100.0/(W*H));

}


void initImageSSE(image_SSE* Image)
{
	int i,j;
	Image->w = W;
	Image->h=H;
	Image->maxInt = INTENSITY;

	int card;
  	int si0, si1; // scalar indices
    int vi0, vi1; // vector indices
    int mi0, mi1; // memory (bounded) indices

 	card = card_vuint8();

    Image->data = vui8vectorArray(-2, H+2);

    si0 = -2;
    si1 = W+2;
    s2v1D(si0, si1, card, &vi0, &vi1);
    v2m1D(vi0, vi1, card, &mi0, &mi1);

    for(i=-2;i<H+3;i++)
    {
    	Image->data[i] = vui8vector(vi0, vi1);
    }
}

void freeImageSSE(image_SSE* image)
{

	int i,j;

	for(i=-2;i<H+3;i++)
	{
		free_vui8vector(image->data[i], -1, W/16);
	}

	free(image->data-2);
	//free_vui8vector(image->data, -1, H+1);
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
			initImageSSE(ImgRead);

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
	for(i=0;i<H;i++)
		{
			for(j=0;j<W;j+=c)
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
	for(i=0;i<H;i++)
		{
			for(j=0;j<W;j+=c)
			{
				 _mm_store_si128(T,imageSSE->data[i][j/c]);
				 for(k=0;k<c;k++)
					imaget->data[i][j+k] = p[k];
			}
		}

}

void cpySSE(image_SSE* in, image_SSE* out)
{

	int i,j;
	for(i=0;i<H;i++)
	{
		for(j=0;j<W/16;j++)
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


void writePGM_SSE(image_SSE* dif, int k, char* dossier)
{

	FILE* fp;
	int i,j;
	char nomFichier[50] = "";

	Conc(dossier,k,nomFichier);
	//vuint8  x;

	fp = fopen(nomFichier,"w");
	if(fp!=NULL)
	{
		fprintf(fp,"P5\n");
		fprintf(fp,"%d %d\n",dif->w,dif->h);
		fprintf(fp,"%d\n",dif->maxInt);

		for(i=0;i<H;i++)
		{
			for(j=0;j<W/16;j++)
			{
				//x = _mm_load_si128(&dif->data[i][j]);
				fwrite(&dif->data[i][j], sizeof(vuint8), 1, fp);
				
				//fprintf(fp,"%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c",(char)x[0],(char)x[1],(char)x[2],(char)x[3],(char)x[4],
				//	(char)x[5],(char)x[6],(char)x[7],(char)x[8],(char)x[9],
				//	(char)x[10],(char)x[11],(char)x[12],(char)x[13],(char)x[14],(char)x[15]);
			}
		}	
		fclose(fp);
	}
	
}

void setVal_image_SSE(image_SSE* img, int val)
{

	int i,j;
	vuint8 value;
	value = _mm_set1_epi8(val);
	
	for(i=0;i<H;i++)
	{
		for(j=0;j<W/16;j++)
		{
			
			//
			_mm_store_si128(&img->data[i][j],value);
		}
	}
}