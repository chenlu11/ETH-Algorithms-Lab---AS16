#include <iostream>
#include <vector>
#include <algorithm>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>

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

class EdgeAdder {
    Graph &G;
    EdgeCapacityMap &capacitymap;
    ReverseEdgeMap  &revedgemap;

public:
    EdgeAdder(Graph & G, EdgeCapacityMap &capacitymap, ReverseEdgeMap &revedgemap):
        G(G), capacitymap(capacitymap), revedgemap(revedgemap){}

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

int main() {
    ios_base::sync_with_stdio(false);
    int t; cin >> t;
    while (t--) {
        int n, m;
        cin >> n >> m;
        // Create Graph and Maps
        Graph G(n + m);
        EdgeCapacityMap capacitymap = get(edge_capacity, G);
        ReverseEdgeMap revedgemap = get(edge_reverse, G);
        ResidualCapacityMap rescapacitymap = get(edge_residual_capacity, G);
        EdgeAdder eaG(G, capacitymap, revedgemap);
        Vertex src = add_vertex(G);
        Vertex sink = add_vertex(G);

        // Add edges
        for (int i = 0; i < m; i++) {
            int a, b, c;
            cin >> a >> b >> c;
            // source -> round vertex
            eaG.addEdge(src, n + i, 1);
            // round vertex -> player vertex representing possible wins
            if (c == 0) {
                eaG.addEdge(n + i, a, 1);
                eaG.addEdge(n + i, b, 1);
            } else if (c == 1) {
                eaG.addEdge(n + i, a, 1);
            } else {
                eaG.addEdge(n + i, b, 1);
            }
        }
        int total_points = 0;
        for (int i = 0; i < n; i++) {
            int s; cin >> s;
            total_points += s;
            eaG.addEdge(i, sink, s);
        }

        // Calculate flow
        long max_flow = push_relabel_max_flow(G, src, sink);
        cout << ((total_points == m && max_flow == m) ? "yes" : "no") << '\n';
    }
    return 0;
}
