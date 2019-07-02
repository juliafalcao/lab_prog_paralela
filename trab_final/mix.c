#include <stdio.h>
#include <mpi.h>

int main(int argc, char **argv) {

	double soma, soma_local, valor;
	int n = 256; // tamanho do vetor
	int np; // processos
	int my_rank; // id do processo
	int i;
	int tag = 0;
	MPI_Status status;

	double a[n], b[n];

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &np);

	int n_local = n / np;
	int resto = n % np;
	
	
	for (i = my_rank*n_local; i < (my_rank+1)*n_local; i++) { // inicialização do vetor
		a[i] = i * 0.5;
		b[i] = i * 2.0;
	}

	#pragma omp parallel for reduction(+:soma_local) shared(my_rank,n_local) private(i,valor)
	for (i = my_rank*n_local; i < (my_rank+1)*n_local; i++) { // cálculo da soma local
		valor = a[i]*b[i];
		soma_local += valor;
	}

	if (my_rank < resto) { // cálculo do resto
		int offset = n_local*np;
		int i = offset+my_rank;

		a[i] = i * 0.5;
		b[i] = i * 2.0;
		valor = a[i] * b[i];
		soma_local += valor;
	}

	if (my_rank == 0) { // master recebe as somas locais
		soma += soma_local;

		for (int i = 1; i < np; i++) {
			MPI_Recv(&soma_local, 1, MPI_DOUBLE, MPI_ANY_SOURCE, tag, MPI_COMM_WORLD, &status);
			soma += soma_local;
		}
	}

	else { // outros processos enviam suas somas locais
		MPI_Send(&soma_local, 1, MPI_DOUBLE, 0, tag, MPI_COMM_WORLD);
	}

	if (my_rank == 0) printf("Produto escalar: %f\n", soma);

	MPI_Finalize();
}

