#pragma once
#include "headersSTD.h"
class helpers
{
public:
	//make formated string
	static std::string makeFString(const char* fmt, ...);

	static bool loadTexture(const char* filename, GLuint* out_texture, int* out_width, int* out_height);

	static LPWSTR ConvertStringToLPWSTR(const char* narrowString);
	static void openProcess(LPSTR& exePath, LPSTR& workingDir);
	static void openProcessWithParams(LPSTR& exePath, LPSTR& workingDir, LPSTR& params);
	static void closeProcess(const string& exeName);
	static bool isProcessRunning(const string& exeName);

	static vector<std::string> getCfgFilesInFolder();
	static vector<std::string> getTomlFilesInFolder();

	static std::string checkCfgFileForMod(const std::string& filePath);

	static std::vector<std::string> split(const std::string& s, char delimiter);

	static bool verifyModPath(const std::string& modPath);

	static std::string extractModPathFromLine(const std::string& line);

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

	static bool compareFiles(wstring& oneFile, wstring& nextFile);

	static void getCurrentPath(string& path);
	static std::string remove_extension(const std::string &filename);
	static LPSTR ConvertWideStringToLPSTR(const std::wstring &wideString);
	static std::wstring stringToWstring(const std::string& str);
private:
	static void setModFolder(string& modFolder);
};
