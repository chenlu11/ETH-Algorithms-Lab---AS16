#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int main()
{
    int t; cin >> t;
    for (int i = 0; i < t; i++)
    {   
        int n; cin >> n;
        vector<int> t(n);
        for (int j = 0; j < n; j++)
            cin >> t[j];
        for (int j = 1; j < n; j++)
            t[j] = min(t[j], t[(j - 1) / 2] - 1);
        sort(t.begin(), t.end());
        bool if_deactivated = true;
        for (int j = 0; j < n; j++)
        {
            if (t[j] < j + 1)
            {
                if_deactivated = false;
                break;
            }
        }
        cout << (if_deactivated ? "yes" : "no") << '\n';
    }
}