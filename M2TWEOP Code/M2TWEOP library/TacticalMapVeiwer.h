#pragma once
#include <vector>
class TacticalMapViewer
{
public:
	bool isTacticalMapViewerNeeded = true;
	void Init(int gameVer);

	void View(int x, int y);
	void UnView();


	void Draw();

	static int __fastcall GetPathCashe(TacticalMapViewer* we);
private:
	typedef void(__stdcall* StartTransitionToMapView)(int x, int y);
	typedef void(__stdcall* InitPathCashe)();

	struct
	{
		int* PathCasheOffset = 0x0;
	}offsets;
	struct
	{
		StartTransitionToMapView startTransitionToMapView = nullptr;
		InitPathCashe initPathCashe = nullptr;
	}functions;
	struct
	{
		std::vector<uint8_t> backupedData = {};

		bool IsBattleGeneratorWorking = false;
		int cords[2]{};
	}state;

};

