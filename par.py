import ray
import time
ray.init()

@ray.remote
def f(x):
    return x * x - x

begin = time.time()
futures = [f.remote(i) for i in range(int(1e6))]
ray.get(futures)
end = time.time()
print(end-begin)