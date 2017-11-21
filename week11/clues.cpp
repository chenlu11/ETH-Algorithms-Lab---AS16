#include <iostream>
#include <vector>
#include <map>
#include <cmath>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/bipartite.hpp>
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

double compute_shortest_edge(vector<P> &pts)
{
    Triangulation t;
    t.insert(pts.begin(), pts.end());

    K::FT squared_d_min = DBL_MAX;
    for (Edge_iterator e = t.finite_edges_begin(); e != t.finite_edges_end(); ++e)
    {
        K::FT squared_d = t.segment(e).squared_length();
        if (squared_d < squared_d_min)
            squared_d_min = squared_d;
    }
    return CGAL::to_double(squared_d_min);      
}

int main()
{
    ios_base::sync_with_stdio(false);
    int testcases; cin >> testcases;
    while (testcases--)
    {
        int n, m, r;
        cin >> n >> m >> r;
        vector<P> stations(n);
        map<P, int> station_id_map;
        for (int i = 0; i < n; i++)
        {
            cin >> stations[i];
            station_id_map[stations[i]] = i;
        }

        Triangulation t;
        t.insert(stations.begin(), stations.end());
    
        Graph G(n);
        for (Edge_iterator e = t.finite_edges_begin(); e != t.finite_edges_end(); ++e)
        {
            if (CGAL::to_double(t.segment(e).squared_length()) <= pow(r, 2))
            {
                // get the vertices of e
                Triangulation::Vertex_handle p1 = e->first->vertex((e->second + 1) % 3);
                Triangulation::Vertex_handle p2 = e->first->vertex((e->second + 2) % 3);
                int u = station_id_map[p1->point()];
                int v = station_id_map[p2->point()];
                Edge e; bool success;
                tie(e, success) = add_edge(u, v, G);
            }
        }

        // bool bipartite = is_bipartite(G);
        vector<default_color_type> partition_map(n);
        bool bipartite = is_bipartite(G, get(vertex_index, G), make_iterator_property_map(partition_map.begin(), get(vertex_index, G)));
        if (bipartite)
        {
            vector<P> pts_white, pts_black;
            for (int i = 0; i < n; i++)
            {
                // cout << ((partition_map[i] == color_traits<default_color_type>::white()) ? "white" : "black") << '\n';
                if (partition_map[i] == color_traits<default_color_type>::white())
                    pts_white.push_back(stations[i]);
                else
                    pts_black.push_back(stations[i]);
            }
            if (compute_shortest_edge(pts_white) <= pow(r, 2) || compute_shortest_edge(pts_black) <= pow(r, 2))
                bipartite = false;
        }
        

        vector<int> componentmap(n);
        int ncc = connected_components(G, make_iterator_property_map(componentmap.begin(), get(vertex_index, G))); 

        for (int i = 0; i < m; i++)
        {
            P clue_a, clue_b;
            cin >> clue_a >> clue_b;
            K::FT squared_d_a_b = CGAL::squared_distance(clue_a, clue_b);
            if (!bipartite)
                cout << 'n';
            else
            {
                if (CGAL::to_double(squared_d_a_b) <= pow(r, 2))
                    cout << 'y';
                else
                {
                    Triangulation::Vertex_handle nearest_a = t.nearest_vertex(clue_a);
                    Triangulation::Vertex_handle nearest_b = t.nearest_vertex(clue_b);
                    if (componentmap[station_id_map[nearest_a->point()]] != componentmap[station_id_map[nearest_b->point()]])
                        cout << 'n';
                    else
                    {
                        K::FT squared_d_a = CGAL::squared_distance(clue_a, nearest_a->point());
                        K::FT squared_d_b = CGAL::squared_distance(clue_b, nearest_b->point());
                        if (CGAL::to_double(squared_d_a) > pow(r, 2) || CGAL::to_double(squared_d_b) > pow(r, 2))
                            cout << 'n';
                        else
                            cout << 'y';               
                    }             
                }
            }
        }
        cout << '\n';
    }
}