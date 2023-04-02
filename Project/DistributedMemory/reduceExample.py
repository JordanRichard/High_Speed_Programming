from mpi4py import MPI
import random

comm = MPI.COMM_WORLD
size = comm.Get_size()
rank = comm.Get_rank()

# Generate a random array of integers between 1 and 10 on each process
data = [random.randint(1, 10) for i in range(size)]
print("Process ", rank, " has ", data)

# Perform the reduction operation to calculate the sum of all arrays
total_sum = comm.reduce(sum(data), op=MPI.SUM, root=0)

# Print the result on the root process
if rank == 0:
    comm.Barrier()
    print("Total sum:", total_sum)