#include <iostream>
#include <vector>
#include <map>
#include <cmath>
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
        int n, r;
        cin >> n >> r;
        vector<P> planet(n);
        map<P, int> pos_id_map;
        for (int i = 0; i < n; i++)
        {
            cin >> planet[i];
            pos_id_map[planet[i]] = i;
        }
        // Binary Search
        // Find the largest k <= n / 2, which max. component in G(Pk, ..., Pn-1) >= k
        int low = 0, high = n / 2;
        while (low < high)
        {
            int mid = low + (high - low + 1) / 2;

            int k = mid;
            Triangulation t;
            t.insert(planet.begin() + k, planet.end());

            Graph G(n - k);
            for (Edge_iterator e = t.finite_edges_begin(); e != t.finite_edges_end(); ++e)
            {
                if(CGAL::to_double(t.segment(e).squared_length()) <= pow(r, 2))
                {
                    Triangulation::Vertex_handle p1 = e->first->vertex((e->second + 1) % 3);
                    Triangulation::Vertex_handle p2 = e->first->vertex((e->second + 2) % 3);
                    int u = pos_id_map[p1->point()] - k;
                    int v = pos_id_map[p2->point()] - k;
                    Edge e; bool success;
                    tie(e, success) = add_edge(u, v, G);
                }
            }

            vector<int> component_map(n);
            int ncc = connected_components(G, make_iterator_property_map(component_map.begin(), get(vertex_index, G))); 
            vector<int> component_count(ncc);
            for (int i = 0; i < n - k; i++)
                component_count[component_map[i]]++;
            int max_component = *max_element(component_count.begin(), component_count.end());
            
            if (max_component >= k)
                low = mid;
            else
                high = mid - 1;
        }
        cout << low << '\n';
    }
}