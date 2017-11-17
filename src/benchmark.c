#include "benchmark.h"
#include "mouvement.h"
/*
 __inline__ uint64_t rdtsc(void)
   {
uint32_t lo, hi;
__asm__ __volatile__ (
        "xorl %%eax,%%eax \n        cpuid"
        ::: "%rax", "%rbx", "%rcx", "%rdx");
__asm__ __volatile__ ("rdtsc" : "=a" (lo), "=d" (hi));
return (uint64_t)hi << 32 | lo;
 }
*/

#ifdef __i386
extern __inline__ uint64_t rdtsc(void) {
  uint64_t x;
  __asm__ volatile ("rdtsc" : "=A" (x));
  return x;
}
#elif defined __amd64
extern __inline__ uint64_t rdtsc(void) {
  uint64_t a, d;
  __asm__ volatile ("rdtsc" : "=a" (a), "=d" (d));
  return (d<<32) | a;
}
#endif


// Lit la fréquence du processeur
// Renvoie 0 en cas d'échec
int LitFrequenceCpu (double* frequence)
{
  const char* prefixe_cpu_mhz = "cpu MHz";
  FILE* F;
  char ligne[300+1];
  char *pos;
  int ok=0;
 
  // Ouvre le fichier
  F = fopen(NOMFICH_CPUINFO, "r");
  if (!F) return 0;
 
  // Lit une ligne apres l'autre
  while (!feof(F))
  {
    // Lit une ligne de texte
    fgets (ligne, sizeof(ligne), F);
 
    // C'est la ligne contenant la frequence?
    if (!strncmp(ligne, prefixe_cpu_mhz, strlen(prefixe_cpu_mhz)))
    {
      // Oui, alors lit la frequence
      pos = strrchr (ligne, ':') +2;
      if (!pos) break;
      if (pos[strlen(pos)-1] == '\n') pos[strlen(pos)-1] = '\0';
      strcpy (ligne, pos);
      strcat (ligne,"e6");
      *frequence = atof (ligne);
      ok = 1;
      break;
    }
  }
  fclose (F);
  return ok;
}


void chrono(void (*ptrfonction)(void))
{
    double t0,t1;
    int i;
    double freq ;
    LitFrequenceCpu(&freq);

    t0= rdtsc();
    //printf("il faut %f cycles\n",t0);

    for(i=0;i<NRUN;i++)
    {
     (*ptrfonction)();
    }

      t1= rdtsc();
    //printf("il faut %f cycles\n",t1);
      
    printf(" total cycles : %f,\n   cycles par iteration : %f ,\n  cycles par image : %f ,\n  cycles par point : %f \n\n",(t1-t0),(t1-t0)/NRUN, (t1-t0)/ (NRUN*200), (t1-t0)/(NRUN*200*320*240));
    printf(" total temps(ms) : %f ,\n  temps par iteration : %f ,\n  temps par image : %f ,\n  temps par pixel : %f \n\n",((t1-t0)/freq)*1000,((t1-t0)/(NRUN*freq))*1000, ((t1-t0)/ (NRUN*200*freq))*1000, ((t1-t0)/(NRUN*200*320*240* freq))*1000);
}


void chronoFD()
{
    double t0,t1;
    int i;
    double freq ;
    LitFrequenceCpu(&freq);

   image_t ImageSSE1,ImageSSE2, Ot;
    readPGM("car3/car_3100.pgm",&ImageSSE1);
    readPGM("car3/car_3101.pgm",&ImageSSE2);
    initImage_t(&Ot);

    t0= rdtsc();
    //printf("il faut %f cycles\n",t0);

    for(i=0;i<NRUN;i++)
    {
     FD_1_Step(&ImageSSE1, &ImageSSE2, &Ot);
    }

      t1= rdtsc();
    //printf("il faut %f cycles\n",t1);
      freeImage_t(&ImageSSE1);
      freeImage_t(&ImageSSE2);
      freeImage_t(&Ot);
      

      
    printf(" total cycles : %f,\n   cycles par iteration : %f ,\n  cycles par image : %f ,\n  cycles par point : %f \n\n",(t1-t0),(t1-t0)/NRUN, (t1-t0)/ (NRUN), (t1-t0)/(NRUN*320*240));
    printf(" total temps(ms) : %f ,\n  temps par iteration : %f ,\n  temps par image : %f ,\n  temps par pixel : %f \n\n",((t1-t0)/freq)*1000,((t1-t0)/(NRUN*freq))*1000, ((t1-t0)/ (NRUN*freq))*1000, ((t1-t0)/(NRUN*320*240* freq))*1000);
}

