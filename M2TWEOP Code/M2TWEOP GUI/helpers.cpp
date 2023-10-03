#include "helpers.h"
#include "dataG.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <TlHelp32.h>
std::string helpers::makeFString(const char* fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	std::vector<char> v(1024);
	while (true)
	{
		va_list args2;
		va_copy(args2, args);
		int res = vsnprintf(v.data(), v.size(), fmt, args2);
		if ((res >= 0) && (res < static_cast<int>(v.size())))
		{
			va_end(args);
			va_end(args2);
			return std::string(v.data());
		}
		size_t size;
		if (res < 0)
			size = v.size() * 2;
		else
			size = static_cast<size_t>(res) + 1;
		v.clear();
		v.resize(size);
		va_end(args2);
	}
}

bool helpers::loadTexture(const char* filename, GLuint* out_texture, int* out_width, int* out_height)
{
	// Load from file
	int image_width = 0;
	int image_height = 0;
	unsigned char* image_data = stbi_load(filename, &image_width, &image_height, NULL, 4);
	if (image_data == NULL)
		return false;

	// Create a OpenGL texture identifier
	GLuint image_texture;
	glGenTextures(1, &image_texture);
	glBindTexture(GL_TEXTURE_2D, image_texture);

	// Setup filtering parameters for display
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Upload pixels into texture
#if defined(GL_UNPACK_ROW_LENGTH) && !defined(__EMSCRIPTEN__)
	glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
#endif
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
	stbi_image_free(image_data);

	*out_texture = image_texture;
	*out_width = image_width;
	*out_height = image_height;

	return true;
}

void helpers::closeGame(const string& exeName)
{
	HANDLE gameHNDL = NULL;
	PROCESSENTRY32 Pc = { sizeof(PROCESSENTRY32) };
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPALL, 0);
	if (Process32First(hSnapshot, &Pc)) {
		do {
			if (!strcmp(Pc.szExeFile, exeName.c_str())) {
				gameHNDL= OpenProcess(PROCESS_ALL_ACCESS, TRUE, Pc.th32ProcessID);
				break;
			}
		} while (Process32Next(hSnapshot, &Pc));
	}
	if (gameHNDL == NULL)return;

	DWORD fdwExit = 0;
	GetExitCodeProcess(gameHNDL, &fdwExit);
	TerminateProcess(gameHNDL, fdwExit);
	CloseHandle(gameHNDL);
}




void helpers::updateMetrics()
{
	dataG::data.screen.screenSize.x = (float)GetSystemMetrics(SM_CXSCREEN);
	dataG::data.screen.screenSize.y = (float)GetSystemMetrics(SM_CYSCREEN);

	dataG::data.screen.screenHalfSize.x = dataG::data.screen.screenSize.x / 2;
	dataG::data.screen.screenHalfSize.y = dataG::data.screen.screenSize.y / 2;

	dataG::data.screen.screenUpperPos.x = dataG::data.screen.screenSize.x / 2;
	dataG::data.screen.screenUpperPos.y = dataG::data.screen.screenSize.y *0.3f;
}

screenS& helpers::getScreen()
{
	return dataG::data.screen;
}

GLImage* helpers::findImage(const char* name, int nameLen)
{
	for (GLImage* img : dataG::data.staticImagesCollection)
	{
		if (img->path.size() != nameLen)continue;

		if (strncmp(name, img->path.c_str(), nameLen) == 0)
		{
			return img;
		}
	}
	return findImage("empty", 5);
}

ImFont* helpers::findFont(const char* name)
{
	for (fontS* fnt : dataG::data.staticFontsCollection)
	{
		if (strcmp(name, fnt->name.c_str()) == 0)
		{
			return fnt->font;
		}
	}
	return findFont("mainFont");
}
#include <boost/process.hpp>

void helpers::runGame(const char* exeFile, const char* exeParam)
{
	namespace bp = boost::process;
	string line;
	line+= "\"";
	line += dataG::data.gameData.gamePath;
	line += "\"";

	line += exeParam;
	bp::child c(bp::cmd(line)
		, bp::start_dir = "..\\.."
	);
	c.wait();

}

