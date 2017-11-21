#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;

int main()
{
    while (true)
    {
        // read number of existing points
        int n;
        std::cin >> n;
        if (n == 0)
            break;
        // read points
        std::vector<K::Point_2> pts;
        pts.reserve(n);
        for (std::size_t i = 0; i < n; ++i) 
        {
            int a, b;
            std::cin >> a >> b;
            K::Point_2 p(a,b);            
            pts.push_back(p);
        }
        // construct triangulation
        Triangulation t;
        t.insert(pts.begin(), pts.end());

        // read number of available points
        int m;
        std::cin >> m;
        for (std::size_t i = 0; i < m; ++i) 
        {
            int a, b;
            std::cin >> a >> b;
            K::Point_2 p(a,b);         
            Triangulation::Vertex_handle nearest = t.nearest_vertex(p);
            K::FT d = CGAL::squared_distance(p,nearest->point());
            std::cout << std::setiosflags(std::ios::fixed) << std::setprecision(0) << CGAL::to_double(d) << std::endl;
        }
    }
}