void chronoFD_SSE()
{
    double t0,t1;
    int i;
    double freq ;
    LitFrequenceCpu(&freq);

   image_SSE ImageSSE1,ImageSSE2, Ot;
    readPGM_SSE("car3/car_3100.pgm",&ImageSSE1);
    readPGM_SSE("car3/car_3101.pgm",&ImageSSE2);
    initImageSSE(&Ot);

    t0= rdtsc();
    //printf("il faut %f cycles\n",t0);

    for(i=0;i<NRUN;i++)
    {
     FD_1_Step_SSE(&ImageSSE1, &ImageSSE2, &Ot);
    }

    freeImageSSE(&ImageSSE1);
      freeImageSSE(&ImageSSE2);
      freeImageSSE(&Ot);

      t1= rdtsc();
    //printf("il faut %f cycles\n",t1);
      
    printf(" total cycles : %f,\n   cycles par iteration : %f ,\n  cycles par image : %f ,\n  cycles par point : %f \n\n",(t1-t0),(t1-t0)/NRUN, (t1-t0)/ (NRUN), (t1-t0)/(NRUN*320*240));
    printf(" total temps(ms) : %f ,\n  temps par iteration : %f ,\n  temps par image : %f ,\n  temps par pixel : %f \n\n",((t1-t0)/freq)*1000,((t1-t0)/(NRUN*freq))*1000, ((t1-t0)/ (NRUN*freq))*1000, ((t1-t0)/(NRUN*320*240*freq))*1000);
}

void chrono_Difference_FD()
{
  chronoFD();
  chronoFD_SSE();
}


void chronoFD_morpho3_3_dilatation()
{
    double t0,t1;
    int i;
    double freq ;
    LitFrequenceCpu(&freq);

   image_t ImageSSE1,ImageSSE2;
    readPGM("car3/car_3100.pgm",&ImageSSE1);
    readPGM("car3/car_3101.pgm",&ImageSSE2);
 

    t0= rdtsc();
    //printf("il faut %f cycles\n",t0);

    for(i=0;i<NRUN;i++)
    {
     morpho_Dilatation3_3(&ImageSSE1, &ImageSSE2);
    }

      t1= rdtsc();

       freeImage_t(&ImageSSE1);
      freeImage_t(&ImageSSE2);
    //printf("il faut %f cycles\n",t1);
      
    printf(" total cycles : %f,\n   cycles par iteration : %f ,\n  cycles par image : %f ,\n  cycles par point : %f \n\n",(t1-t0),(t1-t0)/NRUN, (t1-t0)/ (NRUN), (t1-t0)/(NRUN*320*240));
    printf(" total temps(ms) : %f ,\n  temps par iteration : %f ,\n  temps par image : %f ,\n  temps par pixel : %f \n\n",((t1-t0)/freq)*1000,((t1-t0)/(NRUN*freq))*1000, ((t1-t0)/ (NRUN*freq))*1000, ((t1-t0)/(NRUN*320*240*freq))*1000);
}


void chronoFD_morpho3_3_dilatation_SSE()
{
    double t0,t1;
    int i;
    double freq ;
    LitFrequenceCpu(&freq);

   image_SSE ImageSSE1,ImageSSE2;
    readPGM_SSE("car3/car_3100.pgm",&ImageSSE1);
    readPGM_SSE("car3/car_3101.pgm",&ImageSSE2);
  

    t0= rdtsc();
    //printf("il faut %f cycles\n",t0);

    for(i=0;i<NRUN;i++)
    {
      morpho_SSE_Dilatation3_3(&ImageSSE1, &ImageSSE2);
    }

      t1= rdtsc();
      freeImageSSE(&ImageSSE1);
      freeImageSSE(&ImageSSE2);
    //printf("il faut %f cycles\n",t1);
      
    printf(" total cycles : %f,\n   cycles par iteration : %f ,\n  cycles par image : %f ,\n  cycles par point : %f \n\n",(t1-t0),(t1-t0)/NRUN, (t1-t0)/ (NRUN), (t1-t0)/(NRUN*320*240));
    printf(" total temps(ms) : %f ,\n  temps par iteration : %f ,\n  temps par image : %f ,\n  temps par pixel : %f \n\n",((t1-t0)/freq)*1000,((t1-t0)/(NRUN*freq))*1000, ((t1-t0)/ (NRUN*freq))*1000, ((t1-t0)/(NRUN*320*240* freq))*1000);
}

