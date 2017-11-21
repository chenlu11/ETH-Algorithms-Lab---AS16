// https://www.topcoder.com/community/data-science/data-science-tutorials/line-sweep-algorithms/
#include <iostream>
#include <vector>
#include <queue>
using namespace std;

typedef pair<int, int> Pair;
typedef priority_queue<Pair> MaxQueue;
typedef priority_queue<Pair, vector<Pair>, greater<Pair> > MinQueue;

int main()
{
    ios_base::sync_with_stdio(false);
    int t; cin >> t;
    for (int testcase = 0; testcase < t; ++testcase)
    {
        int n; cin >> n;
        vector<int> frequency(n);
        vector<vector<int> > position(n);
        vector<int> selected(n);
        MaxQueue maxpq;
        MinQueue minpq;

        for (int i = 0; i < n; ++i)
            cin >> frequency[i];
        for (int i = 0; i < n; ++i)
        {
            for (int j = 0; j < frequency[i]; ++j)
            {
                int pos; cin >> pos;
                position[i].push_back(pos);
            }                
            maxpq.push(make_pair(position[i][0], i));
            minpq.push(make_pair(position[i][0], i));
        }

        int min_interval = maxpq.top().first - minpq.top().first + 1;
        while (true)
        {
            int i = minpq.top().second;
            minpq.pop();
            if (selected[i] == frequency[i] - 1)
                break;
            else
            {
                selected[i]++;
                minpq.push(make_pair(position[i][selected[i]], i));
                maxpq.push(make_pair(position[i][selected[i]], i));
                int interval = maxpq.top().first - minpq.top().first + 1;
                if (interval < min_interval)
                    min_interval = interval;
            }
        }
        cout << min_interval << endl;
    }
    return 0;
}