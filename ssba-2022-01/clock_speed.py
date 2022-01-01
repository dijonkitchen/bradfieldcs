import time

iterations = 1000000000
start_time = time.time()

for _ in range(iterations):
    pass

end_time = time.time()
seconds = end_time - start_time
operations = iterations / seconds

print("Python VM's 'clock' speed is {:.3f} GHz".format(operations / 1000000000))