void chronoFD_morpho3_3_dilatation_SSE_reducColumn()
{
    double t0,t1;
    int i;
    double freq ;
    LitFrequenceCpu(&freq);

   image_SSE ImageSSE1,ImageSSE2;
    readPGM_SSE("car3/car_3100.pgm",&ImageSSE1);
    readPGM_SSE("car3/car_3101.pgm",&ImageSSE2);
  
    t0= rdtsc();
    //printf("il faut %f cycles\n",t0);

    for(i=0;i<NRUN;i++)
    {
      morpho_SSE_Dilatation3_3_reducColumn(&ImageSSE1, &ImageSSE2);
    }

      t1= rdtsc();
      freeImageSSE(&ImageSSE1);
      freeImageSSE(&ImageSSE2);
    //printf("il faut %f cycles\n",t1);
      
    printf(" total cycles : %f,\n   cycles par iteration : %f ,\n  cycles par image : %f ,\n  cycles par point : %f \n\n",(t1-t0),(t1-t0)/NRUN, (t1-t0)/ (NRUN), (t1-t0)/(NRUN*320*240));
    printf(" total temps(ms) : %f ,\n  temps par iteration : %f ,\n  temps par image : %f ,\n  temps par pixel : %f \n\n",((t1-t0)/freq)*1000,((t1-t0)/(NRUN*freq))*1000, ((t1-t0)/ (NRUN*freq))*1000, ((t1-t0)/(NRUN*320*240* freq))*1000);
}

void chrono_Difference_morpho3_3_dilatation()
{
  chronoFD_morpho3_3_dilatation();
  chronoFD_morpho3_3_dilatation_SSE();
  chronoFD_morpho3_3_dilatation_SSE_reducColumn();
}


void chronoFD_morpho3_3_erosion()
{
    double t0,t1;
    int i;
    double freq ;
    LitFrequenceCpu(&freq);

   image_t ImageSSE1,ImageSSE2;
    readPGM("car3/car_3100.pgm",&ImageSSE1);
    readPGM("car3/car_3101.pgm",&ImageSSE2);
 

    t0= rdtsc();
    //printf("il faut %f cycles\n",t0);

    for(i=0;i<NRUN;i++)
    {
     morpho_Erosion3_3(&ImageSSE1, &ImageSSE2);
    }

      t1= rdtsc();
      freeImage_t(&ImageSSE1);
      freeImage_t(&ImageSSE2);
    //printf("il faut %f cycles\n",t1);
      
    printf(" total cycles : %f,\n   cycles par iteration : %f ,\n  cycles par image : %f ,\n  cycles par point : %f \n\n",(t1-t0),(t1-t0)/NRUN, (t1-t0)/ (NRUN), (t1-t0)/(NRUN*320*240));
    printf(" total temps(ms) : %f ,\n  temps par iteration : %f ,\n  temps par image : %f ,\n  temps par pixel : %f \n\n",((t1-t0)/freq)*1000,((t1-t0)/(NRUN*freq))*1000, ((t1-t0)/ (NRUN*freq))*1000, ((t1-t0)/(NRUN*320*240*freq))*1000);
}


