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
        int n, e, s, a, b;
        cin >> n >> e >> s >> a >> b;
        int u[e], v[e], w[e][s], h[s];
        for (int i = 0; i < e; ++i) {
            cin >> u[i] >> v[i];
            for (int j = 0; j < s; ++j)
                cin >> w[i][j];
        }
        for (int i = 0; i < s; ++i)
            cin >> h[i];

        Graph H(n);
        WeightMap weightmap_H = get(edge_weight, H);
        for (int i = 0; i < s; ++i) {
            Graph G(n);
            WeightMap weightmap = get(edge_weight, G);
            for (int j = 0; j < e; ++j) {
                Edge e; bool success;
                tie(e, success) = add_edge(u[j], v[j], G);
                weightmap[e] = w[j][i];
            }
            // Prim minimum spanning tree
            vector<Vertex> predecessor(n);
            prim_minimum_spanning_tree(G, make_iterator_property_map(predecessor.begin(), get(vertex_index, G)),
                    root_vertex(h[i]));
            for (int j = 0; j < n; ++j) {
                if (predecessor[j] != j) {
                    Edge e_G, e_H; bool success;
                    tie(e_G, success) = edge(predecessor[j], j, G);
                    tie(e_H, success) = add_edge(predecessor[j], j, H);
                    weightmap_H[e_H] = weightmap[e_G];
                }
            }
        }


        // Dijkstra shortest paths
        vector<int> distance(n);
        Vertex start = a;
        dijkstra_shortest_paths(H, start,
            distance_map(make_iterator_property_map(distance.begin(), get(vertex_index, H))));
        cout << distance[b] << "\n";
    }
    return 0;
}