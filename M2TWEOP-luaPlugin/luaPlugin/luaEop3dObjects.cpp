///
//![Lua logo](../Lua.png)
//M2TWEOP structures and functions. There are not many examples and descriptions here. Also note that the examples do not include many of the checks that would be required when creating modifications.
//@module LuaPlugin
//@author youneuoy
//@license GPL-3.0
#include "luaP.h"
#include "plugData.h"
#include "gameDataAllHelper.h"
#include "battleHandlerHelpers.h"
#include "eopMasterTypes.h"
#include "eop3dObjectsHelpers.h"

void luaP::initEop3dObjects()
{
	struct
	{
		sol::table M2TWEOP3dObjectsTable;
		sol::usertype<Text3DDrawable> text3dDrawable;
	}tables;

	///M2TWEOP3dObjects
	//@section M2TWEOP3dObjects

	/***
	Basic M2TWEOP3dObjects table. Contains descriptions of m2tweop simple 3d objects feathures. Not call this functions onPluginLoad(graphics system not initialized here yet). 

	@tfield MakeTextFont MakeTextFont
	@tfield MakeTextFont DeleteTextFont
	@tfield Make3dText Make3dText

	@table M2TWEOP3dObjects
	*/

	tables.M2TWEOP3dObjectsTable = luaState.create_table("M2TWEOP3dObjects");

	/***
	Create new 3d text font. Fonts using for creating text(you not need font after it)
	@function M2TWEOP3dObjects.MakeTextFont
	@tparam string fontName name of font
	@tparam int weight weight of font(optional). Default - 400
	@tparam bool isItalic is font italic(optional). Default - true
	@treturn int fondID ID of created font.

	@usage
	newFont = M2TWEOP3dObjects.MakeTextFont("Times New Roman");
	*/
	tables.M2TWEOP3dObjectsTable.set_function("MakeTextFont", &eop3dObjectsHelpers::MakeTextFont);
	/***
	Delete 3d text font. Do it where not need anymore
	@function M2TWEOP3dObjects.DeleteTextFont
	@tparam int fondID ID of created font.

	@usage
	newFont = M2TWEOP3dObjects.MakeTextFont("Times New Roman");
		newText = M2TWEOP3dObjects.Make3dText(newFont, "Some text");
	M2TWEOP3dObjects.DeleteTextFont(newFont);
	*/
	tables.M2TWEOP3dObjectsTable.set_function("DeleteTextFont", &eop3dObjectsHelpers::DeleteTextFont);
	/***
	Create line of 3d text.
	@function M2TWEOP3dObjects.Make3dText
	@tparam int fondID ID of created font.
	@tparam string text utf8 string with text.
	@treturn Eop3dText text utf8 string with text.

	@usage
	newFont = M2TWEOP3dObjects.MakeTextFont("Times New Roman");
	newText = M2TWEOP3dObjects.Make3dText(newFont, "Some text");
	M2TWEOP3dObjects.DeleteTextFont(newFont);
	*/
	tables.M2TWEOP3dObjectsTable.set_function("Make3dText", &eop3dObjectsHelpers::MakeText);

	///3dText
	//@section Eop3dText

	/***
	3d text object. Not use it onPluginLoad(graphics system not initialized here yet).  
	@tfield float xSize default value 1
	@tfield float ySize default value 0.2
	@tfield float zSize	default value 1
	@tfield float xRoll default value 1
	@tfield float yRoll default value 90
	@tfield float zRoll default value 1
	@tfield float xCoord
	@tfield float yCoord
	@tfield float zCoord
	@tfield Scale Scale
	@tfield ChangeColor ChangeColor
	@tfield SetCoords SetCoords
	@tfield StartDrawing StartDrawing
	@tfield StopDrawing StopDrawing
	@tfield DrawOnce DrawOnce
	@tfield Delete Delete

	@table Eop3dText
	*/

	tables.text3dDrawable = luaState.new_usertype<Text3DDrawable>("Eop3dText");

	tables.text3dDrawable.set("xSize", &Text3DDrawable::xSize);
	tables.text3dDrawable.set("ySize", &Text3DDrawable::ySize);
	tables.text3dDrawable.set("zSize", &Text3DDrawable::zSize);

	tables.text3dDrawable.set("xRoll", &Text3DDrawable::xRoll);
	tables.text3dDrawable.set("yRoll", &Text3DDrawable::yRoll);
	tables.text3dDrawable.set("zRoll", &Text3DDrawable::zRoll);

	tables.text3dDrawable.set("xCoord", &Text3DDrawable::xCoord);
	tables.text3dDrawable.set("yCoord", &Text3DDrawable::yCoord);
	tables.text3dDrawable.set("zCoord", &Text3DDrawable::zCoord);
	/***
	Scale 3d text.
	@function Eop3dText:Scale
	@tparam float sizeMultiplier
	@usage
	newFont = M2TWEOP3dObjects.MakeTextFont("Times New Roman");
	newText = M2TWEOP3dObjects.Make3dText(newFont, "Some text");
	newText:Scale(0.3);
	*/
	tables.text3dDrawable.set_function("Scale", &eop3dObjectsHelpers::ScaleText);

	/***
	Set 3d text color.
	@function Eop3dText:ChangeColor
	@tparam int a
	@tparam int r
	@tparam int g
	@tparam int b
	@usage
	newFont = M2TWEOP3dObjects.MakeTextFont("Times New Roman");
	newText = M2TWEOP3dObjects.Make3dText(newFont, "Some text");
	newText:ChangeColor(255,0,255,177);
	*/
	tables.text3dDrawable.set_function("ChangeColor", &eop3dObjectsHelpers::ChangeColorText);
	/***
	Set 3d text coords.
	@function Eop3dText:SetCoords
	@tparam float x
	@tparam float y
	@tparam float z
	@usage
	newFont = M2TWEOP3dObjects.MakeTextFont("Times New Roman");
	newText = M2TWEOP3dObjects.Make3dText(newFont, "Some text");
	newText:SetCoords(10,20,0.2);
	*/
	tables.text3dDrawable.set_function("SetCoords", &eop3dObjectsHelpers::SetCoordsText);
	/***
	Start draw text. Text go on display until stop function called
	@function Eop3dText:StartDrawing
	@usage
	newFont = M2TWEOP3dObjects.MakeTextFont("Times New Roman");
	newText = M2TWEOP3dObjects.Make3dText(newFont, "Some text");
	newText:SetCoords(10,20,0.2);
	newText:StartDrawing();
	*/
	tables.text3dDrawable.set_function("StartDrawing", &eop3dObjectsHelpers::StartDrawingText);
	/***
	Stop draw text.
	@function Eop3dText:StopDrawing
	@usage
	newFont = M2TWEOP3dObjects.MakeTextFont("Times New Roman");
	newText = M2TWEOP3dObjects.Make3dText(newFont, "Some text");
	newText:SetCoords(10,20,0.2);
	newText:StartDrawing();
	--some time we draw
	newText:StopDrawing();
	*/
	tables.text3dDrawable.set_function("StopDrawing", &eop3dObjectsHelpers::StopDrawingText);
	/***
	Draw text once(at one frame only). Can be called in draw loop. Not need call stop after it.
	@function Eop3dText:DrawOnce
	@usage
	newFont = M2TWEOP3dObjects.MakeTextFont("Times New Roman");
	newText = M2TWEOP3dObjects.Make3dText(newFont, "Some text");
	newText:SetCoords(10,20,0.2);
	newText:DrawOnce();
	*/
	tables.text3dDrawable.set_function("DrawOnce", &eop3dObjectsHelpers::DrawOnceText);
	/***
	Delete text. Not use after it!
	@function Eop3dText:Delete
	@usage
	newFont = M2TWEOP3dObjects.MakeTextFont("Times New Roman");
	newText = M2TWEOP3dObjects.Make3dText(newFont, "Some text");
	newText:SetCoords(10,20,0.2);
	newText:DrawOnce();
	newText:Delete();
	*/
	tables.text3dDrawable.set_function("Delete", &eop3dObjectsHelpers::DeleteText);
}