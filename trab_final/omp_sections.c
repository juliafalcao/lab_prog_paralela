#include <stdio.h>
#include <stdlib.h>

int main(int* argc, char* argv[]) {
	double sum, value;
	double a[256], b[256];
	int n = 256, i;
	double value1, sum1;

	for (i = 0; i < n; i++) {
		a[i] = i * 0.5;
		b[i] = i * 2.0;
	}

	sum = 0;
	sum1 = 0;
	value = 0;
	value1 = 0;

	#pragma omp parallel default(none) shared(sum,sum1,a,b,n) private(value,value1,i)
	{	
		#pragma omp sections
		{	
			#pragma omp section
			for (i = 1; i < n; i++) {
				value = a[i]*b[i];
				sum += value;
			}

			#pragma omp section
			for (i = 1; i < n; i++) {
				value1 = a[i]*b[i];
				sum1 *= value1;
			}
		}
	}

	printf("Sum = %f\n", sum);
	printf("Sum1 = %f\n", sum1);
}