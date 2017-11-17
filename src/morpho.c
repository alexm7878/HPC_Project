
#include "morpho.h"


// AND
void morpho_Erosion3_3(image_t* in, image_t* out)
{
	int i,j,k,l;
	uint8_t res;
	for(i=0;i<in->h;i++ )
	{
		for(j=0;j<in->w;j++ )
		{
			res =255;
			for(k=i-1;k<i+2;k++ )
			{
				for(l=j-1;l<j+2;l++ )
				{
				  res = in->data[k][l] & res;
				}
			}
			out->data[i][j] = res;
		}
	}
}

// OR 
void morpho_Dilatation3_3(image_t* in, image_t* out)
{
	int i,j,k,l;
	uint8_t res;
	for(i=0;i<in->h;i++ )
	{
		for(j=0;j<in->w;j++ )
		{
			res =0;
			for(k=i-1;k<i+2;k++ )
			{
				for(l=j-1;l<j+2;l++ )
				{
				 	 res = in->data[k][l] | res;
				}
			}
			out->data[i][j] = res;
		}

	}
}

void morpho_Erosion5_5(image_t* in,image_t* out)
{

	int i,j,k,l;
	uint8_t res;
	for(i=0;i<in->h;i++ )
	{
		for(j=0;j<in->w;j++ )
		{
			res =255;
			for(k=i-2;k<i+3;k++ )
			{
				for(l=j-2;l<j+3;l++ )
				{
				  res = in->data[k][l] & res;
				}
			}
			out->data[i][j] = res;
		}

	}
}


//OR
void morpho_Dilatation5_5(image_t* in,image_t* out)
{
	int i,j,k,l;
	uint8_t res;
	for(i=0;i<in->h;i++ )
	{
		for(j=0;j<in->w;j++ )
		{
			res =0;
			for(k=i-2;k<i+3;k++ )
			{
				for(l=j-2;l<j+3;l++ )
				{
				  res = in->data[k][l] | res;
				}
			}
			out->data[i][j] = res;
		}

	}

}

//EROSION +DILATATION
void fermeture3_3(image_t* in,image_t* inter,image_t* out)
{
	morpho_Erosion3_3(in,inter);
	morpho_Dilatation3_3(inter,out);

}

void ouverture3_3(image_t* in, image_t* inter ,image_t* out)
{
	morpho_Dilatation3_3(in,inter);
	morpho_Erosion3_3(inter,out);
}

void fermeture5_5(image_t* in, image_t* inter ,image_t* out)
{
	morpho_Erosion5_5(in,inter);
	morpho_Dilatation5_5(inter,out);
}

void ouverture5_5(image_t* in, image_t* inter, image_t* out)
{
	morpho_Dilatation5_5(in,inter);
	morpho_Erosion5_5(inter,out);
}


//VERSION SSE

void morpho_SSE_Erosion3_3(image_SSE* in, image_SSE* out)
{
	int i,j,k,l;
	vuint8 res,c0,x,y,z,c0_dec1,c0_decMoin1,c1, cMoin1,valMax;
	vuint8 mask1_0_inv,mask1_0,mask1_255_inv,mask1_255;

	valMax = _mm_set1_epi8(255);
	mask1_0_inv =_mm_set_epi8(255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,0);
	mask1_0 =_mm_set_epi8(0,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255);

	mask1_255_inv =_mm_set_epi8(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,255);
	mask1_255 =_mm_set_epi8(255,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0);


	for(i=0;i<in->h;i++)
	{
		for(j=0;j<in->w/16;j++)
		{
			x = _mm_load_si128(&in->data[i-1][j]);
			y = _mm_load_si128(&in->data[i][j]);
			z = _mm_load_si128(&in->data[i+1][j]);
		
			c0 = _mm_and_si128(x,y);
			c0 = _mm_and_si128(c0,z);

			//base décalé de 1 a droite
			c0_dec1=_mm_bsrli_si128(c0,1);
			c0_dec1 = _mm_or_si128(mask1_255,c0_dec1);

		
			//base décalé de 1 a gauche
			c0_decMoin1=_mm_bslli_si128(c0,1);
			c0_decMoin1 = _mm_or_si128(mask1_255_inv,c0_decMoin1);

			//RECUPERATION DE LA LIGNE suivante
			x = _mm_load_si128(&in->data[i-1][j+1]);
			y = _mm_load_si128(&in->data[i][j+1]);
			z = _mm_load_si128(&in->data[i+1][j+1]);
			
			c1 = _mm_and_si128(x,y);
			c1 = _mm_and_si128(c1,z);
			c1=_mm_bslli_si128(c1,15);
			c1 = _mm_or_si128(mask1_0,c1);

				
		//RECUPERATION DE LA LIGNE PRECEDENTE
			x = _mm_load_si128(&in->data[i-1][j-1]);
			y = _mm_load_si128(&in->data[i][j-1]);
			z = _mm_load_si128(&in->data[i+1][j-1]);
			
			cMoin1 = _mm_and_si128(x,y);
			cMoin1 = _mm_and_si128(cMoin1,z);
			cMoin1=_mm_bsrli_si128(cMoin1,15);
			cMoin1 = _mm_or_si128(mask1_0_inv,cMoin1);


			res = _mm_and_si128(c0,c0_dec1);
			res = _mm_and_si128(res,c0_decMoin1);
			res = _mm_and_si128(res,c1);
			res = _mm_and_si128(res,cMoin1);

			_mm_store_si128(&out->data[i][j],res);

		}
	}
}

