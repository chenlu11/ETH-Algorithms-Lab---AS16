#include <iostream>
#include <cstdlib>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/cycle_canceling.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>
#include <boost/graph/find_flow_cost.hpp>

using namespace boost;
using namespace std;

typedef adjacency_list_traits<vecS, vecS, directedS> Traits;
typedef adjacency_list<vecS, vecS, directedS, no_property,
    property<edge_capacity_t, long,
        property<edge_residual_capacity_t, long,
            property<edge_reverse_t, Traits::edge_descriptor,
                property <edge_weight_t, long> > > > > Graph;

typedef property_map<Graph, edge_capacity_t>::type      EdgeCapacityMap;
typedef property_map<Graph, edge_weight_t >::type       EdgeWeightMap;
typedef property_map<Graph, edge_residual_capacity_t>::type ResidualCapacityMap;
typedef property_map<Graph, edge_reverse_t>::type       ReverseEdgeMap;
typedef graph_traits<Graph>::vertex_descriptor          Vertex;
typedef graph_traits<Graph>::edge_descriptor            Edge;
typedef graph_traits<Graph>::out_edge_iterator  OutEdgeIt; // Iterator

// Custom Edge Adder Class, that holds the references
// to the graph, capacity map, weight map and reverse edge map
// ===============================================================
class EdgeAdder {
    Graph &G;
    EdgeCapacityMap &capacitymap;
    EdgeWeightMap &weightmap;
    ReverseEdgeMap  &revedgemap;

public:
    EdgeAdder(Graph & G, EdgeCapacityMap &capacitymap, EdgeWeightMap &weightmap, ReverseEdgeMap &revedgemap) 
        : G(G), capacitymap(capacitymap), weightmap(weightmap), revedgemap(revedgemap) {}

    void addEdge(int u, int v, long c, long w) {
        Edge e, reverseE;
        tie(e, tuples::ignore) = add_edge(u, v, G);
        tie(reverseE, tuples::ignore) = add_edge(v, u, G);
        capacitymap[e] = c;
        weightmap[e] = w;
        capacitymap[reverseE] = 0;
        weightmap[reverseE] = -w;
        revedgemap[e] = reverseE; 
        revedgemap[reverseE] = e; 
    }
};

int main() 
{
    ios_base::sync_with_stdio(false);
    int T; cin >> T;
    while (T--)
    {
        int n, m, s;
        cin >> n >> m >> s;
        int N = n + m + s + 2;

        Graph G(N);
        Vertex src = 0, sink = N - 1;
        // 0: src, 1..n: buyers, n+1..n+m: sites, n+m+1..n+m+s: states, n+m+s+1: target
        EdgeCapacityMap capacitymap = get(edge_capacity, G);
        EdgeWeightMap weightmap = get(edge_weight, G);
        ReverseEdgeMap revedgemap = get(edge_reverse, G);
        ResidualCapacityMap rescapacitymap = get(edge_residual_capacity, G);
        EdgeAdder eaG(G, capacitymap, weightmap, revedgemap);

        for (int i = 1; i <= n; i++)
            eaG.addEdge(src, i, 1, 0);
        for (int i = 0; i < s; i++)
        {
            int li; cin >> li;
            eaG.addEdge(n + m + 1 + i, sink, li, 0);
        }
        for (int i = 0; i < m; i++)
        {
            int si; cin >> si;
            eaG.addEdge(n + 1 + i, n + m + si, 1, 0);
        }
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < m; j++) 
            {
                int bij; cin >> bij;
                eaG.addEdge(1 + i, n + 1 + j, 1, 100 - bij);
            }
        }

        // Min Cost Max Flow with successive_shortest_path_nonnegative_weights
        successive_shortest_path_nonnegative_weights(G, src, sink);
        int cost = find_flow_cost(G);
        int flow = 0;
        // Iterate over all edges leaving the source to sum up the flow values.
        OutEdgeIt e, eend;
        for(tie(e, eend) = out_edges(vertex(src,G), G); e != eend; ++e) {
            flow += capacitymap[*e] - rescapacitymap[*e];
        }
        int profit = flow * 100 - cost;
        cout << flow << " " << profit << endl;
    }
}