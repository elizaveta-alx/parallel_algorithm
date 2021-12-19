#include <iostream>
#include <cstdlib>
#include <omp.h>
#include <time.h>

using namespace std;

void zero_init_matrix(int** matrix, int n){
	srand(time(NULL));
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
			matrix[i][j] = 0;
}

int ijk_order(int** a, int** b, int** c, int size){
    int i, j, k;
    double t1, t;
    t1 = omp_get_wtime();
    for (i = 0; i < size; i++){
        for (j = 0; j < size; j++){
            for (k = 0; k < size; k++){
                c[i][j] += a[i][k] * b[k][j];
            }
        }
    }
    t1 = omp_get_wtime() - t1;

    for (unsigned int nthreads = 1; nthreads < 11; nthreads++){
        t = omp_get_wtime();
        #pragma omp parallel num_threads(nthreads) shared(size) private(i,j,k)
        {
        #pragma omp for schedule(static)
            for (i = 0; i < size; i++){
                for (j = 0; j < size; j++){
                    for (k = 0; k < size; k++){
                        c[i][j] += a[i][k] * b[k][j];
                    }
                }
            }
        }
        t = omp_get_wtime() - t;
    
        cout << "The number of threads: " << nthreads << "\tTime: " << t << "\tEfficiency: " << t1 / t << endl;
    }
    return 0;
}

int jki_order(int** a, int** b, int** c, int size){
    int i, j, k;
    double t1, t;

    t1 = omp_get_wtime();
    for (j = 0; j < size; j++){
        for (k = 0; k < size; k++){
            for (i = 0; i < size; i++){
                c[i][j] += a[i][k] * b[k][j];
            }
        }
    }

    t1 = omp_get_wtime() - t1;

    for (unsigned int nthreads = 1; nthreads <= 10; nthreads++){
        t = omp_get_wtime();
        #pragma omp parallel num_threads(nthreads) shared(size) private(i,j,k)
        {
        #pragma omp for schedule(static)
            for (j = 0; j < size; j++){
                for (k = 0; k < size; k++){
                    for (i = 0; i < size; i++){
                        c[i][j] += a[i][k] * b[k][j];
                    }
                }
            }
        }
        
        t = omp_get_wtime() - t;
    
        cout << "The number of threads: " << nthreads << "\tTime: " << t << "\tEfficiency: " << t1 / t << endl;
        
    }
    return 0;
}

int ikj_order(int** a, int** b, int** c, int size){
    int i, j, k;
    double t1, t;

    t1 = omp_get_wtime();
    for (i = 0; i < size; i++){
        for (k = 0; k < size; k++){
            for (j = 0; j < size; j++){
                c[i][j] += a[i][k] * b[k][j];
            }
        }
    }

    t1 = omp_get_wtime() - t1;

    for (unsigned int nthreads = 1; nthreads <= 10; nthreads++){
        t = omp_get_wtime();
        #pragma omp parallel num_threads(nthreads) shared(size) private(i,j,k)
        {
        #pragma omp for schedule(static)
            for (i = 0; i < size; i++){
                for (k = 0; k < size; k++){
                    for (j = 0; j < size; j++){
                        c[i][j] += a[i][k] * b[k][j];                        
                    }
                }
            }       
        }
        
        t = omp_get_wtime() - t;
    
        cout << "The number of threads: " << nthreads << "\tTime: " << t << "\tEfficiency: " << t1 / t << endl;
    }
    return 0;
}

int main(int argc, char* argv[]){
    int N = atoi(argv[1]);

    int **a = new int* [N];
    int **b = new int* [N];
    int **c = new int* [N];

    for (unsigned int i = 0; i < N; i++)
        a[i] = new int [N];

    for (unsigned int i = 0; i < N; i++)
        b[i] = new int [N];

    for (unsigned int i = 0; i < N; i++)
        c[i] = new int [N];

    for (unsigned int i = 0; i < N; i++){
        for (unsigned int j = 0; j < N; j++){
            a[i][j] = rand() % 100;
            b[i][j] = rand() % 100;
        }
    }
    /*for (unsigned int i = 0; i < N; i++){
        for (unsigned int j = 0; j < N; j++){
             cout << a[i][j] << " ";
         }
        cout << endl;
    }
    cout << endl;

    for (unsigned int i = 0; i < N; i++){
        for (unsigned int j = 0; j < N; j++){
            cout << b[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;*/

    cout << "IJK" << endl;
    zero_init_matrix(c, N);
    ijk_order(a, b, c, N);

    /*for (unsigned int i = 0; i < N; i++){
        for (unsigned int j = 0; j < N; j++){
            cout << c[i][j] << " ";
        }
        cout << endl;
    }*/

    cout << "JKI" << endl;
    zero_init_matrix(c, N);
    jki_order(a, b, c, N);
    /*for (unsigned int i = 0; i < N; i++){
        for (unsigned int j = 0; j < N; j++){
            cout << c[i][j] << " ";
        }
        cout << endl;
    }*/
    cout << "IKJ" << endl;
    zero_init_matrix(c, N);
    ikj_order(a, b, c, N);
    /*for (unsigned int i = 0; i < N; i++){
        for (unsigned int j = 0; j < N; j++){
            cout << c[i][j] << " ";
        }
        cout << endl;
    }*/
    for (unsigned int i = 0; i < N; i++)
        delete[]a[i];
    delete [] a;

    for (unsigned int i = 0; i < N; i++)
        delete[]b[i];
    delete [] b;

    for (unsigned int i = 0; i < N; i++)
        delete[]c[i];
    delete [] c;


    return 0;
}