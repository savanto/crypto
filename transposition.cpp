#include "Transposition.h"

//////////////////////////////////
// CONSTRUCTORS
//////////////////////////////////

/* Constructor (default)
 * Output: empty Transposition object
 */
Transposition::Transposition()
{
	ciphertext = "";
}


/* Constructor (full)
 * Input: string of ciphertext (mandatory); write-in method (optional);
 *	rows and cols values (optional); starting write-in coordinates (optional);
 *	write-in direction (optional); transposing even or odd columns/rows (optional)
 *
 * Output: new Transposition object with s stored as ciphertext;
 *	grid written in by method; row and col vowels containing respective vowel percentages
 */
Transposition::Transposition(string s, Method mWriteIn /* = ROWS */, Method mReadOut /* = COLUMNS */,
		Direction dWriteIn /* = LEFTRIGHT */, Direction dReadOut /* = UPDOWN */, 
		int nrows /* = 0 */, int ncols /* = 0 */, bool odd /* = true */)
{
	// Set the ciphertext
	ciphertext = s;

	// Determine grid dimensions from given rows/cols
	if (! ncols && ! nrows)		// ncols == 0, nrows == 0
	{
		// Use closest square method
		cols = closestSquare(ciphertext.length());
		rows = cols;
	}
	else if (ncols && ! nrows)	// ncols != 0, nrows == 0
	{
		// Set the cols, determine rows
		cols = ncols;
		rows = ciphertext.length() / cols;
		if (ciphertext.length() % cols != 0)	// remainder, add row
			rows++;
	}
	else if (! ncols && nrows)	// ncols == 0, nrows != 0
	{
		// Set the rows, determine cols
		rows = nrows;
		cols = ciphertext.length() / rows;
		if (ciphertext.length() % rows != 0)	// remainder, add col
			cols++;
	}
	else						// ncols != 0, nrows != 0
	{
		// Set the rows and cols
		rows = nrows;
		cols = ncols;
	}

	// Produce the grid based on the write-in method and direction
	switch(mWriteIn)
	{
		default:	// ROWS is the default write-in method
			/* FALLTHROUGH */
		case ROWS:
			// Write cols-sized chunks of text into each row
			for (int i = 0; i < ciphertext.length(); i+=cols)
			    grid.push_back(ciphertext.substr(i, cols));

			// Pad the final row in case it is short
			grid[grid.size()-1] = pad(grid[grid.size()-1], cols);

			// Set the rows
//			rows = grid.size();
			break;
		case COLUMNS:
			// Write rows-sized chunks of text into each row
			for (int i = 0; i < ciphertext.length(); i+=rows)
			    grid.push_back(ciphertext.substr(i, rows));
			
			// Pad the final row in case it is short
			grid[grid.size()-1] = pad(grid[grid.size()-1], rows);
			
			// Transpose the grid to make it written in by columns
			grid = transpose(grid);
			break;
	}

	// Produce the plaintext based on the read-out method and direction
	switch(mReadOut)
	{
		case ROWS:
			break;
		case COLUMNS:
			plaintext = toLower(ciphertext);

			break;
	}
}


//////////////////////////////////
// ACCESSORS
//////////////////////////////////


/* getCiphertext
 * Output: return the string containing the ciphertext
 */
string Transposition::getCiphertext()
{
	return ciphertext;
}

/* getPlaintext
 * Output: return the string containing the plaintext
 */
string Transposition::getPlaintext()
{
	return plaintext;
}

/* getGrid
 * Output: return the grid containing the ciphertext
 */
Grid Transposition::getGrid()
{
	return grid;
}

/* getVowels
 * Input: select rows or columns
 * Output: return the vector containing the desired vowels
 */
vector<float> Transposition::getVowels(Method m /* = ROWS */)
{
	switch(m)
	{
		case ROWS:
			return rowVowels;
			break;
		case COLUMNS:
			return colVowels;
			break;
		default:
			return vector<float>();
			break;
	}
}

/* getLength
 * Output: the number of characters in the ciphertext
 */
int Transposition::getLength()
{
	return ciphertext.length();
}

/* getFactors
 * Output: the vector containing the factors of the ciphertext length
 */
vector<int> Transposition::getFactors()
{

}

/* getSquare
 * Output: if the ciphertext length is a perfect square, the square root; else 0
 */
int Transposition::getSquare()
{

}

/* getCols
 * Output: the number of columns of the grid
 */
int Transposition::getCols()
{
	return cols;
}

/* getRows
 * Output: the number of rows of the grid
 */