void morpho_SSE_Dilatation3_3(image_SSE* in, image_SSE* out)
{
	int i,j,k,l;
	vuint8 c0,res,x,y,z,c0_dec1,c0_decMoin1,c1, cMoin1,valMax;
	vuint8 mask1_0_inv,mask1_0,mask1_255_inv,mask1_255;

	valMax = _mm_set1_epi8(255);
	mask1_0_inv =_mm_set_epi8(255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,0);
	mask1_0 =_mm_set_epi8(0,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255);

	mask1_255_inv =_mm_set_epi8(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,255);
	mask1_255 =_mm_set_epi8(255,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0);


	for(i=0;i<in->h;i++)
	{
		for(j=0;j<in->w/16;j++)
		{
			x = _mm_load_si128(&in->data[i-1][j]);
			y = _mm_load_si128(&in->data[i][j]);
			z = _mm_load_si128(&in->data[i+1][j]);
		
			c0 = _mm_or_si128(x,y);
			c0 = _mm_or_si128(c0,z);

			//base décalé de 1 a droite
			c0_dec1=_mm_bsrli_si128(c0,1);
			c0_dec1 = _mm_and_si128(mask1_0,c0);
		
			//base décalé de 1 a gauche
			c0_decMoin1=_mm_bslli_si128(c0,1);
			c0_decMoin1 = _mm_and_si128(mask1_0_inv,c0_decMoin1);


			//RECUPERATION DE LA LIGNE suivante
			x = _mm_load_si128(&in->data[i-1][j+1]);
			y = _mm_load_si128(&in->data[i][j+1]);
			z = _mm_load_si128(&in->data[i+1][j+1]);

			c1 = _mm_or_si128(x,y);
			c1 = _mm_or_si128(c1,z);
			c1=_mm_bslli_si128(c1,15);
			c1 = _mm_and_si128(mask1_255,c1);
				
		//RECUPERATION DE LA LIGNE PRECEDENTE
			x = _mm_load_si128(&in->data[i-1][j-1]);
			y = _mm_load_si128(&in->data[i][j-1]);
			z = _mm_load_si128(&in->data[i+1][j-1]);
		
			cMoin1 = _mm_or_si128(x,y);
			cMoin1 = _mm_or_si128(cMoin1,z);
			cMoin1=_mm_bsrli_si128(cMoin1,15);
			cMoin1 = _mm_and_si128(mask1_255_inv,cMoin1);

			res = _mm_or_si128(c0,c0_dec1);
			res = _mm_or_si128(res,c0_decMoin1);
			res = _mm_or_si128(res,c1);
			res = _mm_or_si128(res,cMoin1);

			_mm_store_si128(&out->data[i][j],res);

		}
	}
	
}

void fermeture_SSE3_3(image_SSE* in, image_SSE* inter, image_SSE* out)
{
	morpho_SSE_Erosion3_3(in,inter);
	morpho_SSE_Dilatation3_3(inter,out);
}
void ouverture_SSE3_3(image_SSE* in, image_SSE* inter, image_SSE* out)
{
	morpho_SSE_Dilatation3_3(inter,out);
	morpho_SSE_Erosion3_3(in,inter);
	
}


