#include <iostream>
#include <vector>
#include <algorithm>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/edmonds_karp_max_flow.hpp>

using namespace std;
using namespace boost;


typedef adjacency_list_traits<vecS, vecS, directedS> Traits;
typedef adjacency_list<vecS, vecS, directedS, no_property,
    property<edge_capacity_t, long,
        property<edge_residual_capacity_t, long,
            property<edge_reverse_t, Traits::edge_descriptor> > > > Graph;

typedef property_map<Graph, edge_capacity_t>::type      EdgeCapacityMap;
typedef property_map<Graph, edge_residual_capacity_t>::type ResidualCapacityMap;
typedef property_map<Graph, edge_reverse_t>::type       ReverseEdgeMap;
typedef graph_traits<Graph>::vertex_descriptor          Vertex;
typedef graph_traits<Graph>::edge_descriptor            Edge;


// Custom Edge Adder Class, that holds the references
// to the graph, capacity map and reverse edge map
// ===================================================
class EdgeAdder {
    Graph &G;
    EdgeCapacityMap &capacitymap;
    ReverseEdgeMap  &revedgemap;

public:
    // to initialize the Object
    EdgeAdder(Graph & G, EdgeCapacityMap &capacitymap, ReverseEdgeMap &revedgemap):
        G(G), capacitymap(capacitymap), revedgemap(revedgemap){}

    // to use the Function (add an edge)
    void addEdge(int from, int to, long capacity) {
        Edge e, reverseE;
        bool success;
        tie(e, success) = add_edge(from, to, G);
        tie(reverseE, success) = add_edge(to, from, G);
        capacitymap[e] = capacity;
        capacitymap[reverseE] = 0;
        revedgemap[e] = reverseE;
        revedgemap[reverseE] = e;
    }
};

int m, n;
int in_vertex_id(int i, int j) {
    return 2 * (i * (m + 2) + j);
}
int out_vertex_id(int i, int j) {
    return 2 * (i * (m + 2) + j) + 1;
}

int main() {
    ios_base::sync_with_stdio(false);
    int T;  cin >> T;
    while (T--) {
        int k, c;
        cin >> m >> n >> k >> c;

        Graph G(2 * (n + 2) * (m + 2));
        EdgeCapacityMap capacitymap = get(edge_capacity, G);
        ReverseEdgeMap revedgemap = get(edge_reverse, G);
        ResidualCapacityMap rescapacitymap = get(edge_residual_capacity, G);
        EdgeAdder eaG(G, capacitymap, revedgemap);
        Vertex src = add_vertex(G);
        Vertex sink = add_vertex(G);
        
        for (int i = 0; i < m + 2; i++) {
            eaG.addEdge(in_vertex_id(0, i), sink, 1);
            eaG.addEdge(in_vertex_id(n + 1, i), sink, 1);
        }
        for (int i = 0; i < n + 2; i++) {
            eaG.addEdge(in_vertex_id(i, 0), sink, 1);
            eaG.addEdge(in_vertex_id(i, m + 1), sink, 1);
        }
        
        for (int i = 1; i <= n; i++) {
            for (int j = 1; j <= m; j++) {
                eaG.addEdge(in_vertex_id(i, j), out_vertex_id(i, j), c);
                eaG.addEdge(out_vertex_id(i, j), in_vertex_id(i - 1, j), 1);
                eaG.addEdge(out_vertex_id(i, j), in_vertex_id(i + 1, j), 1);
                eaG.addEdge(out_vertex_id(i, j), in_vertex_id(i, j - 1), 1);
                eaG.addEdge(out_vertex_id(i, j), in_vertex_id(i, j + 1), 1);
            }
        }

        for (int i = 0; i < k; i++) {
            int x, y;
            cin >> x >> y;
            eaG.addEdge(src, in_vertex_id(y + 1, x + 1), 1);
        }
        

        // Calculate Max Flow
        long flow = push_relabel_max_flow(G, src, sink);
        cout << flow << endl;
    }
    return 0;
}
