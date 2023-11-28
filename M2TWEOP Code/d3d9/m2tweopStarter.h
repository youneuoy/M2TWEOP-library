#pragma once
#include <d3d9.h>
#include <string>

namespace m2tweopStarter
{
	void doM2TWEOP();

	void onCreateDevice(IDirect3DDevice9* pDevice);
	void onEndScene(IDirect3DDevice9* pDevice);

	void onReset(IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters);
	void afterReset(IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters);
};

