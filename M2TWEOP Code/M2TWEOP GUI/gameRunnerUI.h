#pragma once
#include "headersSTD.h"
namespace gameRunnerUI
{
	void setRunParams(const string&exePath,const string&exeArgs,const string&eopArgs,bool isEopNeeded);
	int drawUI(bool* isOpen);
	void maintainGUI();
};

