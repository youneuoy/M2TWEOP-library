#pragma once
#include <windows.h>
#include <vector>
#include <string>

#include "functionsOffsets.h"
#include "realGameTypes.h"
#include "imgui/ImFileDialog.h"

namespace techFuncs
{
	void writeData(const void* ptr, DWORD to, size_t size);

	void nopBytes(DWORD address, size_t size);

	DWORD allocateGameMem(size_t amount);
	
	std::string readFile(const std::filesystem::path& path);

	template <typename  data>
	void read(DWORD from, data* p, size_t size = 4)
	{
		memset(p, 0, size);
		memcpy(p, (LPVOID)from, size);
	}
	template<typename T>
	T* allocateGameClass(size_t amount)
	{
		DWORD retMem = 0;
		DWORD adrFunc = codes::offsets.allocMemFunc;
		_asm
		{
			push amount
			mov eax, adrFunc
			call eax
			add esp, 0x4
			mov retMem, eax
		}
		return reinterpret_cast<T*>(retMem);
	}
	
	template<typename T>
	T* createGameClass()
	{
		DWORD retMem = 0;
		size_t amount = sizeof(T);
		DWORD adrFunc = codes::offsets.allocMemFunc;
		_asm
		{
			push amount
			mov eax, adrFunc
			call eax
			add esp, 0x4
			mov retMem, eax
		}
		return reinterpret_cast<T*>(retMem);
	}
	
	std::vector<std::string>unzip(std::string const& zipFile, std::string const& path);
	void zip(std::string const& zipFile, std::vector<std::string>& files, const std::string& saveFile, const std::string& nameOfSaveFile);

	std::wstring convertUtf8ToWide(const std::string& str);

	std::string uniToAnsi(UNICODE_STRING**& uniStr);

	void deleteFiles(std::vector<std::string>& files);
	
	std::vector<std::string> getEopArchiveFiles(const std::string& savePath);

	//create archive with files
	void saveGameMakeArchive(UNICODE_STRING**& savePath, const std::vector<std::string>& files);

	//unpack archive
	std::vector<std::string> loadGameLoadArchive(std::vector<std::string> files, UNICODE_STRING**& savePath);
};

