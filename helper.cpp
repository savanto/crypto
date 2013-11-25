#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

int toBase(char c)
{
	if (c >= 'A' && c <= 'Z')
		return int(c) - 65;
	else if (c == '*')
		return 26;
	else
		return -1;
}

char toChar(int c)
{
	if (c >= 0 && c <= 25)
		return char(c + 65);
	else if (c == 26)
		return '*';
	else
		return '.';
}

void quicksort(int v[], int low, int high)
{
	int i = low, j = high;
	int x = v[(low+high)/2];

	do
	{
		while (v[i] > x) i++;
		while (v[j] < x) j--;

		if (i <= j)
		{
			int temp = v[i];
			v[i] = v[j];
			v[j] = temp;

			i++;
			j--;
		}
	} while (i <= j);

	if (low < j)
		quicksort(v, low, j);
	if (i < high)
		quicksort(v, i, high);
}
int main(int argc, char * argv[])
{
	if (argc < 2)
	{
		cout << "No file specified." << endl;
		return 1;
	}

	// Read in file
	ifstream file;
	file.open(argv[1]);
	string ciphertext = "";
	while (file.good())
	{
		string line;
		getline(file, line);
		ciphertext += line;
	}
	file.close();

	// Count up mono- di- and tri-graphs
	int mono[27] = { 0 };
	int di[27][27] = { { 0 } };
	int tri[27][27][27] = { { { 0 } } };

	// First 3 chars
	int ch[3];
	for (int i = 0; i < 3; i++)
		ch[i] = toBase(ciphertext[i]);

	// All subsequent chars
	for (int i = 3; i < ciphertext.length(); i++)
	{
		mono[ch[0]]++;
		di[ch[0]][ch[1]]++;
		di[ch[1]][ch[2]]++;
		tri[ch[0]][ch[1]][ch[2]]++;
		
		ch[0] = ch[1];
		ch[1] = ch[2];
		ch[2] = toBase(ciphertext[i]);
	}
	// All final two characters to counts
	mono[ch[0]]++;
	mono[ch[1]]++;
	mono[ch[2]]++;
	di[ch[0]][ch[1]]++;
	di[ch[1]][ch[2]]++;
	tri[ch[0]][ch[1]][ch[2]]++;

	// Write tallies out to files
	int chars = 0, stars = mono[26];
	for (int i = 0; i < 27; i++)
		chars += mono[i];

	ofstream outfile;//outMono, outDi, outTri;
	outfile.open("helper_info");
//	outMono.open("mono_helper");
//	outDi.open("di_helper");
//	outTri.open("tri_helper");
	outfile << "----INFO----" << endl;
	outfile << "Chars: " << chars << " | no stars: " << chars - stars << " | stars: " << stars << endl;
	outfile << "----MONO----" << endl;
	for (int i = 0; i < 27; i++)
	{
		for (int n = 0; n < mono[i]; n++)
			outfile << toChar(i);
		outfile << ": " << fixed << mono[i] << " (" << setprecision(4) << mono[i] / float(chars) << ")" << endl;
	}
	outfile << "----DI----" << endl;
	for (int i = 0; i < 27; i++)
		for (int j = 0; j < 27; j++)
			if (di[i][j])
			{
				for (int n = 0; n < di[i][j]; n++)
					outfile << toChar(i) << toChar(j) << " ";
				outfile << ": " << fixed << di[i][j] << " (" << setprecision(4) << di[i][j] / (float(chars) / 2) << ")" << endl;
			}
	outfile << "----TRI----" << endl;
	for (int i = 0; i < 27; i++)
		for (int j = 0; j < 27; j++)
			for (int k = 0; k < 27; k++)
				if (tri[i][j][k])
				{
					for (int n = 0; n < tri[i][j][k]; n++)
						outfile << toChar(i) << toChar(j) << toChar(k) << " ";
					outfile << ": " << fixed << tri[i][j][k] << " (" << setprecision(4) << tri[i][j][k] / (float(chars) / 3) << ")" << endl;
				}

	outfile.close();

	// Count non-overlapping digrams
	int diNoOverlap[27][27] = { { 0 } };
	int digrams = 0;
	for (int i = 0; i < ciphertext.length() - 1; i+=2)
	{
		ch[0] = toBase(ciphertext[i]);
		ch[1] = toBase(ciphertext[i+1]);
		diNoOverlap[ch[0]][ch[1]]++;
		digrams++;
	}

	int uniqueDigrams = 0;
	outfile.open("digrams");
	for (int i = 0; i < 27; i++)
		for (int j = 0; j < 27; j++)
			if (diNoOverlap[i][j])
			{
				for (int n = 0; n < diNoOverlap[i][j]; n++)
					outfile << toChar(i) << toChar(j) << " ";
				outfile << ": " << fixed << diNoOverlap[i][j] << " (" << setprecision(4) << diNoOverlap[i][j] / float(digrams) << ")" << endl;
				uniqueDigrams++;
			}

	outfile << "------------------------------" << endl << "Unique digrams: " << uniqueDigrams << endl;
	outfile.close();
	
	// Count non-overlapping trigrams
	int triNoOverlap[27][27][27] = { { { 0 } } };
	int trigrams = 0;
	for (int i = 0; i < ciphertext.length() - 2; i+=3)
	{
		ch[0] = toBase(ciphertext[i]);
		ch[1] = toBase(ciphertext[i+1]);
		ch[2] = toBase(ciphertext[i+2]);
		triNoOverlap[ch[0]][ch[1]][ch[2]]++;
		trigrams++;
	}

	int uniqueTrigrams = 0;
	outfile.open("trigrams");
	for (int i = 0; i < 27; i++)
		for (int j = 0; j < 27; j++)
			for (int k = 0; k < 27; k++)
				if (triNoOverlap[i][j][k])
				{
					for (int n = 0; n < triNoOverlap[i][j][k]; n++)
						outfile << toChar(i) << toChar(j) << toChar(k) << " ";
					outfile << ": " << triNoOverlap[i][j][k] << " (" << fixed << setprecision(4) << triNoOverlap[i][j][k] / float(trigrams) << ")" << endl;
					uniqueTrigrams++;
				}
	outfile << "------------------------------" << endl << "Unique trigrams: " << uniqueTrigrams << endl;
	outfile.close();

	
	
	return 0;
}
