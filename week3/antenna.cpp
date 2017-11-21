#include <CGAL/Exact_predicates_exact_constructions_kernel_with_sqrt.h>
#include <CGAL/Min_circle_2.h>
#include <CGAL/Min_circle_2_traits_2.h>
#include <iostream>
using namespace std;

typedef  CGAL::Exact_predicates_exact_constructions_kernel_with_sqrt K;
typedef  CGAL::Min_circle_2_traits_2<K>  Traits;
typedef  CGAL::Min_circle_2<Traits>      Min_circle;

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
    K::Point_2* P = new K::Point_2[n];
    for (int i = 0; i < n; ++i)
    {
      double x, y;
      cin >> x >> y;
      P[i] = K::Point_2(x,y);
    }
    Min_circle mc(P, P+n, true);
    Traits::Circle c = mc.circle();
    cout << setprecision(0) << ceil_to_double(sqrt(c.squared_radius())) << endl;
    delete[] P;
  }
}