void morpho_SSE_Erosion5_5(image_SSE* in, image_SSE* out)
{
	int i,j,k,l;
	vuint8 res, c0, x, y, z, c0_dec1, c0_dec2, c0_decMoin1, c0_decMoin2, res2, c1 , res3, cMoin1 ,valMax , c1_dec1, c1_dec2, cMoin1_dec1, cMoin1_dec2;

	vuint8 mask1_0_inv,mask2_0_inv, mask1_0, mask2_0 ,mask1_255_inv, mask2_255_inv ,mask1_255, mask2_255;

	valMax = _mm_set1_epi8(255);
	mask1_0_inv =_mm_set_epi8(255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,0);
	mask2_0_inv =_mm_set_epi8(255,255,255,255,255,255,255,255,255,255,255,255,255,255,0,0);
	mask1_0 =_mm_set_epi8(0,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255);
	mask2_0 =_mm_set_epi8(0,0,255,255,255,255,255,255,255,255,255,255,255,255,255,255);

	mask1_255_inv =_mm_set_epi8(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,255);
	mask2_255_inv =_mm_set_epi8(0,0,0,0,0,0,0,0,0,0,0,0,0,0,255,255);
	mask1_255 =_mm_set_epi8(255,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0);
	mask2_255 =_mm_set_epi8(255,255,0,0,0,0,0,0,0,0,0,0,0,0,0,0);


	for(i=0;i<in->h;i++)
	{
		for(j=0;j<in->w/16;j++)
		{
		x = _mm_load_si128(&in->data[i-1][j]);
		y = _mm_load_si128(&in->data[i][j]);
		z = _mm_load_si128(&in->data[i+1][j]);
	
		//display_vuint8(x, "%d ", "X "); puts("");
		c0 = _mm_and_si128(x,y);
		c0 = _mm_and_si128(c0,z);

		//display_vuint8(res, "%d ", "res "); puts("");

		//base décalé de 1 a droite
		c0_dec1=_mm_bsrli_si128(c0,1);
		//display_vuint8(base1, "%d ", "base1 "); puts("");
		c0_dec1 = _mm_or_si128(mask1_255,c0_dec1);
	//	display_vuint8(base1, "%d ", "base1 "); puts("");

		//base décalé de 2 a droite
		c0_dec2=_mm_bsrli_si128(c0,2);
		c0_dec2 = _mm_or_si128(mask2_255,c0_dec2);

		//base décalé de 1 a gauche
		c0_decMoin1=_mm_bslli_si128(c0,1);
		//display_vuint8(basem1, "%d ", "basem1 "); puts("");
		c0_decMoin1 = _mm_or_si128(mask1_255_inv,c0_decMoin1);
		
		//display_vuint8(basem1, "%d ", "basem1 "); puts("");

		//base décalé de 2 a gauche
		c0_decMoin2=_mm_bslli_si128(c0,2);
		//display_vuint8(basem1, "%d ", "basem1 "); puts("");
		c0_decMoin2 = _mm_or_si128(mask2_255_inv,c0_decMoin2);

		//RECUPERATION DE LA LIGNE suivante
		x = _mm_load_si128(&in->data[i-1][j+1]);
		y = _mm_load_si128(&in->data[i][j+1]);
		z = _mm_load_si128(&in->data[i+1][j+1]);

		
		c1 = _mm_and_si128(x,y);

		c1 = _mm_and_si128(c1,z);
		//display_vuint8(res2, "%d ", "z "); puts("");
		
		c1_dec1=_mm_bslli_si128(c1,15);
		c1_dec2 = _mm_bslli_si128(c1,14);
		//res2=_mm_bslli_si128(res2,15);
		
		c1_dec1 = _mm_or_si128(mask1_0,c1_dec1);
		c1_dec2 = _mm_or_si128(mask2_0,c1_dec2);
	//	display_vuint8(res2, "%d ", "res2 "); puts(""); 

	//RECUPERATION DE LA LIGNE PRECEDENTE
		x = _mm_load_si128(&in->data[i-1][j-1]);
		y = _mm_load_si128(&in->data[i][j-1]);
		z = _mm_load_si128(&in->data[i+1][j-1]);
		
		cMoin1 = _mm_and_si128(x,y);
		cMoin1 = _mm_and_si128(cMoin1,z);

		cMoin1_dec1=_mm_bsrli_si128(cMoin1,15);
		cMoin1_dec1 = _mm_or_si128(mask1_0_inv,cMoin1_dec1);

		cMoin1_dec2=_mm_bsrli_si128(cMoin1,14);
		cMoin1_dec2 = _mm_or_si128(mask2_0_inv,cMoin1_dec2);

		res = _mm_and_si128(c0,c0_dec1);
		res = _mm_and_si128(res,c0_decMoin1);
		res = _mm_and_si128(res,c0_dec2);
		res = _mm_and_si128(res,c0_decMoin2);
		res = _mm_and_si128(res,c1_dec1);
		res = _mm_and_si128(res,c1_dec2);
		res = _mm_and_si128(res,cMoin1_dec1);
		res = _mm_and_si128(res,cMoin1_dec2);

		_mm_store_si128(&out->data[i][j],res);

		}
	}
}

