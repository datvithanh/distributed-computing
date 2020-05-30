import networkx as nx

from tqdm import tqdm

def load_raw_graph(path, verbose=True):
    g = nx.Graph()

    with open(path, 'rt') as f:
        edges = None
        if 'youtube' in path or 'dblp' in path:
            edges = [[int(u) for u in tmp.strip().split()] for tmp in f.readlines()[4:]]
        else:
            edges = [[int(u) for u in tmp.strip().split()] for tmp in f.readlines()]
        g.add_edges_from(edges)

    return g

if __name__ == "__main__":
    g = load_raw_graph('data/twitter/com-youtube-ungraph.txt')
    print(f'g has {len(g)} nodes')