// Result: ok ok tl tl ok
#include <iostream>
#include <vector>
#include <map>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/connected_components.hpp>

using namespace std;
using namespace boost;

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;
typedef Triangulation::Edge_iterator  Edge_iterator;
typedef K::Point_2 P;

typedef adjacency_list<vecS, vecS, undirectedS,     // Use vecS for the VertexList! Choosing setS for the OutEdgeList disallows parallel edges.
        no_property,                // interior properties of vertices  
        no_property         // interior properties of edges
        >                   Graph;
typedef graph_traits<Graph>::edge_descriptor        Edge;       // Edge Descriptor: an object that represents a single edge.
typedef graph_traits<Graph>::vertex_descriptor      Vertex;     // Vertex Descriptor: with vecS vertex list, this is really just an int in the range [0, num_vertices(G)).  
typedef graph_traits<Graph>::edge_iterator      EdgeIt;     // to iterate over all edges
typedef graph_traits<Graph>::out_edge_iterator      OutEdgeIt;  // to iterate over all outgoing edges of a vertex

int main()
{
    ios_base::sync_with_stdio(false);
    int testcases; cin >> testcases;
    while (testcases--)
    {
        int n, m; double p;
        cin >> n >> m >> p;
        vector<P> jammer(n);
        map<P, int> pos_id_map;
        for (int i = 0; i < n; i++)
        {
            cin >> jammer[i];
            pos_id_map[jammer[i]] = i;
        }

        vector<P> mission_start(m), mission_target(m);
        for (int i = 0; i < m; i++)
            cin >> mission_start[i] >> mission_target[i];

        Triangulation t;
        t.insert(jammer.begin(), jammer.end());

        Graph G(n);
        for (Edge_iterator e = t.finite_edges_begin(); e != t.finite_edges_end(); ++e)
        {
            if(CGAL::to_double(t.segment(e).squared_length()) <= p)
            {
                Triangulation::Vertex_handle p1 = e->first->vertex((e->second + 1) % 3);
                Triangulation::Vertex_handle p2 = e->first->vertex((e->second + 2) % 3);
                int u = pos_id_map[p1->point()];
                int v = pos_id_map[p2->point()];
                Edge e; bool success;
                tie(e, success) = add_edge(u, v, G);
            }
        }

        // Task 1
        vector<bool> execute(m, true);
        vector<double> squared_d_start(m), squared_d_target(m);
        vector<int> nearest_start_id(m), nearest_target_id(m);
        vector<int> component_map(n);
        int ncc = connected_components(G, make_iterator_property_map(component_map.begin(), get(vertex_index, G))); 
        for (int i = 0; i < m; i++) 
        {
            P nearest_start = t.nearest_vertex(mission_start[i])->point();
            P nearest_target = t.nearest_vertex(mission_target[i])->point();
            squared_d_start[i] = CGAL::to_double(CGAL::squared_distance(mission_start[i], nearest_start));
            squared_d_target[i] = CGAL::to_double(CGAL::squared_distance(mission_target[i], nearest_target));
            nearest_start_id[i] = pos_id_map[nearest_start];
            nearest_target_id[i] = pos_id_map[nearest_target];
            if (squared_d_start[i] > p / 4 || squared_d_target[i] > p / 4)
                execute[i] = false;
            else if (component_map[nearest_start_id[i]] != component_map[nearest_target_id[i]])
                execute[i] = false;
            cout << (execute[i] ? 'y' : 'n');
        }
        cout << '\n';

        // Task 2
        // Binary Search over the whole value range of double
        double low = 0, high = pow(2, 53);
        while (low < high)
        {
            double mid = floor(low + (high - low) / 2);
            // cout << mid << endl;
            Graph G(n);
            for (Edge_iterator e = t.finite_edges_begin(); e != t.finite_edges_end(); ++e)
            {
                if(CGAL::to_double(t.segment(e).squared_length()) <= mid)
                {
                    Triangulation::Vertex_handle p1 = e->first->vertex((e->second + 1) % 3);
                    Triangulation::Vertex_handle p2 = e->first->vertex((e->second + 2) % 3);
                    int u = pos_id_map[p1->point()];
                    int v = pos_id_map[p2->point()];
                    Edge e; bool success;
                    tie(e, success) = add_edge(u, v, G);
                }
            }

            bool result = true;
            vector<int> component_map(n);
            int ncc = connected_components(G, make_iterator_property_map(component_map.begin(), get(vertex_index, G))); 
            for (int i = 0; i < m; i++) 
            {
                if (squared_d_start[i] > mid / 4 || squared_d_target[i] > mid / 4) {
                    result = false;
                    break;
                } else if (component_map[nearest_start_id[i]] != component_map[nearest_target_id[i]]) {
                    result = false;
                    break;
                }
            }

            if (result)
                high = mid;
            else
                low = mid + 1;
        }
        cout << setprecision(0) << setiosflags(ios::fixed) << low << '\n';

        // Task 3
        // Binary Search
        high = low, low = 0;
        while (low < high)
        {
            double mid = floor(low + (high - low) / 2);
            // cout << mid << endl;
            Graph G(n);
            for (Edge_iterator e = t.finite_edges_begin(); e != t.finite_edges_end(); ++e)
            {
                if(CGAL::to_double(t.segment(e).squared_length()) <= mid)
                {
                    Triangulation::Vertex_handle p1 = e->first->vertex((e->second + 1) % 3);
                    Triangulation::Vertex_handle p2 = e->first->vertex((e->second + 2) % 3);
                    int u = pos_id_map[p1->point()];
                    int v = pos_id_map[p2->point()];
                    Edge e; bool success;
                    tie(e, success) = add_edge(u, v, G);
                }
            }

            bool result = true;
            vector<int> component_map(n);
            int ncc = connected_components(G, make_iterator_property_map(component_map.begin(), get(vertex_index, G))); 
            for (int i = 0; i < m; i++) 
            {
                if (!execute[i])
                    continue;
                if (squared_d_start[i] > mid / 4 || squared_d_target[i] > mid / 4) {
                    result = false;
                    break;
                } else if (component_map[nearest_start_id[i]] != component_map[nearest_target_id[i]]) {
                    result = false;
                    break;
                }
            }

            if (result)
                high = mid;
            else
                low = mid + 1;
        }
        cout << setprecision(0) << setiosflags(ios::fixed) << low << '\n';
    
    }
}