void morpho_SSE_Dilatation5_5(image_SSE* in, image_SSE* out)
{
	int i,j,k,l;
	vuint8 res, c0,x,y,z,c0_dec1,c0_dec2, c0_decMoin1, c0_decMoin2, res2, c1, res3, cMoin1, valMax , c1_dec1, c1_dec2, cMoin1_dec1, cMoin1_dec2;

	vuint8 mask1_0_inv,mask2_0_inv, mask1_0, mask2_0 ,mask1_255_inv, mask2_255_inv ,mask1_255, mask2_255;


	valMax = _mm_set1_epi8(255);
	mask1_0_inv =_mm_set_epi8(255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,0);
	mask2_0_inv =_mm_set_epi8(255,255,255,255,255,255,255,255,255,255,255,255,255,255,0,0);
	mask1_0 =_mm_set_epi8(0,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255);
	mask2_0 =_mm_set_epi8(0,0,255,255,255,255,255,255,255,255,255,255,255,255,255,255);

	mask1_255_inv =_mm_set_epi8(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,255);
	mask2_255_inv =_mm_set_epi8(0,0,0,0,0,0,0,0,0,0,0,0,0,0,255,255);
	mask1_255 =_mm_set_epi8(255,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0);
	mask2_255 =_mm_set_epi8(255,255,0,0,0,0,0,0,0,0,0,0,0,0,0,0);


	for(i=0;i<in->h;i++)
	{
		for(j=0;j<in->w/16;j++)
		{
		x = _mm_load_si128(&in->data[i-1][j]);
		y = _mm_load_si128(&in->data[i][j]);
		z = _mm_load_si128(&in->data[i+1][j]);
	
		//display_vuint8(x, "%d ", "X "); puts("");
		c0 = _mm_or_si128(x,y);
		c0 = _mm_or_si128(c0,z);

		//display_vuint8(res, "%d ", "res "); puts("");

		//base décalé de 1 a droite
		c0_dec1=_mm_bsrli_si128(c0,1);
		//display_vuint8(base1, "%d ", "base1 "); puts("");
		c0_dec1 = _mm_and_si128(mask1_0,c0_dec1);
	//	display_vuint8(base1, "%d ", "base1 "); puts("");

		//base décalé de 2 a droite
		c0_dec2=_mm_bsrli_si128(c0,2);
		c0_dec2 = _mm_and_si128(mask2_0,c0_dec2);

		//base décalé de 1 a gauche
		c0_decMoin1=_mm_bslli_si128(c0,1);
		//display_vuint8(basem1, "%d ", "basem1 "); puts("");
		c0_decMoin1 = _mm_and_si128(mask1_0_inv,c0_decMoin1);
		
		//display_vuint8(basem1, "%d ", "basem1 "); puts("");

		//base décalé de 2 a gauche
		c0_decMoin2=_mm_bslli_si128(c0,2);
		//display_vuint8(basem1, "%d ", "basem1 "); puts("");
		c0_decMoin2 = _mm_and_si128(mask2_0_inv,c0_decMoin2);

		//RECUPERATION DE LA LIGNE suivante
		x = _mm_load_si128(&in->data[i-1][j+1]);
		y = _mm_load_si128(&in->data[i][j+1]);
		z = _mm_load_si128(&in->data[i+1][j+1]);

		
		c1 = _mm_or_si128(x,y);

		c1 = _mm_or_si128(c1,z);
		//display_vuint8(res2, "%d ", "z "); puts("");
		
		c1_dec1=_mm_bslli_si128(c1,15);
		c1_dec2 = _mm_bslli_si128(c1,14);
		//res2=_mm_bslli_si128(res2,15);
		
		c1_dec1 = _mm_and_si128(mask1_255,c1_dec1);
		c1_dec2 = _mm_and_si128(mask2_255,c1_dec2);
	//	display_vuint8(res2, "%d ", "res2 "); puts(""); 

	//RECUPERATION DE LA LIGNE PRECEDENTE
		x = _mm_load_si128(&in->data[i-1][j-1]);
		y = _mm_load_si128(&in->data[i][j-1]);
		z = _mm_load_si128(&in->data[i+1][j-1]);
		
		cMoin1 = _mm_or_si128(x,y);
		cMoin1 = _mm_or_si128(cMoin1,z);
		//display_vuint8(res3, "%d ", "res  "); puts("");


		cMoin1_dec1=_mm_bsrli_si128(cMoin1,15);
		cMoin1_dec2=_mm_bsrli_si128(cMoin1,14);
		//display_vuint8(res3, "%d ", "res3 "); puts("");
		cMoin1_dec1 = _mm_and_si128(mask1_255_inv,cMoin1_dec1);
		cMoin1_dec2 = _mm_and_si128(mask2_255_inv,cMoin1_dec2);
		//display_vuint8(res3, "%d ", "res4 "); puts("");

		res = _mm_or_si128(c0,c0_dec1);
		res = _mm_or_si128(res,c0_decMoin1);
		res = _mm_or_si128(res,c0_dec2);
		res = _mm_or_si128(res,c0_decMoin2);
		res = _mm_or_si128(res,c1_dec1);
		res = _mm_or_si128(res,c1_dec2);
		res = _mm_or_si128(res,cMoin1_dec1);
		res = _mm_or_si128(res,cMoin1_dec2);
		//display_vuint8(res, "%d ", "final "); puts("");

		_mm_store_si128(&out->data[i][j],res);

		}
	}
}

