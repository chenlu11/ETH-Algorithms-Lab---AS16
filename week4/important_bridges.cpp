#include <iostream>
#include <vector>
#include <algorithm>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/biconnected_components.hpp>

using namespace std;
using namespace boost;

namespace boost
{
  struct edge_component_t
  {
    enum
    { num = 555 };
    typedef edge_property_tag kind;
  }
  edge_component;
}

typedef adjacency_list<vecS, vecS, undirectedS,
        no_property, 
        property<edge_component_t, int> 
        > Graph;
typedef graph_traits<Graph>::edge_descriptor Edge;
typedef graph_traits<Graph>::vertex_descriptor Vertex;
typedef graph_traits<Graph>::edge_iterator EdgeIt;
typedef property_map<Graph, edge_component_t>::type ComponentMap;


int main() 
{
    ios_base::sync_with_stdio(false);
    int t; cin >> t;
    while(t--) {
        // Read and build graph
        int n, m;
        cin >> n >> m;
        Graph G(n);
        ComponentMap componentmap = get(edge_component, G);
        for (int i = 0; i < m; i++) {
            int u, v;
            cin >> u >> v;
            Edge e; bool success;
            tie(e, success) = add_edge(u, v, G);
        }

        // Biconnected Connected components
        // The biconnected components of a graph are the maximal subsets of vertices such that the removal of a vertex from a particular component will not disconnect the component.  
        int nbcc = biconnected_components(G, componentmap); 
        vector<int> componentdegree(nbcc);
        // Iterate over all edges
        EdgeIt ebeg, eend;
        for (tie(ebeg, eend) = edges(G); ebeg != eend; ++ebeg) { 
            // ebeg is EdgeIterator, *ebeg is EdgeDescriptor
            componentdegree[componentmap[*ebeg]]++;
        }

        int k = 0;
        vector<pair<int, int> > criticaledges;
        for (tie(ebeg, eend) = edges(G); ebeg != eend; ++ebeg) { 
            // ebeg is EdgeIterator, *ebeg is EdgeDescriptor
            if (componentdegree[componentmap[*ebeg]] == 1) {
                k++;
                Vertex u = source(*ebeg, G);
                Vertex v = target(*ebeg, G);
                if (u > v) 
                    swap(u, v);
                criticaledges.push_back(make_pair(u, v));
            }
        }
        cout << k << endl;
        sort(criticaledges.begin(), criticaledges.end());
        for (int i = 0; i < criticaledges.size(); i++) {
            cout << criticaledges[i].first << ' ' << criticaledges[i].second << endl;
        }
    }
    return 0;
}
