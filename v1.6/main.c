#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>

// Same as version 1.5 but using loop unrolling for the innermost loop
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
    double exponent1, exponent2, exponent3, exponent4, exponent5, exponent6, exponent7, exponent8, exponent9, exponent10, norm;
    norm = 1./NumPoints;
    clock_t start, end;
    start = clock();

    double aux_real, aux_imag;
    
    // First do the frequencies which have a counterpart
    for (j=0; j<=Ncells/2; j++){
        for (l=0; l<=Ncells/2; l++){
            for (m=0; m<=Ncells/2; m++){
                aux_real = 0;
                aux_imag = 0;
                for (counter=0; counter<=NumPoints/10-1; counter++){
                    exponent1 = point_set[counter*30] * grid_x[j] + point_set[counter*30+1] * grid_x[l*Ncells] + point_set[counter*30+2] * grid_x[m];
                    exponent2 = point_set[counter*30+3] * grid_x[j] + point_set[counter*30+4] * grid_x[l*Ncells] + point_set[counter*30+5] * grid_x[m];
                    exponent3 = point_set[counter*30+6] * grid_x[j] + point_set[counter*30+7] * grid_x[l*Ncells] + point_set[counter*30+8] * grid_x[m];
                    exponent4 = point_set[counter*30+9] * grid_x[j] + point_set[counter*30+10] * grid_x[l*Ncells] + point_set[counter*30+11] * grid_x[m];
                    exponent5 = point_set[counter*30+12] * grid_x[j] + point_set[counter*30+13] * grid_x[l*Ncells] + point_set[counter*30+14] * grid_x[m];
                    exponent6 = point_set[counter*30+15] * grid_x[j] + point_set[counter*30+16] * grid_x[l*Ncells] + point_set[counter*30+17] * grid_x[m];
                    exponent7 = point_set[counter*30+18] * grid_x[j] + point_set[counter*30+19] * grid_x[l*Ncells] + point_set[counter*30+20] * grid_x[m];
                    exponent8 = point_set[counter*30+21] * grid_x[j] + point_set[counter*30+22] * grid_x[l*Ncells] + point_set[counter*30+23] * grid_x[m];
                    exponent9 = point_set[counter*30+24] * grid_x[j] + point_set[counter*30+25] * grid_x[l*Ncells] + point_set[counter*30+26] * grid_x[m];
                    exponent10 = point_set[counter*30+27] * grid_x[j] + point_set[counter*30+28] * grid_x[l*Ncells] + point_set[counter*30+29] * grid_x[m];


                    aux_real += cos(exponent1) + cos(exponent2) + cos(exponent3) + cos(exponent4) + cos(exponent5) + cos(exponent6) + cos(exponent7) + cos(exponent8) + cos(exponent9) + cos(exponent10);
                    aux_imag += -sin(exponent1) - sin(exponent2) - sin(exponent3) - sin(exponent4) - sin(exponent5) - sin(exponent6) - sin(exponent7) - sin(exponent8) - sin(exponent9) - sin(exponent10);
                }
                delta_k_real[j*Ncells*Ncells + l*Ncells + m] += norm*aux_real;
                delta_k_imag[j*Ncells*Ncells + l*Ncells + m] += norm*aux_imag;
                if (j>0 && l>0 && m>0 && j<Ncells/2 && l<Ncells/2 && m<Ncells/2){ // Taking care of the extra cases
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
    printf("%f %fj\n", delta_k_real[10], delta_k_imag[10]);

    free(point_set);
    free(grid_x);
    free(delta_k_real);
    free(delta_k_imag);
    return 0;
}
