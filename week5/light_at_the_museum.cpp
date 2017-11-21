// Result: correct
#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <climits>

using namespace std;

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
        vector<vector<vector<int> > > brightness(n, vector<vector<int> >(m, vector<int>(2)));
        for (int i = 0; i < n; i++)
            for (int j = 0; j < m; j++)
                cin >> brightness[i][j][0] >> brightness[i][j][1];
            
        // use the trick: Split and List
        int stateNum1 = 1 << (n / 2);
        map<vector<int>, int> part1;
        for (int s = 0; s < stateNum1; s++) 
        {
            int temp = s, flicking = 0;
            vector<int> tempBrightness(m, 0);
            for (int i = 0; i < n / 2; i++) 
            {
                if (temp % 2 == 1)
                    flicking++;
                for (int j = 0; j < m; j++)
                    tempBrightness[j] += brightness[i][j][temp % 2];
                temp /= 2;
            }
            bool flag = true;
            for (int i = 0; i < m; i++)
                if (tempBrightness[i] > target[i]) {
                    flag = false;
                    break;
                }
            if (flag) {
                if (part1.find(tempBrightness) == part1.end())
                    part1[tempBrightness] = flicking;
                else
                    part1[tempBrightness] = min(part1[tempBrightness], flicking);
            }
        }

        int stateNum2 = 1 << (n - n / 2);
        int min_flicking = INT_MAX;
        for (int s = 0; s < stateNum2; s++) 
        {
            int temp = s, flicking = 0;
            vector<int> tempBrightness(m, 0);
            for (int i = n / 2; i < n; i++) 
            {
                if (temp % 2 == 1)
                    flicking++;
                for (int j = 0; j < m; j++)
                    tempBrightness[j] += brightness[i][j][temp % 2];
                temp /= 2;
            }

            for (int i = 0; i < m; i++)
                tempBrightness[i] = target[i] - tempBrightness[i];
            map<vector<int>, int>::iterator iter = part1.find(tempBrightness);
            if (iter != part1.end())
                min_flicking = min(min_flicking, flicking + iter->second);
        }
        
        if (min_flicking == INT_MAX)
            cout << "impossible" << '\n';
        else
            cout << min_flicking << '\n';
    }
    return 0;
}