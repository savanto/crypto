#ifndef _SUBSTITUTION_H
#define _SUBSTITUTION_H

#include <ncurses.h>
#include <string>
#include <sstream>
#include <vector>
//#include <cmath>
#include "Frequency.h"
#include "stringmanip.h"
using namespace std;

#define SUBSTITUTION_CONTROLS "F1:Monoalphabetic|F2:Homophones|F3:Polyalphabetic|F4:Caesar/Vigenere|F10:Exit"
#define PLAINTEXT_COLS 40
//#define NEWLINE '\n'

typedef vector<string> Grid;
// Sort options
enum SortDirection { ASCENDING, DESCENDING };
// Substitution modules
enum SubModule { MONO, POLY, HOMOPHONES, CAESARVIGENERE };



class Substitution
{
	private:
		string ciphertext;		// the ciphertext string being analyzed
		string text;			// transition text -- partly analyzed
		string plaintext;		// the product of the analysis, the plaintext
		vector<string> keys;	// keys of each alphabet used in the encryption
	public:

		/* Constructor
		 * Input: the ciphertext, the number of alphabets to use
		 */
		Substitution(string s, int n);

		//////////////////////////////
		// ACCESSORS
		//////////////////////////////

		/* getPlaintext
		 * Output: this.plaintext
		 */
		string getPlaintext();

		/* getCiphertext
		 * Output: this.ciphertext
		 */
		string getCiphertext();

		/* getKeys
		 * Output: this.keys
		 */
		vector<string> getKeys();


		//////////////////////////////
		// MODIFIERS
		//////////////////////////////

		/* replaceChar
		 * Input: target character, replacement
		 * Output: target character replaced with replacement in analysis text
		 */
		void replaceChar(char target, char replacement, int key);

		/* replaceChar
		* Input: vector of keys
		* Output: keys have been used to replace characters in the ciphertext into the plaintext
		*/
		void replaceChar(const vector<vector<int> > & keys);

		void updatePlaintext();

		void replaceChar(string target, string replacement);
		
		void replaceSingleChar(int index, char replacement);

		/* deleteChar
		 * Input: position
		 * Output: target position in string has been deleted
		 */
		void deleteChar(int pos);

		/* insertChar
		 * Input: position, character
		 * Output: target position has had character inserted
		 */
		void insertChar(int pos, char c);

		/* shiftAlphabet
		 * Input: step to shift by (direction is indicated by sign), key number
		 * Output: plaintext has been shifted by step in the direction indicated by the sign
		 */
		void shiftAlphabet(int step, int key);

		void setPlaintext(string s);

		void setKeys(const vector<vector<int> > & k);
};

/* substituion
 * Input: the ciphertext string to analyze
 * Reads user input and analyzes the ciphertext using various methods of
 *  substitution cipher analysis
 * Output: the presumed plaintext
 */
string substitution(string ciphertext);

/* makeGrid
 * Input: string and the number of columns
 * Output: Grid with cols columns (or length of string, if smaller)
 */
Grid makeGrid(string s, int cols);
Grid makeGrid(string s);

/* getChar
 * Input: Grid and x,y coordinates
 * Output: the character in the Grid at coordinates x,y
 *  x,y must be within range!
 */
char getChar(const Grid & grid, int y, int x);
string getChar(const Grid & grid, int y, int x, int length);

/* drawFreqWins
 * Input: ciphertext frequency vector, language frequency vector, window placement coordinates
 * Draws these vectors to the screen
 */
void drawFreqWins(Frequency & f, int numAlphabets, int x);

template <class M, class S>
void parallelQuickSort(vector<M> & master, vector<S> & slave, int low, int high, SortDirection dir = DESCENDING);
//template <class M, class S>
//void paraqsort(vector<M> & master, vector<S> & slave, int low, int high);
template <class T>
void shift(vector<T> & v, int step);

int numCols(Grid grid);
int stringPosFromGrid(Grid grid, int y, int x);

/* sumVector
 * Input: vector of numbers
 * Output: sum of the values in the vector
 */
float sumVector(const vector<float> & v);
/*
{
	float sum = 0;
	for (int i = 0; i < v.size(); i++)
		sum += v[i];
	return sum;
}
*/
float sumVector(const Matrix & v);
/*
{
	float sum = 0;
	for (int i = 0; i < v.size(); i++)
		for (int j = 0; j < v[i].size(); j++)
			sum += v[i][j];
	return sum;
}
*/
/* subtractVectors
 * Input: two vectors of numbers of equal length
 * Output: a vector containing the difference between the two
 */
vector<float> subtractVectors(const vector<float> & v, const vector<float> & w);
/*
{
	vector<float> diff;
	for (int i = 0; i < v.size(); i++)
		diff.push_back(abs(v[i] - w[i]));
	return diff;
}
*/
Matrix subtractVectors(const Matrix & v, const Matrix & w);
/*
{
	Matrix diff;
	for (int i = 0; i < v.size(); i++)
	{
		vector<float> row;
		for (int j = 0; j < v[i].size(); j++)
			row.push_back(abs(v[i][j] - w[i][j]));
		diff.push_back(row);
	}
	return diff;
}
*/
vector<vector<int> > hillClimb(vector<Matrix> D, const Matrix & E, vector<vector<int> > & k, const vector<vector<int> > & s);
vector<vector<int> > hillClimb(vector<vector<float> > D, const vector<float> & E, vector<vector<int> > & k);
//vector<int> keyGen(const vector<int> & L, const vector<int> & S);
float difference(const vector<vector<float> > & D, const vector<vector<float> > & E);
float difference(const vector<float> & D, const vector<float> & E);
float sum(const vector<float> & v);
int getIndex(const vector<int> & k, int alpha);
int getIndex(const Grid & grid, int y, int x);
void swapValues(int & i, int & j);
void swapRows(vector<vector<float> > & D, int a, int b);
void swapCols(vector<vector<float> > & D, int a, int b);

int getKey(const Grid & grid, int y, int x, int n);

void drawBusyWin();
void drawStatsWin(int nchars, int nspaces, int nsymbols, int numAlphabets, int x);
void drawKeysWin(const vector<string> & keys, int x);
int drawNumAlphabetsWin();
void drawAnalysisWin(const vector<string> & cipherGrid, const vector<string> & plainGrid, int cursory, int cursorx);
#endif	// _SUBSTITION_H
