#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
using namespace std;

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;
typedef K::Point_2 P;

int main()
{
    ios_base::sync_with_stdio(false);
    int T; cin >> T;
    while (T--)
    {
        int m, n, h;
        cin >> m >> n;
        vector<P> participants_pos(m);
        vector<int> participants_radius(m);
        vector<P> lamps(n);
        for (int i = 0; i < m; i++)
            cin >> participants_pos[i] >> participants_radius[i];
        cin >> h;
        for (int i = 0; i < n; i++)
            cin >> lamps[i];

        Triangulation t;
        t.insert(lamps.begin(), lamps.end());

        bool survive_to_last = false;
        for (int i = 0; i < m; i++)
        {
            Triangulation::Vertex_handle nearest = t.nearest_vertex(participants_pos[i]);
            K::FT squared_d = CGAL::squared_distance(participants_pos[i], nearest->point());
            if (CGAL::to_double(squared_d) >= pow(participants_radius[i] + h, 2))
            {
                cout << i << ' ';
                survive_to_last = true;
            }
        }
        if (!survive_to_last)
        {
            vector<int> survive_time(m, 0);
            for (int i = 0; i < m; i++)
            {
                double squared_d = pow(participants_radius[i] + h, 2);
                for (int j = 0; j < n; j++)
                    if(CGAL::to_double(CGAL::squared_distance(participants_pos[i], lamps[j])) >= squared_d)
                        survive_time[i] = j + 1;
                    else
                        break;
            }
            int max_survive_time = *max_element(survive_time.begin(), survive_time.end());
            for (int i = 0; i < m; i++)
                if (survive_time[i] == max_survive_time)
                    cout << i << ' ';
        }
        cout << '\n';
    }
}