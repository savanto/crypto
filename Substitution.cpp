#include "Substitution.h"
#include "Frequency.h"
#include <iomanip>
#include <cmath>
#include <cstdlib>
#include <ctime>

Substitution::Substitution(string s, int n)
{
	// Set the ciphertext
	ciphertext = s;
	
	// Set the plaintext
	plaintext = "";
	for (int i = 0; i < ciphertext.length(); i++)
	{
		if (isAlpha(ciphertext[i]))
			plaintext += '-';
		else
			plaintext += ciphertext[i];
	}

	// Set initial keys to blanks
	for (int a = 0; a < n; a++)
	{
		string key = "";
		for (int i = 0; i < LETTERS; i++)
			key += '-';//toLower(i);
		keys.push_back(key);
	}
}

string Substitution::getPlaintext()
{
	return plaintext;
}

string Substitution::getCiphertext()
{
	return ciphertext;
}

void Substitution::setPlaintext(string s)
{
	plaintext = s;
}

/* replaceChar
 * Input: target character, replacement, key number
 * Output: target character in keys[key] is replaced with replacement character
 */
void Substitution::replaceChar(char target, char replacement, int key)
{
	// Search if target ciphercharacter already has a plaincharacter in the key
	for (int i = 0; i < keys[key].size(); i++)
		if (keys[key][i] == target)//toUpper(target))
			keys[key][i] = '-';
	// Update the key
	if (isUpper(target))
	{
//		for (int i = 0; i < keys[key].size(); i++)
//			if ( == toBase(target))
				keys[key][toBase(replacement)] = target;//toUpper(target);
	}
}

void Substitution::updatePlaintext()
{
	// Each key in keys is to be used for its own alphabet only
	// number of alphabets is equal to the size of the keys vector

/*	// Strip ciphertext of non-alpha characters for ease of traversal
	string strippedCiphertext = strip(ciphertext);
	// The stripped plaintext will be constructed and used to preserve
	// the spaces in the final plaintext
	string strippedPlaintext = "";
	for (int i = 0; i < strippedCiphertext.length(); i++)
	{
			strippedPlaintext[i] += '-';
	}
	// Traverse the keys vector (ie. traverse alphabets)
	for (int a = 0; a < keys.size(); a++)
	{
		// Traverse the stripped ciphertext, looking up the character in the key vector
		for (int i = a; i < strippedCiphertext.length(); i+=keys.size())
		{
			if (isAlpha(strippedCiphertext[i]))
			{
				// look up key
				for (int j = 0; j < keys[a].size(); j++)
				{
					if (strippedCiphertext[i] == keys[a][j])
					{
						// key found, set the plaintext
						strippedPlaintext[i] = toLower(j);
						break;
					}
					// else plaintext remains the same as ciphertext
				}
			}
		}
	}

	// strippedPlaintext now contains ciphertext deciphered with a number of alphabets
	// However formatting has been lost. Now traverse plaintext, filling it from strippedPlaintext
	plaintext = ciphertext;
	int n = 0;
	for (int i = 0; i < ciphertext.length(); i++)
	{
		if (isAlpha(ciphertext[i]))
			plaintext[i] = strippedPlaintext[n++];
	}
*/
	// Set the plaintext
	plaintext = "";
	for (int i = 0; i < ciphertext.length(); i++)
	{
		if (isAlpha(ciphertext[i]))
			plaintext += '-';
		else
			plaintext += ciphertext[i];
	}

	int a = 0;
	for (int i = 0; i < ciphertext.length(); i++)
	{
		if (a >= keys.size())
			a = 0;
		if (isAlpha(ciphertext[i]))
		{
			for (int j = 0; j < keys[a].size(); j++)
			{
				if (ciphertext[i] == keys[a][j])
				{
					plaintext[i] = toLower(j);
					break;
				}
			}
			a++;
		}
	}
}

void Substitution::replaceSingleChar(int index, char replacement)
{
	if (isPrintable(ciphertext[index]))
		plaintext[index] = replacement;
}

/*
void Substitution::replaceChar(string target, string replacement)
{
//	if (target.length() == 1 && target[0] == replacement)
	if (target == replacement)
		return;
	unsigned found = plaintext.find(target);
	while (found != string::npos)
	{
		plaintext.replace(found, target.length(), replacement);
		found = plaintext.find(target);
	}
}
*/

/* deleteChar
 * Input: position
 * Output: target position in string has been deleted
 */
void Substitution::deleteChar(int pos)
{
	string deleted = plaintext.substr(0, pos);
	deleted += plaintext.substr(pos + 1, string::npos);
	plaintext = deleted;
}

/* insertChar
 * Input: position, character
 * Output: target position has had character inserted
 */
void Substitution::insertChar(int pos, char c)
{
	string inserted = plaintext.substr(0, pos);
	inserted += c;
	inserted += plaintext.substr(pos, string::npos);
	plaintext = inserted;
}

/* shiftAlphabet
 * Input: step to shift by (direction is indicated by sign)
 * Output: plaintext has been shifted by step in the direction indicated by the sign
 */
