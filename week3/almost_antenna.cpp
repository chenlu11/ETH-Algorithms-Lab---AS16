#include <CGAL/Exact_predicates_exact_constructions_kernel_with_sqrt.h>
#include <CGAL/Min_circle_2.h>
#include <CGAL/Min_circle_2_traits_2.h>
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

typedef  CGAL::Exact_predicates_exact_constructions_kernel_with_sqrt K;
typedef  CGAL::Min_circle_2_traits_2<K> Traits;
typedef  CGAL::Min_circle_2<Traits> Min_circle;
typedef  K::Point_2 Point;


double ceil_to_double(const K::FT& x)
{
  double a = ceil(CGAL::to_double(x));
  while (a < x) a += 1;
  while (a-1 >= x) a -= 1;
  return a;
}

int main()
{
  ios_base::sync_with_stdio(false);
  cout.setf(ios::fixed);
  while (true)
  {
    int n; cin >> n;
    if (n == 0)
      break;
    vector<Point> pts(n);
    for (int i = 0; i < n; ++i)
    {
      double x, y;
      cin >> x >> y;
      pts[i] = Point(x,y);
    }

    Min_circle mc(pts.begin(), pts.end(), true);
    Traits::Circle c = mc.circle();
    K::FT min_squared_radius = c.squared_radius();

    /* //Iterate over all points: slow!
    for (int i = 0; i < n; ++i)
    {
      Point p = pts.front();
      pts.erase(pts.begin());
      Min_circle mc(pts.begin(), pts.end(), true);
      Traits::Circle c = mc.circle();
      K::FT squared_radius = c.squared_radius();
      if (squared_radius < min_squared_radius)
        min_squared_radius = squared_radius;
      pts.push_back(p);
    }*/

    for(Min_circle::Support_point_iterator it = mc.support_points_begin(); it != mc.support_points_end(); ++it)
    {
      // InputIt find( InputIt first, InputIt last, const T& value ); Defined in header <algorithm>
      vector<Point>::iterator support_point_it = find(pts.begin(), pts.end(), *it);
      // Important: You should keep the following line, otherwise there will be a error
      Point support_point = *support_point_it;
      pts.erase(support_point_it);
      Min_circle mc_new(pts.begin(), pts.end(), true);
      Traits::Circle c_new = mc_new.circle();
      K::FT squared_radius = c_new.squared_radius();
      if (squared_radius < min_squared_radius)
        min_squared_radius = squared_radius;
      pts.push_back(support_point);
    }
    cout << setprecision(0) << ceil_to_double(sqrt(min_squared_radius)) << endl;
  }
}