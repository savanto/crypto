#ifndef _TRANSPOSITION_H
#define _TRANSPOSITION_H

#include <ncurses.h>
#include <string>
#include <sstream>
#include <vector>
#include "stringmanip.h"
using namespace std;

#define MIN_COLUMNS 2
#define INITIAL_COLUMNS 5
#define TRANSPOSITION_CONTROLS "F1:Write-In | F2:Read-Out | F3:Vowels"




// Method by which the grid is to be written in
enum Method { ROWS, COLUMNS, TRANSROWS, TRANSCOLUMNS, DIAGONALS, TRANSDIAGONALS,
	CWSPIRAL, CCWSPIRAL, MAGICSQUARE };
// Direction by which the grid is to be written in
enum Direction { LEFTRIGHT, RIGHTLEFT, UPDOWN, DOWNUP };

// A Grid of text is a vector of strings
typedef vector<string> Grid;

class Transposition
{
	private:
		string ciphertext;			// ciphertext stripped of whitespace etc.
		string plaintext;			// plaintext generated from analysis of ciphertext
		Grid grid;					// grid containing the ciphertext written in by a cetrain method
		int rows, cols, square;		// the grid dimensions
		vector<float> rowVowels;	// vector containing the vowel percentages calculated by rows
		vector<float> colVowels;	// vector containing the vowel percentages calculated by columns
	public:
		//////////////////////////////////
		// CONSTRUCTORS
		//////////////////////////////////
		
		/* Constructor (default)
		 * Output: empty Transposition object
		 */
		Transposition();

		/* Constructor (no grid)
		 * Input: string of ciphertext
		 * Output: new Transposition object with s stored as ciphertext
		 */
//		Transposition(string s);

		/* Constructor (full)
		 * Input: string of ciphertext (mandatory); write-in method (optional);
		 *	rows and cols values (optional); starting write-in coordinates (optional);
		 *	write-in direction (optional); transposing even or odd columns/rows (optional)
		 *
		 * Output: new Transposition object with s stored as ciphertext;
		 *	grid written in by method; row and col vowels containing respective vowel percentages
		 */
	  	Transposition(string s, Method mWriteIn = ROWS, Method mReadOut = COLUMNS,
				Direction dWriteIn = LEFTRIGHT, Direction dReadOut = UPDOWN, 
				int nrows = 0, int ncols = 0, bool odd = true);

		//////////////////////////////////
		// MODIFIERS
		//////////////////////////////////


		//////////////////////////////////
		// ACCESSORS
		//////////////////////////////////
		
		/* getCiphertext
		 * Output: return the string containing the ciphertext
		 */
		string getCiphertext();

		/* getPlaintext
		 * Output: return the string containing the plaintext
		 */
		string getPlaintext();

		/* getGrid
		 * Output: return the Grid containing the ciphertext
		 */
		Grid getGrid();

		/* getVowels
		 * Input: select rows or columns
		 * Output: return the vector containing the desired vowels
		 */
		vector<float> getVowels(Method m = ROWS);

		/* getLength
		 * Output: the number of characters in the ciphertext
		 */
		int getLength();

		/* getFactors
		 * Output: the vector containing the factors of the ciphertext length
		 */
		vector<int> getFactors();

		/* getSquare
		 * Output: if the ciphertext length is a perfect square, the square root; else 0
		 */
		int getSquare();

		/* getCols
		 * Output: the number of columns of the grid
		 */
		int getCols();

		/* getRows
		 * Output: the number of rows of the grid
		 */
		int getRows();
};

//////////////////////////////////
// MISC HELPERS (non-member)
//////////////////////////////////

/* transpose
 * Input: a non-empty Grid of text
 * Output: a Grid of text with columns now rows, and rows now columns
 */
Grid transpose(Grid g);

/* pad
 * Input: string, desired length, padding character (optional)
 * Output: string increased to desired length by adding 0 or more padding characters
 */
string pad(string s, int length, char c = '#');

/* closestSquare
 * Input: n, presumed to be message length
 * Output: the best-sized square to use as initial grid
 */
int closestSquare(int n);

/* methodToString
 * Input: Method enum value
 * Output: string describing method
 */
string methodToString(Method m);

/* directionToString
 * Input: Direction enum value
 * Output: string describing direction
 */
string directionToString(Direction d);

//////////////////////////////////
// INPUT/OUTPUT functions
//////////////////////////////////

/* transposition
 * Input: the ciphertext to analyze
 * Reads user input and displays various transpositional analyses of the ciphertext
 * Output: returns the plaintext string
 */
string transposition(string ciphertext);

#endif // _TRANSPOSITION_H
