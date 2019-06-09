#include <stdio.h>
#include <mpi.h>

float calcula(float a_local, float b_local, int n_local, float h);
float f(float x); // função a integrar


int main(int argc, char **argv) {
    int my_rank, np; // rank e número de processos
    float a = 0.0, b = 1.0; // intervalo
    int n = 1074; // número de trapezoides
    float h; // base do trapezoide
    float a_local, b_local; // intervalo local
    int n_local; // número de trapezoides no local
    float integral; // integral nesse intervalo
    float integral_resto;
    float total; // integral total
    int source; // remetente
    int dest = 0; // destinatário das integrais
    int tag = 200;
    MPI_Status status;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &np);

    h = (b - a) / n;
	n_local = n / np;
    int resto = n % np;
    a_local = a + my_rank * n_local * h;
    b_local = a_local + n_local * h;
    
    integral = calcula(a_local, b_local, n_local, h);
    
    if (my_rank < resto) {
    	a_local = (n - resto) * h + my_rank * h;
    	b_local = a_local + h;
    	integral_resto = calcula(a_local, b_local, 1, h);
    	integral += integral_resto;
    }

    if (my_rank == 0) {
        total = integral;
        
        for (source = 1; source < np; source++) {
            MPI_Recv(&integral, 1, MPI_FLOAT, source, tag, MPI_COMM_WORLD, &status); // TODO: trocar por MPI_Reduce
            total += integral;
        }
    }

    else {
        MPI_Send(&integral, 1, MPI_FLOAT, dest, tag, MPI_COMM_WORLD);
    }


    if (my_rank == 0) printf("Resultado: %f\n", total);
   
   
    MPI_Finalize();
}

float calcula(float a_local, float b_local, int n_local, float h) {
    float integral;
    float x;
    int i;
    int n_threads = 4;
    int n_local_thread = n_local / n_threads;
    int resto = n_local % n_threads;

    integral = (f(a_local) + f(b_local)) / 2.0;


    #pragma omp parallel num_threads(n_threads) reduction(+:integral)
    {
    	int id_thread = omp_get_thread_num();
	    x = a_local + h * id_thread;

        for (i = 0; i <= n_local_thread; i++) { // a parte de cada thread
            x += h;
            integral += f(x);
        }
	    
	    if (id_thread < resto) { // resto
            float a_local_resto = (n_local - resto + id_thread) * h;
            float b_local_resto = a_local_resto + h;
            integral = (f(a_local_resto) + f(b_local_resto)) / 2.0;

            x = a_local_resto;
    		x += h;
    		integral += f(x);
	    }

        integral *= h;

    }


    return integral;
}

float f(float x) {
    float fx; // retorno

    fx = x * x; // exemplo: função quadrática

    return fx;
}