void chronoFD_morpho3_3_erosion_SSE()
{
    double t0,t1;
    int i;
    double freq ;
    LitFrequenceCpu(&freq);

   image_SSE ImageSSE1,ImageSSE2;
    readPGM_SSE("car3/car_3100.pgm",&ImageSSE1);
    readPGM_SSE("car3/car_3101.pgm",&ImageSSE2);
  

    t0= rdtsc();
    //printf("il faut %f cycles\n",t0);

    for(i=0;i<NRUN;i++)
    {
      morpho_SSE_Erosion3_3(&ImageSSE1, &ImageSSE2);
    }

      t1= rdtsc();

      freeImageSSE(&ImageSSE1);
      freeImageSSE(&ImageSSE2);
    //printf("il faut %f cycles\n",t1);
      
    printf(" total cycles : %f,\n   cycles par iteration : %f ,\n  cycles par image : %f ,\n  cycles par point : %f \n\n",(t1-t0),(t1-t0)/NRUN, (t1-t0)/ (NRUN), (t1-t0)/(NRUN*320*240));
    printf(" total temps(ms) : %f ,\n  temps par iteration : %f ,\n  temps par image : %f ,\n  temps par pixel : %f \n\n",((t1-t0)/freq)*1000,((t1-t0)/(NRUN*freq))*1000, ((t1-t0)/ (NRUN*freq))*1000, ((t1-t0)/(NRUN*320*240* freq))*1000);
}


void chrono_morpho3_3_erosion_SSE_reducColumn()
{
  double t0,t1;
    int i;
    double freq ;
    LitFrequenceCpu(&freq);

   image_SSE ImageSSE1,ImageSSE2;
    readPGM_SSE("car3/car_3100.pgm",&ImageSSE1);
    readPGM_SSE("car3/car_3101.pgm",&ImageSSE2);
  

    t0= rdtsc();
    //printf("il faut %f cycles\n",t0);

    for(i=0;i<NRUN;i++)
    {
      morpho_SSE_Erosion3_3_reducColumn(&ImageSSE1, &ImageSSE2);
    }

      t1= rdtsc();
       freeImageSSE(&ImageSSE1);
      freeImageSSE(&ImageSSE2);
    //printf("il faut %f cycles\n",t1);
      
    printf(" total cycles : %f,\n   cycles par iteration : %f ,\n  cycles par image : %f ,\n  cycles par point : %f \n\n",(t1-t0),(t1-t0)/NRUN, (t1-t0)/ (NRUN), (t1-t0)/(NRUN*320*240));
    printf(" total temps(ms) : %f ,\n  temps par iteration : %f ,\n  temps par image : %f ,\n  temps par pixel : %f \n\n",((t1-t0)/freq)*1000,((t1-t0)/(NRUN*freq))*1000, ((t1-t0)/ (NRUN*freq))*1000, ((t1-t0)/(NRUN*320*240* freq))*1000);
}

void chrono_Difference_morpho3_3_erosion()
{
  chronoFD_morpho3_3_erosion();
  chronoFD_morpho3_3_erosion_SSE();
  chrono_morpho3_3_erosion_SSE_reducColumn();
}

void chronoFD_morpho5_5_erosion()
{
    double t0,t1;
    int i;
    double freq ;
    LitFrequenceCpu(&freq);

   image_t ImageSSE1,ImageSSE2;
    readPGM("car3/car_3100.pgm",&ImageSSE1);
    readPGM("car3/car_3101.pgm",&ImageSSE2);
 

    t0= rdtsc();
    //printf("il faut %f cycles\n",t0);

    for(i=0;i<NRUN;i++)
    {
     morpho_Erosion5_5(&ImageSSE1, &ImageSSE2);
    }

      t1= rdtsc();
      freeImage_t(&ImageSSE1);
      freeImage_t(&ImageSSE2);
    //printf("il faut %f cycles\n",t1);
      
    printf(" total cycles : %f,\n   cycles par iteration : %f ,\n  cycles par image : %f ,\n  cycles par point : %f \n\n",(t1-t0),(t1-t0)/NRUN, (t1-t0)/ (NRUN), (t1-t0)/(NRUN*320*240));
    printf(" total temps(ms) : %f ,\n  temps par iteration : %f ,\n  temps par image : %f ,\n  temps par pixel : %f \n\n",((t1-t0)/freq)*1000,((t1-t0)/(NRUN*freq))*1000, ((t1-t0)/ (NRUN*freq))*1000, ((t1-t0)/(NRUN*320*240*freq))*1000);
}


