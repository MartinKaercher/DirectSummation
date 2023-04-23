#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>

double vec_mult_3D(double *v1, double *v2){
    double ret_vec;
    ret_vec = (v1[0] * v2[0] + v1[1] * v2[1] + v1[2] * v2[2]);
    return ret_vec;
}

int main(int argc, char *argv[]){

    // So far so classic
    double test_vec_1[] = {1.234, 2.345, 3.456};
    double test_vec_2[] = {3.434, 3.4324, 0.234};

    clock_t start, end;
    int i=0;
    int time_counter = 0;
    double result_serial;
    start = clock();
    for (time_counter=0; time_counter<1000000; time_counter++)
    {
        result_serial = vec_mult_3D(test_vec_1, test_vec_2);
    }
    end = clock();
    printf("Elapsed time is: %f \n", ((double)end-start)/CLOCKS_PER_SEC);
    printf("Result from serial mult and add %f \n", result_serial);

    return 0;
}
