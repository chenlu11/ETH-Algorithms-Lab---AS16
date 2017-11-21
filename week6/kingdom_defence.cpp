// correct
#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
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
    int src, sink;

public:
    int capacity_surplus;


public:
    EdgeAdder(Graph & G, EdgeCapacityMap &capacitymap, ReverseEdgeMap &revedgemap):
        G(G), capacitymap(capacitymap), revedgemap(revedgemap) {}

    EdgeAdder(Graph & G, EdgeCapacityMap &capacitymap, ReverseEdgeMap &revedgemap, int src, int sink):
        G(G), capacitymap(capacitymap), revedgemap(revedgemap), src(src), sink(sink), capacity_surplus(0) {}

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

    void addEdge(int from, int to, long min_capacity, long max_capacity) {
        if (max_capacity == INT_MAX)
            addEdge(from, to, INT_MAX);
        else if (max_capacity > min_capacity)
            addEdge(from, to, max_capacity - min_capacity);
        addEdge(src, to, min_capacity);
        addEdge(from, sink, min_capacity);
        capacity_surplus += min_capacity;
    }
};

int main() {
    ios_base::sync_with_stdio(false);
    int t; cin >> t;
    while (t--) {
        int n, m;
        cin >> n >> m;
        // Create Graph and Maps
        Graph G(n + 2);
        EdgeCapacityMap capacitymap = get(edge_capacity, G);
        ReverseEdgeMap revedgemap = get(edge_reverse, G);
        ResidualCapacityMap rescapacitymap = get(edge_residual_capacity, G);
        int src = n;
        int sink = n + 1;
        EdgeAdder eaG(G, capacitymap, revedgemap, src, sink);

        // Add edges
        int minimal_soldiers = 0;
        for (int i = 0; i < n; i++) {
            int g, d; cin >> g >> d;
            minimal_soldiers += d;
            eaG.addEdge(src, i, g);
            eaG.addEdge(i, sink, d);
        }

        for (int i = 0; i < m; i++) {
            int u, v, min_c, max_c;
            cin >> u >> v >> min_c >> max_c;
            eaG.addEdge(u, v, min_c, max_c);
        }

        // Calculate flow
        long max_flow = push_relabel_max_flow(G, src, sink);
        cout << ((max_flow == minimal_soldiers + eaG.capacity_surplus) ? "yes" : "no") << '\n';
    }
    return 0;
}
