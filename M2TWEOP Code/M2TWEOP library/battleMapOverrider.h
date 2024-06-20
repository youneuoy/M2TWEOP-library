#pragma once
#include "realGameTypes.h"
#include "headersSTD.h"
#include "imgui/imgui.h"
class BattleMapOverrider
{
public:
	bool Draw();


	string SelectWorldpkgdesc(const std::string& selectedRec, const std::string& selectedGroup);
private:
	struct
	{
		string selectedGroup = "none";
		int selectedGroupItem = 0;

		string selectedRecord = "none";
		int selectedRecordItem = 0;


		//size of one selectable
		ImVec2 selectableSize{ 250,50 };
		//size of selectables child
		ImVec2 selectablesSize{};
	}overidedSel;

	void Updateinternal();
};

