#include "redistUI.h"

#include "headersSTD.h"
#include "helpers.h"
#include "dataG.h"

#include <shellapi.h>
#include <filesystem>

#include <boost/process.hpp>
#include <boost/process/windows.hpp>
#include <atomic>
namespace redistUI
{
	void TextAtCenter(const char* text)
	{
		ImVec2 tSize = ImGui::CalcTextSize(text);
		float font_size = tSize.x;
		ImGui::SameLine(
			ImGui::GetWindowSize().x / 2 -
			font_size + (font_size / 2)
		);

		ImGui::Text(text);
	}
	namespace bp = boost::process;

	struct procS
	{
	public:
		enum class installersState
		{
			nothing,
			dx,
			msvc,
			done,
			error,
			forceStop,
			stopped
		};
		std::string getStateStringFromState()
		{
			string res;
			switch (installState)
			{
			case redistUI::procS::installersState::nothing:
				res = "Now we do nothing";
				break;
			case redistUI::procS::installersState::dx:
				res = "Installing DirectX SDK 9.0c";
				break;
			case redistUI::procS::installersState::msvc:
				res = "Installing Visual C++ Redistributable x86 Runtimes";
				break;
			case redistUI::procS::installersState::done:
				res = "Installation done";
				break;
			case redistUI::procS::installersState::error:
				res = "Installation error";
				break;
			case redistUI::procS::installersState::forceStop:
				res = "Stopping installation";
				break;
			case redistUI::procS::installersState::stopped:
				res = "Installation terminated";
				break;
			default:
				res = "";
			}
			return res;
		}
		std::atomic<installersState> installState{ installersState::nothing };

		void runInstallers()
		{
			worker();
		}
	private:
		void runAndWait(const std::string& path, const std::string& arg)
		{
			bp::child process(
				path.c_str(),
				bp::args(arg.c_str())
			);
			if (!process.valid())
			{
				throw;
			}
			while (!process.wait_for(std::chrono::seconds(1)))
			{
				if (installState == installersState::forceStop)
				{
					process.terminate();
					return;
				}
			}
		}

		void runInstallersInternal()
		{
			try
			{
				installState = installersState::msvc;
				//runAndWait("eopRedistributables\\MS_VisualC_Redist_Hybrid\\VCR_Hybrid_x86.exe", "/S");
			}
			catch (...)
			{
				installState = installersState::error;
			}
			if (installState == installersState::error)
			{
				return;
			}


			if (installState == installersState::forceStop)
			{
				installState = installersState::stopped;
				return;
			}


			try
			{
				installState = installersState::dx;
				//runAndWait("eopRedistributables\\DirectX_SDK\\DXSETUP.exe", "/Silent");
			}
			catch (...)
			{
				installState = installersState::error;
			}
			if (installState == installersState::error)
			{
				return;
			}

			if (installState == installersState::forceStop)
			{
				installState = installersState::stopped;
				return;
			}
			installState = installersState::done;
		}
		void worker()
		{
			std::thread thrUrl(
				&procS::runInstallersInternal, this);
			thrUrl.detach();
		}
	}startProcesses;
	void drawPopup(ImVec2& size, bool*& isOpen)
	{
		ImGui::SetNextWindowPos(helpers::getScreen().screenHalfSize, ImGuiCond_Once, ImVec2(0.5f, 0.5f));
		ImGui::SetNextWindowSize(ImVec2(size.x, -1), ImGuiCond_Once);

		ImGui::Begin("M2TWEOP Health Checker##2", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);

		switch (startProcesses.installState)
		{
		case redistUI::procS::installersState::dx:
		case redistUI::procS::installersState::msvc:
		case redistUI::procS::installersState::forceStop:
			ImGui::ProgressBar(ImGui::GetTime() * -0.2f, ImVec2(-1.0f, 0.0f), "");
			TextAtCenter(startProcesses.getStateStringFromState().c_str());
			if (ImGui::Button("Terminate install", helpers::getScreen().centerXButton))
			{
				startProcesses.installState = redistUI::procS::installersState::forceStop;
			}
			break;

		case redistUI::procS::installersState::nothing:
		case redistUI::procS::installersState::done:
		case redistUI::procS::installersState::error:
		case redistUI::procS::installersState::stopped:
		default:
			ImGui::ProgressBar(1.f, ImVec2(-1.0f, 0.0f), "");
			TextAtCenter(startProcesses.getStateStringFromState().c_str());
			if (ImGui::Button("To main menu", helpers::getScreen().centerXButton))
			{
				*isOpen = false;
			}
		}

		ImGui::End();

	}

	void drawWindow(ImVec2& size, bool*& isOpen)
	{
		ImGui::SetNextWindowPos(helpers::getScreen().screenHalfSize, ImGuiCond_Once, ImVec2(0.5f, 0.5f));
		ImGui::SetNextWindowSize(ImVec2(size.x, -1), ImGuiCond_Once);

		ImGui::Begin("M2TWEOP Health Checker##1", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);

		const char logoName[] = "eopData/resources/images/deathDance.png";
		GLImage* imgFinded = helpers::findImage(logoName, sizeof logoName - 1);

		ImVec2 const csz = ImGui::GetContentRegionAvail();
		ImVec2 logoSize = ImVec2((float)imgFinded->xSize, (float)imgFinded->ySize);
		if (logoSize.x > csz.x) {
			const float r = logoSize.y / logoSize.x;
			logoSize.x = csz.x;
			logoSize.y = csz.x * r;
		}
		ImGui::SetCursorPosX((size.x - logoSize.x) * 0.5f);
		ImGui::Image((void*)(intptr_t)imgFinded->image, logoSize);

		ImGuiContext& g = *GImGui;
		const ImGuiStyle& style = g.Style;
		const ImVec2 label_size = ImGui::CalcTextSize("Possible startup problems detected. Consider these paths for a solution.", NULL, true);
		if (ImGui::Button("Possible startup problems detected. Consider these paths for a solution.", ImVec2(-1.0f, (label_size.y + style.FramePadding.y * 2.0f) * 2)))
		{
		}
		ImGui::NewLine();
		if (ImGui::Button("Go to the M2TWEOP discord server for help", helpers::getScreen().centerXButton))
		{
			ShellExecuteA(NULL, "open", "https://discord.gg/xpPrysj", NULL, NULL, SW_SHOWNORMAL);
		}
		if (ImGui::Button("View information on the M2TWEOP website", helpers::getScreen().centerXButton))
		{
			ShellExecuteA(NULL, "open", "https://youneuoy.github.io/M2TWEOP-library/ ", NULL, NULL, SW_SHOWNORMAL);
		}
		ImGui::NewLine();
		if (ImGui::Button("To main menu", helpers::getScreen().centerXButton))
		{
			*isOpen = false;
		}

		ImGui::End();
	}
	int draw(bool* isOpen)
	{
		ImVec2 windowSize = ImGui::CalcTextSize("Possible startup problems detected. Consider these paths for a solution.");
		windowSize.x *= 1.2;

		if (startProcesses.installState == redistUI::procS::installersState::nothing)
		{
			drawWindow(windowSize, isOpen);
		}
		else
		{
			drawPopup(windowSize, isOpen);
		}
		if (*isOpen == 0)
		{
			return 1;
		}

		return 0;
	}
};