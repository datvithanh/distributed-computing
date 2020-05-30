import time
if __name__ == "__main__":
    from graph.loader import load_raw_graph
    from graph.partition.louvain import *
    from community.parallel import best_partition
#     from community.sequential import best_partition

    
    g = load_raw_graph('data/twitter/egos/12831.edges')
    print(f'g has {len(g)} nodes')
    
    start = time.time()
    partition = best_partition(g)
    end = time.time()
    
    size = float(len(set(partition.values())))
    print(f'g has total {size} communities')
    print(f'total time to partition g: {end - start}')