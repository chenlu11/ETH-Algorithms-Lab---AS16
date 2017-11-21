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
};

class CompareBoat
{
public:
    bool operator()(Boat& x, Boat& y)
    {
    	if (x.min_end == y.min_end)
    		return (x.ring_pos > y.ring_pos);
    	return (x.min_end > y.min_end);
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
		priority_queue<Boat, vector<Boat>, CompareBoat> boatpq;
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