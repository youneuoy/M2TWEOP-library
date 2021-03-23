#pragma once
#include "headersSTD.h"
class helpers
{
public:
	static void loadTexture(d3dImage* image);
	static IDirect3DTexture9* loadTextureFromMem(char* mem, int x, int y);

	static void updateMetrics();

	static screenS& getScreen();

	static d3dImage* findImage(const char* name, int nameLen);

	static ImFont* findFont(const char* name);

	static bool runGame(const char* exeFile, const char* exeParam);

	static bool addModPathArg(string& args, int gameMode);

	static bool selectGameExe(int gameMode);

	static bool compareFiles(string& oneFile, string& nextFile);

	static void getCurrentPath(string& path);
private:
	static void setModFolder(string& modFolder);
};
