# DirectSummation
Implementation of direct summation to compute power spectra from a point set with a particular focus on speed.

# Content
## V0
-DirectSummation: Plain for loops, no parallelization
-DS\_vectorize: Python vectorization of for loops over k-grid, no parallelization
-DS\_full\_vectorize: Full python vectorization, no for loop, no parallelization
## V1
-Plain for loops, but in C, no parallelization
## V1\_5
-Plain for loops in C using Fourier symmetry of real field, no parallelization
## V2
-Same as V1\_5 but omp parallelization of outmost for loop
## V3
-Same as V1\_5 but using AVX for dot product, no parallelization

# Approximate timings (1000points, 32grid) (on workstation)
## V0
    -DirectSummation.py => 57.1 sec
    -DS\_vectorize.py => 1.7 sec
    -DS\_full\_vectorize.py => 1.68 sec
## V1
    - 0.86 sec
## V1\_5
    - 0.14 sec (32 grid)
    - 12.4 sec (100000 points)
## V2
    - 2.18 sec (100000 points)
