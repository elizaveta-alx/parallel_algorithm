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
    int part_vec_size;
    long int sum = 0,part_sum = 0;
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
    for (int i = 0; i < VECTOR_SIZE / size; i++)
        part_sum += part_vector[i];
    cout << "Sum on the process " << rank << ": " << part_sum << endl;
    if (rank == 0)
        time_start = MPI_Wtime();
    MPI_Reduce(&part_sum, &sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    if (rank == 0){
        time_finish = MPI_Wtime();
        cout << "Sum: " << sum << "\tProcesses: " << size << "\tExecution time: " << time_finish - time_start << endl;
    } 
    MPI_Finalize();
    return 0;
}