#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include <immintrin.h>

double vec_mult_3D(double *v1, double *v2){
    double ret_vec;
    ret_vec = (v1[0] * v2[0] + v1[1] * v2[1] + v1[2] * v2[2]);
    return ret_vec;
}

int main(int argc, char *argv[]){

    // So far so classic
    double test_vec_1[] = {1.234, 2.345, 3.456, 4.567};
    double test_vec_2[] = {1.234, 2.345, 3.456, 4.567};

    int i=0;
    double mult_result;
    for (i=0;i<4;i++)
    {
        mult_result = test_vec_1[i]*test_vec_2[i];
        printf("%f \n", mult_result);
    }

    // Now initialize the same vector but as a __m256 vector
    __m256d AVX_test_vec_1 = _mm256_set_pd(1.234, 2.345, 3.456, 4.567);
    __m256d AVX_test_vec_2 = _mm256_set_pd(1.234, 2.345, 3.456, 4.567);
    //
    // __m256 AVX_mult_result;
    __m256d AVX_mult_result = _mm256_mul_pd(AVX_test_vec_1, AVX_test_vec_2);
    double result[4];
    double* result_pd =  result;
    _mm256_store_pd(result_pd, AVX_mult_result);

    for (i=0;i<4;i++)
    {
        printf("%f \n", result_pd[i]);
    }
    return 0;
}
