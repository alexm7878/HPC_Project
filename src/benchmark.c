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
     printf(" total temps(ms) : %f ,\n  temps par iteration : %f ,\n  temps par image : %f ,\n  temps par point : %f \n\n",((t1-t0)/freq)*1000,((t1-t0)/(NRUN*freq))*1000, ((t1-t0)/ (NRUN*200*freq))*1000, ((t1-t0)/(NRUN*200*320*240* freq))*1000);
}