#include "helpers.h"
#include "dataG.h"
void helpers::loadTexture(d3dImage* image)
{
	HRESULT res = D3DXCreateTextureFromFile(dataG::data.d3d.g_pd3dDevice, image->path.c_str(), &image->image);
	string t = std::system_category().message(res);
	if (res != D3D_OK)
	{
		MessageBoxA(NULL, t.c_str(), "Loading texture err!", MB_OK | MB_ICONASTERISK);
	}


	D3DSURFACE_DESC my_image_desc;
	image->image->GetLevelDesc(0, &my_image_desc);

	image->xSize = (int)my_image_desc.Width;
	image->ySize = (int)my_image_desc.Height;
}

void helpers::updateMetrics()
{
	dataG::data.screen.screenSize.x = GetSystemMetrics(SM_CXSCREEN);
	dataG::data.screen.screenSize.y = GetSystemMetrics(SM_CYSCREEN);

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

	// Start the child process. 
	if (!CreateProcessA(exeFile,   // No module name (use command line)
		const_cast<char*>(exeParam),        // Command line
		NULL,           // Process handle not inheritable
		NULL,           // Thread handle not inheritable
		FALSE,          // Set handle inheritance to FALSE
		0,              // No creation flags
		NULL,           // Use parent's environment block
		TEXT("..\\.."),           // Use parent's starting directory 
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
	
	else if (gameMode ==4)
	{
		args += "--features.mod=mods/crusades ";
	}
	
	else if (gameMode ==5)
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
	string exePath = "..\\..\\medieval2.exe";
	int gameVersion=getVersion(exePath);
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
	if (gameVersion == 2|| gameVersion == 1)
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

