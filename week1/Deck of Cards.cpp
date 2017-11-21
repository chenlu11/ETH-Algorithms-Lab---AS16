// Deck of Cards

#include <iostream>
#include <vector>
using namespace std;

int main()
{
    int t, n, k, i, j, sum, min, min_i, min_j;
    cin >> t;
    for (int x = 0; x < t; x++)
    {
        cin >> n >> k;
        vector<int> v(n);
        for (int y = 0; y < n; y++)
            cin >> v[y];
        i = 0;
        j = 0;
        sum = v[0];
        min = abs(k - sum);
        min_i = i;
        min_j = j;
        while (i < n)
        {
            if (abs(k - sum) < min)
            {
                min = abs(k - sum);
                min_i = i;
                min_j = j;
            }
            if (sum == k)
                break;
            else if (sum < k && j < n - 1)
            {
                j++;
                sum += v[j];
            }
            else
            {
                sum -= v[i];
                i++;
            }
        }
        cout << min_i << ' ' << min_j << endl;
    }
    return 0;
}