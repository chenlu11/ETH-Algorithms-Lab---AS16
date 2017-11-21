// Result: correct
// sort intervals according to the left end
// make sure all left ends of intervals are distinct
#include <iostream>
#include <algorithm>
#include <vector>
#include <map>
using namespace std;

int main()
{
    ios_base::sync_with_stdio(false);
    int t, n, m;
    cin >> t;
    for (int i = 0; i < t; i++)
    {
        cin >> n >> m;
        vector<int> left;
        map<int, int> right;
        map<int, bool> multi;

        for (int j = 1; j <= n; j++)
        {
            int p, q; cin >> p >> q;
            if (p > 0)
            {
                auto search = right.find(p);
                if (search == right.end())
                {
                    left.push_back(p);
                    right[p] = q;
                    multi[p] = false;
                }
                else if (search->second < q)
                {
                    right[p] = q;
                    multi[p] = false;
                }
                else if (search->second == q)
                {
                    multi[p] = true;
                }
            }
        }

        sort(left.begin(), left.end());

        // Check whether for each human there exists an alien which wounds him 
        // - if not then the answer is 0!
        bool allHumanWound = true;
        int rightmost = 0;
        for(vector<int>::iterator it = left.begin(); it != left.end(); ++it) 
        {
            if (*it > rightmost + 1)
            {
                // human rightmost+1 is not wounded
                allHumanWound = false;
                break;
            }
            else
                rightmost = max(rightmost, right[*it]);
        }

        if (!allHumanWound)
        {
            cout << 0 << endl;
            continue;
        }

        // Find the number of intervals which are not contained in any other interval
        int braggingNum = 0;
        rightmost = 0;
        for(vector<int>::iterator it = left.begin(); it != left.end(); ++it) 
        {
            if (right[*it] > rightmost)
            {
                if (!multi[*it])
                    braggingNum++;
                rightmost = right[*it];
            }
        }
        cout << braggingNum << endl;
    }
}