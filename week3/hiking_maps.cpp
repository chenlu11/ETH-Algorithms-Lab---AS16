#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <iostream>
#include <stdexcept>
#include <iomanip>
#include <vector>
#include <queue>
using namespace std;

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::Point_2 P;
typedef K::Ray_2 R;

typedef pair<int, int> Pair;
typedef priority_queue<Pair> MaxQueue;
typedef priority_queue<Pair, vector<Pair>, greater<Pair> > MinQueue;

void check_intersection(vector<P>& map, int a, int b, int c, int d)
{
    // Make sure ray P_a,P_b intersect with ray P_c,P_d
    if (CGAL::do_intersect(R(map[a], map[b]), R(map[c], map[d]))) {}
    else if (CGAL::do_intersect(R(map[a], map[b]), R(map[d], map[c]))) 
        swap(map[d], map[c]);
    else if (CGAL::do_intersect(R(map[b], map[a]), R(map[c], map[d]))) 
        swap(map[a], map[b]);
    else
    {
        swap(map[a], map[b]);
        swap(map[d], map[c]);
    }
}

void calculate_coverage(int id, vector<P>& map, vector<P>& path, vector<vector<int> >& position)
{
    // First determine the order of 6 points
    // Make sure ray P_0,P_1 intersect with ray P_3,P_2
    check_intersection(map, 0, 1, 3, 2);
    // Make sure ray P_1,P_0 intersect with ray P_4,P_5
    check_intersection(map, 1, 0, 4, 5);

    auto o = CGAL::intersection(R(map[0], map[1]), R(map[3], map[2]));
    if (const P* op = boost::get<P>(&*o))
    {
        P intersect = *op;
        if (!CGAL::left_turn(map[1], intersect, map[2]))
        {
            swap(map[0], map[5]);
            swap(map[1], map[4]);
            swap(map[2], map[3]);
        }
    }

    // Calculate coverage over P_i
    vector<bool> if_cover(path.size());
    for (int i = 0; i < path.size(); i++)
    {
        if_cover[i] = !( CGAL::right_turn(map[0], map[1], path[i]) 
                      || CGAL::right_turn(map[2], map[3], path[i])
                      || CGAL::right_turn(map[4], map[5], path[i]));


        /*            
        bool left_turn1 = CGAL::left_turn(map[0], map[1], path[i]) == if_left_turn;
        bool left_turn2 = CGAL::left_turn(map[2], map[3], path[i]) == if_left_turn;
        bool left_turn3 = CGAL::left_turn(map[4], map[5], path[i]) == if_left_turn;
        if (left_turn1 && left_turn2 && left_turn3)
        {
            if_cover[i] = true;
            continue;
        }
        bool collinear1 = CGAL::collinear(map[0], map[1], path[i]);
        if (collinear1 && left_turn2 && left_turn3)
        {
            if_cover[i] = true;
            continue;
        } 
        bool collinear2 = CGAL::collinear(map[2], map[3], path[i]);
        if (collinear2 && left_turn1 && left_turn3)
        {
            if_cover[i] = true;
            continue;
        } 
        bool collinear3 = CGAL::collinear(map[4], map[5], path[i]);
        if (collinear3 && left_turn1 && left_turn2)
        {
            if_cover[i] = true;
            continue;
        }
        if ((collinear1 && collinear2) || (collinear1 && collinear3) || (collinear2 && collinear3))
        {
            if_cover[i] = true;
            continue;
        }
        */
    }
    // Calculate coverage over E_i
    // If map covers P_i and P_(i+1), then it covers E_i
    for (int i = 0; i < path.size() - 1; i++)
    {
        if (if_cover[i] && if_cover[i + 1])
            position[i].push_back(id);
    }
}

int main()
{
    ios_base::sync_with_stdio(false);
    int c; cin >> c;
    for (; c > 0; c--)
    {
        int m, n;
        cin >> m >> n;
        vector<P> path(m);
        for (int j = 0; j < m; j++)
        {
            int x, y;
            cin >> x >> y;
            path[j] = P(x, y);
        }
        
        vector<vector<int> > position(m - 1); 
        //position[i] stands for which maps cover edge i (i = 0...m-2)
        for (int j = 0; j < n; j++)
        {
            vector<P> map(6);
            for (int k = 0; k < 6; k++)
            {
                int x, y;
                cin >> x >> y;
                map[k] = P(x, y);
            }
            calculate_coverage(j, map, path, position);
        }

        // Reference to search_snippets.cpp
        MaxQueue maxpq;
        MinQueue minpq;
        vector<int> selected(m - 1);
        for (int i = 0; i < m - 1; i++)
        {
            maxpq.push(make_pair(position[i][0], i));
            minpq.push(make_pair(position[i][0], i));
        }
        int min_interval = maxpq.top().first - minpq.top().first + 1;
        while (true)
        {
            int i = minpq.top().second;
            minpq.pop();
            if (selected[i] == position[i].size() - 1)
                break;
            else
            {
                selected[i]++;
                minpq.push(make_pair(position[i][selected[i]], i));
                maxpq.push(make_pair(position[i][selected[i]], i));
                int interval = maxpq.top().first - minpq.top().first + 1;
                min_interval = min(min_interval, interval);
            }
        }
        cout << min_interval << '\n';
    }
    return 0;
}