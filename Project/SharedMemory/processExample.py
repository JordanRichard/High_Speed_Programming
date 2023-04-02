from multiprocessing import Process

def f(name):
    print('Hello, World! From process: ', name)

if __name__ == '__main__':

    # Define 3 named processes targeting the function 'f'
    p1 = Process(target=f, args=('P1',))
    p2 = Process(target=f, args=('P2',))
    p3 = Process(target=f, args=('P3',))
   
    # Launch the processes
    p1.start() 
    p2.start()
    p3.start()

    # Wait for the processes to finish
    p1.join()
    p2.join()
    p3.join()