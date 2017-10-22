
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

	vuint8 x,y,z ,zero, valMax,teta ,cmp;
 	zero =_mm_set1_epi8(0);
 	valMax = _mm_set1_epi8(255);
	teta = _mm_set1_epi8(TETA);

	for(i=0;i<ImageSEE1->h;i++)
	{
		for(j=0;j<ImageSEE1->w/c;j++)
			{
			x = _mm_load_si128(&ImageSEE2->data[i][j]);
			y = _mm_load_si128(&ImageSEE1->data[i][j]);
			z =_mm_subs_epu8(x,y);

			//display_vuint8(x, "%d ", "x "); puts("");
			//display_vuint8(y, "%d ", "y "); puts("");
			//display_vuint8(z, "%d ", "z "); puts("");
			cmp =_mm_cmplt_epi8(z ,teta);
			x =_mm_or_si128(_mm_and_si128(cmp,zero),_mm_andnot_si128(cmp,valMax));

			//display_vuint8(x, "%d ", "x "); puts("");
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

	readPGM("car3/car_3000.pgm",&ImgRead);

	initImageSEE(&ImageSEE1,ImgRead.w,ImgRead.h,ImgRead.maxInt);
	initImageSEE(&ImageSEE2,ImgRead.w,ImgRead.h,ImgRead.maxInt);
	initImageSEE(&dif,ImgRead.w,ImgRead.h,ImgRead.maxInt);



	for(i=0;i<199;i++){
		Conc("car3/car_",3000+i,nomFichier);
		Conc("car3/car_",3000+i+1,nomFichier2);

		readPGM(nomFichier,&ImgRead);
			//printf("L'image a bien été lu\n ");
		readPGM(nomFichier2,&ImgRead1);
			//printf("L'image a bien été lu\n ");

		copyImage_t_to_Image_SEE(&ImgRead, &ImageSEE1);
		copyImage_t_to_Image_SEE(&ImgRead1, &ImageSEE2);

		FD_1_Step_SEE(&ImageSEE1,&ImageSEE2,&dif);

		copyImage_SEE_to_Image_t(&dif,&ImgRead);
		writePGM(&ImgRead,i,"FDSEE/FDcar_");
	}
	//printf("Fin FD sans morpho\n");
	//freeImage_t(&ImgRead);
}