#include <iostream>
#include <vector>
using namespace std;

int main()
{
    ios_base::sync_with_stdio(false);
    int t;
    for (cin >> t; t > 0; t--)
    {
        int n; cin >> n;
        vector<int> v(n);
        for (int i = 0; i < n; i++)
            cin >> v[i];

        vector<vector<int> > T(n, vector<int>(n));
        for (int i = 0; i < n; i++)
            T[i][i] = v[i];
        for (int i = 0; i < n - 1; i++)
            T[i][i+1] = max(v[i], v[i+1]);
        for (int k = (n % 2 == 0) ? 4 : 3; k <= n; k = k + 2)
        {
            for (int i = 0; i <= n - k; i++)
            {
                int j = i + k - 1;
                T[i][j] = max(v[i] + min(T[i+2][j], T[i+1][j-1]), v[j] + min(T[i][j-2], T[i+1][j-1]));
            }
        }
        cout << T[0][n-1] << '\n';
    }
    return 0;
}