bool helpers::addModPathArg(string& args, int gameMode)
{
	if (gameMode == 2)
	{
		args += "--features.mod=mods/americas ";
	}
	else if (gameMode == 3)
	{
		args += "--features.mod=mods/british_isles ";
	}

	else if (gameMode == 4)
	{
		args += "--features.mod=mods/crusades ";
	}

	else if (gameMode == 5)
	{
		args += "--features.mod=mods/teutonic ";
	}
	else
	{
		return false;
	}

	return true;
}

#define BOOST_DATE_TIME_NO_LIB 1
#include <boost/interprocess/windows_shared_memory.hpp>
#include <boost/interprocess/mapped_region.hpp>
bool helpers::doPipe(const string& message, int waitSeconds)
{
	namespace bip = boost::interprocess;



	//Create a native windows shared memory object.
	bip::windows_shared_memory shm(bip::create_only, "M2TWEOPStartMem1", bip::read_write, message.size() + 5);

	//Map the whole shared memory in this process
	bip::mapped_region region(shm, bip::read_write);
	char* adr = reinterpret_cast<char*>(region.get_address());
	for (int i = 0; i < message.size(); i++)
	{
		adr[i] = message[i];
	}
	adr[message.size()] = 0;

	adr = (char*)region.get_address();

	ULONGLONG startTime = GetTickCount();
	ULONGLONG endTime = startTime + 1000ull * waitSeconds;

	int responce = 1;
	do
	{
		responce = *adr;

		Sleep(1);
	} while (responce != 0 && GetTickCount() < endTime);
	Sleep(1000);
	if (responce == 0)
	{
		return true;
	}

	return false;
}



int makeFullPath(string& path)
{
	TCHAR fullPath[MAX_PATH];
	int iret = GetFullPathNameA(path.c_str(), MAX_PATH, fullPath, NULL);
	path = fullPath;

	return iret;
}

int getVersion(string& exePath)
{
	ifstream f1(exePath, ios::binary);

	if (!f1.is_open())
	{
		return 0;
	}
	DWORD checkOffset = 0x00a04f4c - 0x00400000;
	DWORD read = 0;
	int ver = 3;

	f1.seekg(checkOffset);
	f1.read(reinterpret_cast<char*>(&read), 4);

	f1.close();

	DWORD steam = 0x7ce239e8;
	DWORD disk = 0xb60f0350;
	if (read == steam)
		ver = 2;
	else if (read == disk)
		ver = 1;

	if (!makeFullPath(exePath))
	{
		return 0;
	}

	return ver;
}

bool helpers::selectGameExe(int gameMode)
{
	setModFolder(dataG::data.gameData.modFolder);
	string exePath = "..\\..\\medieval2.exe";
	int gameVersion = getVersion(exePath);
	if (gameMode == 1)
	{
		if (gameVersion != 0)
		{
			dataG::data.gameData.exeName = "medieval2.exe";
			dataG::data.gameData.gameVer = gameVersion;
			dataG::data.gameData.gamePath = exePath;
			return true;
		}
	}
	if (gameVersion == 2 || gameVersion == 1)
	{
		dataG::data.gameData.exeName = "medieval2.exe";
		dataG::data.gameData.gameVer = gameVersion;
		dataG::data.gameData.gamePath = exePath;
		return true;
	}

	exePath = "..\\..\\kingdoms.exe";
	gameVersion = getVersion(exePath);
	if (gameVersion == 2 || gameVersion == 1)
	{
		dataG::data.gameData.exeName = "kingdoms.exe";
		dataG::data.gameData.gameVer = gameVersion;
		dataG::data.gameData.gamePath = exePath;
		return true;
	}

	return false;
}

