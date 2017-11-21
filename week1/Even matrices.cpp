// Even matrices

#include <iostream>
using namespace std;

int main()
{
	ios_base::sync_with_stdio(false);
	int t, n, x, count, oddsumCount, evensumCount;
	cin >> t;
	for (int i = 1; i <= t; i++)
	{
		cin >> n;
		int** sum = new int*[n+1];
		for (int j = 0; j <= n; j++)
			sum[j] = new int[n+1];
		for (int j = 0; j <= n; j++)
		{
			sum[0][j] = 0;
			sum[j][0] = 0;
		}

		for (int j = 1; j <= n; j++)
		{
			for (int k = 1; k <= n; k++)
			{
				cin >> x;
				sum[j][k] = sum[j][k-1] + x;
			}
			for (int k = 1; k <= n; k++)
				sum[j][k] += sum[j-1][k];
		}

		count = 0;
		for (int j = 0; j < n; j++)
		{
			for (int k = j + 1; k <= n; k++)
			{
				oddsumCount = 0;
				evensumCount = 1;
				for (int m = 1; m <= n; m++)
				{
					if ((sum[m][k] - sum[m][j]) % 2 == 0)
						evensumCount++;
					else
						oddsumCount++;
				}
				count += oddsumCount * (oddsumCount - 1) / 2 + evensumCount * (evensumCount - 1) / 2;
			}
		}

		cout << count << endl;
		for (int j = 0; j <= n; j++)
			delete [] sum[j];
		delete [] sum;
	}
	return 0;
}