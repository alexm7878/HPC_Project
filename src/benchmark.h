#ifndef BENCHMARK_H
#define BENCHMARK_H
 #define NOMFICH_CPUINFO "/proc/cpuinfo"
#include "mouvement.h"

#define NRUN 10

	__inline__ uint64_t rdtsc(void);
	 
	int LitFrequenceCpu (double* frequence);

#endif