// Brute Force
// Result: tl tl tl ok
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int find_max_independent_set(int k, int &n, vector<int> &selection, vector<vector<int> > &adjacency_list)
{
    if (k == n) {
        return count(selection.begin(), selection.end(), 1);
    }

    // stormtrooper k selected
    selection[k] = 1;
    vector<int>::iterator iter = adjacency_list[k].begin();
    vector<int> record;
    while (iter != adjacency_list[k].end() && *iter < k)
        iter++;
    while (iter != adjacency_list[k].end()) {
        if (selection[*iter] == -1) {
            selection[*iter] = 0;
            record.push_back(*iter);
        }
        iter++;
    }   
    int next_k = k + 1;
    while (next_k < n && selection[next_k] != -1)
        next_k++;
    int max_independent_set = find_max_independent_set(next_k, n, selection, adjacency_list);
    for (int i = 0; i < record.size(); i++)
        selection[record[i]] = -1;

    // stormtrooper k not selected
    selection[k] = 0;
    next_k = k + 1;
    while (next_k < n && selection[next_k] != -1)
        next_k++;
    max_independent_set = max(max_independent_set, find_max_independent_set(next_k, n, selection, adjacency_list));
    selection[k] = -1;
    return max_independent_set;
}

int main() 
{
    ios_base::sync_with_stdio(false);
    int t; cin >> t;
    while(t--) {
        int k, s, m;
        cin >> k >> s >> m;
        int n = k * s;
        vector<vector<int> > adjacency_list(n);

        for (int i = 0; i < m; i++) {
            int u, v, h;
            cin >> u >> v >> h;
            for (int j = 0; j < h; j++) {
                int x, y;
                cin >> x >> y;
                adjacency_list[u * s + x].push_back(v * s + y);
                adjacency_list[v * s + y].push_back(u * s + x);
            }
        }

        for (int i = 0; i < n; i++)
            sort(adjacency_list[i].begin(), adjacency_list[i].end());
        vector<int> selection(n, -1); // 1: selected  0: not selected  -1: not decided
        int max_independent_set = find_max_independent_set(0, n, selection, adjacency_list);
        cout << max_independent_set << '\n';
    }
    return 0;
}