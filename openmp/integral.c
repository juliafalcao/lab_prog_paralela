#include <stdio.h>
 
static long num_steps = 100000;
double step;
	
void main() {
	double sum = 0;
	double pi;
  	#pragma omp parallel reduction(+:sum)
	{
		int i;
		double x;
		step = 1.0 / (double) num_steps;
		int id = omp_get_thread_num();
		int n_threads = omp_get_num_threads();
		int part = num_steps / n_threads;
		int resto = num_steps % n_threads;

		for (i = id * part; i < (id+1)*part; i++) {
			x = (i+0.5) * step;
			sum = sum + 4.0/(1.0 + x*x);
		}

		if (id == 0) {
			for (i = num_steps-resto; i <= num_steps; i++) {
				x = (i+0.5) * step;
				sum = sum + 4.0/(1.0 + x*x);
			}
		}
	}

	pi = step * sum;

	printf("Pi: %.5f\n", pi);
}