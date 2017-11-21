// correct
// bitmask dp: dp[root-i][bitmask]
#include <array>
#include <iostream>
#include <map>
#include <vector>

struct center {
    
    center(int n) {
        sup = std::vector<uint16_t>(n, 0);
    }
    
    std::vector<uint16_t> sup;
    std::map<uint16_t, std::map<int, int> > extsup;
    std::map<int, int> bestmap;
};

int best(std::vector<center>& centers, int c, int s, int mask) {
    auto bestit = centers[c].bestmap.find(mask);
    if (bestit != centers[c].bestmap.end()) {
        return (*bestit).second;
    }
    int max = 0;
    for (int i = 0; i < (0x1 << s); i++) {
        if ((i & mask) != 0) {
            continue;
        }
        bool valid = true;
        for (int j = 0; j < s && valid; j++) {
            if ((i & (0x1 << j)) && ((i & centers[c].sup[j]) != 0)) {
                valid = false;
            }
        }
        if (!valid) {
            continue;
        }
        int count = 0;
        for (int j = 0; j < s; j++) {
            if (i & (0x1 << j)) {
                count++;
            }
        }
        for (auto& esup : centers[c].extsup) {
            int nmask = 0;
            for (auto& p : esup.second) {
                if (i & (0x1 << p.first)) {
                    nmask |= p.second;
                }
            }
            count += best(centers, esup.first, s, nmask);
        }
        max = std::max(max, count);
    }
    centers[c].bestmap[mask] = max;
    return max;
}

int main(int argc, char* args[])
{
    int t;
    std::cin >> t;
    while (t--) {
        int k, s, m;
        std::cin >> k >> s >> m;
        std::vector<center> centers(k, center(s));
        for (int i = 0; i < m; i++) {
            int u, v, h;
            std::cin >> u >> v >> h;
            if (u == v) {
                for (int j = 0; j < h; j++) {
                    int x, y;
                    std::cin >> x >> y;
                    centers[u].sup[x] |= 0x1 << y;
                }
            }
            else {
                std::map<int, int> esup;
                for (int j = 0; j < h; j++) {
                    int x, y;
                    std::cin >> x >> y;
                    esup[x] |= 0x1 << y;
                }
                centers[u].extsup[v] = esup;
            }
        }
        std::cout << best(centers, 0, s, 0) << std::endl;
    }
}