#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <vector>
#include "stringmanip.h"
using namespace std;

void writeChart(int count[], int size, float nchars, string filename);

// Counts up various desired frequencies in the given file
// Creates distribution charts
int main(int argc, char * argv[])
{
	// Command line switch flags
	bool mono = false, di = false, tri = false, words = false,
		 spaces = false, symbols = false;
	// Check validity of arguments, modify flags
	if (argc < 2)	// insufficient arguments
	{
		cout << "Usage: " << argv[0] << " [options] <file1> [file2] [file3] ... [fileN]" << endl;
		return 1;
	}
	// The filenames passed in
	vector<string> filenames;
	// Loop through given args
	for (int i = 1; i < argc; i++)
	{
		if (string(argv[i]) == "--mono" || string(argv[i]) == "-m")				// count monographs
			mono = true;
		else if (string(argv[i]) == "--di" || string(argv[i]) == "-d")			// count digraphs
			di = true;
		else if (string(argv[i]) == "--tri" || string(argv[i]) == "-t")			// count trigraphs
			tri = true;
		else if (string(argv[i]) == "--words" || string(argv[i]) == "-w")		// count words
			words = true;
		else if (string(argv[i]) == "--spaces" || string(argv[i]) == "-s")		// count spaces
			spaces = true;
		else if (string(argv[i]) == "--symbols" || string(argv[i]) == "-y")		// count symbols
			symbols = true;
		else													// not a switch, should be file
			filenames.push_back(argv[i]);
	}

	// Perform the counts
	for (int i = 0; i < filenames.size(); i++)
	{
		//int ch[3];//1, ch2, ch3;
		float nchars = 0.0;
		int monoCount[LETTERS] = { 0 };
		int diCount[LETTERS*LETTERS] = { 0 };
//		int triCount[LETTERS*LETTERS*LETTERS] = { 0 };

		ifstream file;
		file.open(filenames[i].c_str());
		if (file.is_open())
		{
			int prev = -1, next = -1;
//			ch[0] = file.get();
//			ch[1] = file.get();
//			ch[2] = file.get();

			// get first char
			while (file.good() && prev < 0)
				prev = toBase(file.get());
			// process first character
			if (file.good())
			{
				monoCount[prev]++;
				nchars++;
			}
			// process remaining characters
			while (file.good())
			{
				next = toBase(file.get());
				if (next >= 0)
				{
					// monograph count
					monoCount[next]++;
					nchars++;
					// digraph count
					if (prev >= 0)
						diCount[prev*LETTERS+next]++;

					prev = next;
				}

//				if (isAlpha(ch[0]))
//				{
//					ch[0] = toLower(ch[0]);
//					monoCount[ch[0]-LOWER]++;
//					nchars++;
//				
//					if (isAlpha(ch[1]))
//					{
//						ch[1] = toLower(ch[1]);
//						diCount[(ch[0]-LOWER)*LETTERS+(ch[1]-LOWER)]++;
//
//						if (isAlpha(ch[2]))
//						{
//							ch[2] = toLower(ch[2]);
//							triCount[(ch[0]-LOWER)*LETTERS*LETTERS+(ch[1]-LOWER)*LETTERS+(ch[2]-LOWER)]++;
//						}
//					}
//				}
//				ch[0] = ch[1];
//				ch[1] = ch[2];
//				ch[2] = file.get();
//				ch[1] = file.get();
			}
			if (mono)
				writeChart(monoCount, LETTERS, nchars, "count_mono");
			if (di)
				writeChart(diCount, LETTERS*LETTERS, nchars, "count_di");
//			if (tri)
//				writeChart(triCount, LETTERS*LETTERS*LETTERS, nchars, "count_tri");
//			if (words)
//				writeWordsChart(wordCount, filenames[i] + "_words");
		}
		else	// unable to open file
			cout << "Unable to open file '" << filenames[i] << "'. Proceeding with others." << endl;
	}

	return 0;
}

void writeChart(int count[], int size, float nchars, string filename)
{
	int n;
	if (size == LETTERS)
		n = 1;
	else if (size == LETTERS*LETTERS)
		n = 2;
	else if (size == LETTERS*LETTERS*LETTERS)
		n = 3;
	else
	{
		cout << "Error writing to '" << filename << "'." << endl;
		return;
	}

	ofstream file;
	file.open(filename.c_str());
	for (int i = 0; i < size; i++)
		file << fixed << setprecision(4) << count[i] / (nchars / n) << endl;
	file.close();
}
