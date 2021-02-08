#include "pch.h"
#include "technicalStringsFuncs.h"

void technicalStringsFuncs::findOWParam(int* arg, string s, string param)
{
	if (s.find(param) != string::npos && s[0] != ';')
	{
		int pos = s.find_first_of("0123456789");
		int endpos = s.find_first_not_of("0123456789", pos + 1);
		string result = s.substr(pos, endpos - pos);
		*arg = stoi(result);
	}
}

void technicalStringsFuncs::readStringInQuotes(string* arg, string s, string param)
{
	if (s.find(param) != string::npos && s[0] != ';')
	{
		size_t pos = s.find_first_of("\"");
		size_t endpos = s.find_last_of("\"");
		if (pos == string::npos || endpos == string::npos || pos == endpos) return;
		pos++;
		*arg = s.substr(pos, endpos - pos);
	}
	string f = "\\n";
	string r = "\n";
	replaceAll(*arg, f, r);
}

void technicalStringsFuncs::replaceAll(string& s, const string& search, const string& replace)
{
	for (size_t pos = 0; ; pos += replace.length()) {

		pos = s.find(search, pos);
		if (pos == string::npos) break;

		s.erase(pos, search.length());
		s.insert(pos, replace);
	}
}

void technicalStringsFuncs::findFloatParam(float* arg, string s, string param)
{
	if (s.find(param) != string::npos && s[0] != ';')
	{
		int pos = s.find_first_of("0123456789.");
		int endpos = s.find_first_not_of("0123456789.", pos + 1);
		string result = s.substr(pos, endpos - pos);
		*arg = stof(result);
	}
}
