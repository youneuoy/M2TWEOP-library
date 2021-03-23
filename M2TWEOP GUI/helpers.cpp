#include "helpers.h"
#include "dataG.h"
#include "DXERR.h"

#pragma comment(lib, "DXERR.lib")

void helpers::loadTexture(d3dImage* image)
{
	HRESULT res = D3DXCreateTextureFromFile(dataG::data.d3d.g_pd3dDevice, image->path.c_str(), &image->image);
	if (res != D3D_OK)
	{
		MessageBoxA(NULL, DXGetErrorString(res), "Loading texture err!", MB_OK | MB_ICONASTERISK);
		return;
	}

	D3DSURFACE_DESC my_image_desc;
	image->image->GetLevelDesc(0, &my_image_desc);

	image->xSize = (int)my_image_desc.Width;
	image->ySize = (int)my_image_desc.Height;
}

IDirect3DTexture9* helpers::loadTextureFromMem(char* mem, int x, int y)
{
	IDirect3DTexture9* d3dTexture = NULL;

	HRESULT err;
	IDirect3DTexture9* texture = NULL;
	err = D3DXCreateTexture(dataG::data.d3d.g_pd3dDevice, x, y, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &d3dTexture);
	if (err != D3D_OK)
	{
		MessageBoxA(NULL, DXGetErrorString(err), "Loading texture err!", MB_OK | MB_ICONASTERISK);
		return nullptr;
	}

	D3DLOCKED_RECT lockRect;

	err = d3dTexture->LockRect(0, &lockRect, NULL, 0);
	if (err != D3D_OK)
	{
		MessageBoxA(NULL, DXGetErrorString(err), "Loading texture err!", MB_OK | MB_ICONASTERISK);
		return nullptr;
	}

	for (int line = 0; line < y; line++)
	{
		int dwOffset = line * x;

		for (int width = 0; width < x; width)
		{
			char b = mem[2] & 0xFF;
			char g = mem[1] & 0xFF;
			char r = mem[0] & 0xFF;
			char a = 0xFF;
			mem += 3;
			((int*)lockRect.pBits)[dwOffset + width] = ((a << 24L) + (r << 16L) + (g << 8L) + (b));
			width++;
		}
	}

	err = d3dTexture->UnlockRect(0);
	if (err != D3D_OK)
	{
		MessageBoxA(NULL, DXGetErrorString(err), "Loading texture err!", MB_OK | MB_ICONASTERISK);
		return nullptr;
	}

	return d3dTexture;
}

void helpers::updateMetrics()
{
	dataG::data.screen.screenSize.x = (float)GetSystemMetrics(SM_CXSCREEN);
	dataG::data.screen.screenSize.y = (float)GetSystemMetrics(SM_CYSCREEN);

	dataG::data.screen.screenHalfSize.x = dataG::data.screen.screenSize.x / 2;
	dataG::data.screen.screenHalfSize.y = dataG::data.screen.screenSize.y / 2;
}

screenS& helpers::getScreen()
{
	return dataG::data.screen;
}

d3dImage* helpers::findImage(const char* name, int nameLen)
{
	for (d3dImage* img : dataG::data.staticImagesCollection)
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

bool helpers::runGame(const char* exeFile, const char* exeParam)
{
	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));

	string line = "\"";
	line += dataG::data.gameData.gamePath;
	line += "\" \"";

	line += exeParam;
	line += "\"";

	// Start the child process.
	if (!CreateProcessA(NULL,   // No module name (use command line)
		const_cast<char*>(line.c_str()),        // Command line
		NULL,           // Process handle not inheritable
		NULL,           // Thread handle not inheritable
		FALSE,          // Set handle inheritance to FALSE
		0,              // No creation flags
		NULL,           // Use parent's environment block
		TEXT("..\\.."),           // starting directory
		&si,            // Pointer to STARTUPINFO structure
		&pi            // Pointer to PROCESS_INFORMATION structure
	))
	{
		DWORD err = GetLastError();
		string s = "Error: ";
		s += to_string(err);

		MessageBox(NULL, "CreateProcess failed", s.c_str(), MB_OK);
		return false;
	}

	//wait for game start
	WaitForSingleObject(pi.hProcess, INFINITE);

	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
	return true;
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
		in1.close();
		in2.close();
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
