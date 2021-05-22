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


	static bool addModPathArg(string& args, int gameMode);



	static bool compareFiles(string& oneFile, string& nextFile);

	static void getCurrentPath(string& path);
};