void Substitution::shiftAlphabet(int step, int key)
{
	// Traverse the given key, and shift each character by step, checking for wraparound
	for (int i = 0; i < keys[key].size(); i++)
	{
		// Since key has been changed by user, capitalize all keys
		int temp = int(toUpper(keys[key][i])) + step;
		// Shift character by step
		if (temp >= UPPER + LETTERS)
			keys[key][i] = char(temp - LETTERS);
		else if (temp < UPPER)
			keys[key][i] = char(temp + LETTERS);
		else
			keys[key][i] = char(temp);
	}
/*
	for (int i = 0; i < plaintext.length(); i++)
	{
		// Only shift alphabet characters
		if (isUpper(plaintext[i]))
		{
			// Check for wrap-around
			if (int(plaintext[i]) + step >= UPPER + LETTERS)
				plaintext[i] = toLower(char(int(plaintext[i]) + step - LETTERS));//char(UPPER + step - 1);
			else if (int(plaintext[i]) + step < UPPER)
				plaintext[i] = toLower(char(int(plaintext[i]) + step + LETTERS));//char(UPPER + LETTERS - 1);
			else
				plaintext[i] = toLower(char(int(plaintext[i]) + step));
		}
		else if (isLower(plaintext[i]))
		{
			// Check for wrap-around
			if (int(plaintext[i]) + step >= LOWER + LETTERS)
				plaintext[i] = char(int(plaintext[i]) + step - LETTERS);//char(UPPER + step - 1);
			else if (int(plaintext[i]) + step < LOWER)
				plaintext[i] = char(int(plaintext[i]) + step + LETTERS);//char(UPPER + LETTERS - 1);
			else
				plaintext[i] = char(int(plaintext[i]) + step);

			if (int(plaintext[i]) >= LOWER + LETTERS)
				plaintext[i] = char(LOWER + step - 1);
			else if (int(plaintext[i]) < LOWER)
				plaintext[i] = char(LOWER + LETTERS - 1);

		}

	}
*/
}

vector<string> Substitution::getKeys()
{
	return keys;
}

void Substitution::setKeys(const vector<vector<int> > & k)
{
	keys.clear();
	for (int a = 0; a < k.size(); a++)
	{
		string key = "";
//		vector<int> temp = k[a];
//		vector<int> dummy;
//		for (int i = 0; i < LETTERS; i++)
//			dummy.push_back(i);
//		parallelQuickSort(temp, dummy, 0, temp.size() - 1, ASCENDING);
//		for (int i = 0; i < dummy.size(); i++)
		for (int i = 0; i < k[a].size(); i++)
		{
//			if (k[a][i] - i  - 1< 0)
//				key += toUpper(k[a][i] - i  - 1 + 26);
//			else
//				key += toUpper(k[a][i] - i - 1);
			key += toUpper(k[a][i]);
		}
		keys.push_back(key);
	}
}

