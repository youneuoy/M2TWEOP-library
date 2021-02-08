#pragma once
#include"headersSTD.h"
template <typename param_type>
void findIntParam(param_type* arg, string s, string param)
{
	if (s.find(param) != string::npos && s[0] != ';')
	{
		int pos = s.find_first_of("0123456789");
		int endpos = s.find_first_not_of("0123456789", pos + 1);
		string result = s.substr(pos, endpos - pos);
		*arg = stoi(result);
	}
}
class technicalStringsFuncs
{
public:
	static void findOWParam(int* arg, string s, string param);
	static void readStringInQuotes(string* arg, string s, string param);
	static void findFloatParam(float* arg, string s, string param);
private:
	static void replaceAll(string& s, const string& search, const string& replace);
};

