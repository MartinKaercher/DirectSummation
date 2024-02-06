#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>

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

    int j;
    double grid_x[Ncells];
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
    double delta_k_real[Ncells][Ncells][Ncells];
    double delta_k_imag[Ncells][Ncells][Ncells];
    int l;
    int m; 
    counter = 0;
    double exponent;
    clock_t start, end;
    start = clock();

    for (j=0; j<=Ncells-1; j++){
        for (l=0; l<=Ncells-1; l++){
            for (m=0; m<=Ncells-1; m++){
                for (counter=0; counter<=NumPoints-1; counter++){
                    exponent = point_set[counter*3] * grid_x[j] + point_set[counter*3+1] * grid_x[l] + point_set[counter*3+2] * grid_x[m];
                    delta_k_real[j][l][m] += 1./NumPoints*cos(exponent);
                    delta_k_imag[j][l][m] += -1./NumPoints*sin(exponent);
                }
            }
        }
    }

    end = clock();
    printf("Elapsed time is: %f \n", ((double)end-start)/CLOCKS_PER_SEC);
    printf("%f %fj\n", delta_k_real[0][0][10], delta_k_imag[0][0][10]);

    return 0;
}