string substitution(string ciphertext)
{
	const int BORDER = 0;
	const int WINDOW_SPACING = 7;

	// Setup the variables storing user-supplied info
	int ch;							// user keyboard input character
	int cursorx = 0, cursory = 1;	// position of the cursor manipulated by user
	int indexx = 0, indexy = 0;
	bool freqsort = true;			// true: sort by frequency; false: sort by alphabet
	const int length = 1;			// the length of the strings being manipulated by the user
	int numAlphabets = 1;			// numAlphabets for polyalphabetic ciphers
	SubModule module = MONO;		// start with the monoalphabetic module
	
	// Create new Substitution object
	Substitution s = Substitution(ciphertext, numAlphabets);
	
	// Calculate the frequencies
	Frequency f = Frequency(ciphertext, numAlphabets);

	// Make plaintext output grid and ciphertext internal grid
	Grid grid = makeGrid(s.getPlaintext());
	Grid cgrid = makeGrid(s.getCiphertext());

	// Setup the display margins
	int ymargin = 0, xmargin = 0;
	// The main process prints the file name on line #0
	ymargin++;

	xmargin += 70;//numCols(grid) + BORDER + WINDOW_SPACING;
//	wborder(analysiswin, '|', '|', '-', '-', '+', '+', '+', '+');

	// contentwin contains all of the content that may be updated on the screen
	// This is used to reset the screen for each draw
//	WINDOW * contentwin = newwin(LINES - 2, COLS, 1, 0);

	// Print the frequencies windows
	drawFreqWins(f, numAlphabets, xmargin + 40);
	while (ch != 27)//KEY_CANCEL)//F(10))
	{
//		stringstream keypress;
//		keypress << "Key: " << ch;
//		mvprintw(0,0,keypress.str().c_str());
//		refresh();
//		wclear(analysiswin);
//		wclear(contentwin);	
		// Parse the user input
		switch (ch)
		{
//			case int('1')://KEY_F(1):	// toggle frequency charts sorting
//				freqsort = ! freqsort;
//				drawFreqWins(f, freqsort, ymargin, xmargin);
//				break;
			// Navigation keys
			case KEY_UP:	// up arrow moves cursor up
				if (cursory > 1)	// cursor is not at 1 or less
					cursory -= 2;
				// Extra check to see if cursor coordinates lie outside of line length
				if (cursorx >= cgrid[(cursory-1)/2].length())
					cursorx = cgrid[(cursory-1)/2].length() - 1;
				break;
			case KEY_DOWN:	// down arrow moves cursor down
				if (cursory < cgrid.size() * 2 - 1)
					cursory += 2;
				// Extra check to see if cursor coordinates lie outside of line length
				if (cursorx >= cgrid[(cursory-1)/2].length())
					cursorx = cgrid[(cursory-1)/2].length() - 1;
				break;
			case KEY_LEFT:	// left arrow moves cursor left
				if (cursorx > 0)	// cursor is not at 0 or less
					cursorx--;
				break;
			case KEY_RIGHT:	// right arrow moves cursor right
				if (cursorx < cgrid[(cursory-1)/2].length() - 1)
					cursorx++;
				break;
			case KEY_HOME:	// home button moves cursor to start of line
				cursorx = 0;
				break;
			case KEY_END:	// end button moves cursor to end of line
				cursorx = cgrid[(cursory-1)/2].length() - 1;
				break;
			case KEY_NPAGE:	// page down key moves cursor to bottom of column
				cursory = cgrid.size() * 2 - 1;
				// Extra check to see if cursor coordinates lie outside of line length
				if (cursorx >= cgrid[(cursory-1)/2].length())
					cursorx = cgrid[(cursory-1)/2].length() - 1;
				break;
			case KEY_PPAGE:	// page up key moves cursor to top of column
				cursory = 1;
				// Extra check to see if cursor coordinates lie outside of line length
				if (cursorx >= cgrid[(cursory-1)/2].length())
					cursorx = cgrid[(cursory-1)/2].length() - 1;
				break;
			// Manipulation keys
			case KEY_DC:	// if key pressed is delete
				// if so, delete the character under the cursor
				s.deleteChar(getIndex(grid, cursory, cursorx));
				// Remake the grid from the updated plaintext
				grid = makeGrid(s.getPlaintext());//, PLAINTEXT_COLS);
				// Correct position of cursor if necessary
				if (grid[cursory].length() <= 0)	// all characters on line deleted
					cursory--;	// move up one line
				if (cursorx >= grid[cursory].length())
					cursorx = grid[cursory].length() - 1;
				break;
			case KEY_BACKSPACE:		// if key pressed is backspace
				// if so, delete the character one space previous to cursor
				if (cursorx > 0)	// cursor is not at the start of a line
					s.deleteChar(getIndex(grid, cursory, --cursorx)); // move cursor back one
				// Remake the grid from the updated plaintext
				grid = makeGrid(s.getPlaintext());//, PLAINTEXT_COLS);
				// Correct position of cursor if necessary
				if (grid[cursory].length() <= 0)	// all characters on line deleted
					cursory--;	// move up one line
				if (cursorx >= grid[cursory].length())
					cursorx = grid[cursory].length() - 1;
				break;
			case int(' '):		// if key pressed is space
				// if so, insert space at cursor
				s.insertChar(getIndex(cgrid, cursory, cursorx), ' ');
				// Remake the grid from the updated plaintext
				grid = makeGrid(s.getPlaintext());//, PLAINTEXT_COLS);
				// Advance cursor
				cursorx++;
				break;
			case int('+'):		// if plus pressed, shift alphabet of current key +1 step
				s.shiftAlphabet(-1, getKey(cgrid, (cursory - 1) / 2, cursorx, numAlphabets));
				s.updatePlaintext();
				// Remake the grid from the updated plaintext
				grid = makeGrid(s.getPlaintext());
				break;
			case int('-'):		// if minus pressed, shift alphabet of current key -1 step
				s.shiftAlphabet(1, getKey(cgrid, (cursory - 1) / 2, cursorx, numAlphabets));
				s.updatePlaintext();
				// Remake the grid from the updated plaintext
				grid = makeGrid(s.getPlaintext());
				break;
//			case int('_'):		// if underscore is pressed, replace with spaces
//				if (length == 1)
//					s.replaceChar(getChar(cgrid, cursory, cursorx), ' ');
//				else if (length > 1)
//					s.replaceChar(getChar(grid, cursory, cursorx, length), " ");
//				// Remake the grid from the updated plaintext
//				grid = makeGrid(s.getPlaintext());
//				break;
			case int('?'):		// perform hill-climbing analysis
				switch (module)
				{
					default:
						{
							drawBusyWin();
							// Produce starting values for each alphabet being analyzed
							vector<int> L;			// vector of alphabet sorted by language frequencies
							vector<vector<int> > K;	// initial key to begin hill climb
							vector<vector<int> > S;	// vector containing ciphertext key sorted by frequency
							vector<float> N = f.languageMonoFreqs();	// vector of language frequencies
							vector<vector<float> > M;// = f.ciphertextMonoFreqs(0);	// vector of ciphertext frequencies
							
							for (int i = 0; i < LETTERS; i++)
								L.push_back(i);

							// Initialize initial values for each alphabet
							for (int a = 0; a < numAlphabets; a++)
							{
								M.push_back(f.ciphertextMonoFreqs(a));
								S.push_back(L);
								K.push_back(L);
								// Sort each S to contain ciphertext frequencies
								parallelQuickSort(M[a], S[a], 0, M[a].size() - 1);
								// Set each K = S, to be sorted
								K[a] = S[a];
							}
							// Sort L to contain natural language frequencies
							parallelQuickSort(N, L, 0, N.size() - 1);
							// Generate initial keys by sorting K vectors parallel with L
							for (int a = 0; a < numAlphabets; a++)
							{
								vector<int> copyL = L;
								parallelQuickSort(copyL, K[a], 0, copyL.size() - 1, ASCENDING);
							}
					
							// Perform the hill-climbing algorithm on each alphabet
							K = hillClimb(f.ciphertextDiFreqs(), f.languageDiFreqs(), K, S);

							// Perform letter replacements in the plaintext
							// based on hill-climb results
							// Each alphabet has its key vector

							// Make cipher keys from K
							s.setKeys(K);
							s.updatePlaintext();
							break;
						}	
					case CAESARVIGENERE:
						{
							drawBusyWin();
							// Make initial keys
							vector<vector<int> > K;
							vector<int> initialKey;
							for (int i = 0; i < LETTERS; i++)
								initialKey.push_back(i);
							for (int a = 0; a < numAlphabets; a++)
								K.push_back(initialKey);

							// Peform hill-climb-type algorithm using just monograph frequencies
							K = hillClimb(f.ciphertextMonoFreqs(), f.languageMonoFreqs(), K);

							// Make cipher keys from K
							s.setKeys(K);
							s.updatePlaintext();
						}
						break;
				}
				// Remake the grid from the updated plaintext
				grid = makeGrid(s.getPlaintext());
				break;
//			case int('1'):	// manipulate monographs
//
//				break;
//			case int('2'):	// manipulate digraphs
//
//				break;
//			case int('3'):	// manipulate trigraphs
//
//				break;
			// Module controls
			case KEY_F(1):	// F1: monoalphabetic
				numAlphabets = 1;
				module = MONO;
				// reset the Substitution object
				s = Substitution(ciphertext, numAlphabets);
				// recalculate Frequencies
				f = Frequency(ciphertext, numAlphabets);
				// Print the frequencies windows
				drawFreqWins(f, numAlphabets, xmargin + 40);
				// Remake the grid 
				grid = makeGrid(s.getPlaintext());
				break;
			case KEY_F(2):	// F2: homophones
				module = HOMOPHONES;
				break;
			case KEY_F(3):	// F3: polyalphabetic
				{
					// Prompt user for numAlphabets to try
					int numAlphabetsCh = drawNumAlphabetsWin();
					if (numAlphabetsCh >= int('1') && numAlphabetsCh <= int('9'))
						numAlphabets = numAlphabetsCh - int('0');
					// Reset the Substitution analysis object
					s = Substitution(ciphertext, numAlphabets);
					// Reset/recalculate the frequencies
					f = Frequency(ciphertext, numAlphabets);
					// Print the frequencies windows
					drawFreqWins(f, numAlphabets, xmargin + 40);
					// Remake the grid
					grid = makeGrid(s.getPlaintext());
					// Set module to PolyAlphabetic
					module = POLY;
				}
				break;
			case KEY_F(4):
				{
					// Prompt user for numAlphabets to try
					int numAlphabetsCh = drawNumAlphabetsWin();
					if (numAlphabetsCh >= (int('1') && numAlphabetsCh <= int('9')))
						numAlphabets = numAlphabetsCh - int('0');
					// Reset the Substitution analysis object
					s = Substitution(ciphertext, numAlphabets);
					// Reset/recalculate the frequencies
					f = Frequency(ciphertext, numAlphabets);
					// Print the frequencies windows
					drawFreqWins(f, numAlphabets, xmargin + 40);
					// Remake the grid
					grid = makeGrid(s.getPlaintext());
					// Set module to Caesar/Vigenere
					module = CAESARVIGENERE;
				}
				break;
			case KEY_F(10):	// F10: exit
				break;
			default:
				// check that the key pressed is a lowercase letter
				if (isLower(char(ch)))
				{
					// if so, replace all appearances of the string under the cursor with
					//  the user supplied character in the plaintext
					if (length == 1)
						s.replaceChar(getChar(cgrid, (cursory - 1) / 2, cursorx), char(ch), 
										getKey(cgrid, (cursory - 1) / 2, cursorx, numAlphabets));
					s.updatePlaintext();
					// Remake the grid from the updated plaintext
					grid = makeGrid(s.getPlaintext());
				}
				// else check that the key pressed is an upper case letter
				else if (isUpper(char(ch)))
				{
					// if so, replace only THIS specific appearance of the string under the cursor with
					//  the user supplied character in the plaintext
					// Cipher keys are not affected
					if (length == 1)
						s.replaceSingleChar(getIndex(cgrid, (cursory - 1) / 2, cursorx), char(ch + UPPER2LOWER));
					// Remake the grid from the updated plaintext
					grid = makeGrid(s.getPlaintext());//, PLAINTEXT_COLS);
				}

				// else check that the key is a number
//				else if (isNum(char(ch)))
//				{
//					// Make sure it's not zero
//					if (ch - NUMBERS)
//						length = ch - NUMBERS;	 // set the length of strings we are manipulating
//				}
				// else do nothing
				break;
		}
	
		// Draw the stats window
		drawStatsWin(f.getNchars(), f.getNspaces(), f.getNsymbols(), numAlphabets, xmargin);
		// Draw the keys window
		drawKeysWin(s.getKeys(), xmargin);
		// Print the text under analysis
		drawAnalysisWin(cgrid, grid, cursory, cursorx);
		ch = getch();
	}
	
	return s.getPlaintext();
}

