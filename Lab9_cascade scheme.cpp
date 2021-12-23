#include <iostream>
#include <mpi.h>
#define VECTOR_SIZE 1000000
using namespace std;

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);    
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Status status;

    int* vector, * part_vector;
    int part_vec_size, part_sum = 0;
    double time_start, time_finish;
    part_vec_size = VECTOR_SIZE / size;

    vector = new int[VECTOR_SIZE];
    part_vector = new int[part_vec_size];

    if (rank == 0) {
        for (int i = 0; i < VECTOR_SIZE; i++)
            vector[i] = 1;
    }
    MPI_Bcast(&part_vec_size, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Scatter(vector, part_vec_size, MPI_INT, part_vector, part_vec_size, MPI_INT, 0, MPI_COMM_WORLD);
    for (int i = 0; i < part_vec_size; i++)
        part_sum += part_vector[i];
    if (rank == 0)
        time_start = MPI_Wtime();
    for (int i = 2, recv_val; i <= size; i *= 2) {
        if (rank % i == 0) {
            if (rank + i / 2 < size) {
                MPI_Recv(&recv_val, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
                part_sum += recv_val;
            }
        }
        else {
            MPI_Send(&part_sum, 1, MPI_INT, rank - (rank % i), 0, MPI_COMM_WORLD);
            break;
        }
    }
    if (rank == 0) {
        time_finish = MPI_Wtime();
        cout << "Sum: " << part_sum << "\tProcesses: " << size << "\tExecution time: " << time_finish - time_start <<endl;
    }
    MPI_Finalize();
    return 0;
}