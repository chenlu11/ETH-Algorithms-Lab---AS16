#include <iostream>
#include <vector>
#include <climits>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/prim_minimum_spanning_tree.hpp>

using namespace std;
using namespace boost;

typedef adjacency_list<vecS, vecS, undirectedS,
        no_property,
        property<edge_weight_t, int>
        > Graph;
typedef graph_traits<Graph>::edge_descriptor Edge;
typedef graph_traits<Graph>::vertex_descriptor Vertex;
typedef graph_traits<Graph>::edge_iterator EdgeIt;
typedef property_map<Graph, edge_weight_t>::type WeightMap;

int main() 
{
    ios_base::sync_with_stdio(false);
    int t; cin >> t;
    while(t--) {
        // Read and build graph
        int n, m;
        cin >> n >> m;
        Graph G(n);
        WeightMap weightmap = get(edge_weight, G);
        for (int i = 0; i < m; i++) {
            int u, v, w;
            cin >> u >> v >> w;
            Edge e; bool success;
            tie(e, success) = add_edge(u, v, G);
            weightmap[e] = w;
        }

        // Prim minimum spanning tree
        vector<Vertex> predecessor(n);
        prim_minimum_spanning_tree(G, make_iterator_property_map(predecessor.begin(), get(vertex_index, G)));
        int totalweight = 0;
        for (int i = 0; i < n; ++i) {
            if (predecessor[i] != i) {
                Edge e; bool success;
                tie(e, success) = edge(i, predecessor[i], G);
                totalweight += weightmap[e];
            }
        }

        // Dijkstra shortest paths
        // vector<Vertex> predecessor(n);
        vector<int> distance(n);
        Vertex start = 0;
        dijkstra_shortest_paths(G, start, // We MUST provide at least one of the two maps
            predecessor_map(make_iterator_property_map(predecessor.begin(), get(vertex_index, G))). // predecessor map as Named Parameter
            distance_map(make_iterator_property_map(distance.begin(), get(vertex_index, G))));   // distance map as Named Parameter
        int maxdist = 0;
        for (int i = 0; i < n; ++i) {
            if (distance[i] < INT_MAX) { 
                if (distance[i] > maxdist)
                    maxdist = distance[i];
            }
        }

        cout << totalweight << ' ' << maxdist << "\n";
    }
    return 0;
}