Grid makeGrid(string s)
{
	const int MAX_LENGTH = 60;
	Grid grid;
	string word;
	stringstream stream;
	stream << s;
	stream >> word;
	while (stream.good())
	{
		string line = "";//, word;
		while (line.length() + word.length() < MAX_LENGTH && stream.good())
		{
			if (! line.empty())
				line += " ";
			line += word;
			stream >> word;
		}
		
		// defer to newlines in file
//		getline(stream, line);
/*		// but if line is too long, break it up
		if (line.length() > MAX_LINE)
		{
			for (int i = 0; i < line.length(); i+= MAX_LINE)
				grid.push_back(line.substr(i, MAX_LINE));
		}
		else
*/
		// omit blank lines
//		if (! line.empty())
			grid.push_back(line);
	}

	return grid;
}

char getChar(const Grid & grid, int y, int x)
{
	return grid[y][x];
}

int getIndex(const Grid & grid, int y, int x)
{
	int index = 0;
	for (int i = 0; i < y; i++)
		for (int j = 0; j < grid[y].length(); j++)
			index++;
	index += x;
	return index;
}

string getChar(const Grid & grid, int y, int x, int length)
{
	string target = "";
	for (int i = 0; i < length; i++)
	{
		if (x >= grid[y].length())
		{
			y++;
			x = 0;
		}
		if (y >= grid.size())
			break;
		target += grid[y][x++];
	}
	return target;
}

