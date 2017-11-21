// Result: ok ok tl
#include <iostream>
#include <set>
using namespace std;

int main()
{
	ios_base::sync_with_stdio(false);
	int t, n, m;
	cin >> t;
	for (int i = 0; i < t; i++)
	{
		cin >> n >> m;
		int* p = new int[n + 1]();
		int* q = new int[n + 1]();
		int* humanWound = new int[m + 1]();

		for (int j = 1; j <= n; j++)
		{
			cin >> p[j] >> q[j];
			for (int k = p[j]; k <= q[j]; k++)
				humanWound[k] = 1;
		}

		int allHumanWound = 1;
		for (int j = 1; j <= m; j++)
		{
			if (humanWound[j] == 0)
			{
				allHumanWound = 0;
				break;
			}
		}

		if (allHumanWound == 0)
		{
			cout << 0 << endl;
		}
		else
		{
			int braggingNum = 0;
			for (int j = 1; j <= n; j++)
			{
				if (p[j] == 0)
					continue;
				int braggingFlag = 1;
				for (int k = 1; k <= n; k++)
				{
					if (j != k && p[j] >= p[k] && q[j] <= q[k])
					{
						braggingFlag = 0;
						break;
					}
				}
				if (braggingFlag == 1)
					braggingNum++;
			}
			cout << braggingNum << endl;
		}
		delete[] p;
		delete[] q;
		delete[] humanWound;
	}
	return 0;
}