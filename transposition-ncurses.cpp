#include <ncurses.h>
#include <fstream>
#include <string>
#include <vector>
#include <iostream>
#include <iomanip>
#include <sstream>
using namespace std;

#define MIN_COLUMNS 2
#define INITIAL_COLUMNS 5
#define ROWS 0
#define COLS 1

// Method for writing the ciphertext into the grid
//enum WriteInMethod { rows, diagonals, transposed, spiral };

/* readFile
 * Input: name of file to open
 * Output: string containing exact file contents
 */
string readFile(string fileName);

/* strip
 * Input: text string to clena up
 * Output: string with all but A-Za-z removed
 */
string strip(string s);

/* explode
 * Input: string, character
 * Output: vector<string> containing substrings separated by the character
 */
vector<string> explode(string s, char c);

/* drawScreen
 * Input: the ciphertext object to draw, and the cursor coordinates
 * Output: ncurses window is updated
 */
void drawScreen(vector<string> grid, string method, string fileName, int cursorx, int cursory);

/* writeIn
 * Input: string, number of columns
 * Output: vector<string> rows of the grid
 */
vector<string> writeIn(string s, int cols, string method);//WriteInMethod method);

/* writeInDiagonals
 * Input: string, method, starting coordinates
 * Output: string that has been written in by the given method and read out by rows
 */



/* space
 * Input: string
 * Output: string with spaces between each character
 */
string space(string s);

/* factor
 * Input: integer to factor
 * Output: vector<int> containing all factors, or empty vector if integer is prime
 */
vector<int> factor(int n);

/* reverse
 * Input: string to reverse
 * Output: returns the string written backwards
 */
string reverse(string s);

/* readOut
 * Input: grid of ciphertext to be read out in columns
 * Output: string containing read-out plaintext
 */
string readOut(vector<string> grid);

/* toLower
 * Input: string
 * Output: string with uppercase letters change to lowercase, others untouched
 */
string toLower(string s);

/* vowels
 * Input: grid of letters, rows or columns to calculate
 * Output: vector<float> containing vowel percentages
 */
vector<float> vowels(vector<string> grid, bool cols);

/* countVowels
 * Input: string
 * Output: number of vowels in string
 */
int countVowels(string s);

/* transpose
 * Input: grid of letters
 * Output: columns transposed to rows
 */
vector<string> transpose(vector<string> grid);

/* pad
 * Input: string, desired length
 * Output: string with spaces added to the end to achieve desired length
 */
string pad(string s, int length);



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
	const string cipherfile = readFile(fileName);

	// Check for file too short (empty)
	if (! cipherfile.length())
	{
		cout << "File '" << argv[1] << "' appears to be empty." << endl;
		return 1;
	}

//	vector<string> cipherWords = explode(ciphertext, ' ');
	const string ciphertext = strip(cipherfile);
	
	// Initialize ncurses
	initscr();					// start ncurses mode
	cbreak();					// capture input from user without line breaks
	keypad(stdscr, TRUE);		// enable F-keys and number pad
	noecho();					// do not echo user input

	int ch;						// character input

	// Get info about the ciphertext
	// Prepare initial columns arrangements
//	vector<int> factors = factor(ciphertext.length());
	int square = 0;
//	for (int i = 0; i < factors.size(); i++)
	for (int i = 1; i <= ciphertext.length() / 2; i++)
	{
		if (i * i == ciphertext.length())
			square = i;
	}

	// Select either the square, or use the preset initial value
	int cols;
	if (square)
		cols = square;
	else
		cols = INITIAL_COLUMNS; //(ciphertext.length() - 1) / 2; //columns[(columns.size()-1)/2];

	// Set initial writing-in method to 'rows'
