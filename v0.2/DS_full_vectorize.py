# The slowest version as it does not vectorize anything
# It's loops over loops

import numpy as np
import matplotlib.pyplot as plt
import time

def DS():
    global PS_grid
    start = time.perf_counter()
    for j in range(Npoints):
        PS_grid += 1/Npoints*np.exp(-1j*(kx*particles[0,j]+ky*particles[1,j]+kz*particles[2,j]))
    end = time.perf_counter()
    print(f'Time needed for the direct summation: {np.round(end-start,3)} seconds')
    print(PS_grid[0,0,10])



if __name__ == "__main__":
    global PS_grid
    Ncells = 32
    L = 1024.
    dt = L/Ncells

    k = 2*np.pi*np.fft.fftfreq(Ncells, dt)
    kx, ky, kz = np.meshgrid(k,k,k, indexing="ij")
    PS_grid = np.zeros_like(kx, dtype=np.complex_)
    Npoints = 1000
    particles = np.transpose(np.loadtxt(f"../Data_Set_{Npoints}.txt"))

    DS()
