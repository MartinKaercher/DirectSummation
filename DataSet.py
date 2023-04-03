import numpy as np


L = 1024.
N_data = 100000
seed = 42
np.random.seed(seed)
xyz = np.random.rand(N_data,3)*L



np.savetxt("Data_Set.txt", xyz, fmt="%.3f")


