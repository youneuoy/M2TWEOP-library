#pragma once
#include "headersSTD.h"
#include "exportHeader.h"
#include "headersMEM.h"
#include "lua/sol.hpp"

struct text3dDrawable
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
namespace mapTextDrawer
{

	void drawTexts();

	//returns "id" of font, which is pointer to object
	void* makeTextFont(const char* fontName, int weight, int isItalic);
	void* makeTextFontLua(const char* fontName, sol::optional<int> weight, sol::optional<bool> isItalic);
	
	void deleteTextFont(void*  fontId);

	//returns "id" of text, which is pointer to object
	text3dDrawable* makeText(void* fontId,const char* utf8Text);
	void delete3dText(text3dDrawable*text);

	void scaleText(const text3dDrawable* text,float scale);
	void setDimensionsTextXyz(text3dDrawable* text,float scaleX, float scaleY, float scaleZ);
	void setRotationTextXyz(text3dDrawable* text,float rotX, float rotY, float rotZ);
	void changeTextColor(text3dDrawable* text, unsigned char a, unsigned char r, unsigned char g, unsigned char b);


	void setTextDrawingCoords(text3dDrawable* text,float x, float y, float z);

	void startDrawingText(text3dDrawable* text);
	void stopDrawingText(text3dDrawable* text);
	void drawingTextOnce(text3dDrawable* text);

	struct coordsVText
	{
	public:
		coordsVText() = delete;
		coordsVText(int x, int y, text3dDrawable* pointText);
		~coordsVText();
		text3dDrawable* PointText = nullptr;
		int X = 0;
		int Y = 0;

		//it self-delete then
		void setTileToLive(float seconds);

		static void tryInit();
		static void draw();
	private:
		static struct dataS{
			float CurrTime = 0;
			std::list<std::shared_ptr<coordsVText>> SelfControlled;
		}m_Data;
		float LiveTimeEnd=0;
	};
};