void chronoFD_morpho5_5_erosion_SSE()
{
    double t0,t1;
    int i;
    double freq ;
    LitFrequenceCpu(&freq);

   image_SSE ImageSSE1,ImageSSE2;
    readPGM_SSE("car3/car_3100.pgm",&ImageSSE1);
    readPGM_SSE("car3/car_3101.pgm",&ImageSSE2);
  

    t0= rdtsc();
    //printf("il faut %f cycles\n",t0);

    for(i=0;i<NRUN;i++)
    {
      morpho_SSE_Erosion5_5(&ImageSSE1, &ImageSSE2);
    }

      t1= rdtsc();
      freeImageSSE(&ImageSSE1);
      freeImageSSE(&ImageSSE2);
    //printf("il faut %f cycles\n",t1);
      
    printf(" total cycles : %f,\n   cycles par iteration : %f ,\n  cycles par image : %f ,\n  cycles par point : %f \n\n",(t1-t0),(t1-t0)/NRUN, (t1-t0)/ (NRUN), (t1-t0)/(NRUN*320*240));
    printf(" total temps(ms) : %f ,\n  temps par iteration : %f ,\n  temps par image : %f ,\n  temps par pixel : %f \n\n",((t1-t0)/freq)*1000,((t1-t0)/(NRUN*freq))*1000, ((t1-t0)/ (NRUN*freq))*1000, ((t1-t0)/(NRUN*320*240* freq))*1000);
}

void chronoFD_morpho5_5_erosion_SSE_reducColumn()
{
    double t0,t1;
    int i;
    double freq ;
    LitFrequenceCpu(&freq);

   image_SSE ImageSSE1,ImageSSE2;
    readPGM_SSE("car3/car_3100.pgm",&ImageSSE1);
    readPGM_SSE("car3/car_3101.pgm",&ImageSSE2);
  

    t0= rdtsc();
    //printf("il faut %f cycles\n",t0);

    for(i=0;i<NRUN;i++)
    {
      morpho_SSE_Erosion5_5_reducColumn(&ImageSSE1, &ImageSSE2);
    }

      t1= rdtsc();
      freeImageSSE(&ImageSSE1);
      freeImageSSE(&ImageSSE2);
    //printf("il faut %f cycles\n",t1);
      
    printf(" total cycles : %f,\n   cycles par iteration : %f ,\n  cycles par image : %f ,\n  cycles par point : %f \n\n",(t1-t0),(t1-t0)/NRUN, (t1-t0)/ (NRUN), (t1-t0)/(NRUN*320*240));
    printf(" total temps(ms) : %f ,\n  temps par iteration : %f ,\n  temps par image : %f ,\n  temps par pixel : %f \n\n",((t1-t0)/freq)*1000,((t1-t0)/(NRUN*freq))*1000, ((t1-t0)/ (NRUN*freq))*1000, ((t1-t0)/(NRUN*320*240* freq))*1000);
}


void chrono_Difference_morpho5_5_erosion()
{
  chronoFD_morpho5_5_erosion();
  chronoFD_morpho5_5_erosion_SSE();
  chronoFD_morpho5_5_erosion_SSE_reducColumn();
}


void chronoFD_morpho5_5_dilatation()
{
    double t0,t1;
    int i;
    double freq ;
    LitFrequenceCpu(&freq);

   image_t ImageSSE1,ImageSSE2;
    readPGM("car3/car_3100.pgm",&ImageSSE1);
    readPGM("car3/car_3101.pgm",&ImageSSE2);
 

    t0= rdtsc();
    //printf("il faut %f cycles\n",t0);

    for(i=0;i<NRUN;i++)
    {
     morpho_Dilatation5_5(&ImageSSE1, &ImageSSE2);
    }

      t1= rdtsc();
      freeImage_t(&ImageSSE1);
      freeImage_t(&ImageSSE2);
    //printf("il faut %f cycles\n",t1);
      
    printf(" total cycles : %f,\n   cycles par iteration : %f ,\n  cycles par image : %f ,\n  cycles par point : %f \n\n",(t1-t0),(t1-t0)/NRUN, (t1-t0)/ (NRUN), (t1-t0)/(NRUN*320*240));
    printf(" total temps(ms) : %f ,\n  temps par iteration : %f ,\n  temps par image : %f ,\n  temps par pixel : %f \n\n",((t1-t0)/freq)*1000,((t1-t0)/(NRUN*freq))*1000, ((t1-t0)/ (NRUN*freq))*1000, ((t1-t0)/(NRUN*320*240*freq))*1000);
}


