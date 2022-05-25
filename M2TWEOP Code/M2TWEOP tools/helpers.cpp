#include "helpers.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <TlHelp32.h>
std::string helpers::makeFString(const char* fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	std::vector<char> v(1024);
	while (true)
	{
		va_list args2;
		va_copy(args2, args);
		int res = vsnprintf(v.data(), v.size(), fmt, args2);
		if ((res >= 0) && (res < static_cast<int>(v.size())))
		{
			va_end(args);
			va_end(args2);
			return std::string(v.data());
		}
		size_t size;
		if (res < 0)
			size = v.size() * 2;
		else
			size = static_cast<size_t>(res) + 1;
		v.clear();
		v.resize(size);
		va_end(args2);
	}
}

bool helpers::loadTexture(const char* filename, GLuint* out_texture, int* out_width, int* out_height)
{
	// Load from file
	int image_width = 0;
	int image_height = 0;
	unsigned char* image_data = stbi_load(filename, &image_width, &image_height, NULL, 4);
	if (image_data == NULL)
		return false;

	// Create a OpenGL texture identifier
	GLuint image_texture;
	glGenTextures(1, &image_texture);
	glBindTexture(GL_TEXTURE_2D, image_texture);

	// Setup filtering parameters for display
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Upload pixels into texture
#if defined(GL_UNPACK_ROW_LENGTH) && !defined(__EMSCRIPTEN__)
	glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
#endif
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
	stbi_image_free(image_data);

	*out_texture = image_texture;
	*out_width = image_width;
	*out_height = image_height;

	return true;
}





void helpers::updateMetrics()
{
	dataG::data.screen.screenSize.x = (float)GetSystemMetrics(SM_CXSCREEN);
	dataG::data.screen.screenSize.y = (float)GetSystemMetrics(SM_CYSCREEN);

	dataG::data.screen.screenHalfSize.x = dataG::data.screen.screenSize.x / 2;
	dataG::data.screen.screenHalfSize.y = dataG::data.screen.screenSize.y / 2;

	dataG::data.screen.screenUpperPos.x = dataG::data.screen.screenSize.x / 2;
	dataG::data.screen.screenUpperPos.y = dataG::data.screen.screenSize.y * 0.3f;
}

screenS& helpers::getScreen()
{
	return dataG::data.screen;
}

GLImage* helpers::findImage(const char* name, int nameLen)
{
	for (GLImage* img : dataG::data.staticImagesCollection)
	{
		if (img->path.size() != nameLen)continue;

		if (strncmp(name, img->path.c_str(), nameLen) == 0)
		{
			return img;
		}
	}
	return findImage("empty", 5);
}

ImFont* helpers::findFont(const char* name)
{
	for (fontS* fnt : dataG::data.staticFontsCollection)
	{
		if (strcmp(name, fnt->name.c_str()) == 0)
		{
			return fnt->font;
		}
	}
	return findFont("mainFont");
}

int makeFullPath(string& path)
{
	TCHAR fullPath[MAX_PATH];
	int iret = GetFullPathNameA(path.c_str(), MAX_PATH, fullPath, NULL);
	path = fullPath;

	return iret;
}


bool helpers::compareFiles(string& oneFile, string& nextFile)
{
	ifstream in1(oneFile, ios::binary);

	ifstream in2(nextFile, ios::binary);
	if (in1.is_open() == false)
	{
		in1.close();
		in2.close();
		return false;
	}
	if (in2.is_open() == false)
	{
		MessageBoxA(NULL, "Cannot run M2TWEOP, missing d3d9.dll. Reinstall it for M2TWEOP(dont try any standard d3d9.dll or files from internet, M2TWEOP use custom one).", "ERROR", MB_OK);
		in1.close();
		in2.close();

		exit(0);
		return false;
	}

	ifstream::pos_type size1, size2;

	size1 = in1.seekg(0, ifstream::end).tellg();
	in1.seekg(0, ifstream::beg);

	size2 = in2.seekg(0, ifstream::end).tellg();
	in2.seekg(0, ifstream::beg);

	if (size1 != size2)
	{
		in1.close();
		in2.close();
		return false;
	}
	static const size_t BLOCKSIZE = 4096;
	size_t remaining = size1;

	while (remaining)
	{
		char buffer1[BLOCKSIZE], buffer2[BLOCKSIZE];
		size_t size = min(BLOCKSIZE, remaining);

		in1.read(buffer1, size);
		in2.read(buffer2, size);

		if (0 != memcmp(buffer1, buffer2, size))
		{
			in1.close();
			in2.close();
			return false;
		}

		remaining -= size;
	}

	in1.close();
	in2.close();
	return true;
}

void helpers::getCurrentPath(string& path)
{
	TCHAR currentPath[MAX_PATH] = { 0 };
	DWORD ret = GetCurrentDirectoryA(MAX_PATH, currentPath);
	path = currentPath;
}

string helpers::getSubstring(const string& s, const string& start, const string& end)
{

	std::string temp;


	size_t pos = s.find_last_of(start);
	size_t endpos = s.find_last_of(end);
	if (pos == std::string::npos || endpos == std::string::npos || pos == endpos) return "";

	pos+= start.size();
	temp = s.substr(pos, endpos - pos);


	int len = temp.size() + 1;


	return temp;
}

