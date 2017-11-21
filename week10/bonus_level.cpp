// Observation: for getting an optimal results, they must not intersect (except in the leftmost upper corner and rightmost bottom corner)
// So for each row y (except first and last), the x coordinates of the lines (x1[y] and x2[y]) will be : x1[y] < x2[y]
// O(n^5): row, i, j, k, l
// Result: ok ok tl

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

        vector<vector<vector<int> > > max_coins(n, vector<vector<int> >(n, vector<int>(n, 0)));
        // row = 0
        max_coins[0][0][0] = a[0][0];
        for (int i = 1; i < n; i++)
            max_coins[0][i][i] = max_coins[0][i-1][i-1] + a[0][i];
        for (int i = 0; i < n - 1; i++)
            for (int j = i + 1; j < n; j++) {
                max_coins[0][i][j] = max_coins[0][i][j-1] + a[0][j];
                // cout << "row 0: max_coins[" << i << "][" << j << "] = " << max_coins[0][i][j] << endl;
            }

        // row = 1 .. n-1
        for (int row = 1; row < n; row++)
        {
            // move down
            for (int i = 0; i < n - 1; i++)
                for (int j = i + 1; j < n; j++) {
                    max_coins[row][i][j] = max_coins[row - 1][i][j] + a[row][i] + a[row][j];
                    // cout << "row " << row << ": max_coins[" << i << "][" << j << "] = " << max_coins[row][i][j] << endl;
                }

            // move right
            vector<vector<int> > max_coins_new(n, vector<int>(n, 0));
            for (int i = 0; i < n - 1; i++) {
                for (int j = i + 1; j < n; j++) {
                    // determine max_coins[row][i][j]
                    int sum1 = 0;
                    for (int m = 0; m <= i; m++)
                        sum1 += a[row][m];
                    for (int k = 0; k <= i; k++) {
                        sum1 -= a[row][k];
                        int sum2 = 0;
                        for (int m = i + 1; m <= j; m++)
                            sum2 += a[row][m];
                        for (int l = i + 1; l <= j; l++) {
                            sum2 -= a[row][l];
                            max_coins_new[i][j] = max(max_coins_new[i][j], max_coins[row][k][l] + sum1 + sum2);
                        }
                    }
                    // cout << "row " << row << ": max_coins[" << i << "][" << j << "] = " << max_coins_new[i][j] << endl;
                }
            }
            for (int i = 0; i < n - 1; i++)
                for (int j = i + 1; j < n; j++)
                    max_coins[row][i][j] = max_coins_new[i][j];
        }

        cout << max_coins[n-1][n-2][n-1] << '\n';
        
    }
}