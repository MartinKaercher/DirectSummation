# DirectSummation
Implementation of direct summation to compute power spectra from a point set with a particular focus on speed.

# ToDo list
## V3
- Read in a 3-vector from memory such that the last entry is a zero (as you are only in three dimensions but the AVX vector contains four double precision numbers)
- For that you need to use a mask
- Multiply two 3-vectors with a dot product
- Sum up the result and output it into a normal double number
 
# Content
## V0
- DirectSummation: Plain for loops, no parallelization
- DS\_vectorize: Python vectorization of for loops over k-grid, no parallelization
- DS\_full\_vectorize: Full python vectorization, no for loop, no parallelization
 
## V1
- Plain for loops, but in C, no parallelization
 
## V1.5
- Plain for loops in C using Fourier symmetry of real field, no parallelization

## V2
- Same as V1\_5 but omp parallelization of outmost for loop

## V2.5
- Just some testing to get familiar with the AVX syntax and some simple minimal working examples for
    - Loading data from memory
    - Doing a dot product
    - Sum up a vector internally
## V3
- Same as V1\_5 but using AVX for dot product, no parallelization
 
## V4
- Same as version V3 but using OMP parallelization

# Approximate timings (1000points, 32grid) (on workstation)
## V0
    - DirectSummation.py => 57.1 sec
    - DS\_vectorize.py => 1.7 sec
    - DS\_full\_vectorize.py => 1.68 sec
## V1
    - 0.86 sec
## V1.5
    - 0.14 sec (32 grid)
    - 12.4 sec (100000 points)
## V2
    - 2.18 sec (100000 points)
