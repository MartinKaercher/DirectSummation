#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include <immintrin.h>

double vec_mult_3D(double *v1, double *v2){
    // Now initialize the same vector but as a __m256 vector
    //__m256i mask = _mm256_set_epi64x(10, -10, -10, -10);
    //__m256d AVX_vec_1 = _mm256_maskload_pd(v1, mask);
    //__m256d AVX_vec_2 = _mm256_maskload_pd(v2, mask);


    __m256d AVX_vec_1 = _mm256_set_pd(0., v1[0], v1[1], v1[2]);
    __m256d AVX_vec_2 = _mm256_set_pd(0., v2[0], v2[1], v2[2]);

    // __m256 AVX_mult_result;
    __m256d AVX_result;

    __m256d AVX_mult_result = _mm256_mul_pd(AVX_vec_1, AVX_vec_2);

    // Just an 0ed proxy vector
    __m256d proxy_vec = _mm256_set_pd(0., 0., 0., 0.);

    // Switching two 128 bit packages
    __m256d AVX_mult_result_reversed = _mm256_permute2f128_pd(AVX_mult_result, AVX_mult_result, 0b00000001);

    //Sum the two vectors to (x1[0]+x1[1], x2[0]+x2[1], x1[2]+x1[3], x2[2]+x2[3])
    __m256d AVX_sum = _mm256_hadd_pd(AVX_mult_result, AVX_mult_result_reversed);

    //Add with proxy vector to get the final sum
    AVX_result = _mm256_hadd_pd(AVX_sum, proxy_vec);

    //double result_sum[4];
    //double* AVX_sum_result = &result_sum[0];
    //_mm256_store_pd(AVX_sum_result, AVX_result);

    //return result_sum[0];
    return ((double*) &AVX_result)[0];
}

// Stack overflow solution
double dotProduct_3(double* u, double* v)   
{  
    __m256d dp = _mm256_mul_pd(_mm256_load_pd(u), _mm256_load_pd(v));
    __m128d a = _mm256_extractf128_pd(dp, 0);
    __m128d b = _mm256_extractf128_pd(dp, 1);
    __m128d c = _mm_add_pd(a, b);
    __m128d yy = _mm_unpackhi_pd(c, c);
    __m128d dotproduct  = _mm_add_pd(c, yy);
    return _mm_cvtsd_f64(dotproduct);
}


int main(int argc, char *argv[]){

    // So far so classic
    double test_vec_1[] = {1.234, 2.345, 3.456};
    double test_vec_2[] = {3.434, 3.4324, 0.234};

    double result;
    clock_t start, end;
    int time_counter=0;
    start = clock();
    for (time_counter=0; time_counter<1000000; time_counter++)
    {
        result = vec_mult_3D(test_vec_1, test_vec_2);
    }
    end = clock();
    printf("Elapsed time is: %f \n", ((double)end-start)/CLOCKS_PER_SEC);


    printf("Result from AVX/SSE mult and add: %f \n", result);
    return 0;
}
