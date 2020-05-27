import time

def f(x):
    return x * x - x

begin = time.time()
futures = []
for i in range(int(1e6)):
    futures.append(f(i))
end = time.time()
print(end-begin)