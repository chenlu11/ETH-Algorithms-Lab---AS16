#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <queue>
#include <algorithm>
using namespace std;

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;
typedef Triangulation::Finite_edges_iterator  Edge_iterator;

int main()
{
    while (true)
    {
        // read number of existing points
        int n;
        std::cin >> n;
        if (n == 0)
            break;
        // read bounding box
        int xmin, ymin, xmax, ymax;
        cin >> xmin >> ymin >> xmax >> ymax;
        // read points
        vector<K::Point_2> pts;
        pts.reserve(n);
        double* min_ds = new double[n];
        map<K::Point_2,int> pointmap;
        for (int i = 0; i < n; ++i) 
        {
            int x, y;
            cin >> x >> y;
            K::Point_2 p(x,y);            
            pts.push_back(p);
            min_ds[i] = min(min(min(xmax - x, x - xmin), ymax - y), y - ymin);
            pointmap.insert(pair<K::Point_2,int>(p,i));
        }

        // construct triangulation
        Triangulation t;
        t.insert(pts.begin(), pts.end());

        // compute the minimum distance for each point
        for (Edge_iterator e = t.finite_edges_begin(); e != t.finite_edges_end(); ++e)
        {
            K::FT squared_d = t.segment(e).squared_length()/4;
            double d = CGAL::to_double(sqrt(squared_d));
            int source = pointmap[t.segment(e).source()];
            int target = pointmap[t.segment(e).target()];            
            if (d < min_ds[source])
                min_ds[source] = d;
            if (d < min_ds[target])
                min_ds[target] = d;
        }
        
        sort(min_ds, min_ds + n);  // Sorts the elements in the range [first, last) in ascending order
        int f, m, l;
        // compute f, m, l
        if (min_ds[0] < 0.5)
            f = 0;
        else
            f = ceil(sqrt(min_ds[0] - 0.5));
        int median = floor(n/2.0);
        if (min_ds[median] < 0.5)
            m = 0;
        else
            m = ceil(sqrt(min_ds[median] - 0.5));
        if (min_ds[n-1] < 0.5)
            l = 0;
        else
            l = ceil(sqrt(min_ds[n-1] - 0.5));
        cout << f << ' ' << m << ' ' << l << endl;
        delete[] min_ds;
    }
}