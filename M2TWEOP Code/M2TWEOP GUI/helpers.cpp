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
std::wstring helpers::get_wstring(const std::string& str)
{
	const char* c = str.c_str();
	const size_t cSize = str.size() + 1;
	wchar_t* wc = new wchar_t[cSize];
	mbstowcs(wc, c, cSize);
	std::wstring w_string(wc);
	delete[] wc;
	return w_string;
}
size_t helpers::ExecuteProcess(std::wstring FullPathToExe, std::wstring Parameters, size_t SecondsToWait)
{
	size_t iMyCounter = 0, iReturnVal = 0, iPos = 0;
	DWORD dwExitCode = 0;
	std::wstring sTempStr = L"";

	/* - NOTE - You should check here to see if the exe even exists */

	/* Add a space to the beginning of the Parameters */
	if (Parameters.size() != 0)
	{
		if (Parameters[0] != L' ')
		{
			Parameters.insert(0, L" ");
		}
	}

	/* The first parameter needs to be the exe itself */
	sTempStr = FullPathToExe;
	iPos = sTempStr.find_last_of(L"\\");
	sTempStr.erase(0, iPos + 1);
	Parameters = sTempStr.append(Parameters);

	/* CreateProcessW can modify Parameters thus we allocate needed memory */
	wchar_t* pwszParam = new wchar_t[Parameters.size() + 1];
	if (pwszParam == 0)
	{
		return 1;
	}
	const wchar_t* pchrTemp = Parameters.c_str();
	wcscpy_s(pwszParam, Parameters.size() + 1, pchrTemp);

	/* CreateProcess API initialization */
	STARTUPINFOW siStartupInfo;
	PROCESS_INFORMATION piProcessInfo;
	memset(&siStartupInfo, 0, sizeof(siStartupInfo));
	memset(&piProcessInfo, 0, sizeof(piProcessInfo));
	siStartupInfo.cb = sizeof(siStartupInfo);

	if (CreateProcessW(const_cast<LPCWSTR>(FullPathToExe.c_str()),
		pwszParam, 0, 0, false,
		CREATE_DEFAULT_ERROR_MODE, 0, L"..\\..",
		&siStartupInfo, &piProcessInfo) != false)
	{
		/* Watch the process. */
		dwExitCode = WaitForSingleObject(piProcessInfo.hProcess, (SecondsToWait * 1000));
	}
	else
	{
		/* CreateProcess failed */
		iReturnVal = GetLastError();
	}

	/* Free memory */
	delete[]pwszParam;
	pwszParam = 0;

	/* Release handles */
	CloseHandle(piProcessInfo.hProcess);
	CloseHandle(piProcessInfo.hThread);

	return iReturnVal;
}

void helpers::runGame(const char* exeFile, const char* exeParam)
{
	helpers::ExecuteProcess(helpers::get_wstring(dataG::data.gameData.gamePath), helpers::get_wstring(exeParam), 20);


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

void helpers::getCurrentPath(string& path)
{
	TCHAR currentPath[MAX_PATH] = { 0 };
	DWORD ret = GetCurrentDirectoryA(MAX_PATH, currentPath);
	path = currentPath;
}

void helpers::setModFolder(string& modFolder)
{
	TCHAR currentPath[MAX_PATH] = { 0 };
	DWORD ret = GetCurrentDirectoryA(MAX_PATH, currentPath);
	modFolder = currentPath;
	size_t pos = modFolder.find_last_of("\\/")+1;
	modFolder = modFolder.substr(pos);
}
