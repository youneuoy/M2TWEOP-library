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
	NOINLINE EOP_EXPORT void* MakeTextFont(const char* fontName, int weight, int isItalic);
	NOINLINE EOP_EXPORT void DeleteTextFont(void*  fontID);

	//returns "id" of text, which is pointer to object
	NOINLINE EOP_EXPORT Text3DDrawable* MakeText(void* fontID,const char* utf8Text);
	NOINLINE EOP_EXPORT void Delete3dText(Text3DDrawable*text);

	NOINLINE EOP_EXPORT void ScaleText(Text3DDrawable* text,float scale);
	NOINLINE EOP_EXPORT void SetDimensionsTextXYZ(Text3DDrawable* text,float scaleX, float scaleY, float scaleZ);
	NOINLINE EOP_EXPORT void SetRotationTextXYZ(Text3DDrawable* text,float rotX, float rotY, float rotZ);
	NOINLINE EOP_EXPORT void ChangeTextColor(Text3DDrawable* text, unsigned char a, unsigned char r, unsigned char g, unsigned char b);


	NOINLINE EOP_EXPORT void SetTextDrawingCoords(Text3DDrawable* text,float x, float y, float z);

	NOINLINE EOP_EXPORT void StartDrawingText(Text3DDrawable* text);
	NOINLINE EOP_EXPORT void StopDrawingText(Text3DDrawable* text);
	NOINLINE EOP_EXPORT void DrawingTextOnce(Text3DDrawable* text);

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

