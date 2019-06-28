#include <stdio.h>
#include <stdlib.h>

int main(int* argc, char* argv[]) {
	double sum;
	double a[256], b[256], resp[256];
	int n = 256, i;

	for (i = 0; i < n; i++) {
		a[i] = i * 0.5;
		b[i] = i * 2.0;
		resp[i] = 0;
	}

	sum = 0;
	double value = 0;

	#pragma omp parallel shared(a,b,sum) private(value,i)
	{
		double temp = 0.0;

		#pragma omp for
		for (i = 1; i < n; i++) {
			resp[i] += a[i]*b[i];
		}		
	}

	for(i = 1; i < n; i++){
		sum += resp[i];
	}

	printf("Sum = %f", sum);
}