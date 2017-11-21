#include <iostream>
#include <cstdlib>
#include <vector>
#include <climits>

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
        int n, m, l;
        cin >> n >> m >> l;
        Graph G(n + 1);
        Vertex src = n, sink = n - 1;
        // src: n, stations: 0..n-1, sink = n-1
        EdgeCapacityMap capacitymap = get(edge_capacity, G);
        EdgeWeightMap weightmap = get(edge_weight, G);
        ReverseEdgeMap revedgemap = get(edge_reverse, G);
        ResidualCapacityMap rescapacitymap = get(edge_residual_capacity, G);
        EdgeAdder eaG(G, capacitymap, weightmap, revedgemap);

        eaG.addEdge(src, 0, l, 128);
        for (int i = 0; i < m; i++)
        {
            int x, y, q; cin >> x >> y >> q;
            eaG.addEdge(x, y, 1, 128 * (y - x) - q);
        }
        for (int i = 0; i < n - 1; i++)
            eaG.addEdge(i, i + 1, INT_MAX, 128);
        
        // Min Cost Max Flow with successive_shortest_path_nonnegative_weights
        successive_shortest_path_nonnegative_weights(G, src, sink);
        int cost = find_flow_cost(G);
        int flow = 0;
        // Iterate over all edges leaving the source to sum up the flow values.
        OutEdgeIt e, eend;
        for(tie(e, eend) = out_edges(vertex(src,G), G); e != eend; ++e) {
            flow += capacitymap[*e] - rescapacitymap[*e];
        }
        int max_profit = flow * n * 128 - cost;
        cout << max_profit << endl;
    }
}