/* drawFreqWins
 * Input: ciphertext frequency vector, language frequency vector, window placement coordinates
 * Draws these vectors to the screen
 */
void drawFreqWins(Frequency & f, int numAlphabets, int x)
{
	const int PRECISION = 4;
	const int LABEL = 5;
	const int SPACE = 6;
	const int WIDTH = COLS - x; //PRECISION + LABEL + SPACE;
	const int PRINTMAX = 26;
	int xmargin = (PRECISION + LABEL) * 2 + SPACE;
	int ymargin = 0;

	// Create copies of the frequency tables for sorting
//	vector<vector<float> > cmf = f.ciphertextMonoFreqs(), lmf = f.languageMonoFreqs(), mdiff;
//	vector<float> csf = f.ciphertextSymbolFreqs();
//	vector<float> cdf = matrix2vector(f.ciphertextDiFreqs()), ldf = matrix2vector(f.languageDiFreqs());
//	vector<float> cdf = f.ciphertextDiFreqs(), ctf = f.ciphertextTriFreqs();
//	vector<string> digraphs = f.ciphertextDigraphs(), trigraphs = f.ciphertextTrigraphs(); 
//	vector<char> symbols = f.ciphertextSymbols();
	// Create alphabet table for parallel sorting
	vector<char> ciphertextAlphabet, languageAlphabet;
	for (int i = int('A'); i <= int('Z'); i++)
	{
		languageAlphabet.push_back(char(i + UPPER2LOWER));
		ciphertextAlphabet.push_back(char(i));
	}

	vector<vector<int> > cmfs;// = f.ciphertextMonoFreqs();
	vector<vector<char> > cas;
	vector<vector<int> > contactCounts = f.ciphertextContactCounts(0);
	vector<float> monoFreqs = f.ciphertextMonoFreqs(0);
//	vector<int> monoCounts = f.ciphertextMonoCounts(0);
//	parallelQuickSort(monoFreqs, contactCounts, 0, monoFreqs.size() - 1);
	for (int a = 0; a < numAlphabets; a++)
	{
		cmfs.push_back(f.ciphertextMonoCounts(a));
		cas.push_back(ciphertextAlphabet);
//		parallelQuickSort(cmfs[a], cas[a], 0, cmfs[a].size() - 1);
	}

	// Parallel sort if required
//	if (freqsort)
//	{
//		parallelQuickSort(lmf, languageAlphabet, 0, lmf.size() - 1);
//		parallelQuickSort(cmf, ciphertextAlphabet, 0, cmf.size() - 1);
//		paraqsort(languageAlphabet, lmf, 0, lmf.size() - 1);
//		paraqsort(ciphertextAlphabet, cmf, 0, cmf.size() - 1);//, ASCENDING);
//		parallelQuickSort(cdf, digraphs, 0, cdf.size() - 1);
//		parallelQuickSort(ldf, digraphs, 0, ldf.size() - 1);
//		parallelQuickSort(ctf, trigraphs, 0, ctf.size() - 1);
//		parallelQuickSort(csf, symbols, 0, csf.size() - 1);
//	}
//	mdiff = subtractVectors(cmf, lmf);
	// Print out the frequency tables
	WINDOW * freqswin = newwin(LINES - 1, WIDTH, 1, x);// cmf.size() + csf.size(), y, x);
	// Ciphertext stats
//	stringstream stats;
//	stats << "Chars: " << f.getNchars() << " | Spaces: " << f.getNspaces() << " | Symbols: " << f.getNsymbols();
//	mvwprintw(freqswin, ymargin++, 0, stats.str().c_str());
	// Print monograph frequencies
	stringstream heading;
	if (numAlphabets == 1)	// only one alphabet, show contact counts
		heading << " [0]| A B C D E F G H I J K L M N O P Q R S T U V W X Y Z";
	else
		for (int a = 0; a < numAlphabets; a++)
			heading << "Alph [" << a << "]|";
	mvwprintw(freqswin, ymargin++, 0, heading.str().c_str());
	int variety[LETTERS] = { 0 };
	for (int i = 0; i < LETTERS; i++)
	{
		stringstream line;
		// Print ciphertext frequency charts
		if (numAlphabets == 1)	// only one alphabet, show contact counts
		{
			line << cas[0][i];
			if (cmfs[0][i] < 10)
				line << " ";
			line << /* fixed << setprecision(PRECISION) << cas[0][i] << */ ":" << cmfs[0][i] << "|";
			int contacts = 0;
			for (int j = 0; j < contactCounts[i].size(); j++)
			{
				if (contactCounts[i][j])
				{
					contacts++;
					variety[j]++;
					if (contactCounts[i][j] < 10)
						line << " " << contactCounts[i][j];
					else if (contactCounts[i][j] < 100)
						line << contactCounts[i][j] / 10 << contactCounts[i][j] % 10;
					else 
						line << "C" << contactCounts[i][j] % 10;
				}
				else
					line << " .";
			}
			line << "|" << contacts;
		}
		else
			for (int a = 0; a < numAlphabets; a++)
				line << fixed << setprecision(PRECISION) << cas[a][i] << ":" << cmfs[a][i] << "|";
//		// Print language frequency chart for comparison
//		line << fixed << setprecision(PRECISION) << " | " 
//			 << languageAlphabet[i] << ": " << lmf[i];

		mvwprintw(freqswin, ymargin++, 0, line.str().c_str());
	}
//	ymargin++;
	// Print variety counts
	stringstream varietyline;
	varietyline << "----|";
	for (int i = 0; i < LETTERS; i++)
	{
		if (variety[i] < 10)
			varietyline << " ";
		varietyline << variety[i];
	}
	varietyline << "|";
	mvwprintw(freqswin, ymargin++, 0, varietyline.str().c_str());
	ymargin++;
	// Print trigraph periods
	mvwprintw(freqswin, ymargin++, 0, "Trigraph repetition periods:");
	vector<int> periods = f.trigraphPeriods();
	stringstream line;
	for (int i = 0; i < periods.size(); i++)
		line << fixed << periods[i] << " ";
	mvwprintw(freqswin, ymargin++, 0, line.str().c_str());
	// print symbols
//	for (int i = 0; i < csf.size(); i++)
//	{
//		stringstream line;
//		line << fixed << setprecision(PRECISION)
//			 << symbols[i] << ": " << csf[i];
//		mvwprintw(freqswin, i + cmf.size(), 0, line.str().c_str());
//	}

	// Print the top digraphs
//	for (int i = 0; i < PRINTMAX; i++)
//	{
//		stringstream line;
//		line << fixed << setprecision(PRECISION)
//			 << " || " << i << ": " << cdf[i]
//			 << " || " << i << ": " << ldf[i];
//		mvwprintw(freqswin, i + ymargin, xmargin, line.str().c_str());
//	}
//	xmargin += PRECISION + 10;

	// Print the top trigraphs
//	int t = trigraphs.size();
//	if (t > PRINTMAX)
//		t = PRINTMAX;
//	for (int i = 0; i < t; i++)
//	{
//		stringstream line;
//		line << fixed << setprecision(PRECISION)
//			 << " || " << trigraphs[i] << ": " << ctf[i];
//		mvwprintw(freqswin, i + ymargin, xmargin, line.str().c_str());
//	}

	wrefresh(freqswin);
}

