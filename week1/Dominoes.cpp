// Dominoes

#include <iostream>
using namespace std;

int main()
{
	ios_base::sync_with_stdio(false);
	int N, n, h, max;
	cin >> N;
	for(int i = 0; i < N; i++)
	{
		cin >> n;
		max = 0;
		int j;
		for(j = 1; j <= n; j++)
		{
			cin >> h;
			if (j + h > max)
				max = j + h;
			if (j + 1 >= max)
				break;
		}
		if (j < n)
		{
			cout << j << endl;
			for(j++; j <= n; j++)
				cin >> h;
		}
		else
			cout << n << endl;
	}
	return 0;
}