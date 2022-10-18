#pragma once
#include "realGameTypes.h"
#include "headersSTD.h"
class RecruitUnitDescription
{
public:
	RecruitUnitDescription(jsn::json unit);
};

typedef vector<shared_ptr<RecruitUnitDescription>>recruitUnits;