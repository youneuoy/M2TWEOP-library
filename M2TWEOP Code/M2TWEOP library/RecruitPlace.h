#pragma once
#include "realGameTypes.h"
#include "headersSTD.h"
#include <mutex>
class RecruitPlace
{
public:
	RecruitPlace(const std::wstring& ourPath, const std::wstring& myPath);
};

typedef vector<shared_ptr<RecruitPlace>>recruitPlaces;
