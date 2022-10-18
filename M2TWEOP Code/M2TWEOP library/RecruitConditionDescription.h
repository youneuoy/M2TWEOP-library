#pragma once
#include "realGameTypes.h"
#include "headersSTD.h"
class RecruitConditionDescription
{
public:
	RecruitConditionDescription(jsn::json condition);
};

typedef vector<shared_ptr<RecruitConditionDescription>>recruitConditions;

