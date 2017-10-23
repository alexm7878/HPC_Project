#ifndef BENCHMARK_H
#define BENCHMARK_H
 #define NOMFICH_CPUINFO "/proc/cpuinfo"
#include "mouvement.h"

#define NRUN 10

	__inline__ uint64_t rdtsc(void);
	 
	int LitFrequenceCpu (double* frequence);

	void main_Bench_FD();
	void main_Bench_SD();

	void roc(image_t* Image, image_t* IVT,int Mroc[2][2]);
	void displayROC(int Mroc[2][2]);

#endif