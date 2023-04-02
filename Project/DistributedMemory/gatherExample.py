''' ***************************************************************************************************
*
*   Source: https://mpi4py.readthedocs.io/en/stable/tutorial.html#collective-communication
*
*************************************************************************************************** '''

from mpi4py import MPI

comm = MPI.COMM_WORLD
size = comm.Get_size()
rank = comm.Get_rank()

data = (rank+1)**2
print("Process ", rank, " Has: ", data)

data = comm.gather(data, root=0)

if rank == 0:
    print("After gathering, Process ", rank, " has: ", data)