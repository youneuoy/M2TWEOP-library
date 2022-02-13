#include "modelsFilesUI.h"


#include "headersSTD.h"
#include "dataG.h"
#include "helpers.h"

#include <ImFileDialog.h>

#include "modelsFilesWorker.h"

namespace modelsFilesUI
{

	struct
	{
		selectedFilesS selectedFiles;

		modelsFilesWorker mWorker;

		atomic_bool convertationStarted{ false };
		atomic_bool convertationEnded{ false };
	}data;


	void checkCfgFileDialog(const char* key, string& selectedString)
	{
		if (ifd::FileDialog::Instance().IsDone(key)) {
			if (ifd::FileDialog::Instance().HasResult()) {
				selectedString = ifd::FileDialog::Instance().GetResult().u8string();
			}
			ifd::FileDialog::Instance().Close();
		}
	}
	void checkFilesDialogs()
	{
		checkCfgFileDialog("Custom_tilesOpenDialog", data.selectedFiles.custom_tiles);
		checkCfgFileDialog("stratFortsModelsChangeListOpenDialog", data.selectedFiles.stratFortsModelsChangeList);
		checkCfgFileDialog("stratModelsChangeList", data.selectedFiles.stratModelsChangeList);
		checkCfgFileDialog("stratPortsModelsChangeList", data.selectedFiles.stratPortsModelsChangeList);

	}

	void makeCfgFileButtDialog(const char* key, const char* title, string& selectedString, const char* buttonName)
	{
		if (ImGui::Button(buttonName, helpers::getScreen().centerXButton))
		{
			ifd::FileDialog::Instance().Open(key, title, "M2TWEOP file (*.yoneuoycfg){.yoneuoycfg,},.*", false);
		}
		if (selectedString.size() > 0)
		{
			ImGui::Text("Selected file:");
			ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetStyle().Colors[ImGuiCol_TextDisabled]);
			ImGui::TextWrapped(selectedString.c_str());
			ImGui::PopStyleColor();
		}
	}

	void makeCfgSaveDialog()
	{
		if (ImGui::Button("Set directory for result files", helpers::getScreen().centerXButton))
		{
			ifd::FileDialog::Instance().Open("CfgDirOpenDialog", "Open a directory", "");
		}
		if (data.selectedFiles.resultsFolder.size() > 0)
		{
			ImGui::Text("Results folder:");
			ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetStyle().Colors[ImGuiCol_TextDisabled]);
			ImGui::TextWrapped(data.selectedFiles.resultsFolder.c_str());
			ImGui::PopStyleColor();
		}
	}

	void checkCfgSaveDialog()
	{
		if (ifd::FileDialog::Instance().IsDone("CfgDirOpenDialog")) {
			if (ifd::FileDialog::Instance().HasResult()) {
				data.selectedFiles.resultsFolder = ifd::FileDialog::Instance().GetResult().u8string();
			}
			ifd::FileDialog::Instance().Close();
		}
	}


	void checkAndMakeConvertation()
	{
		if (ImGui::Button("Convert files", helpers::getScreen().centerXButton))
		{
			if (data.selectedFiles.resultsFolder.size() == 0)
			{
				ImGui::OpenPopup("Attention##cfgFolder");
			}
			else
			{
				data.convertationStarted = true;
				data.mWorker.startConvertation(data.selectedFiles, std::ref(data.convertationEnded));

			}
		}


		if (ImGui::BeginPopupModal("Attention##cfgFolder", NULL, ImGuiWindowFlags_AlwaysAutoResize))
		{
			ImGui::Text("You have not selected a folder to save the results\nSelect it and try again");
			ImGui::NewLine();

			if (ImGui::Button("OK", helpers::getScreen().centerXButton)) { ImGui::CloseCurrentPopup(); }
			ImGui::SetItemDefaultFocus();
			ImGui::EndPopup();
		}
	}


	void drawMenu()
	{
		makeCfgFileButtDialog("Custom_tilesOpenDialog", "Open a custom_tiles", data.selectedFiles.custom_tiles, "Set custom_tiles.yoneuoycfg");
		ImGui::Separator();

		makeCfgFileButtDialog("stratFortsModelsChangeListOpenDialog", "Open a stratFortsModelsChangeList", data.selectedFiles.stratFortsModelsChangeList, "Set stratFortsModelsChangeList.yoneuoycfg");
		ImGui::Separator();

		makeCfgFileButtDialog("stratModelsChangeList", "Open a stratModelsChangeList", data.selectedFiles.stratModelsChangeList, "Set stratModelsChangeList.yoneuoycfg");
		ImGui::Separator();

		makeCfgFileButtDialog("stratPortsModelsChangeList", "Open a stratPortsModelsChangeList", data.selectedFiles.stratPortsModelsChangeList, "Set stratPortsModelsChangeList.yoneuoycfg");
		ImGui::Separator();
		ImGui::NewLine();
		makeCfgSaveDialog();

		checkAndMakeConvertation();
	}

	void drawWait()
	{

	}


	void draw(bool* isOpen)
	{
		ImVec2 windowSize = ImGui::CalcTextSize("Convert m2tweop v1.x models change files");
		windowSize.x *= 2.0f;
		ImGui::SetNextWindowPos(helpers::getScreen().screenHalfSize, ImGuiCond_Once, ImVec2(0.5f, 0.5f));
		ImGui::SetNextWindowSize(ImVec2(windowSize.x, -1.f));
		ImGui::Begin("Select files", isOpen, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);

		if (data.convertationStarted == false)
		{
			drawMenu();
		}
		else if (data.convertationStarted == true
			&& data.convertationEnded == false
			)
		{
			drawWait();
		}
		else if (data.convertationStarted == true
			&& data.convertationEnded == true)
		{
			data.selectedFiles.erase();
			data.convertationStarted = false;
			data.convertationEnded = false;

			ImGui::OpenPopup("Done");
		}

		

		ImGui::End();


		if (data.convertationStarted == false)
		{
			checkFilesDialogs();
			checkCfgSaveDialog();
		}
	}
};
