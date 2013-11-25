#ifndef _FREQUENCY_H
#define _FREQUENCY_H

#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include "stringmanip.h"
using namespace std;

// The language to use in the frequency analysis
enum Language { ENGLISH, GERMAN, FRENCH, ITALIAN, SPANISH, PORTUGUESE, RUSSIAN };

#define FREQUENCIES_FILE	"count"
#define MONO_SUFFIX			"_mono"
#define DI_SUFFIX			"_di"
#define TRI_SUFFIX			"_tri"

//template <class T>
typedef vector<vector<float> > Matrix;

class Frequency
{
	private:
		string ciphertext;					// the ciphertext to be analyzed
//		vector<int> nchars;					// number of characters in the ciphertext
		int nchars;
		int nspaces;				// number of spaces in the ciphertext
		int nsymbols;				// number of symbols in the ciphertext
		vector<vector<int> >monoCounts;	// the frequency of single letters in the ciphertext
		vector<float> symbolFreqs;	// the freqency of single symbols in the ciphertext
		vector<char> symbols;		// the symbols found in the ciphertext
		vector<vector<vector<int> > > diCounts;				// the frequency of digraphs in the ciphertext
		vector<string> digraphs;	// the digraphs present in the ciphertext
		vector<float> triFreqs;		// the frequency of trigraphs in the ciphertext
		vector<string> trigraphs;	// the trigraphs present in the ciphertext
		Language language;			// the language to use for frequency analysis
		vector<float> langMonoFreqs;		// the frequencies of single letters in the language
//		vector<float> langDiFreqs;
		Matrix langDiFreqs;					// the digraph frequencies of the language
		vector<float> langTriFreqs;
		vector<int> periods;				// period of repeated trigraphs



		static const float ENGLISH_MONO_FREQS[26];
		static const float GERMAN_MONO_FREQS[26];

	public:

		/* Constructor
		 * Input: ciphertext string; language to use in analysis (optional, default is English)
		 * Output: Frequency object containing frequency analysis of ciphertext
		 */
		Frequency(string s, int numAlphabets = 1, Language l = ENGLISH);

		/* ciphertextMonoFreqs
		 * Input: the alphabet to access
		 * Output: the vector of single letter frequencies of the ciphertext
		 */
		const vector<float> ciphertextMonoFreqs(int a);
		const vector<vector<float> > ciphertextMonoFreqs();
		const vector<vector<int> > ciphertextMonoCounts();
		const vector<int> ciphertextMonoCounts(int a);

		const vector<vector<int> > ciphertextContactCounts(int a);

		/* languageMonoFreqs
		 * Output: the vector of single letter frequencies of the language in use
		 */
		const vector<float> languageMonoFreqs();

		const Matrix languageDiFreqs();

		const vector<float> languageTriFreqs();

		const vector<vector<float> > ciphertextDiFreqs(int a);
		const vector<vector<vector<float> > > ciphertextDiFreqs();

		const vector<string> ciphertextDigraphs();

		const vector<float> ciphertextTriFreqs();

		const vector<string> ciphertextTrigraphs();

		const vector<float> ciphertextSymbolFreqs();

		const vector<char> ciphertextSymbols();
		
		const vector<int> trigraphPeriods();

		const int getNchars();
//		const int getTotalChars();
		const int getNspaces();
		const int getNsymbols();
};

#endif	// _FREQUENCY_H
