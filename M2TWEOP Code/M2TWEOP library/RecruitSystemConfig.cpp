#include "RecruitSystemConfig.h"
#include "fastFunctsHelpers.h"

void RecruitSystemConfig::StartLoad(const std::wstring& path, std::function<void(int)> callback)
{
	ourPath = path;

	std::thread loadThread([&] 
		{
			LoadJsons();

			callback(1);
		}
	);
}

void RecruitSystemConfig::LoadJsons()
{
	jsn::json json = fastFunctsHelpers::loadJsonFromFile(ourPath+L"\\RecruitSystem.json");

	try
	{
		if (json.contains("isBlockLaunchWithoutEop"))
		{
			getJson(globals::dataS.gameCfg.isBlockLaunchWithoutEop, "isBlockLaunchWithoutEop");
		}
	}
	catch (jsn::json::type_error& e)
	{
		MessageBoxA(NULL, e.what(), "Warning!", MB_APPLMODAL | MB_SETFOREGROUND);
	}
}
