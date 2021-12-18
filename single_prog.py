import subprocess
import csv

for i in range(1):
    times = []
    run = ''

    times.append('CudaX')
    run = './Cuda/extreme'
    
    

    for i in range (10):
        output = subprocess.run([run], stdout=subprocess.PIPE)
        time = output.stdout.decode('utf-8').split(':')[1].split('\n')[0]
        times.append(time)

    #https://www.geeksforgeeks.org/python-save-list-to-csv/
    with open("times.csv", 'a') as f:
        write = csv.writer(f)
        
        write.writerow(times)

        
