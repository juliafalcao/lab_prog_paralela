#include <stdio.h>
 
void main() {
  #pragma omp parallel num_threads(5)
  {
  	int id = omp_get_thread_num();
  	int n_threads = omp_get_num_threads();
    printf("hello from thread %d of %d!\n", id+1, n_threads);
  }
}