#include "benchmark.h"


 __inline__ uint64_t rdtsc(void)
   {
uint32_t lo, hi;
__asm__ __volatile__ (
        "xorl %%eax,%%eax \n        cpuid"
        ::: "%rax", "%rbx", "%rcx", "%rdx");
__asm__ __volatile__ ("rdtsc" : "=a" (lo), "=d" (hi));
return (uint64_t)hi << 32 | lo;
 }


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