/* parallelQuickSort
 * Input: vector to parallelQuickSort
 * Output: vector parallelQuickSorted in decrementing order
 */
template <class M, class S>
void parallelQuickSort(vector<M> & master, vector<S> & slave, int low, int high, SortDirection dir /* = DESCENDING */)
{
	int i = low, j = high;
	M masterTemp;
//	S slaveTemp;

	M x = master[(low+high)/2];

	do
	{
		switch (dir)
		{
			case DESCENDING:
				while (master[i] > x) i++;
				while (master[j] < x) j--;
				break;
			case ASCENDING:
				while (master[i] < x) i++;
				while (master[j] > x) j--;
				break;
		}
		if (i <= j)
		{
			masterTemp = master[i];
			master[i] = master[j];
			master[j] = masterTemp;
			if (! slave.empty())
			{
				S slaveTemp = slave[i];
				slave[i] = slave[j];
				slave[j] = slaveTemp;
			}
			i++;
			j--;
		}
	} while (i <= j);

	switch (dir)
	{
		case DESCENDING:
			if (low < j)
				parallelQuickSort(master, slave, low, j, DESCENDING);
			if (i < high)
				parallelQuickSort(master, slave, i, high, DESCENDING);
			break;
		case ASCENDING:
			if (low < j)
				parallelQuickSort(master, slave, low, j, ASCENDING);
			if (i < high)
				parallelQuickSort(master, slave, i, high, ASCENDING);
			break;
	}
}

int numCols(Grid grid)
{
	int n = 0;
	for (int i = 0; i < grid.size(); i++)
	{
		if (grid[i].length() > n)
			n = grid[i].length();
	}
	return n;
}

