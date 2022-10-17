#include "BattleMapOverrider.h"
#include "globals.h"

bool BattleMapOverrider::Draw()
{
	Updateinternal();

	auto& battlemapWorker = globals::dataS.Modules.battlemapWorker;

    bool isWork = true;

	ImGui::SetNextWindowPos({ 0,0 }, ImGuiCond_Always);
	ImGui::SetNextWindowSize({ 600,-1 }, ImGuiCond_Always);

    ImGui::Begin("Map overrider", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);


    if (ImGui::Button("Close"))
    {
        isWork = false;
    }

	//selectable child
	{
		ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.143f, 0.143f, 0.143f, 1.000f));
		ImGui::BeginChild("PreferencesSelectable", overidedSel.selectablesSize);

		size_t n = 0;
		for (auto currRec : battlemapWorker.worldRecords)
		{
			ImVec2 cp = ImGui::GetCursorScreenPos();
			ImVec2 cp2 = ImVec2(cp.x + overidedSel.selectableSize.x, cp.y + overidedSel.selectableSize.y);


			ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0, 0, 0, 0));
			ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(0, 0, 0, 0));

			if (ImGui::Selectable(currRec.first.c_str(), overidedSel.selectedGroupItem == n, 0, overidedSel.selectableSize))
			{
				overidedSel.selectedGroupItem = n;
				overidedSel.selectedGroup = currRec.first;
			}
			ImGui::PopStyleColor();
			ImGui::PopStyleColor();

			if (overidedSel.selectedGroupItem == n)
			{

				ImU32 col_a = ImGui::GetColorU32(ImGuiCol_Button);


				ImDrawList* draw_list = ImGui::GetWindowDrawList();
				ImU32 col_b = ImGui::GetColorU32(ImGuiCol_ChildBg);
				draw_list->AddRectFilledMultiColor(cp, cp2, col_a, col_b, col_b, col_a);
			}

			++n;
		}


		ImGui::EndChild();
		ImGui::PopStyleColor();
	}
	ImGui::SameLine();


	//settings content child
	{
		ImGui::BeginChild("PreferencesContent", { -1, overidedSel.selectablesSize.y });

		auto& currGroup = battlemapWorker.worldRecords[overidedSel.selectedGroup];
		
		size_t n = 0;
		for (auto currRec : *currGroup)
		{
			ImVec2 cp = ImGui::GetCursorScreenPos();
			ImVec2 cp2 = ImVec2(cp.x + overidedSel.selectableSize.x, cp.y + overidedSel.selectableSize.y);


			ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0, 0, 0, 0));
			ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(0, 0, 0, 0));

			if (ImGui::Selectable(currRec->record.c_str(), overidedSel.selectedRecordItem == n, 0, overidedSel.selectableSize))
			{
				overidedSel.selectedRecordItem = n;
				overidedSel.selectedRecord = currRec->record;
			}
			ImGui::PopStyleColor();
			ImGui::PopStyleColor();

			if (overidedSel.selectedRecordItem == n)
			{

				ImU32 col_a = ImGui::GetColorU32(ImGuiCol_Button);


				ImDrawList* draw_list = ImGui::GetWindowDrawList();
				ImU32 col_b = ImGui::GetColorU32(ImGuiCol_ChildBg);
				draw_list->AddRectFilledMultiColor(cp, cp2, col_a, col_b, col_b, col_a);
			}

			++n;
		}

		ImGui::EndChild();
	}
    ImGui::End();
    return isWork;
}

string BattleMapOverrider::SelectWorldpkgdesc(const std::string& selectedRec, const std::string& selectedGroup)
{
	if (overidedSel.selectedGroup == selectedGroup)
	{
		return overidedSel.selectedRecord;
	}

	return "";
}

void BattleMapOverrider::Updateinternal()
{
	auto& battlemapWorker = globals::dataS.Modules.battlemapWorker;
	if (battlemapWorker.worldRecords.empty())
	{
		return;
	}

	overidedSel.selectablesSize.x = overidedSel.selectableSize.x;
	overidedSel.selectablesSize.y = overidedSel.selectableSize.y * battlemapWorker.worldRecords.size() + ImGui::GetStyle().ItemSpacing.y * battlemapWorker.worldRecords.size();
}
