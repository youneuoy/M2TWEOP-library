#pragma once
#include <windows.h>
using namespace std;
class MmbMover
{
public:
	void Start(LPARAM lParam);
	void Stop();
	void Update(LPARAM lParam);
private:
	bool isMmb = false;
	POINT oldCoords;
};