vector<vector<int> > hillClimb(vector<Matrix> D, const Matrix & E, vector<vector<int> > & k,
		const vector<vector<int> > & s)
{
	const int FINISHED = 10000;	// max fruitless iterations
	int n = 0;					// count of fruitless iterations

	// Perform hill-climb for each alphabet (number of alphabets
	// given by the number of keys in k)

// Algorithm from Jakobsen paper
	// Initial definition of variables
	int active;		// the current alphabet
	int previous;	// the previous active alphabet
	vector<int> a, b;	// keep track of the iterations of each alphabet
	vector<float> v;	// "closeness score" of each alphabet
	float V;			// total "closeness score"
	// Initialize variables
	for (int alphabet = 0; alphabet < k.size(); alphabet++)
	{
		// Key the digraph distribution matrix D to the initial key k
		Matrix temp = D[alphabet];
		for (int i = 0; i < k[alphabet].size(); i++)
			for (int j = 0; j < k[alphabet].size(); j++)
				D[alphabet][i][j] = temp[k[alphabet][i]][k[alphabet][j]];

//0		// set a = b = 1 for every alphabet
		a.push_back(1);
		b.push_back(1);

//1		// k contains initial keys to try for every alphabet

//2		// D contains initial digraph distribution matrices for each alphabet

//3		// v contains initial "closeness scores" for every alphabet
		v.push_back(difference(D[alphabet], E));
	}
	V = sum(v);
	active = 0;
	previous = k.size() - 1;
//4		// Set k1
		vector<vector<int> > k1 = k;
//5		// Set D1
		vector<Matrix> D1 = D;
	// end variable initialization

	// Begin iterations
	while (n < FINISHED)
	{
		// Secondary variable initialization
		// Reset active alphabet if necessary
		if (active >= k.size())
			active = 0;
//6a	// Swap alpha and beta elements in k1[activeAlphabet]
		int alpha = s[active][a[active]-1];
		int beta = s[active][a[active]+b[active]-1];
		int alphaIndex = getIndex(k1[active], alpha);
		int betaIndex = getIndex(k1[active], beta);
		swapValues(k1[active][alphaIndex], k1[active][betaIndex]);
//6b	// Increment a for active alphabet
		a[active]++;
//6c	// Check if a + b of active alphabet <= LETTERS, then go to step 7
		if (a[active] + b[active] <= LETTERS)
		{
//7			// Exchange the corresponding rows in D1 of the active alphabet
			swapRows(D1[active], alphaIndex, betaIndex);
			// Exchange the corresponding columns in D1 of the PREVIOUS alphabet
			swapCols(D1[previous], alphaIndex, betaIndex);
//8			// Calculate v1 for the active alphabet
			vector<float> v1 = v;
			v1[active] = difference(D1[active], E);
			// Calculate v1 for the previous alphabet
			v1[previous] = difference(D1[previous], E);
			// Calculate overall score V1
			float V1 = sum(v1);
//9a		// Compare V1 and V to see if overall closeness improved
			if (V1 >= V) // If not, revert changes, go to step 4
			{
				//4 Revert k1 back to k
				k1 = k;
				//5 Revert D1 back to D
				D1 = D;
				// Increment alphabets
				previous = active;
				active++;
				// Increment fruitless iterations counter
				n++;
				// Continue from step 6
			}
			else			// score improved
			{
//9b			// Reset a = b = 1 for active alphabet
				a[active] = 1;
				b[active] = 1;
//10			// Update v with improved scores
				v = v1;
				V = V1;
//11			// Update k with improved keys
				k = k1;
//12			// Update D with improved distriubion matrices
				D = D1;
//13			// Go to step 6
				// Previous changes improved score, reset fruitless counter
				n = 0;
				// Increment alphabets
				previous = active;
				active++;
			}
		}
		else	// a + b > LETTERS for active alphabet
		{
//6d		// Reset a for the active alphabet
			a[active] = 1;
//6e		// Increment b for the active alphabet
			b[active]++;
//6f		// Check that b does not exceed LETTERS for active alphabet
			if (b[active] >= LETTERS)
				b[active] = 1;	// if so, reset b for active alphabet
		}
	} // end while loop

	return k;

/****************************************************************
 ****************************************************************
 * 
 * WORKING MONOALPHABETIC ALGORITHM
 *
 ****************************************************************
 ****************************************************************

	// Key the digraph distribution matrix D to the initial key k
	Matrix temp = D;
	for (int i = 0; i < k.size(); i++)
		for (int j = 0; j < k.size(); j++)
			D[i][j] = temp[k[i]][k[j]];

// Algorithm from Jakobsen paper
//0
	int a = 1, b = 1;
//3
	float v = difference(D, E);
	while (n < FINISHED)
	{
//4
		vector<int> k1 = k;
//5
		vector<vector<float> > D1 = D;
//6a
		int alpha = s[a-1], beta = s[a+b-1];
		int alphaIndex = getIndex(k1, alpha);
		int betaIndex = getIndex(k1, beta);
		swapValues(k1[alphaIndex], k1[betaIndex]);
//6b
		a++;
//6c
		if (a + b <= LETTERS)	// go to step 7
		{
//7
			swapRows(D1, alphaIndex, betaIndex);
			swapCols(D1, alphaIndex, betaIndex);
//8
			float v1 = difference(D1, E);
//9a
			if (v1 >= v)	// go to step 4
				n++;	// previous change did not imporove key, increment counter
			else
			{
//9b
				a = 1;
				b = 1;
//10
				v = v1;
//11
				k = k1;
//12
				D = D1;
//13
				// previous change improved key, reset counter
				n = 0;
				// go to step 6
			}
		}
		else
		{
//6d
			a = 1;
//6e
			b++;
//6f
			if (b >= 26)
				b = 1;//break;
		}

	}

	return k;
*******************************************************************/
}

/* hillClimb (Caesar/Vigenere Ciphers variant)
 * Input: monograph frequencies for each alphabet; monograph frequencies for the language;
 *		  vector of initial keys
 * Output: optimizes each alphabet according to the frequencies, and set the keys
 *		   by shifting each alphabet until it matches the language frequencies
 *		   The calculated keys are returned to be used in decryption
 */
vector<vector<int> > hillClimb(vector<vector<float> > D, const vector<float> & E, vector<vector<int> > & k)
{
	// Each key is shifted 26 times (the number of key letters),
	//  and the optimum shift is kept
	for (int a = 0; a < k.size(); a++)
	{
		float v = difference(D[a], E);
		vector<int> k1 = k[a];
		vector<float> D1 = D[a];
		for (int i = 0; i < k1.size(); i++)
		{
			shift(k1, 1);	// shift forward 1 step
			shift(D1, 1);
			float v1 = difference(D1, E);
			// Check if score has improved
			if (v1 < v)	// improvement
			{
				// Set the best key to the current shift
				k[a] = k1;
				D[a] = D1;
				v = v1;
			}
			// if no improvement, don't set the best key, but continue to shift k1/D1
		}
		// k[a] now has the best key, move onto the next key
	}

	// Finished optimization shifting
	return k;
}

