#include <iostream>
#include <string>
#include <vector>
#include <fstream>

#include "frequency.h"
#include "transposition.h"
#include "concealment.h"
using namespace std;

#define PROGRAM "crypto"
#define DEFAULT "english"

/* strip
 * Removes all characters except A-Z and a-z from the ciphertext.
 */
string strip(string ciphertext);

/* toUpper
 * Converts all characters in ciphertext to uppercase
 */
string toUpper(string ciphertext);

/* parseFiles
 * Given a vector of filenames, attempts to open each file.
 * Converts to uppercase, stores each file's contents as a string,
 *  and returns a vector of these strings
 */
vector<string> parseFiles(vector<string> ciphertextFiles);

/* usage
 * Returns short usage information
 */
void usage();

/* help
 * Returns more detailed usage and help information
 */
void help();

/* main function
 * Parses the passed-in command line args to determine the type of operation;
 * Calls the necessary support modules depending on flags
 */
int main(int argc, char * argv[])
{
	// Flags for calling program modules
	bool frequencyAnalysis = false;			// flag frequency analysis module
	bool concealmentAnalysis = false;		// flag concealment cipher analysis module
	bool transpositionAnalysis = false;		// flag transposition cipher analysis module
	bool substitutionAnalysis = false;		// flag substituion cipher analysis module
	bool allAnalysis = false;				// flag all cipher analysis options
	bool combinedAnalysis = false;			// flag individual analysis of input files
	string language = DEFAULT;				// flag language to use for analysis
	vector<string> ciphertextFiles;			// vector containing the filenames to analyze


	// Insufficient arguments passed in; print short usage message
	if (argc <= 1)
	{
		usage();
		return 1;
	}

	// Parse command line args
	else
	{
		for (int i = 1; i < argc; i++)
		{
			// Help switch: calls the help display and exits
			if (
					string(argv[i]) == "-h"
					|| string(argv[i]) == "-help"
					|| string(argv[i]) == "--h"
					|| string(argv[i]) == "--help"
					|| string(argv[i]) == "-?"
					|| string(argv[i]) == "--?"
			   )
			{
				help();
				return 1;	// if help switch is given, stop parsing arguments
			}

			// Frequency analysis switch: calls the frequency analysis module
			else if (
					string(argv[i]) == "-f"
					|| string(argv[i]) == "-frequency"
					|| string(argv[i]) == "-frequencies"
					|| string(argv[i]) == "--f"
					|| string(argv[i]) == "--frequency"
					|| string(argv[i]) == "--frequencies"
					)
			{
				frequencyAnalysis = true;
			}
			
			// Language to use
			else if (
					string(argv[i]) == "-l"
					|| string(argv[i]) == "-language"
					|| string(argv[i]) == "--l"
					|| string(argv[i]) == "--language"
					)
			{
				if (i + 1 == argc)	// no given language parameter
				{
					usage();
					return 1;
				}
				else
				{
					language = argv[i+1];
					i++;
				}
			}

			// Concealment analysis: calls the concealment cipher module
			else if (
					string(argv[i]) == "-c"
					|| string(argv[i]) == "-concealment"
					|| string(argv[i]) == "--c"
					|| string(argv[i]) == "--concealment"
					)
			{
				concealmentAnalysis = true;
			}

			// Transposition analysis: calls the transposition module
			else if (
					string(argv[i]) == "-t"
					|| string(argv[i]) == "-transposition"
					|| string(argv[i]) == "--t"
					|| string(argv[i]) == "--transposition"
					)
			{
				transpositionAnalysis = true;
			}

			// Non-switch arguments assumed to be filenames
			else
				ciphertextFiles.push_back(string(argv[i]));


/*			// Unrecognized command line option; print usage and exit
			else
			{
				usage();
				return 1;
			}
*/
		}
	}

	// Attempt to open and parse the given ciphertext files
	vector<string> ciphertexts = parseFiles(ciphertextFiles);

	// Parse the flags and call appropriate modules
	if (combinedAnalysis)
	{
		// Concatenate files together before analysis
	}
	for (int i = 0; i < ciphertexts.size(); i++)
	{
//		if (frequencyAnalysis)
//			frequency(language);
		if (concealmentAnalysis)
			concealment(ciphertexts[i]);
		if (transpositionAnalysis)
			transposition(strip(ciphertexts[i]));
		if (substitutionAnalysis)
			cout << "Substitution" << endl;
	}

	return 0;
}

string strip(string ciphertext)
{
	string strippedCiphertext = "";
	for (int i = 0; i < ciphertext.length(); i++)
	{
		if ((int(ciphertext[i]) >= 65 && int(ciphertext[i]) <= 90)
				|| (int(ciphertext[i]) >= 97 && int(ciphertext[i]) <= 122))
			strippedCiphertext += ciphertext[i];
	}

	return strippedCiphertext;
}

string toUpper(string ciphertext)
{
	string capitalizedCiphertext = "";
	for (int i = 0; i < ciphertext.length(); i++)
	{
		if (int(ciphertext[i]) >= 97)
			capitalizedCiphertext += char(int(ciphertext[i]) - 32);
		else
			capitalizedCiphertext += ciphertext[i];
	}

	return capitalizedCiphertext;
}

vector<string> parseFiles(vector<string> ciphertextFiles)
{
	string ciphertext = "";
	string line;
	vector<string> ciphertexts;
	ifstream file;

	for (int i = 0; i < ciphertextFiles.size(); i++)
	{
		file.open(ciphertextFiles[i].c_str());
		if (file.is_open())
		{
			while (file.good())
			{
				getline(file, line);
				ciphertext += line;
			}
			ciphertexts.push_back(ciphertext);
			file.close();
			ciphertext = "";
		}
		else
		{
			cout << "Unable to open file '" << ciphertextFiles[i] << "'." << endl;
			usage();
		}
	}

	return ciphertexts;
}

void usage()
{
	cout << "Incorrect arguments." << endl;
	cout << "Usage: " << PROGRAM << " [arguments]" << endl;
	cout << "For help use '--help'" << endl;
}

void help()
{
	cout << "HELP" << endl;
}
