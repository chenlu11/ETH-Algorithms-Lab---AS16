#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <queue>

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
        for (int i = 0; i < n; ++i) 
        {
            int x, y;
            std::cin >> x >> y;
            K::Point_2 p(x,y);            
            pts.push_back(p);
        }
        // construct triangulation
        Triangulation t;
        t.insert(pts.begin(), pts.end());

        // read number of available points
        int m;
        std::cin >> m;
        for (int i = 0; i < m; ++i) 
        {
            int x, y;
            double d;
            std::cin >> x >> y >> d;
            K::Point_2 p(x,y);         
            Triangulation::Vertex_handle nearest = t.nearest_vertex(p);
            K::FT squared_d = CGAL::squared_distance(p,nearest->point());
            if (squared_d < d)
                std::cout << 'n';
            else
            {
                Triangulation::Face_handle start = t.locate(p);
                if (t.is_infinite(start))
                    std::cout << 'y';
                else
                {
                    std::queue<Triangulation::Face_handle> visitQueue;
                    std::set<Triangulation::Face_handle> visitedSet;
                    visitQueue.push(start);
                    bool escape = false;
                    while(!visitQueue.empty() && !escape)
                    {
                        Triangulation::Face_handle current = visitQueue.front();
                        visitQueue.pop();
                        if (visitedSet.find(current) == visitedSet.end()) // Not Found
                        {
                            visitedSet.insert(current);
                            for (int j = 0; j < 3; ++j)
                            {
                                Triangulation::Face_handle neighbor = current->neighbor(j);
                                Triangulation::Edge edge(current, j);
                                if (visitedSet.find(neighbor) == visitedSet.end())
                                {
                                    // Check if we can escape through the segment
                                    if (t.segment(edge).squared_length() >= 4 * d)
                                    {
                                        if (t.is_infinite(neighbor))
                                        {
                                            escape = true;
                                            break;
                                        }
                                        else
                                            visitQueue.push(neighbor);
                                    }
                                }                               
                            }
                        }
                    }
                    if (escape == true)
                        std::cout << 'y';
                    else
                        std::cout << 'n';
                }
            }
        }
        std::cout << std::endl;
    }
}