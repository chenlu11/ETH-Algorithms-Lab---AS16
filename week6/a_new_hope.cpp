// ok wa wa ok (因为不全是二分图)
// 对于二分图，最大独立集 = 顶点个数 – 最小顶点覆盖（最大匹配） 
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
        int k, s, m;
        cin >> k >> s >> m;
        int n = k * s;
        Graph G(n);

        for (int i = 0; i < m; i++) {
            int u, v, h;
            cin >> u >> v >> h;
            for (int j = 0; j < h; j++) {
                int x, y;
                cin >> x >> y;
                Edge e; bool success;
                tie(e, success) = add_edge(u * s + x, v * s + y, G);
            }
        }

        // Maximum Cardinality Matching
        vector<Vertex> matemap(n);
        edmonds_maximum_cardinality_matching(G, make_iterator_property_map(matemap.begin(), get(vertex_index, G)));
        long max_matching = matching_size(G, make_iterator_property_map(matemap.begin(), get(vertex_index, G)));
        long max_independent_set = n - max_matching;
        cout << max_independent_set << '\n';
    }
    return 0;
}