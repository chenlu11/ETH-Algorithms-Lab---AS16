// Result: correct
#include <iostream>
#include <vector>
#include <queue>
#include <climits>
using namespace std;

struct Point
{
    int pos;
    bool if_start;
    Point(int pos, bool if_start): pos(pos), if_start(if_start) {}
    bool operator<(const Point& other) const {
        return (pos == other.pos) ? !if_start : pos > other.pos;
    }
};

struct Jedi
{
    int start;
    int end;
    Jedi(int start, int end): start(start), end(end) {}
    bool operator< (const Jedi& other) const {
        return end > other.end;
    }
};

int interval_scheduling(priority_queue<Jedi>& jedipq)
{
    if (jedipq.empty())
        return 0;

    int counter = 1;
    int end = jedipq.top().end;
    jedipq.pop();

    while (!jedipq.empty()) {
        Jedi jedi = jedipq.top();
        jedipq.pop();
        if (end < jedi.start) {
            counter++;
            end = jedi.end;
        }
    }
    return counter;
}

int main()
{
    ios_base::sync_with_stdio(false);
    int t;
    cin >> t;
    for (int i = 0; i < t; i++) {
        int n, m;
        cin >> n >> m;
        vector<Jedi> jedis;
        priority_queue<Point> points;
        int cur_collision = 0;

        for (int j = 0; j < n; j++) {
            int a, b;
            cin >> a >> b;
            if (a <= b) {
                jedis.push_back(Jedi(a, b));
            }
            else {
                jedis.push_back(Jedi(a, b + m));
                cur_collision++; // current jedi protects segment 1
            }
            points.push(Point(a, true));
            points.push(Point(b, false));
        }

        // search segment k where count of collisions is minimal
        while (!points.empty() && points.top().pos == 1 && points.top().if_start == true) {
            cur_collision++;
            points.pop();
        }
        int min_collision = cur_collision, min_seg = 1, cur_seg = 1;
        // cout << "update" << min_seg << ' ' << min_collision << endl;
        while(!points.empty()) {
            int pos = points.top().pos;
            bool if_start = points.top().if_start;
            // cout << pos << ' ' << (if_start ? "S" : "E") << '\n';
            points.pop();

            if (pos > cur_seg) {
                if (pos > cur_seg + 1 && cur_collision < min_collision) {
                    min_collision = cur_collision;
                    min_seg = cur_seg + 1;
                    // cout << "update" << min_seg << ' ' << min_collision << endl;
                }
                cur_seg = pos;
            }

            if (if_start) {
                cur_collision++;
            } else {
                if (min_seg < pos && cur_collision < min_collision) {
                    min_collision = cur_collision;
                    min_seg = pos;
                    // cout << "update" << min_seg << ' ' << min_collision << endl;
                }
                cur_collision--;
            }
        }
        // cout << "update" << min_seg << ' ' << min_collision << endl;
        
        priority_queue<Jedi> jedipq;
        // no jedi protects segment min_seg, initial count = 0
        for (int j = 0; j < n; j++) {
            if (jedis[j].end < min_seg)
                jedipq.push(Jedi(jedis[j].start + m, jedis[j].end + m));
            else if (jedis[j].start > min_seg && jedis[j].end < min_seg + m)
                jedipq.push(Jedi(jedis[j].start, jedis[j].end));
        }
        int counter = interval_scheduling(jedipq);

        // one jedi protects segment min_seg, initial count = 1
        for (int j = 0; j < n; j++) {
            // jedi j protects segment min_seg
            if (jedis[j].start <= min_seg && min_seg <= jedis[j].end) {
                for (int k = 0; k < n; k++) {
                    if (jedis[k].end < jedis[j].start)
                        jedipq.push(Jedi(jedis[k].start + m, jedis[k].end + m));
                    else if (jedis[k].start > jedis[j].end && jedis[k].end < jedis[j].start + m)
                        jedipq.push(Jedi(jedis[k].start, jedis[k].end));
                }
                counter = max(counter, 1 + interval_scheduling(jedipq));
            } else if (min_seg + m <= jedis[j].end) {
                for (int k = 0; k < n; k++) {
                    if (jedis[k].start > jedis[j].end - m && jedis[k].end < jedis[j].start)
                        jedipq.push(Jedi(jedis[k].start, jedis[k].end));
                }
                counter = max(counter, 1 + interval_scheduling(jedipq));
            }
        }
        cout << counter << '\n';
    }
    return 0;
}