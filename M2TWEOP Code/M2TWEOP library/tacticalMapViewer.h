#pragma once
#include <vector>
class tacticalMapViewer
{
public:
	bool isTacticalMapViewerNeeded = true;
	void init(int gameVer);

	void view(int x, int y);
	void unView();


	void draw();

	static int __fastcall getPathCache(const tacticalMapViewer* we);
private:
	typedef void(__stdcall* startTransitionToMapViewFunc)(int x, int y);
	typedef void(__stdcall* initPathCacheFunc)();

	struct
	{
		int* pathCacheOffset = nullptr;
	}offsets;
	struct
	{
		startTransitionToMapViewFunc startTransitionToMapView = nullptr;
		initPathCacheFunc initPathCache = nullptr;
	}functions;
	struct
	{
		std::vector<uint8_t> backupData = {};
		bool isBattleGeneratorWorking = true;
		int coords[2]{};
	}state;
};

