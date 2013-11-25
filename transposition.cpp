#include "transposition.h"
#include <iostream>
#include <vector>
#include <string>
using namespace std;

void transposition(string ciphertext)
{
	vector<int> factors = factor(ciphertext.length());
	// If ciphertext length is a prime number...
	if (factors.size() <= 1)
	{
		cout << "Cannot factor ciphertext length: it is a prime number." << endl;
		return;
	}

	// Check for squares and start with those
	int square = 0;
	for (int i = 0; i < factors.size(); i++)
	{
		if (factors[i] * factors[i] == ciphertext.length())
		{
			// Promote the square to the head of the factors list
			square = factors[i];
			factors[0] = square;
			factors[i] = 1;
			break;
		}
	}

	// Try likely remaining factors
	for (int i = 0; i < factors.size(); i++)
	{
		if (factors[i] > 3)
		{
//			cout << "-------------------------------------" << endl;
		    cout << factors[i] << "x" << ciphertext.length() / factors[i] << " grid:" << endl;
			cout << "-------------------------------------" << endl;

			for (int k = 0; k < ciphertext.length() / factors[i]; k++)
			{
				for (int j = 0; j < factors[i]; j++)
					cout << ciphertext[j+factors[i]*k] << " ";
				cout << endl;
			}
			cout << endl;

			// Taking text out by columns forwards
			for (int j = 0; j < factors[i]; j++)
				for (int k = 0; k < ciphertext.length() / factors[i]; k++)
					cout << ciphertext[j+factors[i]*k];
			cout << endl;

			// Taking text out by columns backwards
/*
			int pos = 0;
			for (int j = 0; j < factors[i]; j++)
			{
				for (int k = 0; k <= j; k++)
				{
					cout << ciphertext[pos] << " ";
					pos++;
				}
				cout << endl;
			}
*/
			// Taking text out by diagonals forwards
			int sum = 0;
			for (int l = 0; l < factors[i]; l++)
			{
				sum += l;
				int k = sum;
				for (int j = 1; j <= factors[i] - l; j++)
				{
					cout << ciphertext[k];
					k = k + j + l + 1;
				}
				k--;
				for (int j = 0; j < l; j++)
				{
					cout << ciphertext[k];
					k = k - j + 7;
				}
			}
			cout << endl << endl;

			// Magic squares
			if (square)
			{
				vector<int> ms;
				for (int i = 1; i <= square; i++)
				{
					for (int j = 1; j <= square; j++)
						ms.push_back(square * ((i + j - 1 + square / 2) % square) 
							+ ((i + 2 * j -2) % square) + 1);
				}
				string sorted = ciphertext;
				paraqsort(ms, sorted, 0, square*square-1);
				cout << "Magic squares:" << endl;
				cout << sorted << endl;
				cout << reverse(sorted) << endl;
			}
		}
	}
}

vector<string> makeGrid(int rows, int cols, string ciphertext)
{
	vector<string> grid;
	for (int i = 0; i < ciphertext.length; i+=cols)
		grid.push_back(ciphertext.substr(i, cols));

	return grid;
}

string reverse(string s)
{
	string r = "";
	for (int i = s.length() - 1; i >= 0; i--)
		r += s[i];
	return r;
}

void paraqsort(vector<int> & master, string & slave, int low, int high)
{
	int i = low, j = high;
	int masterTemp;
	char slaveTemp;

	int x = master[(low+high)/2];

	do
	{
		while (master[i] < x) i++;
		while (master[j] > x) j--;
		if (i <= j)
		{
			masterTemp = master[i];
			master[i] = master[j];
			master[j] = masterTemp;
			slaveTemp = slave[i];
			slave[i] = slave[j];
			slave[j] = slaveTemp;
			i++;
			j--;
		}
	} while (i <= j);

	if (low < j)
		paraqsort(master, slave, low, j);
	if (i < high)
		paraqsort(master, slave, i, high);
}

vector<int> factor(int n)
{
	vector<int> factors;
	for (int i = 1; i <= n / 2; i++)
	{
		if (n % i == 0)
			factors.push_back(i);
	}

	return factors;
}