void chronoFD_morpho5_5_dilatation_SSE()
{
    double t0,t1;
    int i;
    double freq ;
    LitFrequenceCpu(&freq);

   image_SSE ImageSSE1,ImageSSE2;
    readPGM_SSE("car3/car_3100.pgm",&ImageSSE1);
    readPGM_SSE("car3/car_3101.pgm",&ImageSSE2);
  

    t0= rdtsc();
    //printf("il faut %f cycles\n",t0);

    for(i=0;i<NRUN;i++)
    {
      morpho_SSE_Dilatation5_5(&ImageSSE1, &ImageSSE2);
    }

      t1= rdtsc();
      freeImageSSE(&ImageSSE1);
      freeImageSSE(&ImageSSE2);
    //printf("il faut %f cycles\n",t1);
      
    printf(" total cycles : %f,\n   cycles par iteration : %f ,\n  cycles par image : %f ,\n  cycles par point : %f \n\n",(t1-t0),(t1-t0)/NRUN, (t1-t0)/ (NRUN), (t1-t0)/(NRUN*320*240));
    printf(" total temps(ms) : %f ,\n  temps par iteration : %f ,\n  temps par image : %f ,\n  temps par pixel : %f \n\n",((t1-t0)/freq)*1000,((t1-t0)/(NRUN*freq))*1000, ((t1-t0)/ (NRUN*freq))*1000, ((t1-t0)/(NRUN*320*240* freq))*1000);
}


void chronoFD_morpho5_5_dilatation_SSE_reducColumn()
{
    double t0,t1;
    int i;
    double freq ;
    LitFrequenceCpu(&freq);

   image_SSE ImageSSE1,ImageSSE2;
    readPGM_SSE("car3/car_3100.pgm",&ImageSSE1);
    readPGM_SSE("car3/car_3101.pgm",&ImageSSE2);
  

    t0= rdtsc();
    //printf("il faut %f cycles\n",t0);

    for(i=0;i<NRUN;i++)
    {
      morpho_SSE_Dilatation5_5_reducColumn(&ImageSSE1, &ImageSSE2);
    }

      t1= rdtsc();
      freeImageSSE(&ImageSSE1);
      freeImageSSE(&ImageSSE2);
    //printf("il faut %f cycles\n",t1);
      
    printf(" total cycles : %f,\n   cycles par iteration : %f ,\n  cycles par image : %f ,\n  cycles par point : %f \n\n",(t1-t0),(t1-t0)/NRUN, (t1-t0)/ (NRUN), (t1-t0)/(NRUN*320*240));
    printf(" total temps(ms) : %f ,\n  temps par iteration : %f ,\n  temps par image : %f ,\n  temps par pixel : %f \n\n",((t1-t0)/freq)*1000,((t1-t0)/(NRUN*freq))*1000, ((t1-t0)/ (NRUN*freq))*1000, ((t1-t0)/(NRUN*320*240* freq))*1000);
}


void chrono_Difference_morpho5_5_dilatation()
{
  chronoFD_morpho5_5_dilatation();
  chronoFD_morpho5_5_dilatation_SSE();
  chronoFD_morpho5_5_dilatation_SSE_reducColumn();
}


void chronoSD()
{
    double t0,t1;
    int i;
    double freq ;
    LitFrequenceCpu(&freq);

   image_t Mt,Vt, Ot,ImgRead;
    readPGM("car3/car_3100.pgm",&ImgRead);
    readPGM("car3/car_3101.pgm",&Mt);
    readPGM("car3/car_3101.pgm",&Vt);
    initImage_t(&Ot);

    t0= rdtsc();
    //printf("il faut %f cycles\n",t0);

    for(i=0;i<NRUN;i++)
    {
     SD_1_step(&ImgRead, &Ot, &Vt, &Mt);
    }

      t1= rdtsc();

       freeImage_t(&ImgRead);
      freeImage_t(&Mt);
      freeImage_t(&Vt);
      freeImage_t(&Ot);
    //printf("il faut %f cycles\n",t1);
      
    printf(" total cycles : %f,\n   cycles par iteration : %f ,\n  cycles par image : %f ,\n  cycles par point : %f \n\n",(t1-t0),(t1-t0)/NRUN, (t1-t0)/ (NRUN), (t1-t0)/(NRUN*320*240));
    printf(" total temps(ms) : %f ,\n  temps par iteration : %f ,\n  temps par image : %f ,\n  temps par pixel : %f \n\n",((t1-t0)/freq)*1000,((t1-t0)/(NRUN*freq))*1000, ((t1-t0)/ (NRUN*freq))*1000, ((t1-t0)/(NRUN*320*240* freq))*1000);
}