void fermeture_SSE5_5(image_SSE* in, image_SSE* inter, image_SSE* out)
{
	morpho_SSE_Erosion5_5(in,inter);
	morpho_SSE_Dilatation5_5(inter,out);
}
void ouverture_SSE5_5(image_SSE* in, image_SSE* inter, image_SSE* out)
{
	morpho_SSE_Dilatation5_5(inter,out);
	morpho_SSE_Erosion5_5(in,inter);
	
}



void morpho_SSE_Erosion3_3_reducColumn(image_SSE* in, image_SSE* out)
{
	int i=0,j=0,k,l;
	vuint8 res,c0,x,y,z,c0_dec1,c0_decMoin1,c1, cMoin1,res_c1,valMax;
	vuint8 mask1_0_inv,mask1_0,mask1_255_inv,mask1_255;

	valMax = _mm_set1_epi8(255);
	mask1_0_inv =_mm_set_epi8(255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,0);
	mask1_0 =_mm_set_epi8(0,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255);

	mask1_255_inv =_mm_set_epi8(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,255);
	mask1_255 =_mm_set_epi8(255,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0);

			//RECUPERATION DE LA LIGNE
			x = _mm_load_si128(&in->data[-1][0]);
			y = _mm_load_si128(&in->data[0][0]);
			z = _mm_load_si128(&in->data[1][0]);
		
			c0 = _mm_and_si128(x,y);
			c0 = _mm_and_si128(c0,z);

			//RECUPERATION DE LA LIGNE PRECEDENTE
			x = _mm_load_si128(&in->data[-1][-1]);
			y = _mm_load_si128(&in->data[0][-1]);
			z = _mm_load_si128(&in->data[1][-1]);
			
			cMoin1 = _mm_and_si128(x,y);
			cMoin1 = _mm_and_si128(cMoin1,z);
			//display_vuint8(res3, "%d ", "res  "); puts("");

			
	for(i=0;i<in->h;i++)
	{
		for(j=0;j<in->w/16;j++)
		{
			
			//RECUPERATION DE LA LIGNE suivante
			x = _mm_load_si128(&in->data[i-1][j+1]);
			y = _mm_load_si128(&in->data[i][j+1]);
			z = _mm_load_si128(&in->data[i+1][j+1]);

			c1 = _mm_and_si128(x,y);
			c1 = _mm_and_si128(c1,z);

			//display_vuint8(res, "%d ", "res "); puts("");

			//base décalé de 1 a droite
			c0_dec1=_mm_bsrli_si128(c0,1);
			//display_vuint8(base1, "%d ", "base1 "); puts("");
			c0_dec1 = _mm_or_si128(mask1_255,c0_dec1);
		//	display_vuint8(base1, "%d ", "base1 "); puts("");
		
			//base décalé de 1 a gauche
			c0_decMoin1=_mm_bslli_si128(c0,1);
			//display_vuint8(basem1, "%d ", "basem1 "); puts("");
			c0_decMoin1 = _mm_or_si128(mask1_255_inv,c0_decMoin1);
			

			//display_vuint8(res2, "%d ", "z "); puts("");	
			res_c1=_mm_bslli_si128(c1,15);
			//res2=_mm_bslli_si128(res2,15);	
			res_c1 = _mm_or_si128(mask1_0,res_c1);
		//	display_vuint8(res2, "%d ", "res2 "); puts(""); 
				
		

			cMoin1=_mm_bsrli_si128(cMoin1,15);
			//display_vuint8(res3, "%d ", "res3 "); puts("");
			cMoin1 = _mm_or_si128(mask1_0_inv,cMoin1);
			//display_vuint8(res3, "%d ", "res4 "); puts("");

			res = _mm_and_si128(c0,c0_dec1);
			res = _mm_and_si128(res,c0_decMoin1);
			res = _mm_and_si128(res,res_c1);
			res = _mm_and_si128(res,cMoin1);

			cMoin1 =c0;
			c0 = c1;

			_mm_store_si128(&out->data[i][j],res);	
		}
	}
}


