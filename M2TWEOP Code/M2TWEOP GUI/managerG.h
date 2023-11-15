#pragma once
#include "headersSTD.h"
namespace managerG
{
	//load settings and init
	void init();

	void saveJsonSettings();

	bool isRedistsInstallNeeded();

	void loadJsonConfig();
};
