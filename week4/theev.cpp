// ok ok tl ok
#include <iostream>
#include <vector>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Min_circle_2.h>
#include <CGAL/Min_circle_2_traits_2.h>

using namespace std;

typedef  CGAL::Exact_predicates_exact_constructions_kernel K;
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
    int t; cin >> t;
    while(t--) {
        int n; cin >> n;
        vector<pair<K::FT, K::Point_2> > distance;
        for (int i = 0; i < n; i++) {
            double x, y;
            cin >> x >> y;
            if (n <= 2)
                continue;
            K::Point_2 P = K::Point_2(x, y);
            if (i == 0)
                distance.push_back(make_pair(0, P));
            else
                distance.push_back(make_pair(CGAL::squared_distance(distance[0].second, P), P));
        }
        if (n <= 2) {
            cout << 0 << endl;
            continue;
        }
        sort(distance.begin(), distance.end());

        vector<K::Point_2> Q;
        int seg = n;
        K::FT min_squared_distance = distance[n - 1].first;
        Min_circle mc;
        while (seg >= 2) {
            seg--;
            mc.insert(distance[seg].second);
            Traits::Circle c = mc.circle();
            K::FT cur_squared_radius = c.squared_radius();
            K::FT cur_squared_distance = max(distance[seg - 1].first, cur_squared_radius);
            min_squared_distance = min(min_squared_distance, cur_squared_distance);
            if (distance[seg - 1].first <= cur_squared_radius)
                break;
        }
        
        cout << setprecision(0) << ceil_to_double(min_squared_distance) << endl;
    }
    return 0;
}