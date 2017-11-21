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
typedef graph_traits<Graph>::out_edge_iterator          OutEdgeIt;

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
};

int main() {
    ios_base::sync_with_stdio(false);
    int t; cin >> t;
    while (t--) {
        int n, m, s;
        cin >> n >> m >> s;
        // Create Graph and Maps
        Graph G(n + 2);
        EdgeCapacityMap capacitymap = get(edge_capacity, G);
        ReverseEdgeMap revedgemap = get(edge_reverse, G);
        ResidualCapacityMap rescapacitymap = get(edge_residual_capacity, G);
        int src = n;
        int sink = n + 1;
        EdgeAdder eaG(G, capacitymap, revedgemap, src, sink);
        
        vector<int> degree(n);
        vector<bool> visited(n);
        vector<bool> tovisit(n);
        queue<int> Q;

        // Add edges
        for (int i = 0; i < s; i++) {
            int u; cin >> u;
            eaG.addEdge(src, u, 1);
            degree[u]++;
            if (!visited[u]) {
                Q.push(u);
                visited[u] = true;
            }
        }

        for (int i = 0; i < s; i++) {
            int u; cin >> u;
            eaG.addEdge(u, sink, 1);
            degree[u]++;
        }

        for (int i = 0; i < m; i++) {
            int u, v; cin >> u >> v;
            eaG.addEdge(u, v, 1);
            eaG.addEdge(v, u, 1);
            degree[u]++;
            degree[v]++;
            tovisit[u] = true;
            tovisit[v] = true;
        }

        // Calculate flow
        long max_flow = push_relabel_max_flow(G, src, sink);
        if (max_flow != s) {
            cout << "no" << '\n';
        } else {
            // All edge-incident vertice should be visited
            while (!Q.empty()) {
                int u = Q.front();
                Q.pop();
                OutEdgeIt ebeg, eend;
                for (tie(ebeg, eend) = out_edges(u, G); ebeg != eend; ++ebeg) {
                    int v = target(*ebeg, G);
                    if (v != sink && !visited[v]) {
                        Q.push(v);
                        visited[v] = true;
                    }
                }
            }
            bool visited_all_edges = true;
            for (int i = 0; i < n; i++)
                if (visited[i] != tovisit[i]) {
                    visited_all_edges = false;
                    break;
                }
            if (!visited_all_edges) {
                cout << "no" << '\n';
            } else {         
                bool Eulerian = true;
                for (int i = 0; i < n; i++)
                    if (degree[i] % 2 != 0) {
                        Eulerian = false;
                        break;
                    }
                cout << ((Eulerian) ? "yes" : "no") << '\n';
            }
        }
    }
    return 0;
}
