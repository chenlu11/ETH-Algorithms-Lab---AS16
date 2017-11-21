#include <iostream>
#include <vector>
using namespace std;

int count_difference_bits(int bulb, int pattern, int k)
{
    int y = bulb ^ pattern; // xor
    int count = 0;
    for (int i = 0; i < k; i++) // check i-th element
    {
        if ((y & 1 << i) != 0)
            count++;
    }
    return count;
}

int main()
{
    ios_base::sync_with_stdio(false);
    int t;
    for (cin >> t; t > 0; t--)
    {
        int n, k, x, m;
        cin >> n >> k >> x;
        m = n / k;
        vector<int> state(m), diff(m);
        for (int i = 0; i < m; i++)
        {
            for (int j = 0; j < k; j++)
            {
                int bit; cin >> bit;
                state[i] = (state[i] << 1) + bit;
            }
            diff[i] = count_difference_bits(state[i], x, k);
        }

        vector<int> single_min(m), flipping_min(m);
        single_min[0] = diff[0];
        flipping_min[0] = 1 + (k - diff[0]);        
        for (int i = 1; i < m; i++)
        {
            single_min[i] = min(single_min[i-1] + diff[i], flipping_min[i-1] + diff[i]);
            flipping_min[i] = min(flipping_min[i-1] + (k - diff[i]), single_min[i-1] + 2 + (k - diff[i]));
        }
        cout << min(single_min[m-1], flipping_min[m-1]) << '\n';

    }
    return 0;
}