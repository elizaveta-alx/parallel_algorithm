#include <iostream>
#include <mpi.h>
#define MAX_LEN 1000001
using namespace std;
int main(int argc, char** argv)
{
    int rank, size;
    double T, L,R;
    int* a, * b, * c, * d;
    double start_time, finish_time;
    MPI_Status status;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
        
    for (int k = 1; k < MAX_LEN; k *= 10){
        L = k * sizeof(int);
        a = new int[k];
        b = new int[k];
        if (rank == 0)
            T = 0.0;
        for (int j = 0; j < 10; j++){
            for (int i = 0; i < k; i++)
                a[i] = 1;
            if (rank == 0){
                start_time = MPI_Wtime();
                MPI_Send(&a[0], k, MPI_INT, 1, 0, MPI_COMM_WORLD);
                MPI_Recv(&a[0], k, MPI_INT, 1, 0, MPI_COMM_WORLD, &status);
                finish_time = MPI_Wtime();
                T += finish_time - start_time;
            }
            if (rank == 1){
                MPI_Recv(&b[0], k, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
                MPI_Send(&b[0], k, MPI_INT, 0, 0, MPI_COMM_WORLD);
            }
        }
        if (rank == 0){
            R = (2 * 10 * (L / (1024.0 * 1024.0))) / (T / 10);
            cout << "Message lenght: " << k << ", Latency: " << (L / 1024) << ", Time = " << (T / 10) << " Bandwidth: " << R << " MB/s" << endl;
        }
    }
    c = new int[0];
    d = new int[0];
    if (rank == 0)
        T = 0.0;
    for (int j = 0; j < 10; j++){
        if (rank == 0){
            start_time = MPI_Wtime();
            MPI_Send(&c[0], 0, MPI_INT, 1, 0, MPI_COMM_WORLD);
            MPI_Recv(&c[0], 0, MPI_INT, 1, 0, MPI_COMM_WORLD, &status);
            finish_time = MPI_Wtime();
            T += finish_time - start_time;
        }
        if (rank == 1){
            MPI_Recv(&d[0], 0, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
            MPI_Send(&d[0], 0, MPI_INT, 0, 0, MPI_COMM_WORLD);
        }
    }
    if (rank == 0){
        cout << "Time: " << (T / 10) << endl;
        cout << "Latency  s: " << (T / 10) / (2 * 10) << endl;
    }
    MPI_Finalize();
    return 0;
}