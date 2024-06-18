#include "MapTextDrawer.h"
#include "techFuncs.h"

#include "graphicsD3D.h"

#include <mutex>

#include "lua/sol.hpp"

namespace mapTextDrawer
{
	coordsVText::dataS coordsVText::Data;

	struct
	{
		std::list<Text3DDrawable*> textForDrawing;
		bool initialized = false;
	}data;


	static void DrawTextDrawable(Text3DDrawable* text)
	{
		auto device = graphicsExport::GetDevice();

		D3DXMATRIX matRotate;
		float xR = D3DXToRadian(text->xRoll);
		float yR = D3DXToRadian(text->yRoll);
		float zR = D3DXToRadian(text->zRoll);
		D3DXMatrixRotationYawPitchRoll(&matRotate, xR, yR, zR);

		D3DXMATRIX matScale;

		D3DXMatrixScaling(&matScale, text->xSize, text->ySize, text->zSize);



		D3DXMATRIXA16 matWorld;
		D3DXMatrixIdentity(&matWorld);
		matWorld[12] = text->xCoord;
		matWorld[13] = text->zCoord;
		matWorld[14] = text->yCoord;


		D3DXMATRIX wstate = (matRotate * matScale * matWorld);

		device->SetTransform(D3DTS_WORLD, &wstate);
		device->SetTextureStageState(0, D3DTSS_CONSTANT, text->color);

		text->textMesh->DrawSubset(0);
	}

