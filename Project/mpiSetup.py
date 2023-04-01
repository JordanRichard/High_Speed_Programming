from mpi4py import MPI

comm = MPI.COMM_WORLD
size = comm.Get_size()
rank = comm.Get_rank()

print('Hello, World! From Process %d of %d' % (rank, size))