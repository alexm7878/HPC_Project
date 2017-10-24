
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
				  res = MIN(in->data[k][l],res);
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
				 	 res = MAX(in->data[k][l],res);
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
				  res = MIN(in->data[k][l],res);
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
				  res = MAX(in->data[k][l],res);
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


//VERSION SEE

void morpho_SEE_Erosion3_3(image_SEE* in, image_SEE* out)
{
	int i,j,k,l;
	vuint8 res,x,y,z,base1,basem1,res2, res3,valMax;
	vuint8 maskOR,maskORD,maskOR2,maskORD2;

	valMax = _mm_set1_epi8(255);
	maskOR =_mm_set_epi8(255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,0);
	maskORD =_mm_set_epi8(0,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255);

	maskOR2 =_mm_set_epi8(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,255);
	maskORD2 =_mm_set_epi8(255,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0);


for(i=0;i<in->h;i++)
{
	for(j=0;j<in->w;j++)
	{
		x = _mm_load_si128(&in->data[i-1][j]);
		y = _mm_load_si128(&in->data[i][j]);
		z = _mm_load_si128(&in->data[i+1][j]);

		res = _mm_min_epu8(x,y);
		res = _mm_min_epu8(res,z);

		//display_vuint8(res, "%d ", "res "); puts("");

		base1=_mm_bsrli_si128(res,1);
		basem1=_mm_bslli_si128(res,1);
		base1 = _mm_or_si128(maskORD2,base1);
		basem1 = _mm_or_si128(maskOR2,basem1);
		//display_vuint8(base1, "%d ", "base1 "); puts("");
		//display_vuint8(basem1, "%d ", "basem1 "); puts("");

		x = _mm_load_si128(&in->data[i-1][j+1]);
		y = _mm_load_si128(&in->data[i][j+1]);
		z = _mm_load_si128(&in->data[i+1][j+1]);

		res2 = _mm_min_epu8(x,y);

		res2 = _mm_min_epu8(res2,z);

		res2=_mm_bsrli_si128(res2,15);
		res2=_mm_bslli_si128(res2,15);
		res2 = _mm_or_si128(maskORD,res2);
		

		x = _mm_load_si128(&in->data[i-1][j-1]);
		y = _mm_load_si128(&in->data[i][j-1]);
		z = _mm_load_si128(&in->data[i+1][j-1]);

		res3 = _mm_min_epu8(x,y);
		res3 = _mm_min_epu8(res3,z);
		res3=_mm_bslli_si128(res3,15);
		res3 = _mm_or_si128(maskOR,res3);
		//display_vuint8(res3, "%d ", "res3 "); puts("");

		res = _mm_min_epu8(res,base1);
		basem1 = _mm_min_epu8(basem1,res2);
		res = _mm_min_epu8(res,basem1);
		res = _mm_min_epu8(res,res3);
		//display_vuint8(res, "%d ", "res "); puts("");

		_mm_store_si128(&out->data[i][j],res);

	}
}
	

	

	
	

}