from mpi4py import MPI
import random

comm = MPI.COMM_WORLD
size = comm.Get_size()
rank = comm.Get_rank()

# Number of points to use for the Monte Carlo method
n = 10000000

# Each process generates a random set of points within the unit square
random.seed(rank)
count = 0
for i in range(n):
    x = random.uniform(0, 1)
    y = random.uniform(0, 1)
    if x**2 + y**2 <= 1:
        count += 1

# Each process calculates its own estimate of pi
local_pi = 4.0 * count / n

# The estimates of pi from each process are combined using a reduction operation
global_pi = comm.reduce(local_pi, op=MPI.SUM, root=0)

# The root process prints the final estimate of pi
if rank == 0:
    print("Estimate of pi:", global_pi / size)