void morpho_SSE_Dilatation3_3_reducColumn(image_SSE* in, image_SSE* out)
{
	int i=0,j=0,k,l;
	vuint8 c0,res,x,y,z,c0_dec1,c0_decMoin1,c1, cMoin1,res_c1,valMax;
	vuint8 mask1_0_inv,mask1_0,mask1_255_inv,mask1_255;

	valMax = _mm_set1_epi8(255);
	mask1_0_inv =_mm_set_epi8(255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,0);
	mask1_0 =_mm_set_epi8(0,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255);

	mask1_255_inv =_mm_set_epi8(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,255);
	mask1_255 =_mm_set_epi8(255,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0);


			x = _mm_load_si128(&in->data[i-1][j]);
			y = _mm_load_si128(&in->data[i][j]);
			z = _mm_load_si128(&in->data[i+1][j]);
		
			//display_vuint8(x, "%d ", "X "); puts("");
			c0 = _mm_or_si128(x,y);
			c0 = _mm_or_si128(c0,z);

			//RECUPERATION DE LA LIGNE PRECEDENTE
			x = _mm_load_si128(&in->data[i-1][j-1]);
			y = _mm_load_si128(&in->data[i][j-1]);
			z = _mm_load_si128(&in->data[i+1][j-1]);
		
			cMoin1 = _mm_or_si128(x,y);
			cMoin1 = _mm_or_si128(cMoin1,z);

	for(i=0;i<in->h;i++)
	{
		for(j=0;j<in->w/16;j++)
		{
			

			//display_vuint8(res, "%d ", "res "); puts("");

			//base décalé de 1 a droite
			c0_dec1=_mm_bsrli_si128(c0,1);
			//display_vuint8(base1, "%d ", "base1 "); puts("");
			c0_dec1 = _mm_and_si128(mask1_0,c0_dec1);
		//	display_vuint8(base1, "%d ", "base1 "); puts("");
		
			//base décalé de 1 a gauche
			c0_decMoin1=_mm_bslli_si128(c0,1);
			//display_vuint8(basem1, "%d ", "basem1 "); puts("");
			c0_decMoin1 = _mm_and_si128(mask1_0_inv,c0_decMoin1);
			
			//display_vuint8(basem1, "%d ", "basem1 "); puts("");


			//RECUPERATION DE LA LIGNE suivante
			x = _mm_load_si128(&in->data[i-1][j+1]);
			y = _mm_load_si128(&in->data[i][j+1]);
			z = _mm_load_si128(&in->data[i+1][j+1]);

			c1 = _mm_or_si128(x,y);

			c1 = _mm_or_si128(c1,z);

			//display_vuint8(res2, "%d ", "res2 "); puts("");
			res_c1=_mm_bslli_si128(c1,15);
			//res2=_mm_bslli_si128(res2,15);
			
			res_c1 = _mm_and_si128(mask1_255,res_c1);
			//display_vuint8(res2, "%d ", "res2 "); puts(""); 
				
		
			//display_vuint8(res3, "%d ", "res "); puts("");
			cMoin1=_mm_bsrli_si128(cMoin1,15);
			//display_vuint8(res3, "%d ", "res3 "); puts("");
			cMoin1 = _mm_and_si128(mask1_255_inv,cMoin1);
			//display_vuint8(res3, "%d ", "res3 "); puts("");

			res = _mm_or_si128(c0,c0_dec1);
			res = _mm_or_si128(res,c0_decMoin1);
			res = _mm_or_si128(res,res_c1);
			res = _mm_or_si128(res,cMoin1);
			//display_vuint8(res, "%d ", "final "); puts("");

			_mm_store_si128(&out->data[i][j],res);

			cMoin1 = c0;
			c0 = c1;

		}
	}
	
}


