#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>

#include <immintrin.h>
#include "AVX_dot.h"

int main(int argc, char *argv[]){
    printf("# Direct Summation with for loops in C  #\n");

    char *filename = argv[1];

    FILE *fp;
    fp = fopen(filename, "r");
    // Check if file is there
    if (fp == NULL){
        printf("Could not open file %s \n", filename);
        return 1;
    }

    
    int NumPoints = 0;
    double var1, var2, var3;
    while(fscanf(fp, "%lf %lf %lf", &var1, &var2, &var3)>0){
        NumPoints++;
    }
    printf("The point set contain %.i points \n", NumPoints);


    double *point_set = malloc((NumPoints*3)*sizeof(double));
    if (point_set == NULL){
        printf("Malloc did not succeed \n");
        return 1;
    }

    fp = fopen(filename, "r");
    // Reset counter
    int counter = 0;
    while(fscanf(fp, "%lf %lf %lf", &var1, &var2, &var3)>0){
        point_set[counter*3] = var1;
        point_set[counter*3+1] = var2;
        point_set[counter*3+2] = var3;
        counter++;
    }

    printf("Sanity check \n");
    printf("The first point in the set has coordinates: %.3f %.3f %.3f \n", point_set[0], point_set[1], point_set[2]);


    // Set up the Fourier grid, currently only even Ncells works
    int Ncells = 32;
    double L = 1024.;
    double dt = L/Ncells;
    double n_bar = NumPoints/(L*L*L);

    int j;
    double *grid_x = malloc(Ncells*sizeof(double));
    printf("Nyquist frequency is: %.5f \n", M_PI/dt);
    printf("Fundamental frequency is: %.5f \n", 2.*M_PI/L);
    for (j=0; j<=Ncells-1; j++){
        if (j<Ncells/2){ 
        grid_x[j] = 2.*M_PI*j/L; 
        }
        else{
        grid_x[j] = -2.*M_PI*(Ncells-j)/L; 
        }
    }
    
    // Compute power spectrum on this grid
    //double delta_k_real[Ncells][Ncells][Ncells];
    //double delta_k_imag[Ncells][Ncells][Ncells];
    double *delta_k_real = malloc((Ncells*Ncells*Ncells)*sizeof(double));
    double *delta_k_imag = malloc((Ncells*Ncells*Ncells)*sizeof(double));
    int l;
    int m; 
    counter = 0;
    double exponent, norm;
    norm = 1./NumPoints;
    clock_t start, end;
    start = clock();

    //__m256i mask = _mm256_set_epi64x(10, -10, -10, -10);
    __m256d proxy_vec = _mm256_set_pd(0., 0., 0., 0.);
    // First do the frequencies which have a counterpart
    for (j=0; j<=Ncells/2; j++){
        for (l=0; l<=Ncells/2; l++){
            for (m=0; m<=Ncells/2; m++){
                for (counter=0; counter<=NumPoints-1; counter++){
                    //Here you would have to use the AVX_dot product
                    //double k_vec[] = {grid_x[m], grid_x[l], grid_x[m]};
                    //exponent = AVX_dot(&point_set[counter*3], &k_vec[0]);
                    __m256d AVX_vec_1 = _mm256_set_pd(0, point_set[counter*3], point_set[counter*3+1], point_set[counter*3+2]);
                    __m256d AVX_vec_2 = _mm256_set_pd(0, grid_x[j], grid_x[l], grid_x[m]);
                    __m256d AVX_mult_result = _mm256_mul_pd(AVX_vec_1, AVX_vec_2);
                    __m256d AVX_mult_result_reversed = _mm256_permute2f128_pd(AVX_mult_result, AVX_mult_result, 0b00000001);
                    __m256d AVX_sum = _mm256_hadd_pd(AVX_mult_result, AVX_mult_result_reversed);
                    __m256d AVX_result = _mm256_hadd_pd(AVX_sum, proxy_vec);
                    //double result_sum[4];
                    //double* AVX_sum_result = &result_sum[0];
                    //_mm256_store_pd(AVX_sum_result, AVX_result);
                    //exponent = result_sum[0];
                    //exponent = ((double*) &AVX_result)[0];
                    exponent = point_set[counter*3] * grid_x[j] + point_set[counter*3+1] * grid_x[l] + point_set[counter*3+2] * grid_x[m];
                    //delta_k_real[j*Ncells*Ncells + l*Ncells + m] += norm*cos(exponent);
                    //delta_k_imag[j*Ncells*Ncells + l*Ncells + m] += -norm*sin(exponent);
                }
                if (j>0 && l>0 && m>0 && j<Ncells/2 && l<Ncells/2 && m<Ncells/2){
                delta_k_real[(Ncells-j)*Ncells*Ncells + (Ncells-l)*Ncells + (Ncells-m)] = delta_k_real[j*Ncells*Ncells + l*Ncells + m];
                delta_k_imag[(Ncells-j)*Ncells*Ncells  + (Ncells-l)*Ncells  + (Ncells-m)] = -delta_k_imag[j*Ncells*Ncells  + l*Ncells + m];
                delta_k_real[(Ncells-j)*Ncells*Ncells  + (Ncells-l)*Ncells  + m] = delta_k_real[j*Ncells*Ncells  + l*Ncells + m];
                delta_k_imag[(Ncells-j)*Ncells*Ncells  + (Ncells-l)*Ncells  + m] = -delta_k_imag[j*Ncells*Ncells  + l*Ncells + m];
                delta_k_real[(Ncells-j)*Ncells*Ncells  + l*Ncells + (Ncells-m)] = delta_k_real[j*Ncells*Ncells  + l*Ncells + m];
                delta_k_imag[(Ncells-j)*Ncells*Ncells  + l*Ncells + (Ncells-m)] = -delta_k_imag[j*Ncells*Ncells  + l*Ncells + m];
                delta_k_real[j*Ncells*Ncells  + (Ncells-l)*Ncells  + (Ncells-m)] = delta_k_real[j*Ncells*Ncells  + l*Ncells + m];
                delta_k_imag[j*Ncells*Ncells  + (Ncells-l)*Ncells  + (Ncells-m)] = -delta_k_imag[j*Ncells*Ncells  + l*Ncells + m];
                delta_k_real[(Ncells-j)*Ncells*Ncells  + l*Ncells + m] = delta_k_real[j*Ncells*Ncells  + l*Ncells + m];
                delta_k_imag[(Ncells-j)*Ncells*Ncells  + l*Ncells + m] = -delta_k_imag[j*Ncells*Ncells  + l*Ncells + m];
                delta_k_real[j*Ncells*Ncells  + l*Ncells + (Ncells-m)] = delta_k_real[j*Ncells*Ncells  + l*Ncells + m];
                delta_k_imag[j*Ncells*Ncells  + l*Ncells + (Ncells-m)] = -delta_k_imag[j*Ncells*Ncells  + l*Ncells + m];
                delta_k_real[j*Ncells*Ncells  + (Ncells-l)*Ncells  + m] = delta_k_real[j*Ncells*Ncells  + l*Ncells + m];
                delta_k_imag[j*Ncells*Ncells  + (Ncells-l)*Ncells  + m] = -delta_k_imag[j*Ncells*Ncells  + l*Ncells + m];
                }
            }
        }
    }

    // Now we just have to mirror the rest, walls and lines

    // 3 walls with j, l or m =0 and with j, l or m=Ncells/2
    for (l=1; l<=Ncells/2-1; l++){
        for (m=1; m<=Ncells/2-1; m++){
            delta_k_real[(Ncells-l)*Ncells + (Ncells-m)] = delta_k_real[l*Ncells + m];
            delta_k_imag[(Ncells-l)*Ncells + (Ncells-m)] = -delta_k_imag[l*Ncells + m];
            delta_k_real[l*Ncells + (Ncells-m)] = delta_k_real[l*Ncells + m];
            delta_k_imag[l*Ncells + (Ncells-m)] = -delta_k_imag[l*Ncells + m];
            delta_k_real[(Ncells-l)*Ncells + m] = delta_k_real[l*Ncells + m];
            delta_k_imag[(Ncells-l)*Ncells  + m] = -delta_k_imag[l*Ncells + m];
            delta_k_real[Ncells/2*Ncells*Ncells + (Ncells-l)*Ncells  + (Ncells-m)] = delta_k_real[Ncells/2*Ncells*Ncells + l*Ncells + m];
            delta_k_imag[Ncells/2*Ncells*Ncells + (Ncells-l)*Ncells  + (Ncells-m)] = -delta_k_imag[Ncells/2*Ncells*Ncells + l*Ncells + m];
            delta_k_real[Ncells/2*Ncells*Ncells + l*Ncells + (Ncells-m)] = delta_k_real[Ncells/2*Ncells*Ncells + l*Ncells + m];
            delta_k_imag[Ncells/2*Ncells*Ncells + l*Ncells + (Ncells-m)] = -delta_k_imag[Ncells/2*Ncells*Ncells + l*Ncells + m];
            delta_k_real[Ncells/2*Ncells*Ncells + (Ncells-l)*Ncells  + m] = delta_k_real[Ncells/2*Ncells*Ncells + l*Ncells + m];
            delta_k_imag[Ncells/2*Ncells*Ncells + (Ncells-l)*Ncells  + m] = -delta_k_imag[Ncells/2*Ncells*Ncells + l*Ncells + m];
        }
    }

    for (j=1; j<=Ncells-1; j++){
        for (m=1; m<=Ncells-1; m++){
            delta_k_real[(Ncells-j)*Ncells*Ncells + (Ncells-m)] = delta_k_real[j*Ncells*Ncells + m];
            delta_k_imag[(Ncells-j)*Ncells*Ncells + (Ncells-m)] = -delta_k_imag[j*Ncells*Ncells + m];
            delta_k_real[j*Ncells*Ncells + (Ncells-m)] = delta_k_real[j*Ncells*Ncells + m];
            delta_k_imag[j*Ncells*Ncells + (Ncells-m)] = -delta_k_imag[j*Ncells*Ncells + m];
            delta_k_real[(Ncells-j)*Ncells*Ncells + m] = delta_k_real[j*Ncells*Ncells + m];
            delta_k_imag[(Ncells-j)*Ncells*Ncells + m] = -delta_k_imag[j*Ncells*Ncells + m];
            delta_k_real[(Ncells-j)*Ncells*Ncells  + Ncells/2*Ncells + (Ncells-m)] = delta_k_real[j*Ncells*Ncells  + Ncells/2*Ncells + m];
            delta_k_imag[(Ncells-j)*Ncells*Ncells  + Ncells/2*Ncells + (Ncells-m)] = -delta_k_imag[j*Ncells*Ncells  + Ncells/2*Ncells + m];
            delta_k_real[j*Ncells*Ncells  + Ncells/2*Ncells + (Ncells-m)] = delta_k_real[j*Ncells*Ncells  + Ncells/2*Ncells + m];
            delta_k_imag[j*Ncells*Ncells  + Ncells/2*Ncells + (Ncells-m)] = -delta_k_imag[j*Ncells*Ncells  + Ncells/2*Ncells + m];
            delta_k_real[(Ncells-j)*Ncells*Ncells  + Ncells/2*Ncells + m] = delta_k_real[j*Ncells*Ncells  + Ncells/2*Ncells + m];
            delta_k_imag[(Ncells-j)*Ncells*Ncells  + Ncells/2*Ncells + m] = -delta_k_imag[j*Ncells*Ncells  + Ncells/2*Ncells + m];
        }
    }

    for (j=1; j<=Ncells-1; j++){
        for (l=1; l<=Ncells-1; l++){
            delta_k_real[(Ncells-j)*Ncells*Ncells  + (Ncells-l)*Ncells] = delta_k_real[j*Ncells*Ncells  + l*Ncells];
            delta_k_imag[(Ncells-j)*Ncells*Ncells  + (Ncells-l)*Ncells] = -delta_k_imag[j*Ncells*Ncells  + l*Ncells];
            delta_k_real[j*Ncells*Ncells  + (Ncells-l)*Ncells] = delta_k_real[j*Ncells*Ncells  + l*Ncells];
            delta_k_imag[j*Ncells*Ncells  + (Ncells-l)*Ncells] = -delta_k_imag[j*Ncells*Ncells  + l*Ncells];
            delta_k_real[(Ncells-j)*Ncells*Ncells  + l*Ncells] = delta_k_real[j*Ncells*Ncells  + l*Ncells];
            delta_k_imag[(Ncells-j)*Ncells*Ncells  + l*Ncells] = -delta_k_imag[j*Ncells*Ncells  + l*Ncells];
            delta_k_real[(Ncells-j)*Ncells*Ncells  + (Ncells-l)*Ncells  + Ncells/2] = delta_k_real[j*Ncells*Ncells  + l*Ncells + Ncells/2];
            delta_k_imag[(Ncells-j)*Ncells*Ncells  + (Ncells-l)*Ncells  + Ncells/2] = -delta_k_imag[j*Ncells*Ncells  + l*Ncells + Ncells/2];
            delta_k_real[j*Ncells*Ncells  + (Ncells-l)*Ncells  + Ncells/2] = delta_k_real[j*Ncells*Ncells  + l*Ncells + Ncells/2];
            delta_k_imag[j*Ncells*Ncells  + (Ncells-l)*Ncells  + Ncells/2] = -delta_k_imag[j*Ncells*Ncells  + l*Ncells + Ncells/2];
            delta_k_real[(Ncells-j)*Ncells*Ncells  + l*Ncells + Ncells/2] = delta_k_real[j*Ncells*Ncells  + l*Ncells + Ncells/2];
            delta_k_imag[(Ncells-j)*Ncells*Ncells  + l*Ncells + Ncells/2] = -delta_k_imag[j*Ncells*Ncells  + l*Ncells + Ncells/2];
        }
    }

    for (j=1; j<=Ncells-1; j++){
            delta_k_real[(Ncells-j)*Ncells*Ncells] = delta_k_real[j*Ncells*Ncells];
            delta_k_imag[(Ncells-j)*Ncells*Ncells] = -delta_k_imag[j*Ncells*Ncells];
            delta_k_real[(Ncells-j)*Ncells*Ncells  + Ncells/2*Ncells + Ncells/2] = delta_k_real[j*Ncells*Ncells  + Ncells/2*Ncells + Ncells/2];
            delta_k_imag[(Ncells-j)*Ncells*Ncells  + Ncells/2*Ncells + Ncells/2] = -delta_k_imag[j*Ncells*Ncells  + Ncells/2*Ncells + Ncells/2];
        }

    for (l=1; l<=Ncells-1; l++){
            delta_k_real[(Ncells-l)*Ncells] = delta_k_real[l*Ncells];
            delta_k_imag[(Ncells-l)*Ncells] = -delta_k_imag[l*Ncells];
            delta_k_real[Ncells/2*Ncells*Ncells + (Ncells-l)*Ncells  + Ncells/2] = delta_k_real[Ncells/2*Ncells*Ncells + l*Ncells + Ncells/2];
            delta_k_imag[Ncells/2*Ncells*Ncells + (Ncells-l)*Ncells  + Ncells/2] = -delta_k_imag[Ncells/2*Ncells*Ncells + l*Ncells + Ncells/2];
        }

    for (m=1; m<=Ncells-1; m++){
            delta_k_real[(Ncells-m)] = delta_k_real[m];
            delta_k_imag[(Ncells-m)] = -delta_k_imag[m];
            delta_k_real[Ncells/2*Ncells*Ncells + Ncells/2*Ncells + (Ncells-m)] = delta_k_real[Ncells/2*Ncells*Ncells + Ncells/2*Ncells + m];
            delta_k_imag[Ncells/2*Ncells*Ncells + Ncells/2*Ncells + (Ncells-m)] = -delta_k_imag[Ncells/2*Ncells*Ncells + Ncells/2*Ncells + m];
        }


    end = clock();
    printf("Elapsed time is: %f \n", ((double)end-start)/CLOCKS_PER_SEC);
    for (l=0; l<=Ncells-1; l++){
       printf("%f %fj\n", delta_k_real[l], delta_k_imag[l]);
    }

    free(point_set);
    free(grid_x);
    free(delta_k_real);
    free(delta_k_imag);
    return 0;
}
