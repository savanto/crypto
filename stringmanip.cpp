#include "stringmanip.h"


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

string toLower(string s)
{
	string lower = "";
	for (int i = 0; i < s.length(); i++)
	{
		if (isUpper(s[i]))//(int(s[i]) >= 65 && int(s[i]) <= 90)
			lower += char(int(s[i]) + 32);
		else
			lower += s[i];
	}

	return lower;
}

char toLower(char c)
{
	if (isUpper(c))
		return c + UPPER2LOWER;
	else
		return c;
}

char toLower(int i)
{
	if (isUpper(char(i + UPPER)))
		return char(i + LOWER);
	else
		return char(i + UPPER);
}

string strip(string s)
{
	string stripped = "";
	for (int i = 0; i < s.length(); i++)
	{
		if (isAlpha(s[i]))//((int(s[i]) >= 65 && int(s[i]) <= 90) || (int(s[i]) >= 97 && int(s[i]) <= 122))
			stripped += s[i];
	}
	return stripped;
}

string niceStrip(string s)
{
	string stripped = "";
	for (int i = 0; i < s.length(); i++)
	{
		if (isPrintable(s[i]))
			stripped += s[i];
	}
	return stripped;
}

string toUpper(string s)
{
	string upper = "";
	for (int i = 0; i < s.length(); i++)
	{
		if (isLower(s[i]))//if (int(s[i]) >= LOWER && int(s[i]) <= LOWER + LETTERS)
			upper += char(int(s[i]) + UPPER2LOWER);
		else
			upper += s[i];
	}

	return upper;
}

char toUpper(int i)
{
	if (isLower(char(i + LOWER)))
		return char(i + UPPER);
	else
		return char(i);
}

char toUpper(char c)
{
	if (isLower(c))
		return c - UPPER2LOWER;
	else
		return c;
}

bool isUpper(char c)
{
	if (int(c) >= int('A') && int(c) <= int('Z'))
		return true;
	else
		return false;
}

bool isLower(char c)
{  
	if (int(c) >= int('a') && int(c) <= int('z'))
		return true;
	else
		return false;
}

bool isAlpha(char c)
{
	if (isUpper(c) || isLower(c))
		return true;
	else
		return false;
}

bool isWhitespace(char c)
{
	if (int(c) < 33 || int(c) > 126)
		return true;
	else
		return false;
}

bool isNum(char c)
{
	if (int(c) >= 48 && int(c) <= 57)
		return true;
	else
		return false;
}

bool isPrintable(char c)
{
	if (int(c) >= 32 && int(c) <= 126)
		return true;
	else
		return false;
}

int toBase(int ch)
{
	if (ch >= int('A') && ch <= int('Z'))
		return ch - int('A');
	else if (ch >= int('a') && ch <= int('z'))
		return ch - int('a');
	else
		return -1;
}

string testing(string s)
{
	string tested = "";
	for (int i = 0; i < s.length(); i++)
	{
		if (! isPrintable(s[i]))
			tested += "@\n";
		else
			tested += s[i];
	}
	return tested;
}