int Transposition::getRows()
{
	return rows;
}

//////////////////////////////////
// MISC HELPERS (non-member)
//////////////////////////////////

/* transpose
 * Input: a non-empty Grid of text
 * Output: a Grid of text with columns now rows, and rows now columns
 */
Grid transpose(Grid g)
{
	Grid t;
	string row = "";
	for (int c = 0; c < g[0].size(); c++)
	{
		for (int r = 0; r < g.size(); r++)
			row += g[c][r];
		t.push_back(row);
		row = "";
	}
	return t;
}

/* pad
 * Input: string, desired length, padding character (optional)
 * Output: string increased to desired length by adding 0 or more padding characters
 */
string pad(string s, int length, char c /* = '#' */)
{
	string r = s;
	for (int i = 0; i < length - s.length(); i++)
		r += c;
	return r;
}

/* closestSquare
 * Input: n, presumed to be message length
 * Output: the best-sized square to use as initial grid
 */
int closestSquare(int n)
{
	int square = 1;
	while (square < n / square)
		square++;

	return square;
}

/* methodToString
 * Input: Method enum value
 * Output: string describing method
 */
string methodToString(Method m)
{
	switch (m)
	{
		case ROWS:
			return "rows";
			break;
		case COLUMNS:
			return "columns";
			break;
		case TRANSROWS:
			return "transposed rows";
			break;
		case TRANSCOLUMNS:
			return "transposed columns";
			break;
		case DIAGONALS:
			return "diagonals";
			break;
		case TRANSDIAGONALS:
			return "transposed diagonals";
			break;
		case CWSPIRAL:
			return "clockwise spiral";
			break;
		case CCWSPIRAL:
			return "counter-clockwise spiral";
			break;
		case MAGICSQUARE:
			return "magic square";
			break;
		default:
			return "unknown";
			break;
	}
}

/* directionToString
 * Input: Direction enum value
 * Output: string describing direction
 */
string directionToString(Direction d)
{
	switch(d)
	{
		case LEFTRIGHT:
			return "left-to-right";
			break;
		case RIGHTLEFT:
			return "right-to-left";
			break;
		case UPDOWN:
			return "top-to-bottom";
			break;
		case DOWNUP:
			return "bottom-to-top";
			break;
		default:
			return "unknown";
			break;
	}
}


//////////////////////////////////
// INPUT/OUTPUT functions
//////////////////////////////////

/* transposition
 * Input: the ciphertext to analyze
 * Reads user input and displays various transpositional analyses of the ciphertext
 * Output: returns the plaintext string
 */
