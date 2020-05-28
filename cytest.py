import time
if __name__ == "__main__":
    from graph.loader import load_raw_graph
    from graph.partition.louvain import *
    from community.sequential import best_partition
    import cylouvain

    # from community.sequential import best_partition
    
    g = load_raw_graph('data/twitter/egos/12831.edges')
    ug = g.to_undirected()
    print(f'g has {len(g)} nodes')
    
    start = time.time()
    partition = best_partition(ug)
    end = time.time()
    
    size = float(len(set(partition.values())))
    print(f'g has total {size} communities')
    print(f'total time to partition g: {end - start}')
    # Louvain on the NetworkX graph
    print("Apply the algorithm to the NetworkX Graph object")
    start = time.time()
    partition = cylouvain.best_partition(ug)
    modularity = cylouvain.modularity(partition, ug)
    end = time.time()
    print("Output partition:")
    print(partition)
    print("Modularity %0.3f\n" % modularity)
    print(end-start)