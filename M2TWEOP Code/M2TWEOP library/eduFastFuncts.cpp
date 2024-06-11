#include "eduFastFuncts.h"

#include <sstream> 

#include "FastFuncts.h"
#include "functionsOffsets.h"
#include "unitActions.h"

namespace eduFastFuncts
{
	struct pseudoFile {
		char* _ptr;
		int _cnt;
		char* _base;
		int _flag;
		char* _file;
		char* _charbuf;
		int _bufsiz;
		char* _tmpfname;
	};

	static void prepareEduEntry(eduEntry*& entryForFilling)
	{
		GAME_FUNC(void(__thiscall*)(eduEntry*), createEduEntry)(entryForFilling);
	}

	std::string uniStringToStr(UNICODE_STRING**& uniString)
	{
		UNICODE_STRING* uniS = *uniString;
		wchar_t* wstr = (wchar_t*)&uniS->Buffer;

		std::string strTo;
		int wchars_num = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, NULL, 0, NULL, NULL);
		if (wchars_num <= 0)
		{
			return strTo;
		}
		char* szTo = new char[wchars_num];
		szTo[wchars_num-1] = '\0';
		WideCharToMultiByte(CP_UTF8, 0, wstr, -1, szTo, wchars_num, NULL, NULL);

		strTo = szTo;
		delete[] szTo;

		return strTo;
	}
	
	static std::string replace_all(
		const std::string& str,   // where to work
		const std::string& find,  // substitute 'find'
		const std::string& replace //      by 'replace'
	) {
		using namespace std;
		string result;
		size_t find_len = find.size();
		size_t pos, from = 0;
		while (string::npos != (pos = str.find(find, from))) {
			result.append(str, from, pos - from);
			result.append(replace);
			from = pos + find_len;
		}
		result.append(str, from, string::npos);
		return result;
	}


    int readEduFile(std::string fileName, eduEntry* entryForFilling)
    {
		unitActions::logStringGame("Reading EDU file: " + fileName);
		std::string unitString;

		std::ifstream eduFile;
		eduFile.open(fileName);

		std::stringstream strStream;
		strStream << eduFile.rdbuf();
		unitString = strStream.str(); 
		eduFile.close();
		unitString=replace_all(unitString,"\n", " \n");
		prepareEduEntry(entryForFilling);

		pseudoFile ourF;
		ourF._ptr = (char*)unitString.c_str();
		ourF._cnt = unitString.size();
		ourF._base = (char*)(unitString.c_str()+ unitString.size());
		ourF._flag = (int)unitString.c_str();
		ourF._file = (char*)unitString.c_str();
		ourF._charbuf = (char*)unitString.c_str();
		ourF._bufsiz = unitString.size();
		ourF._tmpfname = (char*)&ourF;
		pseudoFile* pp = &ourF;

		int res = 0;

		DWORD adrFunc = codes::offsets.readEDUEntryFunc;
		_asm
		{
			push pp
			mov ecx, entryForFilling
			mov eax, adrFunc
			call eax
			mov res,eax
		}
        return res;
    }
};
