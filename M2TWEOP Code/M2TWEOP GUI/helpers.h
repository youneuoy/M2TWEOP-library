#pragma once
#include "headersSTD.h"
class helpers
{
public:
	//make formated string
	static std::wstring makeFStringW(const wchar_t* fmt, ...);
	static std::string makeFString(const char* fmt, ...);

	static bool loadTexture(const char* filename, GLuint* out_texture, int* out_width, int* out_height);

	static LPWSTR ConvertStringToLPWSTR(const char* narrowString);
	static void openProcess(wstring& exePath, wstring& workingDir);
	static void closeProcess(const wstring& exeName);
	static bool isProcessRunning(const wstring& exeName);

	static vector<std::string> getCfgFilesInFolder();
	static vector<std::string> getTomlFilesInFolder();

	static std::string checkCfgFileForMod(const std::string& filePath);

	static std::vector<std::string> split(const std::string& s, char delimiter);

	static bool verifyModPath(const std::string& modPath);

	static std::string extractModPathFromLine(const std::string& line);

	static void updateMetrics();

	static screenS& getScreen();

	static GLImage* findImage(const char* name, int nameLen);

	static ImFont* findFont(const wchar_t* name);

	//run game without eop only
	static void runGame(const wchar_t* exeFile, const wchar_t* exeParam);

	static bool addModPathArg(wstring& args, int gameMode);

	//false if dont get responce
	static bool doPipe(const wstring &message, int waitSeconds);

	static bool selectGameExe(int gameMode);

	static bool compareFiles(wstring& oneFile, wstring& nextFile);

	static void getCurrentPathW(wstring& path);
	static void getCurrentPath(string& path);
	static std::string remove_extension(const std::string &filename);
	static LPSTR ConvertWideStringToLPSTR(const std::wstring &wideString);

	static std::string wstringTostring(const std::wstring &wideString);
	static std::wstring stringToWstring(const std::string& str);
private:
	static void setModFolder(wstring& modFolder);
};
