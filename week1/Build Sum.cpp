#include <iostream>
using namespace std;


int main()
{
    int t, m;
    double d = 0, sum;
    cin >> t;
    for (int i = 0; i < t; ++i)
    {
        cin >> m;
        sum = 0;
        for (int j = 0; j < m; ++j)
        {
            cin >> d;
            sum += d;
        }
        cout << sum << endl;
    }
    return 0;
}