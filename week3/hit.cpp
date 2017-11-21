#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <iostream>
#include <stdexcept>
using namespace std;

typedef CGAL::Exact_predicates_exact_constructions_kernel K;
typedef K::Point_2 P;
typedef K::Segment_2 S;
typedef K::Ray_2 R;

int main()
{
  while (true)
  {
    int n; cin >> n;
    if (n == 0)
      break;
    double x, y, a, b; 
    cin >> x >> y >> a >> b;
    R ray = R(P(x,y), P(a,b));
    bool isblocked = false;
    double r, s, t, u;
    for (int i = 0; i < n; ++i)
    {     
      cin >> r >> s >> t >> u;
      if (isblocked == true)
        continue;
      S segment = S(P(r,s), P(t,u));
      if (CGAL::do_intersect(ray, segment))
        isblocked = true;     
    }
    if (isblocked == true)
      cout << "yes" << endl;
    else
      cout << "no" << endl; 
  }
  return 0;
}

