#pragma once
#include"headersSTD.h"
#include "patchesWorker.h"
class globals
{
public:
	static struct dataGlob
	{
		//steam =2, kingdoms=1
		UINT32 gamever;
		string modPatch;


		patchesWorker patchesManager;
	}dataS;
};

