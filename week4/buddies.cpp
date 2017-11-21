#include <iostream>
#include <vector>
#include <algorithm>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/max_cardinality_matching.hpp>

using namespace std;
using namespace boost;

typedef adjacency_list<vecS, vecS, undirectedS> Graph;
typedef graph_traits<Graph>::edge_descriptor Edge;
typedef graph_traits<Graph>::vertex_descriptor Vertex;
typedef graph_traits<Graph>::edge_iterator EdgeIt;

int main() 
{
    ios_base::sync_with_stdio(false);
    int t; cin >> t;
    while(t--) {
        // Read and build graph
        int n, c, f;
        cin >> n >> c >> f;
        vector<vector<string> > characteristic(n, vector<string>(c));
        for (int i = 0; i < n; i++)
            for (int j = 0; j < c; j++) 
                cin >> characteristic[i][j];
        for (int i = 0; i < n; i++)
            sort(characteristic[i].begin(), characteristic[i].end());

        Graph G(n);
        for (int i = 0; i < n; i++) {
            for (int j = i + 1; j < n; j++) {
                int overlap = 0, p_i = 0, p_j = 0;
                while (p_i < c && p_j < c) {
                    if (characteristic[i][p_i] == characteristic[j][p_j]) {
                        overlap++;
                        p_i++;
                        p_j++;
                    } else if (characteristic[i][p_i] < characteristic[j][p_j]) {
                        p_i++;
                    } else {
                        p_j++;
                    }
                }
                if (overlap > f) {
                    Edge e; bool success;
                    tie(e, success) = add_edge(i, j, G);
                }
            }
        }

        // Maximum Cardinality Matching
        vector<Vertex> matemap(n);
        edmonds_maximum_cardinality_matching(G, make_iterator_property_map(matemap.begin(), get(vertex_index, G)));
        int matchingsize = matching_size(G, make_iterator_property_map(matemap.begin(), get(vertex_index, G)));
        cout << ((n == 2 * matchingsize) ? "not optimal" : "optimal") << "\n";
    }
    return 0;
}