
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

 	card = card_vfloat32();

 	si0 = 0;
    si1 = Image->h/card -1;
    s2v1D(si0, si1, card, &vi0, &vi1);
    v2m1D(vi0, vi1, card, &mi0, &mi1);

    Image->data = vf32vectorArray(mi0, mi1);

    si0 = 0;
    si1 = Image->w -1;
    s2v1D(si0, si1, card, &vi0, &vi1);
    v2m1D(vi0, vi1, card, &mi0, &mi1);

    for(i=0;i<Image->h/card;i++)
    {
    	Image->data[i] = vf32vector(vi0, vi1);
    }

}

void copyImage_t_to_Image_SEE(image_t* imaget, image_SEE* imageSEE)
{
	int i,j,k;
	for(i=0;i<imaget->h;i++)
		{
			for(j=0;j<imaget->w;j++)
			{
				for(k=0;k<4;k++)
				{
					
					imageSEE->data[i/4][j][k] = imaget->data[i][j];
					
				}
			}
		}

}

void copyImage_SEE_to_Image_t(image_SEE* imageSEE,image_t* imaget)
{
	int i,j,k;
	for(i=0;i<imaget->h;i++)
		{
			for(j=0;j<imaget->w;j++)
			{
				for(k=0;k<4;k++)
					imaget->data[i][j/80] = imageSEE->data[i/4][j][k] ;
			}
		}

}