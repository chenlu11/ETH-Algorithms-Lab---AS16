#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

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

        K::FT squared_d_min = DBL_MAX;
        for (Edge_iterator e = t.finite_edges_begin(); e != t.finite_edges_end(); ++e)
        {
            K::FT squared_d = t.segment(e).squared_length();
            if (squared_d < squared_d_min)
                squared_d_min = squared_d;
        }

        // time(in 1/100s) = d / 2 * 100 = 50 * d = sqrt(50^2 * squared_d)
        double output_time = ceil(CGAL::to_double(sqrt(50 * 50 * squared_d_min)));
        std::cout << std::setiosflags(std::ios::fixed) << std::setprecision(0) << output_time << std::endl;
    }
}
