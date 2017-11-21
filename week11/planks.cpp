#include <algorithm>
#include <iostream>
#include <vector>
#include <cmath>
#include <map>

using namespace std;

void planks() {

    int n;
    cin >> n;

    int sum = 0;
    vector<int> len(n);
    for (int i = 0; i < n; ++i) {
        cin >> len[i];
        sum += len[i];
    }
    int max_len = *max_element(len.begin(), len.end());
    if (sum % 4 != 0 || max_len > sum / 4) {
        cout << 0 << endl;
        return;
    }
    sum = sum / 4;

    int stateNum1 = 1 << ((n / 2) * 2);
    map<vector<int>, int> part1;
    for (int s = 0; s < stateNum1; ++s) {
        int temp = s;
        vector<int> tempSum(4, 0);
        for (int i = 0; i < n / 2; ++i) {
            tempSum[temp % 4] += len[i];
            temp /= 4;
        }
        part1[tempSum] += 1;
    }

    int stateNum2 = 1 << ((n - n / 2) * 2);
    long long count = 0;
    for (int s = 0; s < stateNum2; ++s) {
        int temp = s;
        vector<int> tempSum(4, 0);
        for (int i = n / 2; i < n; ++i) {
            tempSum[temp % 4] += len[i];
            temp /= 4;
        }
        for (int i = 0; i < 4; ++i)
            tempSum[i] = sum - tempSum[i];
        map<vector<int>, int>::iterator iter = part1.find(tempSum);
        if (iter != part1.end()) 
            count += iter->second;
    }
    cout << count / 24 << endl;
}

int main(void) {
    ios_base::sync_with_stdio(false);
    int T;
    cin >> T;
    for (int t = 0; t < T; ++t) {
        planks();
    }
    return 0;
}