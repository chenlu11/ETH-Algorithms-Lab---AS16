#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int main()
{
    ios_base::sync_with_stdio(false);
    int t; cin >> t;
    for (int i = 0; i < t; i++)
    {
        int n, m; cin >> n >> m;
        vector<int> s(n); 
        vector<int> w(m);
        for (int j = 0; j < n; j++)
            cin >> s[j];
        for (int j = 0; j < m; j++)
            cin >> w[j];
        sort(s.begin(), s.end(), greater<int>());
        sort(w.begin(), w.end(), greater<int>());
        // Sorts the elements in the range [first, last) in ascending order. The order of equal elements is not guaranteed to be preserved.
        if (s[0] < w[0])
        {
            cout << "impossible" << endl;
            continue;
        }
        int min_count = 0;
        for (int j = 0, k = 0; j < m && k < n; j++)
        {
            while (k < n - 1 && s[k + 1] >= w[j])
                k++;
            if ((j + 1) % (k + 1) == 0)
                min_count = max(min_count, (j + 1) / (k + 1));
            else
                min_count = max(min_count, (j + 1) / (k + 1) + 1);
        }
        cout << min_count * 3 - 1 << endl;
    }
    return 0;
}