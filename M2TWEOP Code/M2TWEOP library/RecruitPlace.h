#pragma once
#include "realGameTypes.h"
#include "headersSTD.h"
#include <mutex>
#include "RecruitUnitDescription.h"
#include "RecruitConditionDescription.h"
class RecruitPlace
{
public:
	RecruitPlace(const std::wstring& ourPath, const std::wstring& myPath);
private:
	recruitUnits units;
	recruitConditions conditions;
};

typedef vector<shared_ptr<RecruitPlace>>recruitPlaces;
