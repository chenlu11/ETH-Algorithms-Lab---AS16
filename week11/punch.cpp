#include <iostream>
#include <climits>
#include <vector>
#include <cmath>

using namespace std;

void punch() {

    int n, k;
    cin >> n >> k;

    vector<int> c(n);
    vector<int> v(n);
    for (int i = 0; i < n; i++) {
        cin >> c[i] >> v[i];
    }

    // f[i] : min cost for at least volume i
    vector<int> f(k + 1, INT_MAX); 
    // g[i]: max number of distinct beverages in a cheapest punch for at least volume i
    vector<int> g(k + 1, 0);

    // buy only beverage 0
    f[0] = 0;
    for (int j = 1; j <= k; j++) {
        f[j] = f[max(0, j - v[0])] + c[0];
        g[j] = 1;
    }

    for (int i = 1; i < n; i++) {
        vector<bool> choose(k + 1, false); 
        for (int j = 0; j <= k; j++) {
            int before = max(0, j - v[i]);
            if (f[j] > f[before] + c[i]) {
                f[j] = f[before] + c[i];
                g[j] = g[before] + !choose[before];
                choose[j] = true;
            } else if (f[j] == f[before] + c[i]) {
                if (g[j] < g[before] + !choose[before]) {
                    g[j] = g[before] + !choose[before];
                    choose[j] = true;
                }
            } 
        }
    }

    cout << f[k] << " " << g[k] << endl;
}

int main(void) {
    ios_base::sync_with_stdio(false);
    int T;
    cin >> T;
    for (int t = 0; t < T; t++) {
        punch();
    }
    return 0;
}