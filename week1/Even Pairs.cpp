// Even Pairs

#include <iostream>
using namespace std;


int main()
{
	int t, n, x, sum, oddsumCount, evensumCount, count;
	cin >> t;
	for (; t > 0; t--)
	{
		sum = 0;
		oddsumCount = 0;
		evensumCount = 1;
		cin >> n;
		for(int i = 1; i <= n; i++)
		{
			cin >> x;
			sum += x;
			if (sum % 2 == 0)
				evensumCount++;
			else
				oddsumCount++;
		}
		count = oddsumCount * (oddsumCount - 1) / 2 + evensumCount * (evensumCount - 1) / 2;
		cout << count << endl;
	}	
	return 0;
}