#include <fstream>
#include <iostream>
#include <iomanip>
#include "Transposition.h"
#include "Substitution.h"
#include "stringmanip.h"
using namespace std;

// The analysis module to use based on user input
enum Module { CONCEALMENT, TRANSPOSITION, SUBSTITUTION };

/* readFile
 * Input: name of file to open
 * Output: string containing exact file contents
 */
string readFile(string fileName);

/* strip
 * Input: text string to clena up
 * Output: string with all but A-Za-z removed
 */
//string strip(string s);

/* explode
 * Input: string, character
 * Output: vector<string> containing substrings separated by the character
 */
//vector<string> explode(string s, char c);

/* writeIn
 * Input: string, number of columns
 * Output: vector<string> rows of the grid
 */
//vector<string> writeIn(string s, int cols, string method);//WriteInMethod method);

/* space
 * Input: string
 * Output: string with spaces between each character
 */
//string space(string s);

/* factor
 * Input: integer to factor
 * Output: vector<int> containing all factors, or empty vector if integer is prime
 */
//vector<int> factor(int n);

/* reverse
 * Input: string to reverse
 * Output: returns the string written backwards
 */
//string reverse(string s);

/* readOut
 * Input: grid of ciphertext to be read out in columns
 * Output: string containing read-out plaintext
 */
//string readOut(vector<string> grid);

/* toLower
 * Input: string
 * Output: string with uppercase letters change to lowercase, others untouched
 */
//string toLower(string s);

/* vowels
 * Input: grid of letters, rows or columns to calculate
 * Output: vector<float> containing vowel percentages
 */
//vector<float> vowels(vector<string> grid, bool cols);

/* countVowels
 * Input: string
 * Output: number of vowels in string
 */
//int countVowels(string s);

/* transpose
 * Input: grid of letters
 * Output: columns transposed to rows
 */
//vector<string> transpose(vector<string> grid);

/* pad
 * Input: string, desired length
 * Output: string with spaces added to the end to achieve desired length
 */
//string pad(string s, int length);

void drawControlsWin(string controls);

int main(int argc, char * argv[])
{
	// Check for insufficient arguments
	if (argc != 2)
	{
		cout << "Usage: " << argv[0] << " [FILE]" << endl;
		return 1;
	}

	// Load file
	const string fileName = string(argv[1]);
	string cipherfile = readFile(fileName);

	// Check for file too short (empty)
	if (! cipherfile.length())
	{
		cout << "File '" << argv[1] << "' appears to be empty." << endl;
		return 1;
	}

	// If text is to be stripped of whitespace/non-alphabet characters
	bool strip_text = false;
	if (strip_text)
		cipherfile = strip(cipherfile);
	
	// Initialize ncurses
	initscr();					// start ncurses mode
	start_color();				// use colors
	cbreak();					// capture input from user without line breaks
	keypad(stdscr, TRUE);		// enable F-keys and number pad
	noecho();					// do not echo user input
	clear();					// clear the screen before use

	// Print filename
	init_pair(1, COLOR_RED, COLOR_BLACK);
	attron(A_UNDERLINE | COLOR_PAIR(1));
	printw(("File: " + fileName).c_str());
	attroff(A_UNDERLINE | COLOR_PAIR(1));
	refresh();

	string plaintext = "";

	// TODO: Set up module parsing
	//Module m = TRANSPOSITION;
	Module m = SUBSTITUTION;

	switch (m)
	{
		case TRANSPOSITION:
			// Draw controls window
			drawControlsWin(TRANSPOSITION_CONTROLS);
			// Run the transposition module
			plaintext = transposition(cipherfile);
			break;
		case SUBSTITUTION:
			// Draw controls window
			drawControlsWin(SUBSTITUTION_CONTROLS);
			// Run the substitution module
			plaintext = substitution(cipherfile);
			break;
	}

	endwin();					// end curses mode


	cout << fileName << ":" << endl << plaintext << endl;

	return 0;
}

string readFile(string fileName)
{
    string ciphertext = "", line;
	ifstream file;

	file.open(fileName.c_str());
	if (file.is_open())
	{
		while (file.good())
		{
			getline(file, line);
			ciphertext += line + " ";
		}
		file.close();
	}
	else
		cout << "Cannot open file '" << fileName << "'." << endl;

	return ciphertext;
}

void drawControlsWin(string controls)
{
	init_pair(1, COLOR_RED, COLOR_BLACK);
	WINDOW * fcontrolswin = newwin(2, COLS, LINES - 2, 0);
	string border = "";
	for (int i = 0; i < controls.length(); i++)
		border += '_';
	mvwprintw(fcontrolswin, 0, 0, border.c_str());
	wattron(fcontrolswin, COLOR_PAIR(1));
	mvwprintw(fcontrolswin, 1, 0, controls.c_str());
	wrefresh(fcontrolswin);
	wattroff(fcontrolswin, COLOR_PAIR(1));
}

