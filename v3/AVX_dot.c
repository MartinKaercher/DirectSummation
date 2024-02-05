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

double AVX_dot(double *v1, double *v2){
    __m256i mask = _mm256_set_epi64x(10, -10, -10, -10);
    __m256d AVX_vec_1 = _mm256_maskload_pd(v1, mask);
    __m256d AVX_vec_2 = _mm256_maskload_pd(v2, mask);

    // __m256 AVX_mult_result;
    __m256d AVX_mult_result = _mm256_mul_pd(AVX_vec_1, AVX_vec_2);

    // Just an 0ed proxy vector
    __m256d proxy_vec = _mm256_set_pd(0., 0., 0., 0.);

    // Switching two 128 bit packages
    __m256d AVX_mult_result_reversed = _mm256_permute2f128_pd(AVX_mult_result, AVX_mult_result, 0b00000001);

    //Sum the two vectors to (x1[0]+x1[1], x2[0]+x2[1], x1[2]+x1[3], x2[2]+x2[3])
    __m256d AVX_sum = _mm256_hadd_pd(AVX_mult_result, AVX_mult_result_reversed);

    //Add with proxy vector to get the final sum
    __m256d AVX_result = _mm256_hadd_pd(AVX_sum, proxy_vec);

    double result_sum[4];
    double* AVX_sum_result = &result_sum[0];
    _mm256_store_pd(AVX_sum_result, AVX_result);

    return result_sum[0];


}

int main(int argc, char *argv[]){

    // So far so classic
    double test_vec_1[] = {1.234, 2.345, 3.456};
    double test_vec_2[] = {3.434, 3.4324, 0.234};

    int i=0;
    double mult_result;
    double result_serial = 0;
    for (i=0;i<3;i++)
    {
        mult_result = test_vec_1[i]*test_vec_2[i];
        printf("%f \n", mult_result);
        result_serial += mult_result;

    }
    printf("Result from serial mult and add %f \n", result_serial);

    // Now initialize the same vector but as a __m256 vector
    double* vec_1_ptr = &test_vec_1[0];
    double* vec_2_ptr = &test_vec_2[0];
    __m256i mask = _mm256_set_epi64x(10, -10, -10, -10);
    __m256d AVX_vec_1 = _mm256_maskload_pd(vec_1_ptr, mask);
    __m256d AVX_vec_2 = _mm256_maskload_pd(vec_2_ptr, mask);

    //__m256d AVX_test_vec_1 = _mm256_set_pd(1.234, 2.345, 3.456, 4.567);
    //__m256d AVX_test_vec_2 = _mm256_set_pd(1.234, 2.345, 3.456, 4.567);
    //
    // __m256 AVX_mult_result;
    __m256d AVX_mult_result = _mm256_mul_pd(AVX_vec_1, AVX_vec_2);

    // Just an 0ed proxy vector
    __m256d proxy_vec = _mm256_set_pd(0., 0., 0., 0.);

    // Switching two 128 bit packages
    __m256d AVX_mult_result_reversed = _mm256_permute2f128_pd(AVX_mult_result, AVX_mult_result, 0b00000001);

    //Sum the two vectors to (x1[0]+x1[1], x2[0]+x2[1], x1[2]+x1[3], x2[2]+x2[3])
    __m256d AVX_sum = _mm256_hadd_pd(AVX_mult_result, AVX_mult_result_reversed);

    //Add with proxy vector to get the final sum
    __m256d AVX_result = _mm256_hadd_pd(AVX_sum, proxy_vec);




    double result[4];
    double* result_pd =  &result[0];
    _mm256_store_pd(result_pd, AVX_mult_result);

    double result_sum[4];
    double* AVX_sum_result = & result_sum[0];
    _mm256_store_pd(AVX_sum_result, AVX_result);

    for (i=0;i<4;i++)
    {
        printf("%f \n", result_pd[i]);
    }
    printf("Result from AVX/SSE mult and add: %f \n", AVX_sum_result[0]);
    return 0;
}
