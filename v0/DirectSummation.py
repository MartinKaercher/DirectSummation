# The slowest version as it does not vectorize anything
# It's loops over loops

import numpy as np
import matplotlib.pyplot as plt
import time

def DS():
    start = time.perf_counter()
    for i in range(Ncells):
        for j in range(Ncells):
            for l in range(Ncells):
                for m in range(Npoints):
                    PS_grid[i,j,l] += 1/Npoints*np.exp(-1j*(kx[i,j,l]*particles[0,m]+ky[i,j,l]*particles[1,m]+kz[i,j,l]*particles[2,m]))
    end = time.perf_counter()
    print(f'Time needed for the direct summation: {np.round(end-start,3)} seconds')
    print(PS_grid[0,0,10])


if __name__ == "__main__":
    Ncells = 32
    L = 1024.
    dt = L/Ncells

    k = 2*np.pi*np.fft.fftfreq(Ncells, dt)

    kx, ky, kz = np.meshgrid(k,k,k, indexing="ij")

    PS_grid = np.zeros_like(kx, dtype=np.complex_)

    Npoints = 1000
    particles = np.transpose(np.loadtxt(f"../Data_Set_{Npoints}.txt"))

    DS()
