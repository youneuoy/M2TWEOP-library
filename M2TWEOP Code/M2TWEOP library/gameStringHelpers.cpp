#include "pch.h"
#include "gameStringHelpers.h"
#include "globals.h"
#include "functionsOffsets.h"
#include "dataOffsets.h"
#include "techFuncs.h"

void stringWithHash::setString(const std::string& str)
{
	gameStringHelpers::setHashedStringGame(this, str.c_str());
}

namespace gameStringHelpers
{
	void setHashedString(char** targetS, const char* newS)
	{
		GAME_FUNC(void(__thiscall*)(char**, const char*), stringCryptFunc)(targetS, newS);
	}
	
	void setHashedString(const char** targetS, const char* newS)
	{
		GAME_FUNC(void(__thiscall*)(const char**, const char*), stringCryptFunc)(targetS, newS);
	}
	
	void setHashedStringGame(stringWithHash* str, const char* newS)
	{
		GAME_FUNC(void(__thiscall*)(stringWithHash*, const char*), stringCryptFunc)(str, newS);
	}

	

	void freeHashString(stringWithHash* str)
	{
		GAME_FUNC(void(__thiscall*)(stringWithHash*), freeHashString)(str);
	}
	
	char** createHashedString(const char* string)
	{
		const auto cryptS = static_cast<char**>(malloc(8));
		if (cryptS == nullptr)
		{
			MessageBoxA(nullptr, "Cannot allocate memory for m2tweop", "ERROR!", NULL);
			exit(0);
		}
		memset((void*)cryptS, 0, 8);
		setHashedString(cryptS, string);
		return cryptS;
	}
	
	stringWithHash* createHashedStringGame(const char* string)
	{
		stringWithHash* hashedString = techFuncs::createGameClass<stringWithHash>();
		setHashedStringGame(hashedString, string);
		return hashedString;
	}
	
	std::string wstrToAnsiStr(const std::wstring& wstr)
	{
		if (wstr.empty()) return {};
		const auto szTo = new char[wstr.length() + 1];
		szTo[wstr.size()] = '\0';
		WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), -1, szTo, static_cast<int>(wstr.length()), nullptr, nullptr);
		std::string strTo = szTo;
		delete[] szTo;
		return strTo;
	}
	
	std::wstring ansiStrToWstr(const std::string& str)
	{
		if (str.empty()) return {};
		const int sizeNeeded = MultiByteToWideChar(CP_UTF8, 0, str.data(), static_cast<int>(str.size()), nullptr, 0);
		std::wstring wstrTo(sizeNeeded, 0);
		MultiByteToWideChar(CP_UTF8, 0, str.data(), static_cast<int>(str.size()), wstrTo.data(), sizeNeeded);
		return wstrTo;
	}
	
	void createUniString(UNICODE_STRING**& newUniStringPointer, const char* nonUniStr)
	{
		DWORD funcAdr = codes::offsets.createUniString;
		size_t lenS = strlen(nonUniStr);
		newUniStringPointer = reinterpret_cast<UNICODE_STRING**>(techFuncs::allocateGameMem((lenS + 6 + 4) * 2));
		UNICODE_STRING** uniStr = newUniStringPointer;
		_asm {
			mov ecx, uniStr
			push nonUniStr
			mov eax, funcAdr
			call eax
		}
		newUniStringPointer = reinterpret_cast<UNICODE_STRING**>(*uniStr);
		int wCharsNum = MultiByteToWideChar(CP_UTF8, 0, nonUniStr, -1, nullptr, 0);
		auto wStr = new wchar_t[wCharsNum];
		MultiByteToWideChar(CP_UTF8, 0, nonUniStr, -1, wStr, wCharsNum);
		wstring utf16Line = wStr;
		delete[] wStr;
		(*newUniStringPointer)->Length = static_cast<USHORT>(utf16Line.size());
		(*newUniStringPointer)->something2 = static_cast<USHORT>(utf16Line.size());
		auto ptr = reinterpret_cast<unsigned short*>(&(*newUniStringPointer)->Buffer);
		for (UINT32 i = 0; i < utf16Line.size(); i++)
			*(ptr + i) = utf16Line[i];
		*(ptr + utf16Line.size()) = static_cast<unsigned short>(0);
	}
	
	std::string uniStringToStr(UNICODE_STRING**& uniString)
	{
		try
		{
			UNICODE_STRING* uniS = *uniString;
			const wchar_t* wstr = reinterpret_cast<wchar_t*>(&uniS->Buffer);
			std::string strTo;
			const int wCharsNum = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, nullptr, 0, nullptr, nullptr);
			if (wCharsNum <= 0)
				return strTo;
			const auto szTo = new char[wCharsNum];
			szTo[wCharsNum-1] = '\0';
			WideCharToMultiByte(CP_UTF8, 0, wstr, -1, szTo, wCharsNum, nullptr, nullptr);
			strTo = szTo;
			delete[] szTo;
			return strTo;
		}
		catch (...)
		{
			return "";
		}
	}
}

