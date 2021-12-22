#include <iostream>
#include <mpi.h>
#define VECTOR_SIZE 1000000 
using namespace std;

int main(int argc, char** argv)
{
    //declaring variables
    int rank, size;
    double time_start, time_finish;
    MPI_Status status;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int* A = new int[VECTOR_SIZE];
    int* B = new int[VECTOR_SIZE];
    int* sum = new int[size];
    int* A1 = new int[VECTOR_SIZE];
    int* B1 = new int[VECTOR_SIZE];
    int n_elements = VECTOR_SIZE / size;
    int total_sum = 0, sum_part = 0;
    time_start = MPI_Wtime();
    if (rank == 0)
        for (int i = 0; i < VECTOR_SIZE; i++) {
            A[i] = 1;
            B[i] = 1;
        }
    MPI_Scatter(&A[0], n_elements, MPI_INT, &A1[0], n_elements, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Scatter(&B[0], n_elements, MPI_INT, &B1[0], n_elements, MPI_INT, 0, MPI_COMM_WORLD);

    for (int i = 0; i < n_elements; i++)
        sum_part += A1[i] * B1[i];

    MPI_Gather(&sum_part, 1, MPI_INT, &sum[0], 1, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        for (int i = 0; i < size; i++)
            total_sum += sum[i];
        cout << "Total sum is " << total_sum << endl;
        time_finish = MPI_Wtime();
        cout << "Execution time is  " << time_finish - time_start;
    }

    MPI_Finalize();
    return 0;
}