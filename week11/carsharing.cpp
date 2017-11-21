#include <iostream>
#include <cstdlib>
#include <vector>
#include <set>
#include <algorithm>
#include <map>
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
        int n, s;
        cin >> n >> s;
        vector<int> limit(s);
        for (int i = 0; i < s; i++)
            cin >> limit[i];
        vector<set<int> > pts(s);
        vector<int> start(n), target(n), departure(n), arrival(n), profit(n);
        for (int i = 0; i < n; i++)
        {
            int cur_start, cur_target;
            cin >> cur_start >> cur_target >> departure[i] >> arrival[i] >> profit[i];
            start[i] = cur_start - 1; target[i] = cur_target - 1;
            pts[start[i]].insert(departure[i]);
            pts[target[i]].insert(arrival[i]);
        }

        vector<vector<int> > pts_sorted(s);
        vector<int> pts_start_id(s + 1);
        vector<map<int, int> > pts_pos_id_map(s);
        pts_start_id[0] = 1;
        for (int i = 0; i < s; i++)
        {
            pts[i].insert(0);
            pts[i].insert(100000);
            pts_sorted[i] = vector<int>(pts[i].begin(), pts[i].end());
            sort(pts_sorted[i].begin(), pts_sorted[i].end());
            pts_start_id[i + 1] = pts_start_id[i] + pts_sorted[i].size();
            for (int j = 0; j < pts_sorted[i].size(); j++)
                pts_pos_id_map[i][pts_sorted[i][j]] = j;
        }

        int N = pts_start_id[s] + 1;
        Graph G(N);
        Vertex src = 0, sink = N - 1;

        EdgeCapacityMap capacitymap = get(edge_capacity, G);
        EdgeWeightMap weightmap = get(edge_weight, G);
        ReverseEdgeMap revedgemap = get(edge_reverse, G);
        ResidualCapacityMap rescapacitymap = get(edge_residual_capacity, G);
        EdgeAdder eaG(G, capacitymap, weightmap, revedgemap);

        for (int i = 0; i < s; i++)
        {
            eaG.addEdge(src, pts_start_id[i], limit[i], 0);
            eaG.addEdge(pts_start_id[i + 1] - 1, sink, INT_MAX, 0);
        }
        for (int i = 0; i < s; i++)
            for (int j = 0; j < pts_sorted[i].size() - 1; j++)
                eaG.addEdge(pts_start_id[i] + j, pts_start_id[i] + j + 1, INT_MAX, (pts_sorted[i][j + 1] - pts_sorted[i][j]) * 100);
        for (int i = 0; i < n; i++)
        {
            int cur_source = pts_start_id[start[i]] + pts_pos_id_map[start[i]][departure[i]];
            int cur_target = pts_start_id[target[i]] + pts_pos_id_map[target[i]][arrival[i]];
            int cur_weight = (arrival[i] - departure[i]) * 100 - profit[i];
            eaG.addEdge(cur_source, cur_target, 1, cur_weight);
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
        int max_profit = flow * 100000 * 100 - cost;
        cout << max_profit << endl;
    }
}