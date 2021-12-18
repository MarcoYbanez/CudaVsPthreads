import subprocess
import csv

for i in range(4):
    times = []
    run = ''
    if i == 1:
        output = subprocess.run(['make', '-C', 'Sequential/'], stdout=subprocess.PIPE)
        times.append('Sequential')
        run = './Sequential/run'
        
    elif i == 2:
        output = subprocess.run(['make', '-C', 'Pthreads/'], stdout=subprocess.PIPE)
        times.append('Pthreads')
        run = './Pthreads/run'

    elif i == 3:
        output = subprocess.run(['make', '-C', 'Cuda/'], stdout=subprocess.PIPE)
        times.append('Cuda')
        run = './Cuda/run'

    else:
        times.append('CudaX')
        run = './Cuda/extreme'
    
    

    for i in range (10):
        output = subprocess.run([run], stdout=subprocess.PIPE)
        time = output.stdout.decode('utf-8').split(':')[1].split('\n')[0]
        times.append(time)

    #https://www.geeksforgeeks.org/python-save-list-to-csv/
    with open("times.csv", 'w') as f:
        write = csv.writer(f)
        
        write.writerow(times)

        
