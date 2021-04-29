#include "markdownHelper.h"
struct
{
	ImFont* g_font_regular = nullptr;
	ImFont* g_font_bold = nullptr;
	ImFont* g_font_bold_large = nullptr;
}fontsS;

struct my_markdown : public imgui_md
{


	ImFont* get_font() const override
	{
		if (m_is_table_header) {
			return fontsS.g_font_bold;
		}

		switch (m_hlevel)
		{
		case 0:
			return m_is_strong ? fontsS.g_font_bold : fontsS.g_font_regular;
		case 1:
			return fontsS.g_font_bold_large;
		default:
			return fontsS.g_font_bold;
		}
	};


	void open_url() const override
	{
		//platform dependent code
		ShellExecuteA(NULL, "open", m_href.c_str(), NULL, NULL, SW_SHOWNORMAL);

	}


	bool get_image(image_info& nfo) const override
	{

		GLImage* imgFinded = helpers::findImage(m_href.c_str(), strlen(m_href.c_str()));


		nfo.texture_id = (void*)(intptr_t)imgFinded->image;
		nfo.size = ImVec2((float)imgFinded->xSize, (float)imgFinded->ySize);
		nfo.uv0 = { 0,0 };
		nfo.uv1 = { 1,1 };
		nfo.col_tint = { 1,1,1,1 };
		nfo.col_border = { 0,0,0,0 };


		return true;
	}

	void html_div(const std::string& dclass, bool e) override
	{
		if (dclass == "red") {
			if (e) {
				m_table_border = false;
				ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 0, 0, 255));
			}
			else {
				ImGui::PopStyleColor();
				m_table_border = true;
			}
		}
	}
};
//call this function to render your markdown
void markdown(const char* str, const char* str_end)
{
	static my_markdown s_printer;
	s_printer.print(str, str_end);
}
void markdownHelper::setFonts()
{
	fontsS.g_font_regular = helpers::findFont("markH1");
	fontsS.g_font_bold = helpers::findFont("markH2");
	fontsS.g_font_bold_large = helpers::findFont("markH3");
}

void markdownHelper::drawMarkdown(const string& text)
{
	const char* end_ptr = text.c_str() + text.size();
	markdown(text.c_str(), end_ptr);
	
}
