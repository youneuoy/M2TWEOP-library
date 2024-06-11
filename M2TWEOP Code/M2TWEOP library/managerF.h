#pragma once
class managerF
{
public:
	static void init();

	static void debug();
	static void loadJsonSettings();
private:
	static void doPachs();

	static void initThread();


};

namespace managerExport
{
	void initEOP(const char* modPath, int gameVer);

}