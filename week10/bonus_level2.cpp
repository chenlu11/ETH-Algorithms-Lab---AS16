// Observation: If we move along paths at the same time, we are always on the same diagonal
// Therefore, the tokens can only intersect at the same time
// O(n^3): diagonal, i, j
// Result: ok ok ok

#include <iostream>
#include <vector>
using namespace std;

int main()
{
    int t; cin >> t;
    while (t--)
    {
        int n; cin >> n;
        vector<vector<int> > a(n, vector<int>(n));
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
                cin >> a[i][j];

        vector<vector<vector<int> > > max_coins(2 * n - 1, vector<vector<int> >(n, vector<int>(n, 0)));
        // diagonal = 0
        max_coins[0][0][0] = a[0][0];
        
        // diagonal = 1 .. 2(n-1) = row + col
        for (int d = 1; d < 2 * n - 1; d++)
        {
            for (int row1 = max(0, d - n + 1); row1 <= min(d, n - 1); row1++) 
            {
                for (int row2 = max(0, d - n + 1); row2 <= min(d, n - 1); row2++) 
                {
                    int col1 = d - row1;
                    int col2 = d - row2;
                    int coins1 = (col1 != 0 && col2 != 0) ? max_coins[d-1][row1][row2] : 0;
                    int coins2 = (row1 != 0 && col2 != 0) ? max_coins[d-1][row1-1][row2] : 0;
                    int coins3 = (col1 != 0 && row2 != 0) ? max_coins[d-1][row1][row2-1] : 0;
                    int coins4 = (row1 != 0 && row2 != 0) ? max_coins[d-1][row1-1][row2-1] : 0;

                    max_coins[d][row1][row2] = a[row1][col1] + (row1 == row2 ? 0 : a[row2][col2])
                            + max(max(coins1, coins2), max(coins3, coins4));
                }
            }
        }

        cout << max_coins[2*n-2][n-1][n-1] << '\n';
        
    }
}