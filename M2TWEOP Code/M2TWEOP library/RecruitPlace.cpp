#include "RecruitPlace.h"
#include "fastFunctsHelpers.h"

RecruitPlace::RecruitPlace(const std::wstring& ourPath, const std::wstring& myPath)
{
	wstring path = ourPath + L"\\";
	path += myPath;


	jsn::json json = fastFunctsHelpers::loadJsonFromFile(ourPath + L"\\RecruitSystem.json");

	try
	{
		jsn::json unitsJ = json["recruitUnits"];
		jsn::json conditionsJ = json["recruitConditions"];

		for (auto it = unitsJ.begin(); it != unitsJ.end(); ++it)
		{
			units.push_back(make_shared<RecruitUnitDescription>(it.value()));
		}
		for (auto it = conditionsJ.begin(); it != conditionsJ.end(); ++it)
		{
			conditions.push_back(make_shared<RecruitConditionDescription>(it.value()));
		}
	}
	catch (jsn::json::type_error& e)
	{
		MessageBoxA(NULL, e.what(), "Warning!", MB_APPLMODAL | MB_SETFOREGROUND);
	}
}
