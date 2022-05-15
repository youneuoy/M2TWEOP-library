#pragma once
#include <windows.h>
#include <vector>
#include <string>

#include "realGameTypes.h"

namespace techFuncs
{
	template <typename  data>
	void Read(DWORD from, data* p, size_t size = 4)
	{
		memset(p, 0, size);
		memcpy(p, (LPVOID)from, size);
	}

	std::vector<std::string>unzip(std::string const& zipFile, std::string const& path);
	void zip(std::string const& zipFile, std::vector<std::string>& files,std::string saveFile,std::string nameOfSaveFile);

	std::string uniToANSI(UNICODE_STRING**& uniStr);


	void deleteFiles(std::vector<std::string>& files);

	//create archive with files
	void saveGameMakeArchive(UNICODE_STRING**& savePath, std::vector<std::string>& files);

	//unpack archive
	std::vector<std::string> loadGameLoadArchive(UNICODE_STRING**& savePath);
};

