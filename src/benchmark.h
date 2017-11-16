#ifndef BENCHMARK_H
#define BENCHMARK_H
 #define NOMFICH_CPUINFO "/proc/cpuinfo"
#include "mouvement.h"
#include  "mouvement_SSE2.h"

#define NRUN 10

	__inline__ uint64_t rdtsc(void);
	 
	int LitFrequenceCpu (double* frequence);

	void main_Bench_FD();
	void main_Bench_SD();

	void roc(image_t* Image, image_t* IVT,int Mroc[2][2]);
	void displayROC(int Mroc[2][2]);

	void chrono(void (*ptrfonction)(void));

	void chronoFD();
	void chronoFD_SSE();
	void chrono_Difference_FD();

	void chronoFD_morpho3_3_dilatation();
	void chronoFD_morpho3_3_dilatation_SSE();
	void chrono_Difference_morpho3_3_dilatation();

	void chronoFD_morpho3_3_erosion();
	void chronoFD_morpho3_3_erosion_SSE();
	void chrono_Difference_morpho3_3_erosion();

	void chronoFD_morpho5_5_erosion();
	void chronoFD_morpho5_5_erosion_SSE();
	void chrono_Difference_morpho5_5_erosion();

	void chronoFD_morpho5_5_dilatation();
	void chronoFD_morpho5_5_dilatation_SSE();
	void chrono_Difference_morpho5_5_dilatation();

	void chronoSD();
	void chronoSD_SSE();
	void chrono_Difference_SD();

	void chrono_morpho3_3_erosion_SSE_reducColumn();

#endif