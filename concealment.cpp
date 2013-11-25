#include "concealment.h"
#include <iostream>
#include <string>
#include <vector>
using namespace std;

void concealment(string ciphertext)
{
	// Break the ciphertext into words by iterating through it and looking for spaces
	vector<string> cipherwords;
	int lastSpace = 0;
	for (int i = 0; i < ciphertext.length(); i++)
	{
		if (ciphertext[i] == ' ')
		{
			cipherwords.push_back(ciphertext.substr(lastSpace, i - lastSpace));
			lastSpace = i + 1;
		}
	}
	cipherwords.push_back(ciphertext.substr(lastSpace, ciphertext.length()));	// get the last word

	letters(cipherwords);
}

void letters(vector<string> cipherwords)
{
	int longestWord = longest(cipherwords);
	for (int i = 0; i < longestWord; i++)
	{
		string suffix;
		if (i + 1 == 1)
			suffix = "st";
		else if (i + 1 == 2)
			suffix = "nd";
		else if (i + 1 == 3)
			suffix = "rd";
		else
			suffix = "th";

		cout << "-------------------------------------" << endl;
		cout << i + 1 << suffix << " letters:" << endl;
		cout << "-------------------------------------" << endl;

		for (int j = 0; j < cipherwords.size(); j++)
		{
			if (i < cipherwords[j].length())
				cout << cipherwords[j][i];
			else
				cout << "_";
		}
		cout << endl << endl;
	}
}

int shortest(vector<string> cipherwords)
{
	int length = cipherwords[0].length();
	for (int i = 0; i < cipherwords.size(); i++)
		if (cipherwords[i].length() < length)
			length = cipherwords[i].length();

	return length;
}

int longest(vector<string> cipherwords)
{
	int length = 0;
	for (int i = 0; i < cipherwords.size(); i++)
		if (cipherwords[i].length() > length)
			length = cipherwords[i].length();

	return length;
}