/*
vector<string> writeIn(string s, int cols, string method)//WriteInMethod method)
{
	vector<string> grid;
	if (method == "diagonals")
	{
		int pos = 0;
		// First set of characters to the main diagonal
		for (int i = 0; i < cols; i++)
		{
			for (int j = 0; j <= i; j++)
			{
				if (grid.size() < j + 1)
					grid.push_back("");
				grid[j] += s[pos++];
			}
		}
		// Second set of characters after main diagonal
		for (int i = 1; i < grid.size(); i++)
		{
			for (int j = i; j < grid.size(); j++)
				grid[j] += s[pos++];
		}
	}
	else if (method == "spirals")
	{
		for (int i = 0; i < cols; i++)
		{
			string row = "";
			for (int j = 0; j < cols; j++)
				row += "#";
			grid.push_back(row);
		}
		
		int x = cols / 2, y = cols / 2;
		int inc;
		int pos = 0;
		grid[x][y] = s[pos++];
		for (int i = 0; i < cols - 1; i++)
		{
			inc = -1;
			for (int k = 0; k < i; k++)
				inc *= -1;

			for (int j = 0; j <= i; j++)
			{
				y += inc;
				grid[x][y] = s[pos++];
			}
			for (int j = 0; j <= i; j++)
			{
				x += inc;
				grid[x][y] = s[pos++];
			}
		}
		inc = -1;
		for (int j = 0; j < cols -1; j++)
		{
			y += inc;
			grid[x][y] = s[pos++];
		}
	}
	else if (method == "transposed rows")
	{
		for (int i = 0; i < s.length(); i+=cols)
			grid.push_back(s.substr(i, cols));
		// Transpose every other (odd) row
		for (int i = 1; i < grid.size(); i+=2)
			grid[i] = reverse(grid[i]);
	}
	else if (method == "magic square")
	{
		for (int i = 1; i <= cols; i++)
		{
			grid.push_back("");
			for (int j = 1; j <= cols; j++)
				grid[i-1] += s[cols*((i+j-1+cols/2)%cols)+((i+2*j-2)%cols)];
		}
		// Rotate the magic square

	}
	else	// rows is the default method
	{
		for (int i = 0; i < s.length(); i+=cols)
		    grid.push_back(s.substr(i, cols));
	}

    return grid;
}
*/
/*
string space(string s)
{
	string spaced = "";
	for (int i = 0; i < s.length(); i++)
	{
		spaced += s[i];
		spaced += ' ';
	}
	return spaced;
}
*/
/*
vector<int> factor(int n)
{
    vector<int> factors;
    // We are only interested in factors > 1
	for (int i = 2; i <= n / 2; i++)
    {
        if (n % i == 0)
            factors.push_back(i);
    }

	// factors is empty if n is prime
	return factors;
}

string reverse(string s)
{
	string reversed = "";
	for (int i = s.length() - 1; i >=0; i--)
		reversed += s[i];

	return reversed;
}

string readOut(vector<string> grid)
{
	string plaintext = "";
	// Read text out of grid by columns
	for (int col = 0; col < grid[0].length(); col++)
	{
		for (int row = 0; row < grid.size(); row++)
		{
			if (col < grid[row].length())	// some columns will have more letters than others
				plaintext += grid[row][col];
		}
	}

	return plaintext;
}
*/
/*
string toLower(string s)
{
	string lower = "";
	for (int i = 0; i < s.length(); i++)
	{
		if (int(s[i]) >= 65 && int(s[i]) <= 90)
			lower += char(int(s[i]) + 32);
		else
			lower += s[i];
	}

	return lower;
}
*/
/*
vector<float> vowels(vector<string> grid, bool cols)
{
	vector<float> gridVowels;
	// Calculate column vowels
	if (cols)
		grid = transpose(grid);
	// Else row vowels will be calculated
	for (int i = 0; i < grid.size(); i++)
		gridVowels.push_back(float(countVowels(grid[i])) / float(grid[i].length()));

	return gridVowels;
}

int countVowels(string s)
{
	int vowels = 0;
	for (int i = 0; i < s.length(); i++)
	{
		if (s[i] == 'A' || s[i] == 'E' || s[i] == 'I' || s[i] == 'O' || s[i] == 'U')
			vowels++;
	}

	return vowels;
}
*/
/*
vector<string> transpose(vector<string> grid)
{
	vector<string> gridT;
	for (int i = 0; i < grid[0].length(); i++)
	{
		gridT.push_back("");
		for (int j = 0; j < grid.size(); j++)
		{
			if (grid[j].length() > i)
				gridT[i] += grid[j][i];
		}
	}

	return gridT;
}

string pad(string s, int length)
{
	string padded = s;
	for (int i = 0; i < length - s.length(); i++)
		padded += ' ';
	return padded;
}
*/
