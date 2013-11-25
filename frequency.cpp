#include "Frequency.h"

// Static language frequency data
//  A/N      B/O      C/P      D/Q      E/R      F/S      G/T      H/U      I/V      J/W      K/X      L/Y      M/Z
const float Frequency::ENGLISH_MONO_FREQS[26]	/* English single-letter frequencies */
= { 0.08167, 0.01492, 0.02782, 0.04253, 0.12702, 0.02228, 0.02015, 0.06094, 0.06966, 0.00153, 0.00772, 0.04025, 0.02406,
	0.06749, 0.07507, 0.01929, 0.00095, 0.05987, 0.06327, 0.09056, 0.02758, 0.00978, 0.02360, 0.00150, 0.01974, 0.00074 };
const float Frequency::GERMAN_MONO_FREQS[26]	/* German single-letter frequencies */
= { 0.0651,  0.0189,  0.0306,  0.0508,  0.1740,  0.0166,  0.0301,  0.0476,  0.0755,  0.0027,  0.0121,  0.0344,  0.0253,
	0.0978,  0.0251,  0.0079,  0.0002,  0.0700,  0.0727,  0.0615,  0.0435,  0.0067,  0.0189,  0.0003,  0.0004,  0.0113 };

Frequency::Frequency(string s, int numAlphabets /* = 1 */, Language l /* = ENGLISH */)
{
	// Convert ciphertext to uppercase for purposes of counting
	ciphertext = toUpper(s);
	// Set the analysis language
	language = l;

	//////////////////////////////////////////////////////////////
	//															//
	// If language frequency counts exist in the given file,	//
	// use them, otherwise use the monograph frequencies above.	//
	//															//
	//////////////////////////////////////////////////////////////

	// Get monograph language frequencies
	ifstream file;
	file.open((string(FREQUENCIES_FILE) + string(MONO_SUFFIX)).c_str());
	if (file.is_open())
	{
		float fr;
		for (int i = 0; i < LETTERS; i++)
		{
			file >> fr;
			langMonoFreqs.push_back(fr);
		}
		file.close();
	}
	else
		langMonoFreqs = vector<float>(ENGLISH_MONO_FREQS, ENGLISH_MONO_FREQS + LETTERS);

	// Get digraph language frequencies
	file.open((string(FREQUENCIES_FILE) + string(DI_SUFFIX)).c_str());
	if (file.is_open())
	{
		float fr;
		for (int i = 0; i < LETTERS; i++)
		{
			vector<float> row;
			for (int j = 0; j < LETTERS; j++)
			{
				file >> fr;
				row.push_back(fr);
			}
			langDiFreqs.push_back(row);
		}
		file.close();
	}
/*
	// Get trigraph language frequencies
	file.open((string(FREQUENCIES_FILE) + string(TRI_SUFFIX)).c_str());
	if (file.is_open())
	{
		float fr;
		for (int i = 0; i < LETTERS*LETTERS*LETTERS; i++)
		{
			file >> fr;
			langTriFreqs.push_back(fr);
		}
		file.close();
	}
*/
	// Produce different ciphertexts for each alphabet for easier counting
	// initialize the various matrices/vectors holding info
	nchars = strip(ciphertext).length();
	nspaces = 0;
	nsymbols = 0;

	vector<int> vec;
	for (int i = 0; i < LETTERS; i++)
		vec.push_back(0);
	for (int a = 0; a < numAlphabets; a++)
	{
		// monograph frequencies
		monoCounts.push_back(vec);
	}

	vector<vector<int> > mat;
	for (int i = 0; i < LETTERS; i++)
		mat.push_back(vec);
	for (int a = 0; a < numAlphabets; a++)
	{
		// digraph frequencies
		diCounts.push_back(mat);
	}

	// Count ciphertext by alphabets
	int alphabet = 0;						// active alphabet
	int prevAlphabet = numAlphabets - 1;	// previous active alphabet
	int prefixCount = 0;					// count to first alphabetic character
	char prev = ' ', next = ' ';			// read in the characters
	// Process first alphabetic character
	while (! isAlpha(prev))
	{
		next = ciphertext[prefixCount];
		if (isAlpha(next))
		{
			monoCounts[alphabet][toBase(next)]++;
//			nchars++;
			prev = next;
			prevAlphabet = alphabet;
			alphabet++;
		}
		else if (next == ' ')
			nspaces++;
		else if (! isWhitespace(next))
			nsymbols++;
		prefixCount++;
	}
	// Process subsequent characters
	for (int i = prefixCount; i < ciphertext.length(); i++)
	{
		// Reset alphabet count if necessary
		if (alphabet >= numAlphabets)
			alphabet = 0;
		
		next = ciphertext[i];
		if (isAlpha(next))
		{
			monoCounts[alphabet][toBase(next)]++;
//			nchars++;

			diCounts[prevAlphabet][toBase(prev)][toBase(next)]++;

			prev = next;
			prevAlphabet = alphabet;
			alphabet++;
		}
		else if (next == ' ')
			nspaces++;
		else if (! isWhitespace(next))
			nsymbols++;
	}

	// Calculate the mono/digraph frequencies for each alphabet
//	for (int a = 0; a < numAlphabets; a++)
//	{
//		for (int i = 0; i < LETTERS; i++)
//		{
//			// Monographs
//			monoFreqs[a][i] = monoFreqs[a][i] / nchars;
//			// Digraphs
//			for (int j = 0; j < LETTERS; j++)
//				diFreqs[a][i][j] = diFreqs[a][i][j] / (nchars / 2);
//		}
//	}

	// Traverse ciphertext and find repeating trigraphs
	string strippedCiphertext = strip(ciphertext);
	for (int i = 0; i < strippedCiphertext.length() - 3; i++)
	{
		string trigraph = strippedCiphertext.substr(i, 3);
		int pos = i;
//		while (pos > 0)
		{
			pos = strippedCiphertext.find(trigraph, pos + 1);
			if (pos != string::npos)
				periods.push_back(pos - i);
		}
	}
}

