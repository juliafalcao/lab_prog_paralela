#include <stdio.h>

int f(int x);

void main() {
	int n = 50;
	int a[n];

	for (int i = 0; i < n; i++) {
		a[i] = i;
	}

	int value;
	int sum = 0;
	int i;

	#pragma omp parallel shared(a,sum) private(value,i)
	{
		double temp = 0.0;
		
		#pragma omp for
		for (i = 0; i < n; i++) {
			value = f(a[i]);
			temp += value;
		}

		#pragma omp critical
		sum += temp;
	}

	printf("Sum: %d\n", sum);
}

int f(int x) {
	return x*x;
}