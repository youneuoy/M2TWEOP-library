#pragma once
#include "headersSTD.h"
#include "exportHeader.h"
#include "headersMEM.h"
#include "lua/sol.hpp"

namespace MapTextDrawer
{

	struct Text3DDrawable
	{
		LPD3DXMESH textMesh;
		DWORD color;
		float xCoord;
		float yCoord;
		float zCoord;
		float xSize=1;
		float ySize=0.2f;
		float zSize=1;
		float xRoll=1;
		float yRoll=90;
		float zRoll=1;
		bool isDrawOnce;
		bool isDeleteNeeded;
	};
	void DrawTexts();

	//returns "id" of font, which is pointer to object
	void* MakeTextFont(const char* fontName, int weight, int isItalic);
	void* MakeTextFontLua(const char* fontName, sol::optional<int> weight, sol::optional<bool> isItalic)
	
	void DeleteTextFont(void*  fontID);

	//returns "id" of text, which is pointer to object
	Text3DDrawable* MakeText(void* fontID,const char* utf8Text);
	void Delete3dText(Text3DDrawable*text);

	void ScaleText(Text3DDrawable* text,float scale);
	void SetDimensionsTextXYZ(Text3DDrawable* text,float scaleX, float scaleY, float scaleZ);
	void SetRotationTextXYZ(Text3DDrawable* text,float rotX, float rotY, float rotZ);
	void ChangeTextColor(Text3DDrawable* text, unsigned char a, unsigned char r, unsigned char g, unsigned char b);


	void SetTextDrawingCoords(Text3DDrawable* text,float x, float y, float z);

	void StartDrawingText(Text3DDrawable* text);
	void StopDrawingText(Text3DDrawable* text);
	void DrawingTextOnce(Text3DDrawable* text);

	struct coordsVText
	{
	public:
		coordsVText() = delete;
		coordsVText(int x, int y, MapTextDrawer::Text3DDrawable* pointText);
		~coordsVText();
		MapTextDrawer::Text3DDrawable* PointText = nullptr;
		int X = 0;
		int Y = 0;

		//it self-delete then
		void SetTileToLive(float seconds);

		static void TryInit();
		static void Draw();
	private:
		static struct dataS{
			float CurrTime = 0;
			std::list<std::shared_ptr<coordsVText>> SelfControlled;
		}Data;
		float LiveTimeEnd=0;
	};
};

