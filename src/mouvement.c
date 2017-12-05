#include "mouvement.h"
#include "mouvement_SSE2.h"
#include "morpho.h"
#include "benchmark.h"
#include "ppm2pgm.h"
#include "util.h"

//execution de l'algorithm FD sur deux images
void FD_1_Step(image_t* ImgRead1, image_t* ImgRead, image_t* dif)
{
	int i,j;
	for(i=0;i<H;i++)
	{
		for(j=0;j<W;j++)
		{
			dif->data[i][j] = fabs(ImgRead1->data[i][j] - ImgRead->data[i][j]);
		}
	}
	for(i=0;i<H;i++)
	{
		for(j=0;j<W;j++)
		{
			if(dif->data[i][j] < TETA )
				dif->data[i][j] = 0;
			else
				dif->data[i][j] = 255;
		}
	}
}


// fonction lancant l'algorithm FD sur toute les photos
void FD_Full_Step_NO_Morpho()
{
	image_t ImgRead;
	image_t ImgRead1;
	image_t dif;
	int i;
	char nomFichier[50] ="";
	char nomFichier2[50] ="";
		
	initImage_t(&dif);



	for(i=0;i<199;i++){
		Conc("car3/car_",3000+i,nomFichier);
		Conc("car3/car_",3000+i+1,nomFichier2);

		readPGM(nomFichier,&ImgRead);
		readPGM(nomFichier2,&ImgRead1);

		FD_1_Step(&ImgRead1,&ImgRead,&dif);
		writePGM(&dif,i,"FD/FDcar_");

		freeImage_t(&ImgRead);
		freeImage_t(&ImgRead1);
	}
	
	freeImage_t(&dif);
}

void FD_Full_Step_Morpho3_3()
{
	image_t ImgRead;
	image_t ImgRead1;
	image_t dif, out,inter;
	int i;

	char nomFichier[50]="";
	char nomFichier2[50]="";		

	initImage_t(&dif);
	initImage_t(&inter);
	initImage_t(&out);


	for(i=0;i<199;i++){
		Conc("car3/car_",3000+i,nomFichier);
		Conc("car3/car_",3000+i+1,nomFichier2);

		readPGM(nomFichier,&ImgRead);
		readPGM(nomFichier2,&ImgRead1);

		FD_1_Step(&ImgRead1,&ImgRead,&dif);

		morpho_Erosion3_3(&dif,&out);

		writePGM(&out,i,"FD_Morpho3_3/FD_Morpho3_3_car_");

		freeImage_t(&ImgRead);
		freeImage_t(&ImgRead1);
	}
	freeImage_t(&dif);
	freeImage_t(&inter);
	freeImage_t(&out);
}

void FD_Full_Step_Morpho5_5()
{
	image_t ImgRead;
	image_t ImgRead1;
	image_t dif, out,inter;
	int i;

	char nomFichier[50];
	char nomFichier2[50];

	
	initImage_t(&dif);
	initImage_t(&out);
	initImage_t(&inter);


	for(i=0;i<199;i++){
		Conc("car3/car_",3000+i,nomFichier);
		Conc("car3/car_",3000+i+1,nomFichier2);

		readPGM(nomFichier,&ImgRead);
		readPGM(nomFichier2,&ImgRead1);

		FD_1_Step(&ImgRead1,&ImgRead,&dif);

		morpho_Erosion5_5(&dif,&out);
		writePGM(&out,i,"FD_Morpho5_5/FD_Morpho5_5_car_");

		freeImage_t(&ImgRead);
		freeImage_t(&ImgRead1);
	}
	freeImage_t(&dif);
	freeImage_t(&inter);
	freeImage_t(&out);
}

void SD_1_step(image_t* ImgRead, image_t* Ot, image_t* Vt, image_t* Mt)
{
	 int i,j;

        for(i=0;i<H;i++)
        {
                for(j=0;j<W;j++)
                {
                        if(Mt->data[i][j] < ImgRead->data[i][j])
                                Mt->data[i][j] = Mt->data[i][j] +1;
                        else if(Mt->data[i][j] > ImgRead->data[i][j])
                                Mt->data[i][j] = Mt->data[i][j] -1;

                        Ot->data[i][j] = fabs(Mt->data[i][j] - ImgRead->data[i][j]);
                    }
                }
         for(i=0;i<H;i++)
        {
                for(j=0;j<W;j++)
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
        for(i=0;i<H;i++)
        {
                for(j=0;j<W;j++)
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

        image_t ImgRead, Ot, Mt, Vt;
        int k;
        char nomFichier[50];


        initImage_t(&Ot);
        initImage_t(&Vt);

         readPGM("car3/car_3001.pgm",&Mt);

		setVal_image(&Vt,VMIN);

        for(k=0;k<199;k++){
                Conc("car3/car_",3000+k,nomFichier);

                readPGM(nomFichier,&ImgRead);
                SD_1_step(&ImgRead, &Ot, &Vt, &Mt);


                writePGM(&Ot,k,"SD/SDcar_");

				freeImage_t(&ImgRead);
		
        }
        freeImage_t(&Ot);
        freeImage_t(&Vt);
        freeImage_t(&Mt);
}

void SD_Full_Step_Morpho3_3()
{
        image_t ImgRead, Ot, Mt, Vt,inter,out;
        int k;
        char nomFichier[50];

        initImage_t(&Ot);
        initImage_t(&Vt);
        initImage_t(&inter);
         initImage_t(&out);

        readPGM("car3/car_3001.pgm",&Mt);

        cpy_Image(&Vt,&Mt);

        for(k=0;k<199;k++){
                Conc("car3/car_",3000+k,nomFichier);

                readPGM(nomFichier,&ImgRead);
                SD_1_step(&ImgRead, &Ot, &Vt, &Mt);
				
				morpho_Erosion3_3(&Ot,&out);

                writePGM(&out,k,"SD_Morpho3_3/SDcar3_3_");
                 freeImage_t(&ImgRead);
        }

         freeImage_t(&Ot);
         freeImage_t(&Vt);
         freeImage_t(&inter);
         freeImage_t(&out);
         freeImage_t(&Mt);
}

void SD_Full_Step_Morpho5_5()
{
        image_t ImgRead, Ot, Mt, Vt,inter,out;
        int k;
        char nomFichier[50];

        readPGM("car3/car_3000.pgm",&ImgRead);

        initImage_t(&Ot);
        initImage_t(&Vt);
        initImage_t(&inter);
         initImage_t(&out);

        readPGM("car3/car_3001.pgm",&Mt);

        cpy_Image(&Vt,&Mt);

        for(k=0;k<199;k++){
                Conc("car3/car_",3000+k,nomFichier);

                readPGM(nomFichier,&ImgRead);
                SD_1_step(&ImgRead, &Ot, &Vt, &Mt);

                morpho_Erosion5_5(&Ot,&out);
               
                writePGM(&Ot,k,"SD_Morpho5_5/SDcar5_5_");
                freeImage_t(&ImgRead);
        }

        freeImage_t(&Ot);
        freeImage_t(&out);
        freeImage_t(&inter);
        freeImage_t(&Vt);
        freeImage_t(&Mt);

}