	void DrawTexts()
	{
		auto device = graphicsExport::GetDevice();
		if (device == nullptr)
		{
			return;
		}
		auto state = graphicsExport::GetD3dState();
		graphicsExport::SetClearD3dState();

		device->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
		device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

		device->SetTransform(D3DTS_VIEW, graphicsExport::GetMatView());
		device->SetTransform(D3DTS_PROJECTION, graphicsExport::GetMatProj());

		device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
		device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
		device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_CONSTANT);
		device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_CONSTANT);


		auto it = data.textForDrawing.begin();
		while (it != data.textForDrawing.end())
		{
			DrawTextDrawable(*it);

			if ((*it)->isDrawOnce==true)
			{
				it = data.textForDrawing.erase(it);
			}
			else
			{
				++it;
			}
		}

		graphicsExport::SetD3dState(state);
		graphicsExport::ReleaseD3dState(state);
	}

	void* MakeTextFontLua(const char* fontName, sol::optional<int> weight, sol::optional<bool> isItalic)
	{
		return makeTextFont(fontName, weight.value_or(400), isItalic.value_or(1));
	}

	void* makeTextFont(const char* fontName, int weight, int isItalic)
	{
		if (fontName == nullptr)
		{
			return nullptr;
		}

		if (data.initialized == false)
		{
			graphicsExport::AddStratmapDrawCallback(DrawTexts);
		}
		data.initialized = true;


		std::wstring fontWname = techFuncs::ConvertUtf8ToWide(fontName);
		HFONT hFont;

		hFont = CreateFontW(0, 0, 0, 0, weight, isItalic, NULL, NULL, NULL, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
			fontWname.c_str());
		//L"Times New Roman");


		return hFont;
	}

	void deleteTextFont(void* fontID)
	{
		HFONT hFont = reinterpret_cast<HFONT>(fontID);
		DeleteObject(hFont);
	}

	Text3DDrawable* makeText(void* fontID, const char* utf8Text)
	{
		if (utf8Text == nullptr)
		{
			return nullptr;
		}
		if (fontID == nullptr)
		{
			return nullptr;
		}
		std::wstring utf16Text = techFuncs::ConvertUtf8ToWide(utf8Text);

		Text3DDrawable* newText = new Text3DDrawable();
		ChangeTextColor(newText, 255, 255, 255, 255);

		HDC hdc = CreateCompatibleDC(0);

		HFONT hFont;
		HFONT hFontOld;
		hFont = reinterpret_cast<HFONT>(fontID);
		hFontOld = (HFONT)SelectObject(hdc, hFont);
		HRESULT res = D3DXCreateTextW(graphicsExport::GetDevice(), hdc, utf16Text.c_str(), 0.001f, 0.4f, &newText->textMesh, NULL, NULL);

		SelectObject(hdc, hFontOld);
		DeleteDC(hdc);

		if (newText->textMesh == nullptr)
		{
			delete newText;
			newText = nullptr;
		}

		return newText;
	}
	void Delete3dText(Text3DDrawable* text)
	{
		if (text == nullptr)
		{
			return;
		}
		text->isDeleteNeeded = true;
		data.textForDrawing.remove(text);
		text->textMesh->Release();
		delete text;
	}
	void ScaleText(Text3DDrawable* text, float scale)
	{
		if (text == nullptr)
		{
			return;
		}
		LPD3DXMESH TextMesh = text->textMesh;

		D3DXVECTOR3* data = NULL;
		DWORD vSize = TextMesh->GetNumBytesPerVertex();
		DWORD vCount = TextMesh->GetNumVertices();
		TextMesh->LockVertexBuffer(0, (void**)&data);

		for (DWORD i = 0; i < vCount; ++i)
		{
			*data *= scale;
			data = (D3DXVECTOR3*)((BYTE*)data + vSize);
		}

		TextMesh->UnlockVertexBuffer();
	}
	void SetDimensionsTextXYZ(Text3DDrawable* text, float scaleX, float scaleY, float scaleZ)
	{
		if (text == nullptr)
		{
			return;
		}

		text->xSize = scaleX;
		text->ySize = scaleY;
		text->zSize = scaleZ;
	}
	void SetRotationTextXYZ(Text3DDrawable* text, float rotX, float rotY, float rotZ)
	{
		if (text == nullptr)
		{
			return;
		}

		text->xRoll = rotX;
		text->xRoll = rotY;
		text->zRoll = rotZ;
	}
	void ChangeTextColor(Text3DDrawable* text, unsigned char a, unsigned char r, unsigned char g, unsigned char b)
	{
		if (text == nullptr)
		{
			return;
		}

		text->color = D3DCOLOR_ARGB(a, r, g, b);
	}
	void SetTextDrawingCoords(Text3DDrawable* text, float x, float y, float z)
	{
		if (text == nullptr)
		{
			return;
		}
		
		text->xCoord = x;
		text->yCoord = y;
		text->zCoord = z;
	}


	void StartDrawingText(Text3DDrawable* text)
	{
		text->isDrawOnce = false;
		data.textForDrawing.push_back(text);
	}
	void StopDrawingText(Text3DDrawable* text)
	{
		data.textForDrawing.remove(text);
	}
	void drawingTextOnce(Text3DDrawable* text)
	{
		text->isDrawOnce = true;
		data.textForDrawing.push_back(text);
	}
	coordsVText::coordsVText(int x, int y, mapTextDrawer::Text3DDrawable* pointText)
		:PointText(pointText), X(x), Y(y)
	{
		static std::once_flag initFLAG;
		std::call_once(initFLAG, TryInit);
	}
	coordsVText::~coordsVText()
	{
		mapTextDrawer::Delete3dText(PointText);
	}
	void coordsVText::SetTileToLive(float seconds)
	{
		LiveTimeEnd = Data.CurrTime + seconds;
		Data.SelfControlled.emplace_back(this);
	}
	void coordsVText::TryInit()
	{
		graphicsExport::AddImGuiDrawCallback(Draw);
	}
	void coordsVText::Draw()
	{
		Data.CurrTime = ImGui::GetTime();

		auto it = Data.SelfControlled.begin();
		while (it != Data.SelfControlled.end())
		{
			drawingTextOnce((*it)->PointText);

			if ((*it)->LiveTimeEnd < Data.CurrTime)
			{
				it = Data.SelfControlled.erase(it);
			}
			else
			{
				++it;
			}
		}
	}
};