//	WriteInMethod method = rows;
	string method = "rows";

	// Set the initial cursor position to origin
	int x = 0, y = 0;

	// Make initial grid
	vector<string> grid = writeIn(ciphertext, cols, method);

	// Wait for, and parse, user input
	while (ch != int('q'))//KEY_F(10))
	{
		switch(ch)
		{
			// If ch is +/-, change grid dimensions
			case int('+'):
				if (cols < ciphertext.length())
					grid = writeIn(ciphertext, ++cols, method);
				break;
			case int('-'):
				if (cols >= MIN_COLUMNS)
					grid = writeIn(ciphertext, --cols, method);
				break;
			// If ch is one of the arrow keys, move cursor
			case KEY_DOWN:
				if (y < grid.size() - 1)
					y++;
				break;
			case KEY_UP:
				if (y > 0)
					y--;
				break;
			case KEY_RIGHT:
				if (x < (grid[y].length() - 1) * 2)
					x += 2;
				break;
			case KEY_LEFT:
				if (x > 0)
					x -= 2;
				break;
			// Transpose a given row
			case int('t'):
				grid[y] = reverse(grid[y]);
				break;
			// Reset to initial values
			case int('r'):
				if (square)
					cols = square;
				else
					cols = INITIAL_COLUMNS;
				grid = writeIn(ciphertext, cols, method);
				break;
			// Set write-in methods
			case int('R'):	// rows
				method = "rows";
				grid = writeIn(ciphertext, cols, method);
				break;
			case int('D'):	// diagonals
				if (square)
				{
					method = "diagonals";
					grid = writeIn(ciphertext, square, method);	// restrict to squares only
				}
				break;
			case int('S'):	// spirals
				if (square)
				{
					method = "spirals";
					grid = writeIn(ciphertext, square, method);	// restrict to squares only
				}
				break;
			case int('T'):	// transposed rows
				method = "transposed rows";
				grid = writeIn(ciphertext, cols, method);
				break;
			case int('M'):	// magic square
				method = "magic square";
				grid = writeIn(ciphertext, square, method);		// restric to squares only
				break;
			default:
				break;
		}
		// Correct cursor position if necessary
		if (y > grid.size() - 1)
			y = grid.size() - 1;
		if (x > (grid[y].length() - 1) * 2)
			x = (grid[y].length() - 1) * 2;
		
		// Calculate letter stats
		vector<float> rowVowels = vowels(grid, ROWS);
		vector<float> colVowels = vowels(grid, COLS);

		/////////////////////////////////////////
		// DRAW SCREEN
		/////////////////////////////////////////
//		drawScreen(grid, method, fileName, x, y);
		// Take into account extra spacing
		int xmargin = 0, ymargin = 0;
		// Clear previous screen
		clear();
		// Draw borders

		// Draw controls

		// Draw labels
		// File name
		mvprintw(ymargin++, xmargin, fileName.c_str());
		// Grid info
		stringstream gridSize;
		gridSize << grid[0].length() << "x" << grid.size() << " grid, written in by " << method << ":";
		attron(A_UNDERLINE);
		mvprintw(ymargin++, xmargin, gridSize.str().c_str());
		attroff(A_UNDERLINE);

		// Output grid
		for (int i = 0; i < grid.size(); i++)
		{
			mvprintw(ymargin + i, xmargin, space(pad(grid[i], grid[0].length())).c_str());
			// Row vowels
			stringstream rowVowel;
			rowVowel << "| V: " << fixed << setprecision( 2 ) << rowVowels[i];
			printw(rowVowel.str().c_str());
		}
		ymargin += grid.size();
		// Col vowels
		for (int i = 0; i < cols; i++)
			mvprintw(ymargin, xmargin + i * 2, "-");
		ymargin++;
		for (int i = 0; i < colVowels.size(); i++)
			mvprintw(ymargin, xmargin + i * 2, "V");
		ymargin++;
		for (int i = 0; i < colVowels.size(); i++)
			mvprintw(ymargin, xmargin + i * 2, ".");
		ymargin++;
		for (int i = 0; i < 2; i++)
		{
			for (int j = 0; j < colVowels.size(); j++)
			{
				stringstream colVowel;
				colVowel << fixed << setprecision(2) << colVowels[i];
				string temp1 = colVowel.str();
				char temp2 = temp1[i+2];
				mvprintw(ymargin, xmargin + j * 2, temp2);
			}
			ymargin++;
		}

		// Output plaintext
		mvprintw(ymargin + grid.size() + 2, xmargin, toLower(readOut(grid)).c_str());

		// Move cursor to position
		move(y + ymargin, x + xmargin);

		refresh();


		ch = getch();
	}

	endwin();					// end curses mode

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
			ciphertext += line;
		}
		file.close();
	}
	else
		cout << "Cannot open file '" << fileName << "'." << endl;

	return ciphertext;
}

string strip(string s)
{
	string stripped = "";
	for (int i = 0; i < s.length(); i++)
	{
		if ((int(s[i]) >= 65 && int(s[i]) <= 90) || (int(s[i]) >= 97 && int(s[i]) <= 122))
			stripped += s[i];
	}
	return stripped;
}


vector<string> explode(string s, char c)
{
/*
	vector<string> exp;
	size_t found;

	while (found != string::npos)
	{
*/
}

void drawScreen(vector<string> grid, string method, string fileName, int cursorx, int cursory)
{
/*	// Take into account extra spacing
	int xmargin = 0, ymargin = 0;
	// Clear previous screen
	clear();
	// Draw borders
	
	// Draw controls

	// Draw labels
	// File name
	string file = "File: " + fileName;
	mvprintw(ymargin++, xmargin, file.c_str());
	// Grid info
	stringstream gridSize;
	gridSize << grid[0].length() << "x" << grid.size() << " grid, written in by " << method << ":";
	attron(A_UNDERLINE);
	mvprintw(ymargin++, xmargin, gridSize.str().c_str());
	attroff(A_UNDERLINE);

	// Output grid
	for (int i = 0; i < grid.size(); i++)
		mvprintw(ymargin + i, xmargin, space(grid[i]).c_str());	// y, x, string

	// Output plaintext
	mvprintw(ymargin + grid.size() + 2, xmargin, toLower(readOut(grid)).c_str());
	
	// Move cursor to position
	move(cursory + ymargin, cursorx + xmargin);
	
	refresh();
*/
}

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
