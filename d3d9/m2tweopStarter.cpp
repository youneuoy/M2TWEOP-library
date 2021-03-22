#include "m2tweopStarter.h"
#include <string>
#include <fstream>
#include <windows.h>
using namespace std;

namespace m2tweopStarter
{

	typedef void (*m2tweopInit)();
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
	}dataEOP;

	void doM2TWEOP()
	{
		MessageBoxA(NULL, "test", "ATTENTION!", NULL);
		string libPath;
		ifstream f1("M2TWEOPD3D.cfg");
		if (f1.is_open() == false)
		{
			MessageBoxA(NULL,"Cannot find M2TWEOPD3D.cfgm m2tweop dont work now!","ATTENTION!",NULL);
			return;
		}

		getline(f1, libPath);
		f1.close();

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


		dataEOP.eopInitF();
	}

	void onCreateDevice(IDirect3DDevice9* pDevice)
	{

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
		dataEOP.onEndScene(pDevice);
	}

	void onReset(IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters)
	{
		dataEOP.onReset(pDevice, pPresentationParameters);
	}

	void afterReset(IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters)
	{
		dataEOP.afterReset(pDevice, pPresentationParameters);
	}

};

