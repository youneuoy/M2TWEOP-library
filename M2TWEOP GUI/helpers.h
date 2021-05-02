#pragma once
#include "headersSTD.h"
class helpers
{
public:
	//make formated string
	static std::string makeFString(const char* fmt, ...);

	static bool loadTexture(const char* filename, GLuint* out_texture, int* out_width, int* out_height);


	static void updateMetrics();

	static screenS& getScreen();

	static GLImage* findImage(const char* name, int nameLen);

	static ImFont* findFont(const char* name);

	//run game without eop only
	static bool runGame(const char* exeFile, const char* exeParam);

	static bool addModPathArg(string& args, int gameMode);

	//false if dont get responce
	static bool doPipe(const string &message, int waitSeconds);
	static void removePipe();
	static bool selectGameExe(int gameMode);

	static bool compareFiles(string& oneFile, string& nextFile);

	static void getCurrentPath(string& path);
private:
	static void setModFolder(string& modFolder);
};