string transposition(string ciphertext)
{
	int ch;						// character input

	// Set default write-in and read-out methods and directions
	Method mWriteIn = ROWS;
	Method mReadOut = COLUMNS;
	Direction dWriteIn = LEFTRIGHT;
	Direction dReadOut = UPDOWN;

	// Set the initial cursor position to origin
	int x = 0, y = 0;

	// Make initial grid
	Transposition t = Transposition(ciphertext, mWriteIn, mReadOut, dWriteIn, dReadOut);

	// Wait for, and parse, user input
	while (ch != int('q'))//KEY_F(10))
	{
		switch(mWriteIn)
		{
			case ROWS:
				switch(ch)
				{
					// If ch is +/-, change grid columns
					case int('+'):	// increase column number
						if (t.getCols() < t.getLength())
							t = Transposition(ciphertext, mWriteIn, mReadOut, dWriteIn, dReadOut,
									0, t.getCols() + 1, false);
						break;
					case int('-'):	// decrease column number
						if (t.getCols() >= MIN_COLUMNS)
							t = Transposition(ciphertext, mWriteIn, mReadOut, dWriteIn, dReadOut,
									0, t.getCols() - 1, false);
						break;
					default:	// do nothing
						break;
				}
				break;
			case COLUMNS:
				switch(ch)
				{
					// If ch is * or /, change grid rows
					case int('*'):	// increase row number
						if (t.getRows() < t.getLength())
							t = Transposition(ciphertext, mWriteIn, mReadOut, dWriteIn, dReadOut,
									t.getRows() + 1, 0, false);
						break;
					case int('/'):	// decrease row number
						if (t.getRows() >= MIN_COLUMNS)
							t = Transposition(ciphertext, mWriteIn, mReadOut, dWriteIn, dReadOut,
									t.getRows() - 1, 0, false);
						break;
					default:	// do nothing
						break;
				}
				break;
		}
		switch (ch)
		{
			// If ch is one of the arrow keys, move cursor
			case KEY_DOWN:
//				if (y < grid.size() - 1)
//					y++;
				break;
			case KEY_UP:
//				if (y > 0)
//					y--;
				break;
			case KEY_RIGHT:
//				if (x < (grid[y].length() - 1) * 2)
//					x += 2;
				break;
			case KEY_LEFT:
//				if (x > 0)
//					x -= 2;
				break;
			// Transpose a given row
			case int('t'):
//				grid[y] = reverse(grid[y]);
				break;
			// Reset to initial values
			case int('r'):
//				if (square)
//					cols = square;
//				else
//					cols = INITIAL_COLUMNS;
//				grid = writeIn(ciphertext, cols, method);
				break;
			// Set write-in methods
			case int('R'):	// rows
//				method = "rows";
//				grid = writeIn(ciphertext, cols, method);
				break;
			case int('D'):	// diagonals
/*				if (square)
				{
					method = "diagonals";
					grid = writeIn(ciphertext, square, method);	// restrict to squares only
				}
*/				break;
			case int('S'):	// spirals
/*				if (square)
				{
					method = "spirals";
					grid = writeIn(ciphertext, square, method);	// restrict to squares only
				}
*/				break;
			case int('T'):	// transposed rows
//				method = "transposed rows";
//				grid = writeIn(ciphertext, cols, method);
				break;
			case int('M'):	// magic square
//				method = "magic square";
//				grid = writeIn(ciphertext, square, method);		// restric to squares only
				break;
			default:
				break;
		}
		// Correct cursor position if necessary
/*		if (y > grid.size() - 1)
			y = grid.size() - 1;
		if (x > (grid[y].length() - 1) * 2)
			x = (grid[y].length() - 1) * 2;
*/		

		/////////////////////////////////////////
		// DRAW SCREEN
		/////////////////////////////////////////
		// Take into account extra spacing
		int xmargin = 0, ymargin = 0;
		// Clear previous screen
//		clear();
		// Draw borders

		// Draw controls

		// Draw labels
		// File name is printed by main, allow for 1 line
		ymargin++;

		// Create empty drawing window
		//  This is to make it easier to clear on every change
		WINDOW * drawingwin = newwin(LINES - 2, COLS - getmaxy(stdscr), ymargin, 0);

		// Grid info
		stringstream gridSize;
		gridSize << t.getCols() << "x" << t.getRows() << " grid, written in by " << methodToString(mWriteIn) << ", "
			<< "read out by " << methodToString(mReadOut);
		mvwprintw(drawingwin, 0, 0, gridSize.str().c_str());
		ymargin++;

		// Output grid in separate window
		// Padding for the grid window
		const int PADDING = 2;	// 2 borders, plus axis
		// Spacing for the grid window
		const int SPACING = 2;
		// Margins for grid window
		int gridymargin = 1, gridxmargin = 1;
		WINDOW * gridwin = newwin(t.getRows() + PADDING, t.getCols() * SPACING + PADDING, ymargin, xmargin);
		for (int i = 0; i < t.getRows(); i++)
		{
//			stringstream rowaxis;
//			rowaxis << i;
//			mvwprintw(gridwin, gridymargin + i, gridxmargin, rowaxis.str().c_str());
			mvwprintw(gridwin, gridymargin + i, gridxmargin, space(t.getGrid()[i]).c_str());
//			mvprintw(ymargin + i, xmargin, space(pad(grid[i], grid[0].length())).c_str());
			// Row vowels
//			stringstream rowVowel;
//			rowVowel << "| V: " << fixed << setprecision( 2 ) << rowVowels[i];
//			printw(rowVowel.str().c_str());
		}
		wborder(gridwin, '|', '|', '-', '-', '+', '+', '+', '+');
//		box(gridwin, 0, 0);
//		ymargin += grid.size();
/*		// Col vowels
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
*/
		// Output plaintext
//		mvprintw(ymargin + grid.size() + 2, xmargin, toLower(readOut(grid)).c_str());

		// Move cursor to position
//		move(y + ymargin, x + xmargin);

		wrefresh(drawingwin);
		wrefresh(gridwin);
		// Print the transposition controls
//		drawControls(TRANSPOSITION_CONTROLS);


		ch = getch();
	}

	return t.getPlaintext();
}
/*
void drawControls(string controls)
{
	WINDOW * fcontrolswin = newwin(1, COLS, LINES - 1, 0);
	mvwprintw(fcontrolswin, 0, 0, controls.c_str());
	wrefresh(fcontrolswin);
}
*/
