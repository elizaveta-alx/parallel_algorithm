#include <iostream>
#include <cstdlib>
#include <omp.h>
#include <time.h>
using namespace std;


int main(int argc, char* argv[]) {
    int n = atoi(argv[1]);//array size
    int* a = new int[n];
    double start, end;
    for (int i = 0; i < n; i++)
        a[i] = rand()%1000;   
    //print arr a
    /*cout << "Array: ";
    for (int i = 0; i < n; i++)
        cout << a[i] << " ";
    cout << '\n';*/
    int max_val = a[0];
    
    for (int num_threads = 1; num_threads < 11; num_threads++) {
        start = omp_get_wtime();
        #pragma omp parallel for reduction(max:max_val) num_threads(num_threads)
            for (long unsigned int idx = 0; idx < n; idx++) 
                max_val = max_val> a[idx] ? max_val : a[idx];   
        end = omp_get_wtime();
        cout << "Execution time of thread number " << num_threads << ": " << (end - start) << endl;   
    }

    cout << "Max value of array is " << max_val << endl;
    delete [] a;
    return 0;
}