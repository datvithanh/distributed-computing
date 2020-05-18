import networkx as nx

from tqdm import tqdm

def load_raw_graph(path, verbose=True):
    g = nx.DiGraph()

    with open(path, 'rt') as f:
        if verbose:
            f = tqdm(f, desc='Loading graph from %s' % path)
        for line in f:
            line = line.strip()
            id1, id2 = line.split(' ')
            g.add_edge(int(id1), int(id2))
    
    return g

if __name__ == "__main__":
    g = load_raw_graph('data/twitter/egos/12831.edges')
    print(f'g has {len(g)} nodes')