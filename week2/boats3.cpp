// Another way to use priority_queue
// Result: correct
#include <iostream>
#include <queue>
using namespace std;

struct Boat 
{
    int length;
    int ring_pos;
    int min_end;
    Boat(int length, int ring_pos): length(length), ring_pos(ring_pos), min_end(ring_pos) {}
    bool operator< (const Boat& other) const
    {
        if (min_end == other.min_end)
            return (ring_pos > other.ring_pos);
        return (min_end > other.min_end);
    }
};

int main()
{
    ios_base::sync_with_stdio(false);
    int t;
    cin >> t;
    for (int i = 0; i < t; i++)
    {
        int N;
        cin >> N;
        priority_queue<Boat> boatpq;
        for (int j = 0; j < N; j++)
        {
            int l, p;
            cin >> l >> p;
            boatpq.push(Boat(l, p));
        }

        int counter = 1;
        int end = boatpq.top().ring_pos;
        boatpq.pop();
        while (!boatpq.empty())
        {
            Boat boat = boatpq.top();
            boatpq.pop();
            if (end + boat.length <= boat.min_end)
            {
                counter++;
                end = boat.min_end;
            }
            else if (boat.ring_pos >= end)
            {
                boat.min_end = end + boat.length;
                boatpq.push(boat);
            }
        }
        cout << counter << endl;
    }
    return 0;
}