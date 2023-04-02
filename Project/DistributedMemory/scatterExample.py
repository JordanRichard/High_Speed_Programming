''' ***************************************************************************************************
*
*   Source: https://mpi4py.readthedocs.io/en/stable/tutorial.html#collective-communication
*
*************************************************************************************************** '''

from mpi4py import MPI

comm = MPI.COMM_WORLD
size = comm.Get_size()
rank = comm.Get_rank()

if rank == 0:
    data = [(i+1)**2 for i in range(size)]
    print("Data to scatter from process 0: ", data)
else:
    data = None

data = comm.scatter(data, root=0)
print("Process ", rank, " has: ", data)