void morpho_SSE_Erosion5_5_reducColumn(image_SSE* in, image_SSE* out)
{
	int i=0,j=0,k,l;
	vuint8 res, c0, x, y, z, c0_dec1, c0_dec2, c0_decMoin1, c0_decMoin2, res2, c1 , res3, cMoin1 ,valMax , c1_dec1, c1_dec2, cMoin1_dec1, cMoin1_dec2;

	vuint8 mask1_0_inv,mask2_0_inv, mask1_0, mask2_0 ,mask1_255_inv, mask2_255_inv ,mask1_255, mask2_255;

	valMax = _mm_set1_epi8(255);
	mask1_0_inv =_mm_set_epi8(255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,0);
	mask2_0_inv =_mm_set_epi8(255,255,255,255,255,255,255,255,255,255,255,255,255,255,0,0);
	mask1_0 =_mm_set_epi8(0,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255);
	mask2_0 =_mm_set_epi8(0,0,255,255,255,255,255,255,255,255,255,255,255,255,255,255);

	mask1_255_inv =_mm_set_epi8(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,255);
	mask2_255_inv =_mm_set_epi8(0,0,0,0,0,0,0,0,0,0,0,0,0,0,255,255);
	mask1_255 =_mm_set_epi8(255,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0);
	mask2_255 =_mm_set_epi8(255,255,0,0,0,0,0,0,0,0,0,0,0,0,0,0);


		x = _mm_load_si128(&in->data[i-1][j]);
		y = _mm_load_si128(&in->data[i][j]);
		z = _mm_load_si128(&in->data[i+1][j]);
	
		//display_vuint8(x, "%d ", "X "); puts("");
		c0 = _mm_and_si128(x,y);
		c0 = _mm_and_si128(c0,z);

		//RECUPERATION DE LA LIGNE PRECEDENTE
		x = _mm_load_si128(&in->data[i-1][j-1]);
		y = _mm_load_si128(&in->data[i][j-1]);
		z = _mm_load_si128(&in->data[i+1][j-1]);
		
		cMoin1 = _mm_and_si128(x,y);
		cMoin1 = _mm_and_si128(cMoin1,z);


	for(i=0;i<in->h;i++)
	{
		for(j=0;j<in->w/16;j++)
		{
		

		//display_vuint8(res, "%d ", "res "); puts("");

		//base décalé de 1 a droite
		c0_dec1=_mm_bsrli_si128(c0,1);
		//display_vuint8(base1, "%d ", "base1 "); puts("");
		c0_dec1 = _mm_or_si128(mask1_255,c0_dec1);
	//	display_vuint8(base1, "%d ", "base1 "); puts("");

		//base décalé de 2 a droite
		c0_dec2=_mm_bsrli_si128(c0,2);
		c0_dec2 = _mm_or_si128(mask2_255,c0_dec2);

		//base décalé de 1 a gauche
		c0_decMoin1=_mm_bslli_si128(c0,1);
		//display_vuint8(basem1, "%d ", "basem1 "); puts("");
		c0_decMoin1 = _mm_or_si128(mask1_255_inv,c0_decMoin1);
		
		//display_vuint8(basem1, "%d ", "basem1 "); puts("");

		//base décalé de 2 a gauche
		c0_decMoin2=_mm_bslli_si128(c0,2);
		//display_vuint8(basem1, "%d ", "basem1 "); puts("");
		c0_decMoin2 = _mm_or_si128(mask2_255_inv,c0_decMoin2);

		//RECUPERATION DE LA LIGNE suivante
		x = _mm_load_si128(&in->data[i-1][j+1]);
		y = _mm_load_si128(&in->data[i][j+1]);
		z = _mm_load_si128(&in->data[i+1][j+1]);

		
		c1 = _mm_and_si128(x,y);

		c1 = _mm_and_si128(c1,z);
		//display_vuint8(res2, "%d ", "z "); puts("");
		
		c1_dec1=_mm_bslli_si128(c1,15);
		c1_dec2 = _mm_bslli_si128(c1,14);
		//res2=_mm_bslli_si128(res2,15);
		
		c1_dec1 = _mm_or_si128(mask1_0,c1_dec1);
		c1_dec2 = _mm_or_si128(mask2_0,c1_dec2);
	//	display_vuint8(res2, "%d ", "res2 "); puts(""); 

	

		cMoin1_dec1=_mm_bsrli_si128(cMoin1,15);
		cMoin1_dec1 = _mm_or_si128(mask1_0_inv,cMoin1_dec1);

		cMoin1_dec2=_mm_bsrli_si128(cMoin1,14);
		cMoin1_dec2 = _mm_or_si128(mask2_0_inv,cMoin1_dec2);

		res = _mm_and_si128(c0,c0_dec1);
		res = _mm_and_si128(res,c0_decMoin1);
		res = _mm_and_si128(res,c0_dec2);
		res = _mm_and_si128(res,c0_decMoin2);
		res = _mm_and_si128(res,c1_dec1);
		res = _mm_and_si128(res,c1_dec2);
		res = _mm_and_si128(res,cMoin1_dec1);
		res = _mm_and_si128(res,cMoin1_dec2);


		cMoin1 = c0;
		c0 = c1;
		_mm_store_si128(&out->data[i][j],res);

		}
	}
}

