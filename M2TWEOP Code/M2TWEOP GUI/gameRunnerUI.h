#pragma once
#include "headersSTD.h"
namespace gameRunnerUI
{
	void setRunParams(const wstring&exePath,const wstring&exeArgs,const wstring&eopArgs,bool isEopNeeded);
	int drawUI(bool* isOpen);
	void maintainGUI();
};