const vector<float> Frequency::ciphertextMonoFreqs(int a)
{
	vector<float> freqs;
	for (int i = 0; i < monoCounts[a].size(); i++)
		freqs.push_back(monoCounts[a][i] / float(nchars));
	return freqs;
}

const vector<vector<float> > Frequency::ciphertextMonoFreqs()
{
	vector<vector<float> > freqs;
	for (int a = 0; a < monoCounts.size(); a++)
		freqs.push_back(ciphertextMonoFreqs(a));
	return freqs;
}

const vector<vector<int> > Frequency::ciphertextMonoCounts()
{
	return monoCounts;
}

const vector<int> Frequency::ciphertextMonoCounts(int a)
{
	return monoCounts[a];
}

const vector<vector<int> > Frequency::ciphertextContactCounts(int a)
{
	return diCounts[a];
}

const vector<float> Frequency::languageMonoFreqs()
{
/*	vector<float> freqs;
	switch (language)
	{
		default:	// default case is English
		// FALLTHROUGH
		case ENGLISH:
			freqs = vector<float>(ENGLISH_MONO_FREQS, ENGLISH_MONO_FREQS + sizeof(ENGLISH_MONO_FREQS) / sizeof(float));
			break;
		case GERMAN:
			freqs = vector<float>(GERMAN_MONO_FREQS, GERMAN_MONO_FREQS + sizeof(GERMAN_MONO_FREQS) / sizeof(float));
			break;
	}

	return freqs;
*/
	return langMonoFreqs;
}

const vector<vector<float> > Frequency::languageDiFreqs()
{
	return langDiFreqs;
}
/*
const vector<float> Frequency::languageTriFreqs()
{
	return langTriFreqs;
}
*/

const vector<vector<float> > Frequency::ciphertextDiFreqs(int a)
{
	vector<vector<float> > freqs;
	for (int i = 0; i < diCounts[a].size(); i++)
	{
		vector<float> row;
		for (int j = 0; j < diCounts[a][i].size(); j++)
			row.push_back(diCounts[a][i][j] / (float(nchars) / 2));
		freqs.push_back(row);
	}
	return freqs;
}

const vector<vector<vector<float> > > Frequency::ciphertextDiFreqs()
{
	vector<vector<vector<float> > > freqs;
	for (int a = 0; a < diCounts.size(); a++)
		freqs.push_back(ciphertextDiFreqs(a));
	return freqs;
}

/*
const vector<string> Frequency::ciphertextDigraphs()
{
	return digraphs;
}
*/

/*
const vector<float> Frequency::ciphertextTriFreqs()
{
	return triFreqs;
}
*/

const vector<string> Frequency::ciphertextTrigraphs()
{
	return trigraphs;
}

const vector<int> Frequency::trigraphPeriods()
{
	return periods;
}

const vector<float> Frequency::ciphertextSymbolFreqs()
{
	return symbolFreqs;
}
/*
const vector<char> Frequency::ciphertextSymbols()
{
	return symbols;
}
*/

const int Frequency::getNchars()
{
	return nchars;
}

const int Frequency::getNspaces()
{
	return nspaces;
}

const int Frequency::getNsymbols()
{
	return nsymbols;
}
