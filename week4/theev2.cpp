// correct
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
        vector<K::Point_2> P(n);
        for (int i = 0; i < n; i++) {
            double x, y;
            cin >> x >> y; 
            P[i] = K::Point_2(x, y);
        }

        if (n <= 2) {
            cout << 0 << endl;
            continue;
        }

        vector<pair<K::FT, int> > distance;
        for (int i = 0; i < n; i++) {
            distance.push_back(make_pair(CGAL::squared_distance(P[0], P[i]), i));
        }
        sort(distance.begin(), distance.end());
        vector<K::Point_2> Q(n);
        for (int i = 0; i < n; i++) {
            Q[i] = P[distance[i].second];
        }

        int left = 1, right = n - 1;   
        while (left < right) {
            int mid = (left + right) / 2;
            Min_circle mc(Q.begin() + mid, Q.end(), true);
            K::FT cur_squared_radius = mc.circle().squared_radius();
            K::FT cur_squared_distance = max(distance[mid - 1].first, cur_squared_radius);
            if (cur_squared_radius <= distance[mid - 1].first)
                right = mid;
            else
                left = mid + 1;
        }
        Min_circle mc(Q.begin() + left - 1, Q.end(), true);
        K::FT squared_radius = mc.circle().squared_radius();
        K::FT squared_distance = distance[left - 1].first;
        K::FT min_squared_distance = min(squared_radius, squared_distance);
        cout << setprecision(0) << ceil_to_double(min_squared_distance) << endl;
    }
    return 0;
}