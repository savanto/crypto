#ifndef _STRINGMANIP_H
#define _STRINGMANIP_H

#include <string>
using namespace std;

#define NEWLINE '\n'
#define UPPER 65
#define LOWER 97
#define NUMBER 48
#define UPPER2LOWER 32
#define LETTERS 26
#define NUMBERS 10

string strip(string s);
string niceStrip(string s);
string space(string s);

char toLower(int i);
char toLower(char c);
string toLower(string s);

char toUpper(int i);
char toUpper(char c);
string toUpper(string s);

int toBase(int ch);

bool isUpper(char c);
bool isLower(char c);
bool isAlpha(char c);
bool isWhitespace(char c);
bool isNum(char c);
bool isPrintable(char c);

string testing(string s);
#endif // _STRINGMANIP_H
