#pragma once
#include "headersSTD.h"
class helpers
{
public:
	static void loadTexture(d3dImage* image);

	static void updateMetrics();

	static screenS& getScreen();

	static d3dImage* findImage(const char* name,int nameLen);

	static ImFont* findFont(const char* name);
};

