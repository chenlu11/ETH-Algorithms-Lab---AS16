// False Coin

#include <iostream>
using namespace std;

int main()
{
	ios_base::sync_with_stdio(false);
	int t, n, k, p;
	char c;

	cin >> t;
	for (int i = 0; i < t; i++)
	{
		cin >> n >> k;
		int* equalFlag = new int[n + 1]();
		int* lessFlag = new int[n + 1]();
		int* greaterFlag = new int[n + 1]();
		int* leftNum = new int[n]();
		int* rightNum = new int[n]();
		for (int j = 0; j < k; j++)
		{
			cin >> p;
			for (int l = 0; l < p; l++)
				cin >> leftNum[l];
			for (int l = 0; l < p; l++)
				cin >> rightNum[l];
			cin >> c;
			if (c == '=')
			{
				for (int l = 0; l < p; l++)
				{
					equalFlag[leftNum[l]] = 1;
					equalFlag[rightNum[l]] = 1;
				}
			}
			else if (c == '<')
			{
				for (int l = 0; l < p; l++)
				{
					lessFlag[leftNum[l]]++;
					greaterFlag[rightNum[l]]++;
				}
			}
			else
			{
				for (int l = 0; l < p; l++)
				{
					greaterFlag[leftNum[l]]++;
					lessFlag[rightNum[l]]++;
				}
			}
		}
		for (int l = 1; l <= n; l++)
			if (greaterFlag[l] > 0 && lessFlag[l] > 0)
				equalFlag[l] = 1;
		int maxTime = -1;
		int maxNum = 0;
		int falseNum;
		for (int l = 1; l <= n; l++)
		{
			if (equalFlag[l] == 0)
			{
				if (greaterFlag[l] > maxTime || lessFlag[l] > maxTime)
				{
					maxTime = (greaterFlag[l] > lessFlag[l]) ? (greaterFlag[l]) : (lessFlag[l]);
					falseNum = l;
					maxNum = 1;
				}
				else if(greaterFlag[l] == maxTime || lessFlag[l] == maxTime)
				{
					maxNum++;
				}
			}
		}
		if (maxNum == 1)
			cout << falseNum << endl;
		else
			cout << 0 << endl;
		delete [] equalFlag;
		delete [] lessFlag;
		delete [] greaterFlag;
		delete [] leftNum;
		delete [] rightNum;
	}
	return 0;
}