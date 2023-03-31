import multiprocessing

#   Worker function to compute the sum of a list of numbers.
def calculateSum(numbers, result, lock):
    
    s = sum(numbers)
    
    #   Show the current thread's sum
    #print("I HAVE ", s)
    
    with lock:
        result.value += s


if __name__ == '__main__':

    #   Get the number of worker processes from the user
    m = 4

    #   Creates a list of integers between (1,100) to sum
    numbers = list(range(1,101))

    #   Allocate shared memory for the result
    result = multiprocessing.Value('q', 0)

    #   Create a lock object for synchronizing access to the shared memory value
    lock = multiprocessing.Lock()

    #   Create a list of processes and computes chunksize for each
    processes = []
    chunk_size = len(numbers) // m

    #   Begin spawning processes
    for i in range(m):

        start = i * chunk_size
        end = start + chunk_size

        if i == m - 1:
            end = len(numbers)

        #   Create a new process and add it to the list of existing processes   
        p = multiprocessing.Process(target=calculateSum, args=(numbers[start:end], result, lock))
        processes.append(p)

    #   Start the processes
    for p in processes:
        p.start()

    #   Wait for the processes to finish
    for p in processes:
        p.join()

    #   Print the result
    print(f"The sum is: {result.value}")