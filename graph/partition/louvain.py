import community
import networkx as nx

def louvain(g: nx.Graph):
    return community.best_partition(g)