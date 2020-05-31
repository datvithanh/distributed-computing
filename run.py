import os
import time
from graph.loader import load_raw_graph
from graph.partition.louvain import *
from community.parallel import best_partition as parallel_louvain
from community.sequential import best_partition as louvain


if __name__ == "__main__":
    graph_paths = ['data/twitter/egos/12831.edges', 'data/twitter/twitter_combined.txt', 'data/twitter/com-dblp.ungraph.txt', 'data/twitter/com-youtube.ungraph.txt']
       
    for graph_path in graph_paths:
        _, fn = os.path.split(graph_path)
        with open(f'result/{fn}.txt', 'w+') as f:
            g = load_raw_graph(graph_path)
            print(f'g has {len(g)} nodes')
            start = time.time()
            partition = louvain(g)
            end = time.time()
            size = float(len(set(partition.values())))
            print(f'g has total {size} communities')
            print(f'total time to partition g: {end - start}')
            f.write(f'{len(g)},')
            f.write(f'{end-start},')

            for n_jobs in range(1,17):
                print(f'g has {len(g)} nodes')
                start = time.time()
                partition = parallel_louvain(g, n_jobs=n_jobs)
                end = time.time()
                size = float(len(set(partition.values())))
                print(f'g has total {size} communities')
                print(f'total time to partition g: {end - start}')
                f.write(f'{end-start},')
#             f.write('\n')
