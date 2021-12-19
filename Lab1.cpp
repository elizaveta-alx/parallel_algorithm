#include <iostream>
#include <stdlib.h>
#include <ctime>
#include "omp.h"
using namespace std;

int main(int argc, char* argv[]) {
    int n = atoi(argv[1]);//array size
    int* a = new int[n]; //array
    for (int i = 0; i < n; i++) {
        a[i] = rand();
    }
    int max_val = a[0];
    for (int num_threads = 1; num_threads < 11; num_threads++) {
        clock_t start = clock();
        #pragma omp parallel for firstprivate(max_val) num_threads(num_threads)
        for (int idx = 0; idx < n; idx++)
            max_val = max_val > a[idx] ? max_val : a[idx];
        clock_t end = clock();

        cout << "Execution time of thread number " << num_threads << ": " << (end - start) << endl;
        cout << "Array: ";
        for (int k = 0; k < sizeof(a); k++)
            cout << a[k] << " ";
        delete[] a;
        cout << "Max value of array " << "is " << max_val;
        
        
        return 0;
    }
}