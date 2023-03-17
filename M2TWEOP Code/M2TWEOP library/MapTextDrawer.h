#pragma once
#include "headersSTD.h"
#include "exportHeader.h"
#include "headersMEM.h"

namespace MapTextDrawer
{
	struct Text3DDrawable
	{
		LPD3DXMESH textMesh;
		DWORD color;
		float xCoord;
		float yCoord;
		float zCoord;
		bool isDrawOnce;
		bool isDeleteNeeded;
	};
	void DrawTexts();

	//returns "id" of font, which is pointer to object
	NOINLINE EOP_EXPORT void* MakeTextFont(const char* fontName);
	NOINLINE EOP_EXPORT void DeleteTextFont(void*  fontID);

	//returns "id" of text, which is pointer to object
	NOINLINE EOP_EXPORT Text3DDrawable* MakeText(void* fontID,const char* utf8Text);
	NOINLINE EOP_EXPORT void Delete3dText(Text3DDrawable*text);

	NOINLINE EOP_EXPORT void ScaleText(Text3DDrawable* text,float scale);
	NOINLINE EOP_EXPORT void ChangeTextColor(Text3DDrawable* text, unsigned char a, unsigned char r, unsigned char g, unsigned char b);


	NOINLINE EOP_EXPORT void SetTextDrawingCoords(Text3DDrawable* text,float x, float y, float z);

	NOINLINE EOP_EXPORT void StartDrawingText(Text3DDrawable* text);
	NOINLINE EOP_EXPORT void StopDrawingText(Text3DDrawable* text);
	NOINLINE EOP_EXPORT void DrawingTextOnce(Text3DDrawable* text);
};