void chronoSD_SSE()
{
    double t0,t1;
    int i;
    double freq ;
    LitFrequenceCpu(&freq);

   image_SSE Mt,Vt, Ot,ImgRead;
    readPGM_SSE("car3/car_3100.pgm",&ImgRead);
    readPGM_SSE("car3/car_3101.pgm",&Mt);
    readPGM_SSE("car3/car_3101.pgm",&Vt);
    initImageSSE(&Ot);

    t0= rdtsc();
    //printf("il faut %f cycles\n",t0);

    for(i=0;i<NRUN;i++)
    {
       SD_1_Step_SSE(&ImgRead, &Ot, &Vt, &Mt);
    }

      t1= rdtsc();

      freeImageSSE(&ImgRead);
      freeImageSSE(&Mt);
      freeImageSSE(&Vt);
      freeImageSSE(&Ot);
    //printf("il faut %f cycles\n",t1);
      
    printf(" total cycles : %f,\n   cycles par iteration : %f ,\n  cycles par image : %f ,\n  cycles par point : %f \n\n",(t1-t0),(t1-t0)/NRUN, (t1-t0)/ (NRUN), (t1-t0)/(NRUN*320*240));
    printf(" total temps(ms) : %f ,\n  temps par iteration : %f ,\n  temps par image : %f ,\n  temps par pixel : %f \n\n",((t1-t0)/freq)*1000,((t1-t0)/(NRUN*freq))*1000, ((t1-t0)/ (NRUN*freq))*1000, ((t1-t0)/(NRUN*320*240*freq))*1000);
}

void chrono_Difference_SD()
{
  chronoSD();
  chronoSD_SSE();
}



void main_Bench_FD()
{
 printf("Démarage FD sans Morpho\n");
 chrono(FD_Full_Step_NO_Morpho);
 printf("Fin FD sans Morpho\n");

 printf("Démarage FD Morpho 3_3\n");
 chrono(FD_Full_Step_Morpho3_3);
 printf("Fin FD Morpho 3_3\n");

 printf("Démarage FD Morpho 5_5\n");
 chrono(FD_Full_Step_Morpho5_5);
 printf("Fin FD Morpho 5_5\n");

}

void main_Bench_SD()
{
 printf("Démarage SD sans Morpho\n");
 chrono(SD_Full_Step_NO_Morpho);

 printf("Fin SD sans Morpho\n");


 printf("Démarage SD Morpho 3_3\n");
 chrono(SD_Full_Step_Morpho3_3);

 printf("Fin SD Morpho 3_3\n");


 printf("Démarage SD Morpho 5_5\n");
 chrono(SD_Full_Step_Morpho5_5);

 printf("Fin SD Morpho 5_5\n");
}


void roc(image_t* Image, image_t* IVT, int Mroc[2][2])
{
  int i,j;

  for(i=0;i<Image->h;i++)
  {
    for(j=0;j<Image->w;j++)
    {
      if(IVT->data[i][j] == 0)
      {
        if(Image->data[i][j] == 0)
          Mroc[1][1] +=1;
        else
          Mroc[1][0] +=1;
      }else
      {
        if(Image->data[i][j] == 0)
          Mroc[0][1] +=1;
        else
          Mroc[0][0] +=1;
      }
    }
  }
}

void displayROC(int Mroc[2][2])
{
  printf(" VP = %d\t FN = %d\n FP = %d\t VN =%d\n",Mroc[0][0],Mroc[0][1],Mroc[1][0],Mroc[1][1]);
}