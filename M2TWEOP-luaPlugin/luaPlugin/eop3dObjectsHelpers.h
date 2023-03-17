#pragma once
#include <string>
#include "realGameTypes.h"
#include "luaGetSetFuncs.h"
#include "eopMasterTypes.h"
namespace eop3dObjectsHelpers
{
	//returns "id" of font, which is pointer to object
	void* MakeTextFont(const char* fontName);
	void DeleteTextFont(void* fontID);

	//returns "id" of text, which is pointer to object
	Text3DDrawable* MakeText(void* fontID, const char* utf8Text);

	void ScaleText(Text3DDrawable* text, float scale);
	void ChangeColorText(Text3DDrawable* text, int a, int r, int g, int b);


	void SetCoordsText(Text3DDrawable* text, float x, float y, float z);

	void StartDrawingText(Text3DDrawable* text);
	void StopDrawingText(Text3DDrawable* text);
	void DrawOnceText(Text3DDrawable* text);
	void DeleteText(Text3DDrawable* text);
};

