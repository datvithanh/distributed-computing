if __name__ == "__main__":
    from graph.loader import load_raw_graph
    from graph.partition.louvain import *

    g = load_raw_graph('data/twitter/egos/12831.edges')
    ug = g.to_undirected()
    print(f'g has {len(g)} nodes')

    partition = louvain(ug)
    size = float(len(set(partition.values())))
    print(f'g has total {size} communities')
