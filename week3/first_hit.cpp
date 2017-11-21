#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <iostream>
#include <stdexcept>
#include <iomanip>
using namespace std;

typedef CGAL::Exact_predicates_exact_constructions_kernel K;
typedef K::Point_2 P;
typedef K::Segment_2 S;
typedef K::Ray_2 R;

double floor_to_double(const K::FT& x)
{
  double a = floor(CGAL::to_double(x));
  while (a > x) a -= 1;
  while (a+1 <= x) a += 1;
  return a;
}

int main()
{
  ios_base::sync_with_stdio(false);
  while (true)
  {
    int n; cin >> n;
    if (n == 0)
      break;
    double x, y, a, b; 
    cin >> x >> y >> a >> b;
    P start = P(x,y), pass = P(a,b);
    R ray = R(start, pass);
    bool isblocked = false;
    K::FT d_min;
    P intersect_min;
    double r, s, t, u;
    for (int i = 0; i < n; ++i)
    {     
      cin >> r >> s >> t >> u;
      S segment = S(P(r,s), P(t,u));    
      if (CGAL::do_intersect(ray,segment))
      {
        auto o = CGAL::intersection(ray,segment);
        if (const P* op = boost::get<P>(&*o))
        {
          P intersect = *op;
          K::FT d = CGAL::squared_distance(start,intersect);
          if (!isblocked || d < d_min)
          {
            intersect_min = intersect;
            d_min = d;
          }
        }
        else if (const S* os = boost::get<S>(&*o))
        {
          P intersect;
          K::FT d_source = CGAL::squared_distance(start,os->source());
          K::FT d_target = CGAL::squared_distance(start,os->target());
          if (d_source < d_target)
            intersect = os->source();
          else
            intersect = os->target();
          K::FT d = CGAL::squared_distance(start,intersect);
          if (!isblocked || d < d_min)
          {
            intersect_min = intersect;
            d_min = d;
          }
        }
        isblocked = true;
      }
    }
    if (isblocked == true)
      cout << setiosflags(ios::fixed) << setprecision(0) << floor_to_double(intersect_min.x()) << " " << floor_to_double(intersect_min.y()) << endl; 
    else
      cout << "no" << endl; 
  }
  return 0;
}
