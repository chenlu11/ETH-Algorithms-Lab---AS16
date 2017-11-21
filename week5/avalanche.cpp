// Result: correct
#include <iostream>
#include <vector>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/max_cardinality_matching.hpp>

using namespace std;
using namespace boost;

typedef adjacency_list<vecS, vecS, directedS,     // Use vecS for the VertexList! Choosing setS for the OutEdgeList disallows parallel edges.
        no_property,                // interior properties of vertices  
        property<edge_weight_t, int>        // interior properties of edges
        >                   Graph;
typedef graph_traits<Graph>::edge_descriptor        Edge;       // Edge Descriptor: an object that represents a single edge.
typedef graph_traits<Graph>::vertex_descriptor      Vertex;     // Vertex Descriptor: with vecS vertex list, this is really just an int in the range [0, num_vertices(G)).  
typedef graph_traits<Graph>::edge_iterator      EdgeIt;     // to iterate over all edges
typedef graph_traits<Graph>::out_edge_iterator      OutEdgeIt;  // to iterate over all outgoing edges of a vertex
typedef property_map<Graph, edge_weight_t>::type    WeightMap;  // property map to access the interior property edge_weight_t

typedef adjacency_list<vecS, vecS, undirectedS,     // Use vecS for the VertexList! Choosing setS for the OutEdgeList disallows parallel edges.
        no_property,                // interior properties of vertices  
        property<edge_weight_t, int>        // interior properties of edges
        >                   UndirectedGraph;
typedef graph_traits<UndirectedGraph>::vertex_descriptor      UndirectedGraphVertex;     // Vertex Descriptor: with vecS vertex list, this is really just an int in the range [0, num_vertices(G)).  

bool contain_a_matching(int t, int a, int s, int d, vector<vector<int> > &agent_shelter_distance)
{
    UndirectedGraph G(a + s);
    for (int i = 0; i < a; ++i)
        for (int j = 0; j < s; ++j)
            if (agent_shelter_distance[i][j] <= t - d)
                add_edge(i, a + j, G);

    vector<UndirectedGraphVertex> matemap(a + s);      // We MUST use this vector as an Exterior Property Map: Vertex -> Mate in the matching
    edmonds_maximum_cardinality_matching(G, make_iterator_property_map(matemap.begin(), get(vertex_index, G)));
    int matchingsize = matching_size(G, make_iterator_property_map(matemap.begin(), get(vertex_index, G)));
    return (matchingsize == a);
}

bool contain_a_matching2(int t, int a, int s, int d, vector<vector<int> > &agent_shelter_distance)
{
    UndirectedGraph G(a + 2 * s);
    for (int i = 0; i < a; ++i)
        for (int j = 0; j < s; ++j)
            if (agent_shelter_distance[i][j] <= t - 2 * d) {
                add_edge(i, a + 2 * j, G);
                add_edge(i, a + 2 * j + 1, G);
            } else if (agent_shelter_distance[i][j] <= t - d)
                add_edge(i, a + 2 * j, G);

    vector<UndirectedGraphVertex> matemap(a + 2 * s);      // We MUST use this vector as an Exterior Property Map: Vertex -> Mate in the matching
    edmonds_maximum_cardinality_matching(G, make_iterator_property_map(matemap.begin(), get(vertex_index, G)));
    int matchingsize = matching_size(G, make_iterator_property_map(matemap.begin(), get(vertex_index, G)));
    return (matchingsize == a);
}

int main()
{
    ios_base::sync_with_stdio(false);
    int T; cin >> T;
    while (T--) {
        int n, m, a, s, c, d;
        cin >> n >> m >> a >> s >> c >> d;

        Graph G(n);
        WeightMap weightmap = get(edge_weight, G);

        for (int i = 0; i < m; ++i) {
            char w; int x, y, z;            
            cin >> w >> x >> y >> z;
            if (w == 'S') {
                Edge e; bool success;
                tie(e, success) = add_edge(x, y, G);
                weightmap[e] = z;   
            } else {
                Edge e; bool success;
                tie(e, success) = add_edge(x, y, G);
                weightmap[e] = z;
                tie(e, success) = add_edge(y, x, G);
                weightmap[e] = z;
            }
        }

        vector<int> agent_pos(a);
        vector<int> shelter_pos(s);
        for (int i = 0; i < a; ++i)
            cin >> agent_pos[i];
        for (int i = 0; i < s; ++i)
            cin >> shelter_pos[i];

        vector<vector<int> > agent_shelter_distance(a, vector<int>(s));
        int longest_shortest_distance = 0;
        for (int i = 0; i < a; ++i) {
            // Dijkstra shortest paths
            vector<int> distmap(n);
            dijkstra_shortest_paths(G, agent_pos[i], distance_map(make_iterator_property_map(distmap.begin(), get(vertex_index, G))));   // distance map as Named Parameter
            for (int j = 0; j < s; ++j) {
                agent_shelter_distance[i][j] = distmap[shelter_pos[j]];
                if (agent_shelter_distance[i][j] != INT_MAX)
                    longest_shortest_distance = max(longest_shortest_distance, agent_shelter_distance[i][j]);
            }
        }

        if (c == 1) {
            int low = d, high = longest_shortest_distance + d;
            while (low < high) {
                int mid = (low + high) / 2;
                if (contain_a_matching(mid, a, s, d, agent_shelter_distance))
                    high = mid;
                else
                    low = mid + 1;
            }
            cout << low << endl;
        } else if (c == 2) {
            int low = d, high = longest_shortest_distance + 2 * d;
            while (low < high) {
                int mid = (low + high) / 2;
                if (contain_a_matching2(mid, a, s, d, agent_shelter_distance))
                    high = mid;
                else
                    low = mid + 1;
            }
            cout << low << endl;
        }
        
    }
}