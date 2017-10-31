#include "mouvement.h"
#include "mouvement_SSE2.h"
#include "morpho.h"
#include "benchmark.h"
#include "ppm2pgm.h"

#include "nrdef.h"
#include "vnrdef.h"
#include "nrutil.h"




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
void initImage_t(image_t* Image, int w, int h,int intensity)
{
	int i,j;
			//Image = (image_t*) malloc(sizeof(image_t));
			Image->w = w;
			Image->h=h;
			Image->maxInt = intensity;

			//Allocation de la matrice 
			Image->data =ui8matrix(-2,h+2,-2,w+2);

			for(i=-2;i<h+3;i++)
			{
				for(j=-2; j<w+3;j++)
				{
					Image->data[i][j] = 0;
				}
			}
}

void freeImage_t(image_t* Image)
{
		free_ui8matrix(Image->data,-2,Image->h+2,-2,Image->w+2);

		free(Image);
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
			initImage_t(ImgRead,w,h,intensity);

			//Lecture de chacune des valeurs 
			for(i=0;i<ImgRead->h;i++)
			{
				for(j=0;j<ImgRead->w;j++)
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

//execution de l'algorithm FD sur deux images
void FD_1_Step(image_t* ImgRead1, image_t* ImgRead, image_t* dif)
{
	int i,j;
	for(i=0;i<dif->h;i++)
	{
		for(j=0;j<dif->w;j++)
		{
			dif->data[i][j] = fabs(ImgRead1->data[i][j] - ImgRead->data[i][j]);
		

		}
	}
	for(i=0;i<dif->h;i++)
	{
		for(j=0;j<dif->w;j++)
		{
			if(dif->data[i][j] < TETA )
				dif->data[i][j] = 0;
			else
				dif->data[i][j] = 255;
		}
	}
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

	for(i=0;i<Mt->h;i++)
	{
		for(j=0;j<Mt->w;j++)
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

		for(i=0;i<dif->h;i++)
		{
			for(j=0;j<dif->w;j++)
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

		for(i=0;i<dif->h;i++)
		{
			for(j=0;j<dif->w;j++)
			{
				fprintf(fp,"%c",dif->data[i][j]);
				fprintf(fp,"%c",dif->data[i][j]);
				fprintf(fp,"%c",dif->data[i][j]);
			}
		}	
		fclose(fp);
	}
	

}


// fonction lancant l'algorithm FD sur toute les photos
void FD_Full_Step_NO_Morpho()
{
	//printf("Demarage FD sans morpho\n");
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
	//printf("Fin FD sans morpho\n");
	//freeImage_t(&ImgRead);
}

void FD_Full_Step_Morpho3_3()
{
	//printf("Demarage FD morpho 3_3\n");
	image_t ImgRead;
	image_t ImgRead1;
	image_t dif, out,inter;
	int i;

	char nomFichier[50]="";
	char nomFichier2[50]="";

	readPGM("car3/car_3000.pgm",&ImgRead);
		

	initImage_t(&dif,ImgRead.w,ImgRead.h,ImgRead.maxInt);
	initImage_t(&inter,ImgRead.w,ImgRead.h,ImgRead.maxInt);
	initImage_t(&out,ImgRead.w,ImgRead.h,ImgRead.maxInt);


	for(i=0;i<199;i++){
		Conc("car3/car_",3000+i,nomFichier);
		Conc("car3/car_",3000+i+1,nomFichier2);

		readPGM(nomFichier,&ImgRead);
			//printf("L'image a bien été lu\n ");
		readPGM(nomFichier2,&ImgRead1);
			//printf("L'image a bien été lu\n ");

		FD_1_Step(&ImgRead1,&ImgRead,&dif);

		morpho_Erosion3_3(&dif,&out);
		//morpho_Dilatation3_3(&out,&dif);
		//fermeture3_3(&dif,&inter,&out); // provoc des erreurs de sgmentation
		writePGM(&out,i,"FD_Morpho3_3/FD_Morpho3_3_car_");
	}
	//printf("Fin FD morpho 3_3\n");
}

void FD_Full_Step_Morpho5_5()
{
	//printf("Demarage FD morpho 5_5\n");
	image_t ImgRead;
	image_t ImgRead1;
	image_t dif, out,inter;
	int i;

	char nomFichier[50];
	char nomFichier2[50];

	readPGM("car3/car_3000.pgm",&ImgRead);
		

	initImage_t(&dif,ImgRead.w,ImgRead.h,ImgRead.maxInt);
	initImage_t(&out,ImgRead.w,ImgRead.h,ImgRead.maxInt);
	initImage_t(&inter,ImgRead.w,ImgRead.h,ImgRead.maxInt);


	for(i=0;i<199;i++){
		Conc("car3/car_",3000+i,nomFichier);
		Conc("car3/car_",3000+i+1,nomFichier2);

		readPGM(nomFichier,&ImgRead);
			//printf("L'image a bien été lu\n ");
		readPGM(nomFichier2,&ImgRead1);
			//printf("L'image a bien été lu\n ");

		FD_1_Step(&ImgRead1,&ImgRead,&dif);
		ouverture5_5(&dif,&inter,&out);

		writePGM(&out,i,"FD_Morpho5_5/FD_Morpho5_5_car_");
	}
	//printf("Fin FD morpho 5_5\n");
}

void SD_1_step(image_t* ImgRead, image_t* Ot, image_t* Vt, image_t* Mt)
{
	 int i,j;

        for(i=0;i<ImgRead->h;i++)
        {
                for(j=0;j<ImgRead->w;j++)
                {
                        if(Mt->data[i][j] < ImgRead->data[i][j])
                                Mt->data[i][j] = Mt->data[i][j] +1;
                        else if(Mt->data[i][j] > ImgRead->data[i][j])
                                Mt->data[i][j] = Mt->data[i][j] -1;

                        Ot->data[i][j] = fabs(Mt->data[i][j] - ImgRead->data[i][j]);
                    }
                }
         for(i=0;i<ImgRead->h;i++)
        {
                for(j=0;j<ImgRead->w;j++)
                {


                if(Vt->data[i][j] < N * Ot->data[i][j])
                	 Vt->data[i][j] = Vt->data[i][j] +1;
                else if (Vt->data[i][j] > N * Ot->data[i][j])
                        Vt->data[i][j] = Vt->data[i][j] -1;
                else
                        Vt->data[i][j] = Vt->data[i][j];

                Vt->data[i][j] = MAX(MIN(Vt->data[i][j],VMAX),VMIN);
            }
        }
        for(i=0;i<ImgRead->h;i++)
        {
                for(j=0;j<ImgRead->w;j++)
                {

                if(Ot->data[i][j]< Vt->data[i][j])
                        Ot->data[i][j] = 0;
                else
                        Ot->data[i][j]= 255;
                }
        }

}

void SD_Full_Step_NO_Morpho()
{
        //printf("Démarage SD sans Morpho\n");
        image_t ImgRead, Ot, Mt, Vt;
        int k;
        char nomFichier[50];

        readPGM("car3/car_3000.pgm",&ImgRead);

        initImage_t(&Ot,ImgRead.w,ImgRead.h,ImgRead.maxInt);
        initImage_t(&Vt,ImgRead.w,ImgRead.h,ImgRead.maxInt);

        readPGM("car3/car_3001.pgm",&Mt);

        cpy_Image(&Vt,&Mt);

        for(k=0;k<199;k++){
                Conc("car3/car_",3000+k,nomFichier);

                readPGM(nomFichier,&ImgRead);
                SD_1_step(&ImgRead, &Ot, &Vt, &Mt);


                writePGM(&Ot,k,"SD/SDcar_");
        }
        //printf("FIN SD sans Morpho\n");
}

void SD_Full_Step_Morpho3_3()
{
        //printf("Démarage SD Morpho 3_3\n");
        image_t ImgRead, Ot, Mt, Vt,inter,out;
        int k;
        char nomFichier[50];

        readPGM("car3/car_3000.pgm",&ImgRead);

        initImage_t(&Ot,ImgRead.w,ImgRead.h,ImgRead.maxInt);
        initImage_t(&Vt,ImgRead.w,ImgRead.h,ImgRead.maxInt);
        initImage_t(&inter,ImgRead.w,ImgRead.h,ImgRead.maxInt);
         initImage_t(&out,ImgRead.w,ImgRead.h,ImgRead.maxInt);

        readPGM("car3/car_3001.pgm",&Mt);

        cpy_Image(&Vt,&Mt);

        for(k=0;k<199;k++){
                Conc("car3/car_",3000+k,nomFichier);


                readPGM(nomFichier,&ImgRead);
                SD_1_step(&ImgRead, &Ot, &Vt, &Mt);


                //fermeture3_3(&Ot,&inter,&out);
                //ouverture3_3(&out,&inter,&Ot);

               //ouverture3_3(&Ot,&inter,&out);
              	fermeture3_3(&Ot,&inter,&out);

                

                writePGM(&out,k,"SD_Morpho3_3/SDcar3_3_");
        }
        //printf("FIN SD Morpho 3_3\n");
}

void SD_Full_Step_Morpho5_5()
{
       // printf("Démarage SD Morpho 5_5\n");
        image_t ImgRead, Ot, Mt, Vt,inter,out;
        int k;
        char nomFichier[50];

        readPGM("car3/car_3000.pgm",&ImgRead);

        initImage_t(&Ot,ImgRead.w,ImgRead.h,ImgRead.maxInt);
        initImage_t(&Vt,ImgRead.w,ImgRead.h,ImgRead.maxInt);
        initImage_t(&inter,ImgRead.w,ImgRead.h,ImgRead.maxInt);
         initImage_t(&out,ImgRead.w,ImgRead.h,ImgRead.maxInt);

        readPGM("car3/car_3001.pgm",&Mt);

        cpy_Image(&Vt,&Mt);

        for(k=0;k<199;k++){
                Conc("car3/car_",3000+k,nomFichier);

                readPGM(nomFichier,&ImgRead);
                SD_1_step(&ImgRead, &Ot, &Vt, &Mt);


                fermeture5_5(&Ot,&inter,&out);
                ouverture5_5(&out,&inter,&Ot);

               	//ouverture5_5(&Ot,&inter,&out);
              	//fermeture5_5(&out,&inter,&Ot);

                

                writePGM(&Ot,k,"SD_Morpho5_5/SDcar5_5_");
        }
      //  printf("FIN SD Morpho 5_5\n");
}


int main()
{



/*image_SEE in,out;
readPGM_SEE("car3/car_3100.pgm",&in);
	morpho_SEE_Erosion3_3( &in,  &out);*/
	
	//FD_Full_Step_Morpho3_3_SEE();
	//chrono(FD_Full_Step_Morpho3_3_SEE);
	
	//FD_Full_Step_NO_Morpho_SEE();


//FD SANS OPTI

//FD_Full_Step_NO_Morpho();

//FD_Full_Step_Morpho3_3();

//FD_Full_Step_Morpho5_5();


//SD SANS OPTI

//SD_Full_Step_NO_Morpho();

//SD_Full_Step_Morpho3_3();

//SD_Full_Step_Morpho5_5();


//FD_OPTI

//FD_Full_Step_NO_Morpho_SEE();
//SD_Full_Step_NO_Morpho_SEE();
//FD_Full_Step_Morpho3_3_SEE();
//SD_Full_Step_Morpho3_3_SEE();

//SD OPTI
//SD_Full_Step_NO_Morpho_SEE();


  //main_Bench_FD();
  
  //main_Bench_SD();


// BENCH MARQUE DE CHAQUE FONCTION 

//chrono(FD_Full_Step_NO_Morpho);

//chrono(SD_Full_Step_NO_Morpho);

//chrono(FD_Full_Step_Morpho3_3);

//chrono(FD_Full_Step_Morpho5_5);


//chrono(SD_Full_Step_Morpho3_3);
//chrono(SD_Full_Step_Morpho5_5);


chrono(FD_Full_Step_Morpho3_3_SEE);
int MROC[2][2]={0,0,0,0};
image_t image,verite;

readPGM("FDSEE_Morpho3_3/FDSEEcar_99.pgm",&image);
readPGM("Verite/car_100.pgm",&verite);
	roc(&image,&verite,MROC);
	displayROC(MROC);	
	//chrono(FD_Full_Step_Morpho3_3_SEE);
	

	return 0;
}
