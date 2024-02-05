import numpy as np


L = 1024.
N_data = 1000
seed = 42
np.random.seed(seed)
xyz = np.random.rand(N_data,3)*L

np.savetxt(f"Data_Set_{N_data}.txt", xyz, fmt="%.3f")


