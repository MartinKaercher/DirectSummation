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
    double result_serial = 0;
    for (i=0;i<4;i++)
    {
        mult_result = test_vec_1[i]*test_vec_2[i];
        printf("%f \n", mult_result);
        result_serial += mult_result;

    }
    printf("Result from serial mult and add %f \n", result_serial);

    // Now initialize the same vector but as a __m256 vector
    __m256d AVX_test_vec_1 = _mm256_set_pd(1.234, 2.345, 3.456, 4.567);
    __m256d AVX_test_vec_2 = _mm256_set_pd(1.234, 2.345, 3.456, 4.567);
    //
    // __m256 AVX_mult_result;
    __m256d AVX_mult_result = _mm256_mul_pd(AVX_test_vec_1, AVX_test_vec_2);

    // Just an 0ed proxy vector
    __m256d proxy_vec = _mm256_set_pd(0., 0., 0., 0.);

    //Sum the two vectors to (x1[0]+x1[1], x2[0]+x2[1], x1[2]+x1[3], x2[2]+x2[3])
    __m256d AVX_sum = _mm256_hadd_pd(AVX_mult_result, proxy_vec);
    // Extract upper 128 bits
    __m128d AVX_sum_high = _mm256_extractf128_pd(AVX_sum, 1);

    __m128d AVX_result = _mm_add_pd(AVX_sum_high, _mm256_castpd256_pd128(AVX_sum));




    double result[4];
    double* result_pd =  &result[0];
    _mm256_store_pd(result_pd, AVX_mult_result);

    double result_sum[2];
    double* AVX_sum_result = & result_sum[0];
    _mm_store_pd(AVX_sum_result, AVX_result);

    for (i=0;i<4;i++)
    {
        printf("%f \n", result_pd[i]);
    }

    printf("Result from AVX/SSE mult and add: %f \n", AVX_sum_result[0]);
    return 0;
}
