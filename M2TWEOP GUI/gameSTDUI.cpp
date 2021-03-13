#include "gameSTDUI.h"

#include "headersSTD.h"
#include "dataG.h"
#include "helpers.h"
namespace gameSTDUI
{
	struct
	{
		int campaign = 0;
	}gameSTDUIData;

	void drawSTDUI(bool* isOpen)
	{
		ImGui::SetNextWindowPos(helpers::getScreen().screenHalfSize, ImGuiCond_Once, ImVec2(0.5f, 0.5f));
		ImGui::SetNextWindowSize(ImVec2(helpers::getScreen().screenSize.x * 0.4f, -1.f));
		ImGui::Begin("Select campaign", isOpen, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);


		if (ImGui::Button("Grand campaign", helpers::getScreen().centerXButton))
		{
			gameSTDUIData.campaign = 1;
		}
		ImGui::NewLine();
		if (ImGui::Button("Americas", helpers::getScreen().centerXButton))
		{
			gameSTDUIData.campaign = 2;
		}
		if (ImGui::Button("British_isles", helpers::getScreen().centerXButton))
		{
			gameSTDUIData.campaign = 3;
		}
		if (ImGui::Button("Crusades", helpers::getScreen().centerXButton))
		{
			gameSTDUIData.campaign = 4;
		}
		if (ImGui::Button("Teutonic", helpers::getScreen().centerXButton))
		{
			gameSTDUIData.campaign = 5;
		}


		ImGui::End();


		if (gameSTDUIData.campaign!=0)
		{

			STARTUPINFO si;
			PROCESS_INFORMATION pi;

			ZeroMemory(&si, sizeof(si));
			si.cb = sizeof(si);
			ZeroMemory(&pi, sizeof(pi));

			// Start the child process. 
			if (!CreateProcessA(TEXT("..\\..\\medieval2.exe"),   // No module name (use command line)
				NULL,        // Command line
				NULL,           // Process handle not inheritable
				NULL,           // Thread handle not inheritable
				FALSE,          // Set handle inheritance to FALSE
				0,              // No creation flags
				NULL,           // Use parent's environment block
				TEXT("..\\.."),           // Use parent's starting directory 
				&si,            // Pointer to STARTUPINFO structure
				&pi            // Pointer to PROCESS_INFORMATION structure
			))
			{
				DWORD err = GetLastError();
				string s = "Error: ";
				s += to_string(err);
				MessageBox(NULL, "CreateProcess failed",s.c_str(), MB_OK);
			}

			gameSTDUIData.campaign = 0;
		}
	}
};