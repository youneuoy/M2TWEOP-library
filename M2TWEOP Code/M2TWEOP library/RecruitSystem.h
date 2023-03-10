#pragma once
#include "realGameTypes.h"
#include "headersSTD.h"
struct RecruitPlaceData
{
	int x;
	int y;

};

class RecruitSystem
{
public:
	void Init();
private:
	//callback
	void AtInit(int status);

	struct
	{

	}places;


};
