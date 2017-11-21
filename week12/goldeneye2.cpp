// there are no more than n interesting values for ω
// consider the edges by increasing length
// we actually run Kruskal’s algorithm, which at every step adds a shortest edge that connects two components
#include <algorithm>
#include <iostream>
#include <vector>
#include <cmath>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <CGAL/Delaunay_triangulation_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Triangulation_vertex_base_with_info_2<int, K> V;
typedef CGAL::Triangulation_data_structure_2<V> T;
typedef CGAL::Delaunay_triangulation_2<K, T> Triangulation;

typedef Triangulation::Finite_edges_iterator Edge_iter;
typedef Triangulation::Finite_vertices_iterator Vertex_iter;
typedef Triangulation::Vertex_handle Vertex_handle;

using namespace std;

int find(int x, vector<int> &root) {

	if (root[x] == x) {
		return x;
	} else {
		root[x] = find(root[x], root);
		return root[x];
	}

}

void merge(int x, int y, vector<int> &root) {

	int u = find(x, root);
	int v = find(y, root);
	if (u != v) {
		root[v] = u;
	}

}

struct myedge{
	int u, v;
	double len;
	const bool operator< (const myedge edge_inst) const {
		return len < edge_inst.len;
	}
};

void goldeneye() {

	int n, m;
	double p;
	cin >> n >> m >> p;

	vector<int> root(n);
	vector<int> rootA(n);
	vector<int> rootB(n);

	vector<K::Point_2> jammerNoIndex(n);
	vector< pair<K::Point_2, int> > jammer(n);
	for (int i = 0; i < n; ++i) {
		cin >> jammerNoIndex[i];
		jammer[i] = make_pair(jammerNoIndex[i], i);
		root[i] = i;
		rootA[i] = i;
		rootB[i] = i;
	}

	Triangulation tri;
	tri.insert(jammer.begin(), jammer.end());

	vector<myedge> edgeList;
	for (Edge_iter e = tri.finite_edges_begin(); e != tri.finite_edges_end(); ++e) {
		int src = e->first->vertex((e->second + 1) % 3)->info();
		int tgt = e->first->vertex((e->second + 2) % 3)->info();
		double dist = CGAL::to_double(CGAL::squared_distance(jammerNoIndex[src], jammerNoIndex[tgt]));
		if (dist <= p) {
			merge(src, tgt, root);
		}
		myedge tempEdge;
		tempEdge.u = src;
		tempEdge.v = tgt;
		tempEdge.len = dist;
		edgeList.push_back(tempEdge);
	}

	sort(edgeList.begin(), edgeList.end());

	int pEa = 0, pEb = 0;
	double a = 0, b = 0;
	vector<bool> isok(m, false);
	for (int i = 0; i < m; ++i) {
		K::Point_2 src, tgt;
		cin >> src >> tgt;
		int s = tri.nearest_vertex(src)->info();
		int t = tri.nearest_vertex(tgt)->info();
		double dist1 = CGAL::to_double(CGAL::squared_distance(src, jammerNoIndex[s]));
		double dist2 = CGAL::to_double(CGAL::squared_distance(tgt, jammerNoIndex[t]));
		if (dist1 <= (p / 4) && dist2 <= (p / 4)) {
			if (find(s, root) == find(t, root)) {
				isok[i] = true;
			}
		}

		a = max(a, max(dist1 * 4, dist2 * 4));
		while (pEa < edgeList.size()) {
			if (find(s, rootA) != find(t, rootA)) {
				merge(edgeList[pEa].u, edgeList[pEa].v, rootA);
				a = max(a, edgeList[pEa].len);
				++pEa;
			} else {
				break;
			}
		}

		if (isok[i]) {
			b = max(b, max(dist1 * 4, dist2 * 4));
			while (pEb < edgeList.size()) {
				if (find(s, rootB) != find(t, rootB)) {
					merge(edgeList[pEb].u, edgeList[pEb].v, rootB);
					b = max(b, edgeList[pEb].len);
					++pEb;
				} else {
					break;
				}
			}
		}
	}

	for (int i = 0; i < m; ++i) {
		if (isok[i]) {
			cout << "y";
		} else {
			cout << "n";
		}
	}
	cout << endl;
	cout << setprecision(0) << setiosflags(ios::fixed) << a << endl << b << endl;

}

int main(void) {
	std::ios_base::sync_with_stdio(false);
	int T;
	cin >> T;
	for (int t = 0; t < T; ++t) {
		goldeneye();
	}
}