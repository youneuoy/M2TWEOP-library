#include "MapTextDrawer.h"
#include "techFuncs.h"

#include "graphicsD3D.h"
namespace MapTextDrawer
{
	struct
	{
		std::list<Text3DDrawable*> textForDrawing;
		bool initialized = false;
	}data;


	static void DrawTextDrawable(Text3DDrawable* text)
	{
		auto device = graphicsExport::GetDevice();

		D3DXMATRIX matRotate;
		float xR = D3DXToRadian(1);
		float yR = D3DXToRadian(90);
		float zR = D3DXToRadian(1);
		D3DXMatrixRotationYawPitchRoll(&matRotate, xR, yR, zR);

		D3DXMATRIX matScale;

		float scalex = 1;
		float scaley = 0.2;
		float scalez = 1;
		D3DXMatrixScaling(&matScale, scalex, scaley, scalez);



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

	NOINLINE EOP_EXPORT void* MakeTextFont(const char* fontName)
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

		hFont = CreateFontW(0, 0, 0, 0, FW_NORMAL, 1, NULL, NULL, NULL, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
			fontWname.c_str());
		//L"Times New Roman");


		return hFont;
	}

	NOINLINE EOP_EXPORT void DeleteTextFont(void* fontID)
	{
		HFONT hFont = reinterpret_cast<HFONT>(fontID);
		DeleteObject(hFont);
	}

	NOINLINE EOP_EXPORT Text3DDrawable* MakeText(void* fontID, const char* utf8Text)
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
	NOINLINE EOP_EXPORT void Delete3dText(Text3DDrawable* text)
	{
		text->isDeleteNeeded = true;
		data.textForDrawing.remove(text);
		text->textMesh->Release();
		delete text;
	}
	NOINLINE EOP_EXPORT void ScaleText(Text3DDrawable* text, float scale)
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
	NOINLINE EOP_EXPORT void ChangeTextColor(Text3DDrawable* text, unsigned char a, unsigned char r, unsigned char g, unsigned char b)
	{
		if (text == nullptr)
		{
			return;
		}

		text->color = D3DCOLOR_ARGB(a, r, g, b);
	}
	NOINLINE EOP_EXPORT void SetTextDrawingCoords(Text3DDrawable* text, float x, float y, float z)
	{
		if (text == nullptr)
		{
			return;
		}
		
		text->xCoord = x;
		text->yCoord = y;
		text->zCoord = z;
	}


	NOINLINE EOP_EXPORT void StartDrawingText(Text3DDrawable* text)
	{
		text->isDrawOnce = false;
		data.textForDrawing.push_back(text);
	}
	NOINLINE EOP_EXPORT void StopDrawingText(Text3DDrawable* text)
	{
		data.textForDrawing.remove(text);
	}
	NOINLINE EOP_EXPORT void DrawingTextOnce(Text3DDrawable* text)
	{
		text->isDrawOnce = true;
		data.textForDrawing.push_back(text);
	}
};