#include <boost/process.hpp>

#include "gameRunnerUI.h"
#include "mainUI.h"

#include "headersSTD.h"
#include "dataG.h"
#include "helpers.h"

#include <thread>
#include "markdownHelper.h"
namespace gameRunnerUI
{
	void TextAtCenter(const char *text)
	{
		ImVec2 tSize = ImGui::CalcTextSize(text);
		float font_size = tSize.x;
		ImGui::SameLine(
			ImGui::GetWindowSize().x / 2 -
			font_size + (font_size / 2));

		ImGui::Text(text);
	}
	namespace bp = boost::process;
	struct
	{
		string exePath;
		string exeArgs;
		string eopArgs;
		bool isEopNeeded = false;
		;

		atomic_bool isRunStarted{false};
		atomic_bool isRunEnded{false};
		atomic_bool isGetResponce{false};
		bool isRunProcessInitiated = false;

		float xWindowSize = 500.f;

		string loadingText = "If something goes wrong, then try to run the program with administrator rights";
		bool isWindowTooSmall = false;

		float sendingEndTime = 0.f;

	} startProcess;
	void setRunParams(const string &exePath, const string &exeArgs, const string &eopArgs, bool isEopNeeded)
	{

		startProcess.exePath = exePath;
		startProcess.exeArgs = exeArgs;
		startProcess.eopArgs = eopArgs;
		startProcess.isEopNeeded = isEopNeeded;
		startProcess.isRunProcessInitiated = true;

		startProcess.xWindowSize = ImGui::CalcTextSize(startProcess.loadingText.c_str()).x + 100.f;
		if (helpers::getScreen().screenSize.x < startProcess.xWindowSize)
		{
			startProcess.xWindowSize = ImGui::CalcTextSize("Have problems? Run the program with administrator rights").x + 100.f;
			if (helpers::getScreen().screenSize.x < startProcess.xWindowSize)
			{
				startProcess.isWindowTooSmall = true;
			}
			else
			{
				startProcess.loadingText = "Have problems? Run the program with administrator rights";
			}
		}
	}
	void runGameThread(std::atomic_bool &isStarted, std::atomic_bool &isEnded, std::atomic_bool &isGetResponce, const string &exePath, const string &exeArgs, const string &eopArgs, bool isEopNeeded)
	{

		string startArgs = exeArgs;
		startArgs.erase(0, 1);

		bp::child gameProcess(
			dataG::data.gameData.gamePath,
			bp::args(startArgs),
			bp::start_dir = "..\\..");
		gameProcess.detach();
		bool startResult = helpers::doPipe(eopArgs, 200);

		if (startResult == false)
		{
			helpers::closeGame(dataG::data.gameData.exeName);

			isGetResponce = false;
		}

		isEnded = true;
		isGetResponce = true;
	}

	void drawDiscordUI(bool *isOpen)
	{
	std:
		string info = "If you disable Discord Rich Presence, this window will no longer appear.";

		ImGui::SetNextWindowPos(helpers::getScreen().screenUpperPos, ImGuiCond_Once, ImVec2(0.5f, 0.5f));
		if (startProcess.isWindowTooSmall == false)
		{
			ImGui::SetNextWindowSize(ImVec2(startProcess.xWindowSize, -1));
		}
		else
		{
			ImGui::SetNextWindowSize(ImVec2(helpers::getScreen().screenSize.x * 0.7f, -1));
		}

		ImGui::Begin("EOP: Discord Rich Presence", isOpen, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus);

		// Animate a simple progress bar
		if (startProcess.isWindowTooSmall == false)
		{
			ImGui::ProgressBar(ImGui::GetTime() * -0.05f, ImVec2(-1.0f, 0.0f), "");
			TextAtCenter(info.c_str());
		}
		else
		{
			ImGui::TextWrapped(info.c_str());
			ImGui::ProgressBar(ImGui::GetTime() * -0.05f, ImVec2(-1.0f, 0.0f));
		}

		ImGui::End();
	}

	int drawUI(bool *isOpen)
	{
		ImGui::SetNextWindowPos(helpers::getScreen().screenUpperPos, ImGuiCond_Once, ImVec2(0.5f, 0.5f));
		if (startProcess.isWindowTooSmall == false)
		{
			ImGui::SetNextWindowSize(ImVec2(startProcess.xWindowSize, -1));
		}
		else
		{
			ImGui::SetNextWindowSize(ImVec2(helpers::getScreen().screenSize.x * 0.5f, -1));
		}

		ImGui::Begin("This window should close automatically", isOpen, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);

		// Animate a simple progress bar
		if (startProcess.isWindowTooSmall == false)
		{
			ImGui::ProgressBar(ImGui::GetTime() * -0.2f, ImVec2(-1.0f, 0.0f), "");
			TextAtCenter(startProcess.loadingText.c_str());
		}
		else
		{
			ImGui::TextWrapped(startProcess.loadingText.c_str());
			ImGui::ProgressBar(ImGui::GetTime() * -0.2f, ImVec2(-1.0f, 0.0f));
		}

		if (startProcess.isRunStarted == true &&
			(ImGui::GetTime() > startProcess.sendingEndTime || (startProcess.isRunEnded == true && startProcess.isGetResponce == false)))
		{
			const std::string badMSG = R"(
![badLogo](eopData/images/deathDance.png)
## Perhaps something is wrong, read this text carefully and follow the indicated points **in the order in which they are indicated, each time restarting everything to check the solution** if:
* If the game won't start
* If the game starts, but no text appears in the upper left corner indicating the M2TWEOP version
* If the game starts, you see text in the left corner indicating the version of M2TWEOP, but at the same time this window does not close
1. Try to run M2TWEOP GUI.exe with administrator rights.
2. Check for the presence of files with the name **d3d9.dll and also M2TWEOPLibrary.dll in the folder with the modification**, if one of them is missing, reinstall M2TWOP.
3. Check for the existence of a file named **d3d9.dl in the folder with your game**, if it is there, delete it.
## If you are using the steam version of the game and the steam is being initialized at the moment, then there is no problem
)";
			markdownHelper::setFonts();
			markdownHelper::drawMarkdown(badMSG);
		}

		ImGui::End();

		if (startProcess.isRunStarted == false)
		{
			startProcess.isRunStarted = true;

			std::thread thrUrl(
				runGameThread, std::ref(startProcess.isRunStarted), std::ref(startProcess.isRunEnded), std::ref(startProcess.isGetResponce), std::ref(startProcess.exePath), std::ref(startProcess.exeArgs), std::ref(startProcess.eopArgs), startProcess.isEopNeeded);
			thrUrl.detach();

			startProcess.sendingEndTime = ImGui::GetTime() + 34.5f;
		}

		if ((startProcess.isRunEnded == true && startProcess.isGetResponce == true) || *isOpen == false)
		{
			if (startProcess.isGetResponce == false)
			{
				helpers::closeGame(dataG::data.gameData.exeName);
			}
			if (dataG::data.gameData.isDiscordRichPresenceEnabled == false)
			{
				exit(0);
			}
			else
			{
				*isOpen = false;
				return 2;
			}
		}
	}
}