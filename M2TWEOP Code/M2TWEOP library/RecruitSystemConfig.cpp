#include "RecruitSystemConfig.h"
#include "fastFunctsHelpers.h"

void RecruitSystemConfig::StartLoad(const std::wstring& path, std::function<void(int)> callback)
{
	ourPath = path;

	std::thread loadThread([&] 
		{
			LoadJsons();

			//no errors, sucess only
			callback(1);
		}
	);
}

void RecruitSystemConfig::LoadJsons()
{
	vector<wstring> descriptionsPaths;
	jsn::json json = fastFunctsHelpers::loadJsonFromFile(ourPath+L"\\RecruitSystem.json");

	try
	{
		for (auto it = json.begin(); it != json.end(); ++it)
		{
			descriptionsPaths.push_back(it.value());
		}
	}
	catch (jsn::json::type_error& e)
	{
		MessageBoxA(NULL, e.what(), "Warning!", MB_APPLMODAL | MB_SETFOREGROUND);
	}


	for (auto& descrPath : descriptionsPaths) 
	{
		try
		{
			places.push_back(make_shared<RecruitPlace>(ourPath, descrPath));
		}
		catch (exception& e)
		{
			MessageBoxA(NULL, e.what(), "Warning!", MB_APPLMODAL | MB_SETFOREGROUND);
		}
	}
}
