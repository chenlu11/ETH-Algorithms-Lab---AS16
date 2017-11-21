// Result: OK TL TL
#include <iostream>
#include <vector>
#include <climits>
using namespace std;

int compute_min_flicking(int k, int count, int n, int m, vector<int> brightness, vector<int> limit_max, vector<int> limit_min, vector<int> &target, vector<vector<int> > &on, vector<vector<int> > &off)
{
    if (k == n)
    {
        bool flag = true;
        for (int i = 0; i < m; i++)
        {
            if (brightness[i] != target[i])
            {
                flag = false;
                break;
            }
        }
        return (flag ? count : INT_MAX);
    }
    bool flag = true;
    for (int i = 0; i < m; i++)
    {
        if (brightness[i] + limit_min[i] > target[i] || brightness[i] + limit_max[i] < target[i])
        {
            flag = false;
            break;
        }
    }
    if (!flag)
        return INT_MAX;

    for (int i = 0; i < m; i++)
    {
        limit_max[i] -= max(on[k][i], off[k][i]);
        limit_min[i] -= min(on[k][i], off[k][i]);
    }
    // keep the initial states of k-th switch
    for (int i = 0; i < m; i++)
        brightness[i] += on[k][i];
    int count1 = compute_min_flicking(k + 1, count, n, m, brightness, limit_max, limit_min, target, on, off);
    for (int i = 0; i < m; i++)
        brightness[i] -= on[k][i];
    // flick k-th switch
    for (int i = 0; i < m; i++)
        brightness[i] += off[k][i];
    int count2 = compute_min_flicking(k + 1, count + 1, n, m, brightness, limit_max, limit_min, target, on, off);
    for (int i = 0; i < m; i++)
    {
        limit_max[i] += max(on[k][i], off[k][i]);
        limit_min[i] += min(on[k][i], off[k][i]);
    }
    return min(count1, count2);
}

int main()
{
    ios_base::sync_with_stdio(false);
    int t;
    for (cin >> t; t > 0; t--)
    {
        int n, m; cin >> n >> m;
        vector<int> target(m);
        for (int i = 0; i < m; i++)
            cin >> target[i];
        vector<vector<int> > on(n, vector<int>(m)), off(n, vector<int>(m));
        vector<int> limit_max(m), limit_min(m);
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < m; j++)
            {
                cin >> on[i][j] >> off[i][j];
                limit_max[j] += max(on[i][j], off[i][j]);
                limit_min[j] += min(on[i][j], off[i][j]);
            }
        }

        vector<int> brightness(m);
        int min_flicking = compute_min_flicking(0, 0, n, m, brightness, limit_max, limit_min, target, on, off);
        if (min_flicking == INT_MAX)
            cout << "impossible" << '\n';
        else
            cout <<  min_flicking << '\n';
    }
    return 0;
}