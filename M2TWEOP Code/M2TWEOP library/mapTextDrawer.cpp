#include "pch.h"
#include "mapTextDrawer.h"
#include "techFuncs.h"

#include "graphicsD3D.h"

#include "lua/sol.hpp"

namespace mapTextDrawer
{
	coordsVText::dataS coordsVText::m_Data;

	struct
	{
		std::list<Text3DDrawable*> textForDrawing;
		bool initialized = false;
	}DATA;

	namespace
	{
		void drawTextDrawable(const Text3DDrawable* text)
		{
			const auto device = graphicsExport::GetDevice();

			D3DXMATRIX matRotate;
			const float xR = D3DXToRadian(text->xRoll);
			const float yR = D3DXToRadian(text->yRoll);
			const float zR = D3DXToRadian(text->zRoll);
			D3DXMatrixRotationYawPitchRoll(&matRotate, xR, yR, zR);
			D3DXMATRIX matScale;
			D3DXMatrixScaling(&matScale, text->xSize, text->ySize, text->zSize);
		
			D3DXMATRIXA16 matWorld;
			D3DXMatrixIdentity(&matWorld);
			matWorld[12] = text->xCoord;
			matWorld[13] = text->zCoord;
			matWorld[14] = text->yCoord;
		
			const D3DXMATRIX wState = (matRotate * matScale * matWorld);

			device->SetTransform(D3DTS_WORLD, &wState);
			device->SetTextureStageState(0, D3DTSS_CONSTANT, text->color);

			text->textMesh->DrawSubset(0);
		}
	}
	
	void drawTexts()
	{
		const auto device = graphicsExport::GetDevice();
		if (device == nullptr)
			return;
		
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
		
		auto it = DATA.textForDrawing.begin();
		while (it != DATA.textForDrawing.end())
		{
			drawTextDrawable(*it);
			if ((*it)->isDrawOnce==true)
				it = DATA.textForDrawing.erase(it);
			else
				++it;
		}
		graphicsExport::SetD3dState(state);
		graphicsExport::ReleaseD3dState(state);
	}

	void* makeTextFontLua(const char* fontName, const sol::optional<int> weight, const sol::optional<bool> isItalic)
	{
		return makeTextFont(fontName, weight.value_or(400), isItalic.value_or(1));
	}

	void* makeTextFont(const char* fontName, const int weight, const int isItalic)
	{
		if (fontName == nullptr)
			return nullptr;
		
		if (DATA.initialized == false)
			graphicsExport::AddStratmapDrawCallback(drawTexts);
		
		DATA.initialized = true;
		const std::wstring fontWName = techFuncs::convertUtf8ToWide(fontName);
		
		const HFONT hFont = CreateFontW(0, 0, 0, 0, weight, isItalic,
			NULL, NULL, NULL, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
		                                ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
		                                fontWName.c_str());
		return hFont;
	}

	void deleteTextFont(void* fontId)
	{
		const auto hFont = static_cast<HFONT>(fontId);
		DeleteObject(hFont);
	}

	Text3DDrawable* makeText(void* fontId, const char* utf8Text)
	{
		if (!utf8Text || !fontId)
			return nullptr;
		
		const std::wstring utf16Text = techFuncs::convertUtf8ToWide(utf8Text);
		auto newText = new Text3DDrawable();
		changeTextColor(newText, 255, 255, 255, 255);
		const HDC hdc = CreateCompatibleDC(nullptr);
		const auto hFont = static_cast<HFONT>(fontId);
		const auto hFontOld = static_cast<HFONT>(SelectObject(hdc, hFont));
		HRESULT res = D3DXCreateTextW(graphicsExport::GetDevice(), hdc, utf16Text.c_str(), 0.001f, 0.4f,
			&newText->textMesh, nullptr, nullptr);

		SelectObject(hdc, hFontOld);
		DeleteDC(hdc);
		if (newText->textMesh == nullptr)
		{
			delete newText;
			newText = nullptr;
		}
		return newText;
	}
	
	void delete3dText(Text3DDrawable* text)
	{
		if (text == nullptr)
			return;
		text->isDeleteNeeded = true;
		DATA.textForDrawing.remove(text);
		text->textMesh->Release();
		delete text;
	}
	
	void scaleText(const Text3DDrawable* text, const float scale)
	{
		if (text == nullptr)
			return;
		const LPD3DXMESH textMesh = text->textMesh;
		D3DXVECTOR3* data = nullptr;
		const DWORD vSize = textMesh->GetNumBytesPerVertex();
		const DWORD vCount = textMesh->GetNumVertices();
		textMesh->LockVertexBuffer(0, reinterpret_cast<void**>(&data));
		for (DWORD i = 0; i < vCount; ++i)
		{
			*data *= scale;
			data = reinterpret_cast<D3DXVECTOR3*>(reinterpret_cast<BYTE*>(data) + vSize);
		}
		textMesh->UnlockVertexBuffer();
	}
	
	void setDimensionsTextXyz(Text3DDrawable* text, const float scaleX, const float scaleY, const float scaleZ)
	{
		if (text == nullptr)
			return;
		text->xSize = scaleX;
		text->ySize = scaleY;
		text->zSize = scaleZ;
	}
	
	void setRotationTextXyz(Text3DDrawable* text, const float rotX, const float rotY, const float rotZ)
	{
		if (text == nullptr)
			return;
		text->xRoll = rotX;
		text->xRoll = rotY;
		text->zRoll = rotZ;
	}
	
	void changeTextColor(Text3DDrawable* text, const unsigned char a, const unsigned char r, const unsigned char g, const unsigned char b)
	{
		if (text == nullptr)
			return;
		text->color = D3DCOLOR_ARGB(a, r, g, b);
	}
	
	void setTextDrawingCoords(Text3DDrawable* text, const float x, const float y, const float z)
	{
		if (text == nullptr)
			return;
		text->xCoord = x;
		text->yCoord = y;
		text->zCoord = z;
	}
	
	void startDrawingText(Text3DDrawable* text)
	{
		text->isDrawOnce = false;
		DATA.textForDrawing.push_back(text);
	}
	
	void stopDrawingText(Text3DDrawable* text)
	{
		DATA.textForDrawing.remove(text);
	}
	
	void drawingTextOnce(Text3DDrawable* text)
	{
		text->isDrawOnce = true;
		DATA.textForDrawing.push_back(text);
	}
	
	coordsVText::coordsVText(const int x, const int y, Text3DDrawable* pointText)
		:PointText(pointText), X(x), Y(y)
	{
		static std::once_flag initFlag;
		std::call_once(initFlag, tryInit);
	}
	
	coordsVText::~coordsVText()
	{
		delete3dText(PointText);
	}
	
	void coordsVText::setTileToLive(const float seconds)
	{
		LiveTimeEnd = m_Data.CurrTime + seconds;
		m_Data.SelfControlled.emplace_back(this);
	}
	
	void coordsVText::tryInit()
	{
		graphicsExport::AddImGuiDrawCallback(draw);
	}
	
	void coordsVText::draw()
	{
		m_Data.CurrTime = static_cast<float>(ImGui::GetTime());
		auto it = m_Data.SelfControlled.begin();
		while (it != m_Data.SelfControlled.end())
		{
			drawingTextOnce((*it)->PointText);
			if ((*it)->LiveTimeEnd < m_Data.CurrTime)
				it = m_Data.SelfControlled.erase(it);
			else
				++it;
		}
	}
};