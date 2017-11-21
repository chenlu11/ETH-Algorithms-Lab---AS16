#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/tuple/tuple.hpp>

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

int main() {
    ios_base::sync_with_stdio(false);
    int T; cin >> T;
    while (T--)
    {
        int n, m;
        cin >> n >> m;
        Graph G(n);
        EdgeCapacityMap capacitymap = get(edge_capacity, G);
        ReverseEdgeMap revedgemap = get(edge_reverse, G);
        ResidualCapacityMap rescapacitymap = get(edge_residual_capacity, G);
        EdgeAdder eaG(G, capacitymap, revedgemap);
        // build Graph
        for (int i = 0; i < m; i++)
        {
            int a, b, c;
            cin >> a >> b >> c;
            eaG.addEdge(a, b, c);
        }

        int min_cut = INT_MAX;
        Vertex src, sink;
        for (int i = 1; i < n; i++)
        {
            // Find a min cut via maxflow
            int max_flow = push_relabel_max_flow(G, 0, i);
            if (min_cut > max_flow)
            {
                min_cut = max_flow;
                src = 0;
                sink = i;
            }
            max_flow = push_relabel_max_flow(G, i, 0);
            if (min_cut > max_flow)
            {
                min_cut = max_flow;
                src = i;
                sink = 0;
            }
        }
        int max_flow = push_relabel_max_flow(G, src, sink);
        // BFS to find vertex set S
        vector<int> vis(n, false);
        std::queue<int> Q; // BFS queue (from std:: not boost::)
        vis[src] = true; // Mark the source as visited
        Q.push(src);
        while (!Q.empty()) 
        {
            const int u = Q.front();
            Q.pop();
            OutEdgeIt ebeg, eend;
            for (tie(ebeg, eend) = out_edges(u, G); ebeg != eend; ++ebeg) 
            {
                const int v = target(*ebeg, G);
                // Only follow edges with spare capacity
                if (rescapacitymap[*ebeg] == 0 || vis[v]) 
                    continue;
                vis[v] = true;
                Q.push(v);
            }
        }
        cout << min_cut << endl;
        int count = 0;
        for (int i = 0; i < n; ++i) {
            if (vis[i]) count++;
        }
        cout << count << " ";
        // Output S
        for (int i = 0; i < n; ++i) {
            if (vis[i]) cout << i << " ";
        }
        cout << endl;
    }

}