template <class T>
void shift(vector<T> & v, int step)
{
	vector<T> temp;
	if (step > 0)	// shift forward
	{
		// save the ends
		for (int i = v.size() - step; i < v.size(); i++)
			temp.push_back(v[i]);
		// shift all the other cells forward
		for (int i = v.size() - 1; i >= step; i--)
			v[i] = v[i-step];
		// place ends at the beginning
		for (int i = 0; i < temp.size(); i++)
			v[i] = temp[i];
	}
	else if (step < 0)	// shift backwards
	{
		step *= -1;
		// save the beginnings
		for (int i = 0; i < step; i++)
			temp.push_back(v[i]);
		// shift all other cells backwards
		for (int i = 0; i < v.size() - step; i++)
			v[i] = v[i+step];
		// place beginnings at the end
		for (int i = 0; i < temp.size(); i++)
			v[v.size()-step+i] = temp[i];
	}
	// else do nothing
}

float difference(const vector<vector<float> > & D, const vector<vector<float> > & E)
{
	float sum = 0.0;
	for (int i = 0; i < D.size(); i++)
		for (int j = 0; j < D[i].size(); j++)
			sum += abs(D[i][j] - E[i][j]);
	return sum;
}

float difference(const vector<float> & D, const vector<float> & E)
{
	float sum = 0.0;
	for (int i = 0; i < D.size(); i++)
		sum += abs(D[i] - E[i]);
	return sum;
}

float sum(const vector<float> & v)
{
	float sum = 0.0;
	for (int i = 0; i < v.size(); i++)
		sum += v[i];
	return sum;
}

int getIndex(const vector<int> & k, int alpha)
{
	for (int i = 0; i < k.size(); i++)
		if (k[i] == alpha)
			return i;
	return -1;
}

void swapValues(int & i, int & j)
{
	int temp = i;
	i = j;
	j = temp;
}

void swapRows(vector<vector<float> > & D, int a, int b)
{
	// swap rows
	vector<float> temp = D[a];
	D[a] = D[b];
	D[b] = temp;
}

void swapCols(vector<vector<float> > & D, int a, int b)
{	
	// swap cols
	for (int i = 0; i < D.size(); i++)
	{
		float temp = D[i][a];
		D[i][a] = D[i][b];
		D[i][b] = temp;
	}	
}

void drawBusyWin()
{
	WINDOW * busywin = newwin(3, 15, LINES / 2 - 3, COLS / 2 - 40);
	init_pair(1, COLOR_RED, COLOR_BLACK);
	wattron(busywin, COLOR_PAIR(1));
	wborder(busywin, '|', '|', '-', '-', '+', '+', '+', '+');
	mvwprintw(busywin, 1, 1, "Analyzing...");
	wrefresh(busywin);
	wattroff(busywin, COLOR_PAIR(1));
}

void drawStatsWin(int nchars, int nspaces, int nsymbols, int numAlphabets, int x)
{
	const int WIDTH = 40;
	const int HEIGHT = 2;
	const int YPOS = 1;
	WINDOW * statswin = newwin(HEIGHT, WIDTH, YPOS, x);
	stringstream stats;
	stats << nchars << " letters, " << nspaces << " spaces, " << nsymbols << " symbols" << endl;
	stats << "Analysis with " << numAlphabets << " alphabets.";
	mvwprintw(statswin, 0, 0, stats.str().c_str());
	wrefresh(statswin);
}

void drawKeysWin(const vector<string> & keys, int x)
{
	const int WIDTH = LETTERS + 12;
	const int HEIGHT = keys.size() + 1;
	const int YPOS = 5;
	WINDOW * keyswin = newwin(HEIGHT, WIDTH, YPOS, x);
	mvwprintw(keyswin, 0, 0, "Plaintext: abcdefghijklmnopqrstuvwxyz");//abcdefghijklmnopqrstuvwxyz");
	for (int i = 0; i < keys.size(); i++)
	{
		stringstream key;
		key << "Key [" << i << "]:   " << keys[i];
		mvwprintw(keyswin, i + 1, 0, key.str().c_str());
	}
	wrefresh(keyswin);
}

int getKey(const Grid & grid, int y, int x, int n)
{
	int key = n;
	for (int i = 0; i <= y; i++)
	{
		for (int j = 0; j <= x; j++)
		{
			if (isAlpha(grid[i][j]))
			{
				key++;
				if (key >= n)
					key = 0;
			}
		}
	}
	return key;
}

int drawNumAlphabetsWin()
{
	WINDOW * keylengthwin = newwin(3, 40, LINES / 2 - 3, COLS / 2 - 40);
	init_pair(1, COLOR_RED, COLOR_BLACK);
	wattron(keylengthwin, COLOR_PAIR(1));
	wborder(keylengthwin, '|', '|', '-', '-', '+', '+', '+', '+');
	mvwprintw(keylengthwin, 1, 1, "Number of alphabets/key length: [1-9]");
	wrefresh(keylengthwin);
	wattroff(keylengthwin, COLOR_PAIR(1));
	int input = getch();
	return input;
}

void drawAnalysisWin(const vector<string> & cipherGrid, const vector<string> & plainGrid, int cursory, int cursorx)
{
	// The window containing the ciphertext/plaintext currently under analysis
	WINDOW * analysiswin = newwin(LINES - 3, 70, 1, 0);
	// Print the plaintext above the ciphertext
	int ymargin = 0;
	for (int i = 0; i < cipherGrid.size(); i++)
	{
		mvwprintw(analysiswin, ymargin++, 0, plainGrid[i].c_str());
		mvwprintw(analysiswin, ymargin++, 0, cipherGrid[i].c_str());
	}
	// Place cursor at the start
	wmove(analysiswin, cursory, cursorx);
	wrefresh(analysiswin);
}
