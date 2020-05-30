import time
if __name__ == "__main__":
    from graph.loader import load_raw_graph
    from graph.partition.louvain import *
    from community.parallel_2 import best_partition
    # from community.sequential import best_partition

    g = load_raw_graph('data/twitter/egos/12831.edges')
#     g = load_raw_graph('data/twitter/twitter_combined.txt')
    ug = g.to_undirected()
    print(f'g has {len(g)} nodes')

    start = time.time()
    partition = best_partition(ug)
    end = time.time()

    size = float(len(set(partition.values())))
    print(f'g has total {size} communities')
    print(f'total time to partition g: {end - start}')