void morpho_SSE_Dilatation5_5_reducColumn(image_SSE* in, image_SSE* out)
{
	int i=0,j=0,k,l;
	vuint8 res, c0,x,y,z,c0_dec1,c0_dec2, c0_decMoin1, c0_decMoin2, res2, c1, res3, cMoin1, valMax , c1_dec1, c1_dec2, cMoin1_dec1, cMoin1_dec2;

	vuint8 mask1_0_inv,mask2_0_inv, mask1_0, mask2_0 ,mask1_255_inv, mask2_255_inv ,mask1_255, mask2_255;


	valMax = _mm_set1_epi8(255);
	mask1_0_inv =_mm_set_epi8(255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,0);
	mask2_0_inv =_mm_set_epi8(255,255,255,255,255,255,255,255,255,255,255,255,255,255,0,0);
	mask1_0 =_mm_set_epi8(0,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255);
	mask2_0 =_mm_set_epi8(0,0,255,255,255,255,255,255,255,255,255,255,255,255,255,255);

	mask1_255_inv =_mm_set_epi8(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,255);
	mask2_255_inv =_mm_set_epi8(0,0,0,0,0,0,0,0,0,0,0,0,0,0,255,255);
	mask1_255 =_mm_set_epi8(255,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0);
	mask2_255 =_mm_set_epi8(255,255,0,0,0,0,0,0,0,0,0,0,0,0,0,0);


		x = _mm_load_si128(&in->data[i-1][j]);
		y = _mm_load_si128(&in->data[i][j]);
		z = _mm_load_si128(&in->data[i+1][j]);
	
		//display_vuint8(x, "%d ", "X "); puts("");
		c0 = _mm_or_si128(x,y);
		c0 = _mm_or_si128(c0,z);


		//RECUPERATION DE LA LIGNE PRECEDENTE
		x = _mm_load_si128(&in->data[i-1][j-1]);
		y = _mm_load_si128(&in->data[i][j-1]);
		z = _mm_load_si128(&in->data[i+1][j-1]);
		
		cMoin1 = _mm_or_si128(x,y);
		cMoin1 = _mm_or_si128(cMoin1,z);
		//display_vuint8(res3, "%d ", "res  "); puts("");

	for(i=0;i<in->h;i++)
	{
		for(j=0;j<in->w/16;j++)
		{
		

		//display_vuint8(res, "%d ", "res "); puts("");

		//base décalé de 1 a droite
		c0_dec1=_mm_bsrli_si128(c0,1);
		//display_vuint8(base1, "%d ", "base1 "); puts("");
		c0_dec1 = _mm_and_si128(mask1_0,c0_dec1);
	//	display_vuint8(base1, "%d ", "base1 "); puts("");

		//base décalé de 2 a droite
		c0_dec2=_mm_bsrli_si128(c0,2);
		c0_dec2 = _mm_and_si128(mask2_0,c0_dec2);

		//base décalé de 1 a gauche
		c0_decMoin1=_mm_bslli_si128(c0,1);
		//display_vuint8(basem1, "%d ", "basem1 "); puts("");
		c0_decMoin1 = _mm_and_si128(mask1_0_inv,c0_decMoin1);
		
		//display_vuint8(basem1, "%d ", "basem1 "); puts("");

		//base décalé de 2 a gauche
		c0_decMoin2=_mm_bslli_si128(c0,2);
		//display_vuint8(basem1, "%d ", "basem1 "); puts("");
		c0_decMoin2 = _mm_and_si128(mask2_0_inv,c0_decMoin2);

		//RECUPERATION DE LA LIGNE suivante
		x = _mm_load_si128(&in->data[i-1][j+1]);
		y = _mm_load_si128(&in->data[i][j+1]);
		z = _mm_load_si128(&in->data[i+1][j+1]);

		
		c1 = _mm_or_si128(x,y);

		c1 = _mm_or_si128(c1,z);
		//display_vuint8(res2, "%d ", "z "); puts("");
		
		c1_dec1=_mm_bslli_si128(c1,15);
		c1_dec2 = _mm_bslli_si128(c1,14);
		//res2=_mm_bslli_si128(res2,15);
		
		c1_dec1 = _mm_and_si128(mask1_255,c1_dec1);
		c1_dec2 = _mm_and_si128(mask2_255,c1_dec2);
	//	display_vuint8(res2, "%d ", "res2 "); puts(""); 

	


		cMoin1_dec1=_mm_bsrli_si128(cMoin1,15);
		cMoin1_dec2=_mm_bsrli_si128(cMoin1,14);
		//display_vuint8(res3, "%d ", "res3 "); puts("");
		cMoin1_dec1 = _mm_and_si128(mask1_255_inv,cMoin1_dec1);
		cMoin1_dec2 = _mm_and_si128(mask2_255_inv,cMoin1_dec2);
		//display_vuint8(res3, "%d ", "res4 "); puts("");

		res = _mm_or_si128(c0,c0_dec1);
		res = _mm_or_si128(res,c0_decMoin1);
		res = _mm_or_si128(res,c0_dec2);
		res = _mm_or_si128(res,c0_decMoin2);
		res = _mm_or_si128(res,c1_dec1);
		res = _mm_or_si128(res,c1_dec2);
		res = _mm_or_si128(res,cMoin1_dec1);
		res = _mm_or_si128(res,cMoin1_dec2);
		//display_vuint8(res, "%d ", "final "); puts("");


		cMoin1 = c0;
		c0 = c1;
		_mm_store_si128(&out->data[i][j],res);

		}
	}
}
