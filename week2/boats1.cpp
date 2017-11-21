#include <iostream>
#include <vector>
#include <queue>
using namespace std;

typedef pair<double, int> Pair;
typedef priority_queue<Pair, vector<Pair>, greater<Pair> > MinQueue;

int main()
{
    int t; cin >> t;
    for (int i = 0; i < t; i++)
    {
        int n; cin >> n;
        vector<int> l(n), p(n);
        MinQueue minpq;

        for (int j = 0; j < n; j++)
        {
            cin >> l[j] >> p[j];
            minpq.push(make_pair(p[j] + 0.1, j));
            // "+ 0.1" is because when two pairs have the same end coordinate,
            // the one whose end coordinate equals to its ring position should't be chosen.
            // (As illustrated by the test example below)
        }

        int cur_pos = minpq.top().first;
        minpq.pop();
        int count = 1;
        int smallest_coordinate, smallest_i;
        while (!minpq.empty())
        {           
            smallest_coordinate = minpq.top().first;
            smallest_i = minpq.top().second;
            minpq.pop();
            
            if (smallest_coordinate - l[smallest_i] >= cur_pos)
            {
                cur_pos = smallest_coordinate;
                count++;
            }
            else if (p[smallest_i] >= cur_pos)
                minpq.push(make_pair((double)cur_pos + l[smallest_i], smallest_i));
        }
        cout << count << endl;
    }
    return 0;
}
/*
1
10
4 7
2 5
1 9
5 6
1 3
2 10
1 4
3 8
4 1
5 2
*/