bool helpers::compareFiles(string& oneFile, string& nextFile)
{
	ifstream in1(oneFile, ios::binary);

	ifstream in2(nextFile, ios::binary);
	if (in1.is_open() == false)
	{
		in1.close();
		in2.close();
		return false;
	}
	if (in2.is_open() == false)
	{
		MessageBoxA(NULL, "Cannot run M2TWEOP, missing dll. Reinstall M2TWEOP.", "ERROR", MB_OK);
		in1.close();
		in2.close();

		exit(0);
		return false;
	}

	ifstream::pos_type size1, size2;

	size1 = in1.seekg(0, ifstream::end).tellg();
	in1.seekg(0, ifstream::beg);

	size2 = in2.seekg(0, ifstream::end).tellg();
	in2.seekg(0, ifstream::beg);

	if (size1 != size2)
	{
		in1.close();
		in2.close();
		return false;
	}
	static const size_t BLOCKSIZE = 4096;
	size_t remaining = size1;

	while (remaining)
	{
		char buffer1[BLOCKSIZE], buffer2[BLOCKSIZE];
		size_t size = min(BLOCKSIZE, remaining);

		in1.read(buffer1, size);
		in2.read(buffer2, size);

		if (0 != memcmp(buffer1, buffer2, size))
		{
			in1.close();
			in2.close();
			return false;
		}

		remaining -= size;
	}

	in1.close();
	in2.close();
	return true;
}

void helpers::setModFolder(string& modFolder)
{
	TCHAR currentPath[MAX_PATH] = { 0 };
	DWORD ret = GetCurrentDirectoryA(MAX_PATH, currentPath);
	modFolder = currentPath;
	size_t pos = modFolder.find_last_of("\\/")+1;
	modFolder = modFolder.substr(pos);
}

void helpers::getCurrentPath(string& path)
{
	TCHAR currentPath[MAX_PATH] = { 0 };
	DWORD ret = GetCurrentDirectoryA(MAX_PATH, currentPath);
	path = currentPath;
}

vector<std::string> helpers::getCfgFilesInFolder()
{
	string path;
	getCurrentPath(path);
	path += "\\*.cfg";

	WIN32_FIND_DATAA findFileData;
	HANDLE hFind = FindFirstFileA(path.c_str(), &findFileData);

	std::vector<std::string> cfgFiles;

	if (hFind != INVALID_HANDLE_VALUE)
	{
		do {
			const std::string name = findFileData.cFileName;

			if (!(findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{
				string currentPath;
				getCurrentPath(currentPath);
				if (checkCfgFileForMod(currentPath + "\\" + name) != "")
				{
					cfgFiles.push_back(name);
				}
			}

		} while (FindNextFileA(hFind, &findFileData) != 0);

		FindClose(hFind);
	}

	return cfgFiles;
}

std::string helpers::checkCfgFileForMod(const std::string& filePath)
{
	std::ifstream file(filePath);
	if (!file.is_open())
		return "";

	std::string line;
	while (std::getline(file, line))
	{
		line.erase(std::remove_if(line.begin(), line.end(), ::isspace), line.end());
		
		if (line.find("mod=") == 0)
			return extractModPathFromLine(line);
	}

	return ""; 
}

std::vector<std::string> helpers::split(const std::string& s, char delimiter)
{
	std::vector<std::string> tokens;
	std::string token;
	std::istringstream tokenStream(s);
	while (std::getline(tokenStream, token, delimiter))
	{
		tokens.push_back(token);
	}
	return tokens;
}

bool helpers::verifyModPath(const std::string& modPath)
{
	string currentPath;
	getCurrentPath(currentPath);
	auto modPathDirs = split(modPath, '\\');
	auto currentPathDirs = split(currentPath, '\\');
	
	if (currentPathDirs.size() < 2)
		return false;

	std::string lastDir = currentPathDirs[currentPathDirs.size() - 1];
	std::string secondLastDir = currentPathDirs[currentPathDirs.size() - 2];
	std::string expectedPath = secondLastDir + "\\" + lastDir;
	std::replace(expectedPath.begin(), expectedPath.end(), '/', '\\');
	std::replace(expectedPath.begin(), expectedPath.end(), ' ', '');
	std::replace(modPath.begin(), modPath.end(), '/', '\\');
	std::replace(modPath.begin(), modPath.end(), ' ', '');
	std::transform(expectedPath.begin(), expectedPath.end(), expectedPath.begin(), ::tolower);
	std::transform(modPath.begin(), modPath.end(), modPath.begin(), ::tolower);

	return modPath == expectedPath;
}

std::string helpers::extractModPathFromLine(const std::string& line)
{
	size_t pos = line.find("mod=");
	if (pos == std::string::npos)
		return ""; 
	
	return line.substr(pos + 4);
}
