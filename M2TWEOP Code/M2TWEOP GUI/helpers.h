#pragma once
#include "headersSTD.h"
class helpers
{
public:
	static size_t ExecuteProcess(std::wstring FullPathToExe, std::wstring Parameters, size_t SecondsToWait);

	static std::wstring get_wstring(const std::string& str);
	//make formated string
	static std::string makeFString(const char* fmt, ...);

	static bool loadTexture(const char* filename, GLuint* out_texture, int* out_width, int* out_height);

	static void closeGame(const string& exeName);

	static void updateMetrics();

	static screenS& getScreen();

	static GLImage* findImage(const char* name, int nameLen);

	static ImFont* findFont(const char* name);

	//run game without eop only
	static void runGame(const char* exeFile, const char* exeParam);

	static bool addModPathArg(string& args, int gameMode);

	//false if dont get responce
	static bool doPipe(const string &message, int waitSeconds);

	static bool selectGameExe(int gameMode);

	static bool compareFiles(string& oneFile, string& nextFile);

	static void getCurrentPath(string& path);
private:
	static void setModFolder(string& modFolder);
};
