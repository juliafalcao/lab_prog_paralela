#include <stdio.h>

float f(float x) {
    return x*x; // exemplo: função quadrática
}
	
void main() {
	float a = 0.0;
	float b = 1.0;
	int n = 1024;
	double h;
    float integral;
    h = (b - a) / n;

    #pragma omp parallel for reduction(+:integral)
		for (int i = 1; i < n; i++)
			integral += f(a + i*h);

		integral = (integral + (f(a) + f(b))) * h;

	printf("Integral: %f\n", integral);
}

