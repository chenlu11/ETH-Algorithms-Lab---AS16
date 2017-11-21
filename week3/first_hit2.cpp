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
    S seg_min;
    double r, s, t, u;
    vector<S> segment(n);
    for (int i = 0; i < n; ++i)
    {     
      cin >> r >> s >> t >> u;
      segment[i] = S(P(r,s), P(t,u));
    }
    random_shuffle(segment.begin(), segment.end());
    for (int i = 0; i < n; ++i)
    {
      if ((!isblocked && CGAL::do_intersect(ray,segment[i])) || (isblocked && CGAL::do_intersect(seg_min,segment[i])))
      {
        auto o = (!isblocked) ? CGAL::intersection(ray,segment[i]) : CGAL::intersection(seg_min,segment[i]);
        if (const P* op = boost::get<P>(&*o))
        {
          P intersect = *op;
          seg_min = S(start, intersect);
        }
        else if (const S* os = boost::get<S>(&*o))
        {
          P intersect;
          if (CGAL::collinear_are_ordered_along_line(start, os->source(), os->target()))
            intersect = os->source();
          else
            intersect = os->target();
          seg_min = S(start, intersect);
        }
        isblocked = true;
      }
    }
    if (isblocked == true)
      cout << setiosflags(ios::fixed) << setprecision(0) << floor_to_double(seg_min.target().x()) << " " << floor_to_double(seg_min.target().y()) << endl; 
    else
      cout << "no" << endl; 
  }
  return 0;
}
