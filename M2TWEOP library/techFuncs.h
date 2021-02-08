#pragma once
#include <windows.h>
namespace techFuncs
{
	template <typename  data>
	void Read(DWORD from, data* p, size_t size = 4)
	{
		memset(p, 0, size);
		memcpy(p, (LPVOID)from, size);
	}
};

