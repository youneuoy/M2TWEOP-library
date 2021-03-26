#include "m2tweopStarter.h"
#include <string>
#include <fstream>
#include <windows.h>
using namespace std;

namespace m2tweopStarter
{

	typedef void (*m2tweopInit)(const char*, int);
	typedef void (*m2tweopD3DCreate)(IDirect3DDevice9*);
	typedef void (*m2tweopEndScene)(IDirect3DDevice9*);

	typedef void (*m2tweopOnReset)(IDirect3DDevice9*, D3DPRESENT_PARAMETERS*);
	typedef void (*m2tweopAfterReset)(IDirect3DDevice9*, D3DPRESENT_PARAMETERS*);
	struct
	{
		HMODULE hmtw;
		m2tweopEndScene onEndScene;

		m2tweopOnReset onReset;
		m2tweopAfterReset afterReset;

		m2tweopInit eopInitF;


		bool isEOP = false;

		bool isEOPCommandLine = false;


		string modPath;
		int gameVer=0;
	}dataEOP;


	void setGameVer(char* commandLine)
	{
		char* folderStart = commandLine;
		folderStart = strstr(folderStart, "*");
		if (folderStart == nullptr)return;

		char* folderEnd = folderStart + 1;
		folderEnd = strstr(folderEnd, "*");
		if (folderEnd == nullptr)return;

		string modVer;
		for (char* fld = folderStart + 1; fld < folderEnd; fld++)
		{
			modVer.push_back(fld[0]);
		}

		dataEOP.gameVer= stoi(modVer);
	}

	void setModFolder(char* commandLine)
	{
		char* folderStart = commandLine;
		folderStart = strstr(folderStart,"*");
		if (folderStart == nullptr)return;

		char* folderEnd = folderStart + 1;
		folderEnd = strstr(folderEnd, "*");
		if (folderEnd == nullptr)return;

		for (char* fld = folderStart + 1; fld < folderEnd; fld++)
		{
			dataEOP.modPath.push_back(fld[0]);
		}
	}
	void parseEOPCMD(char* commandLine)
	{
		char* modFolder = strstr(commandLine, "eopModFolder");
		if (modFolder != nullptr)
		{
			setModFolder(modFolder);
		}
		
		char* gameVer= strstr(commandLine, "eopGameVer");
		if (gameVer != nullptr)
		{
			setGameVer(gameVer);
		}

	}

	void parseCommandLine()
	{
		char* cmd = GetCommandLineA();
		const char eopCMDsteam[] = "m2tweopStartCommand2";

		const char eopCMDdisk[] = "m2tweopStartCommand1";

		char*  eopS=strstr(cmd, eopCMDdisk);

		//disk - 1,steam-2
		int ver = 0;
		if (eopS == nullptr)
		{
			eopS = strstr(cmd, eopCMDsteam);

			if (eopS == nullptr)
			{
				return;
			}
			else
			{
				ver = 2;
			}
		}
		else
		{
			ver = 1;
		}

		if (ver == 1)
		{
			parseEOPCMD(eopS);
			memset(eopS, 0, strlen(eopS));

			int len = strlen(cmd);
			int symbCount = 0;
			for (int i = 0; i < len; i++)
			{
				if (cmd[i] == '\"')
				{
					symbCount++;
					if (symbCount > 2)
					{
						cmd[i] = ' ';
					}
				}
			}
		}
		else if (ver == 2)
		{
			eopS[19] = '1';
			return;
		}

		dataEOP.isEOPCommandLine = true;
	}

	void doM2TWEOP()
	{
		parseCommandLine();

		if (dataEOP.isEOPCommandLine == false)
		{
			dataEOP.isEOP = false;
			return;
		}


		string libPath = dataEOP.modPath+"\\M2TWEOPLibrary.dll";
		dataEOP.hmtw=LoadLibraryA(libPath.c_str());
		if (dataEOP.hmtw == NULL)
		{
			string errmsg = "Cannot load ";
			errmsg += libPath;
			MessageBoxA(NULL, errmsg.c_str(), "ATTENTION!", NULL);
			exit(0);
		}
		dataEOP.eopInitF = (m2tweopInit)GetProcAddress(dataEOP.hmtw, "initEOP");
		if (dataEOP.eopInitF == NULL)
		{
			MessageBoxA(NULL, "Cannot find initEOP function!", "ATTENTION!", NULL);
			exit(0);
		}

		dataEOP.onEndScene = (m2tweopEndScene)GetProcAddress(dataEOP.hmtw, "onEndScene");
		if (dataEOP.onEndScene == NULL)
		{
			MessageBoxA(NULL, "Cannot find onEndScene function!", "ATTENTION!", NULL);
			exit(0);
		}
		
		dataEOP.onReset = (m2tweopOnReset)GetProcAddress(dataEOP.hmtw, "onReset");
		if (dataEOP.onReset == NULL)
		{
			MessageBoxA(NULL, "Cannot find onReset function!", "ATTENTION!", NULL);
			exit(0);
		}		

		dataEOP.afterReset = (m2tweopAfterReset)GetProcAddress(dataEOP.hmtw, "afterReset");
		if (dataEOP.afterReset == NULL)
		{
			MessageBoxA(NULL, "Cannot find afterReset function!", "ATTENTION!", NULL);
			exit(0);
		}


		dataEOP.eopInitF(dataEOP.modPath.c_str(),dataEOP.gameVer);
		dataEOP.isEOP = true;
	}

	void onCreateDevice(IDirect3DDevice9* pDevice)
	{
		if (dataEOP.isEOP == false)
		{
			return;
		}
		m2tweopD3DCreate eopD3DCreate = (m2tweopD3DCreate)GetProcAddress(dataEOP.hmtw, "onCreateDevice");
		if (eopD3DCreate == NULL)
		{
			MessageBoxA(NULL, "Cannot find onCreateDevice M2TWEOP function!", "ATTENTION!", NULL);
			exit(0);
		}

		eopD3DCreate(pDevice);
	}

	void onEndScene(IDirect3DDevice9* pDevice)
	{
		if (dataEOP.isEOP == false)
		{
			return;
		}
		dataEOP.onEndScene(pDevice);
	}

	void onReset(IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters)
	{
		if (dataEOP.isEOP == false)
		{
			return;
		}
		dataEOP.onReset(pDevice, pPresentationParameters);
	}

	void afterReset(IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters)
	{
		if (dataEOP.isEOP == false)
		{
			return;
		}
		dataEOP.afterReset(pDevice, pPresentationParameters);
	}

};

