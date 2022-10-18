#include "fastFunctsHelpers.h"

#include <filesystem>
namespace fastFunctsHelpers
{
	NOINLINE EOP_EXPORT void setCryptedString(char** targetS, const char* newS)
	{
		UINT32 functionOffset = codes::offsets.stringCryptFunc;
		_asm {
			push newS
			mov ecx, targetS
			mov eax, functionOffset
			call eax
		}
	}
	int getEDUIndex(const char* type)
	{
		eduEntryes* EDB = reinterpret_cast<eduEntryes*>(dataOffsets::offsets.unitTypesStart - 4);

		int unitsNum = EDB->numberOfTupes;
		for (int i = 0; i < unitsNum; i++)
		{
			if (strcmp(EDB->unitTupes[i].Type, type) == 0)
			{
				return i;
			}
		}

		return -1;
	}
	EduEntry* getEDUEntry(const char* type)
	{
		eduEntryes* EDB = reinterpret_cast<eduEntryes*>(dataOffsets::offsets.unitTypesStart - 4);

		int unitsNum = EDB->numberOfTupes;
		for (int i = 0; i < unitsNum; i++)
		{
			if (strcmp(EDB->unitTupes[i].Type, type) == 0)
			{
				return &EDB->unitTupes[i];
			}
		}
		return nullptr;
	}
	EduEntry* getEDUEntryById(int id)
	{
		eduEntryes* EDB = reinterpret_cast<eduEntryes*>(dataOffsets::offsets.unitTypesStart - 4);

		int unitsNum = EDB->numberOfTupes;
		for (int i = 0; i < unitsNum; i++)
		{
			if (EDB->unitTupes[i].Index==id)
			{
				return &EDB->unitTupes[i];
			}
		}
		return nullptr;
	}
	char** makeCryptedString(const char* string)
	{
		char** cryptS = (char**)malloc(8);
		if (cryptS == nullptr)
		{
			MessageBoxA(NULL, "Cannot allocate memory for m2tweop", "ERROR!", NULL);
			exit(0);
			return nullptr;
		}
		memset(cryptS, 0, 8);
		//cryptS[0] = _strdup(string);
		setCryptedString(cryptS, string);
		return cryptS;
	}
	std::string wstrToANSIStr(const std::wstring& wstr)
	{		if (wstr.empty()) return std::string();

		std::string strTo;
		char* szTo = new char[wstr.length() + 1];
		szTo[wstr.size()] = '\0';
		WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), -1, szTo, (int)wstr.length(), NULL, NULL);

		strTo = szTo;
		delete[] szTo;

		return strTo;

	}
	std::wstring ANSIStrToWstr(const std::string& str)
	{
		if (str.empty()) return std::wstring();
		int size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);
		std::wstring wstrTo(size_needed, 0);
		MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &wstrTo[0], size_needed);
		return wstrTo;
	}


	namespace fs = std::filesystem;

	std::string readFile(fs::path path)
	{
		// Open the stream to 'lock' the file.
		std::ifstream f(path, std::ios::in | std::ios::binary);

		// Obtain the size of the file.
		const auto sz = fs::file_size(path);

		// Create a buffer.
		std::string result(sz, '\0');

		// Read the whole file into the buffer.
		f.read(result.data(), sz);

		return result;
	}

	jsn::json loadJsonFromFile(const std::string& fpath)
	{
		jsn::json json;

		std::string content = readFile(fpath);

		json = jsn::json::parse(content,
			/* callback */ nullptr,
			/* allow exceptions */ true,
			/* ignore_comments */ true);

		return json;
	}
	jsn::json loadJsonFromFile(const std::wstring& fpath)
	{
		jsn::json json;


		std::string content = readFile(fpath);

		json = jsn::json::parse(content,
			/* callback */ nullptr,
			/* allow exceptions */ true,
			/* ignore_comments */ true);

		return json;
	}

}

