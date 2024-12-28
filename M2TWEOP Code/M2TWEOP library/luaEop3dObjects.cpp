///
//![Lua logo](../Lua.png)
//M2TWEOP structures and functions. There are not many examples and descriptions here. Also note that the examples do not include many of the checks that would be required when creating modifications.
//@module LuaPlugin
//@author youneuoy
//@license GPL-3.0
#include "pch.h"
#include "luaPlugin.h"
#include "mapTextDrawer.h"
#include <d3dx9mesh.h>

void luaPlugin::initEop3dObjects()
{
	struct
	{
		sol::table M2TWEOP3dObjectsTable;
		sol::usertype<text3dDrawable> text3dDrawable;
	}tables;

	///M2TWEOP3dObjects
	//@section M2TWEOP3dObjects

	/***
	Basic M2TWEOP3dObjects table. Contains descriptions of m2tweop simple 3d objects feathures. Not call this functions onPluginLoad(graphics system not initialized here yet). 

	@tfield makeTextFont makeTextFont
	@tfield deleteTextFont deleteTextFont
	@tfield make3dText make3dText

	@table M2TWEOP3dObjects
	*/

	tables.M2TWEOP3dObjectsTable = luaState.create_table("M2TWEOP3dObjects");

	/***
	Create new 3d text font. Fonts using for creating text(you not need font after it)
	@function M2TWEOP3dObjects.makeTextFont
	@tparam string fontName name of font
	@tparam int weight weight of font(optional). Default - 400
	@tparam bool isItalic is font italic(optional). Default - true
	@treturn int fondID ID of created font.

	@usage
	newFont = M2TWEOP3dObjects.makeTextFont("Times New Roman");
	*/
	tables.M2TWEOP3dObjectsTable.set_function("makeTextFont", &mapTextDrawer::makeTextFontLua);
	/***
	Delete 3d text font. Do it where not need anymore
	@function M2TWEOP3dObjects.deleteTextFont
	@tparam int fondID ID of created font.

	@usage
	newFont = M2TWEOP3dObjects.makeTextFont("Times New Roman");
	newText = M2TWEOP3dObjects.make3dText(newFont, "Some text");
	M2TWEOP3dObjects.deleteTextFont(newFont);
	*/
	tables.M2TWEOP3dObjectsTable.set_function("deleteTextFont", &mapTextDrawer::deleteTextFont);
	/***
	Create line of 3d text.
	@function M2TWEOP3dObjects.make3dText
	@tparam int fondID ID of created font.
	@tparam string text utf8 string with text.
	@treturn eop3dText text utf8 string with text.

	@usage
	newFont = M2TWEOP3dObjects.makeTextFont("Times New Roman");
	newText = M2TWEOP3dObjects.make3dText(newFont, "Some text");
	M2TWEOP3dObjects.deleteTextFont(newFont);
	*/
	tables.M2TWEOP3dObjectsTable.set_function("make3dText", &mapTextDrawer::makeText);

	///3dText
	//@section eop3dText

	/***
	3d text object. Not use it onPluginLoad(graphics system not initialized here yet).  
	@tfield float xSize default value 1
	@tfield float ySize default value 0.2
	@tfield float zSize default value 1
	@tfield float xRoll default value 1
	@tfield float yRoll default value 90
	@tfield float zRoll default value 1
	@tfield float xCoord
	@tfield float yCoord
	@tfield float zCoord
	@tfield scale scale
	@tfield changeColor changeColor
	@tfield setCoords setCoords
	@tfield startDrawing startDrawing
	@tfield stopDrawing stopDrawing
	@tfield drawOnce drawOnce
	@tfield delete delete

	@table eop3dText
	*/

	tables.text3dDrawable = luaState.new_usertype<text3dDrawable>("eop3dText");

	tables.text3dDrawable.set("xSize", &text3dDrawable::xSize);
	tables.text3dDrawable.set("ySize", &text3dDrawable::ySize);
	tables.text3dDrawable.set("zSize", &text3dDrawable::zSize);

	tables.text3dDrawable.set("xRoll", &text3dDrawable::xRoll);
	tables.text3dDrawable.set("yRoll", &text3dDrawable::yRoll);
	tables.text3dDrawable.set("zRoll", &text3dDrawable::zRoll);

	tables.text3dDrawable.set("xCoord", &text3dDrawable::xCoord);
	tables.text3dDrawable.set("yCoord", &text3dDrawable::yCoord);
	tables.text3dDrawable.set("zCoord", &text3dDrawable::zCoord);
	/***
	Scale 3d text.
	@function eop3dText:scale
	@tparam float sizeMultiplier
	@usage
	newFont = M2TWEOP3dObjects.makeTextFont("Times New Roman");
	newText = M2TWEOP3dObjects.make3dText(newFont, "Some text");
	newText:scale(0.3);
	*/
	tables.text3dDrawable.set_function("scale", &mapTextDrawer::scaleText);

	/***
	Set 3d text color.
	@function eop3dText:changeColor
	@tparam int a
	@tparam int r
	@tparam int g
	@tparam int b
	@usage
	newFont = M2TWEOP3dObjects.makeTextFont("Times New Roman");
	newText = M2TWEOP3dObjects.make3dText(newFont, "Some text");
	newText:changeColor(255,0,255,177);
	*/
	tables.text3dDrawable.set_function("changeColor", &mapTextDrawer::changeTextColor);
	/***
	Set 3d text coords.
	@function eop3dText:setCoords
	@tparam float x
	@tparam float y
	@tparam float z
	@usage
	newFont = M2TWEOP3dObjects.makeTextFont("Times New Roman");
	newText = M2TWEOP3dObjects.make3dText(newFont, "Some text");
	newText:setCoords(10,20,0.2);
	*/
	tables.text3dDrawable.set_function("setCoords", &mapTextDrawer::setTextDrawingCoords);
	/***
	Start draw text. Text go on display until stop function called
	@function eop3dText:startDrawing
	@usage
	newFont = M2TWEOP3dObjects.makeTextFont("Times New Roman");
	newText = M2TWEOP3dObjects.make3dText(newFont, "Some text");
	newText:setCoords(10,20,0.2);
	newText:startDrawing();
	*/
	tables.text3dDrawable.set_function("startDrawing", &mapTextDrawer::startDrawingText);
	/***
	Stop draw text.
	@function eop3dText:stopDrawing
	@usage
	newFont = M2TWEOP3dObjects.makeTextFont("Times New Roman");
	newText = M2TWEOP3dObjects.make3dText(newFont, "Some text");
	newText:setCoords(10,20,0.2);
	newText:startDrawing();
	--some time we draw
	newText:stopDrawing();
	*/
	tables.text3dDrawable.set_function("stopDrawing", &mapTextDrawer::stopDrawingText);
	/***
	Draw text once(at one frame only). Can be called in draw loop. Not need call stop after it.
	@function eop3dText:drawOnce
	@usage
	newFont = M2TWEOP3dObjects.makeTextFont("Times New Roman");
	newText = M2TWEOP3dObjects.make3dText(newFont, "Some text");
	newText:setCoords(10,20,0.2);
	newText:drawOnce();
	*/
	tables.text3dDrawable.set_function("drawOnce", &mapTextDrawer::drawingTextOnce);
	/***
	Delete text. Not use after it!
	@function eop3dText:delete
	@usage
	newFont = M2TWEOP3dObjects.makeTextFont("Times New Roman");
	newText = M2TWEOP3dObjects.make3dText(newFont, "Some text");
	newText:setCoords(10,20,0.2);
	newText:drawOnce();
	newText:delete();
	*/
	tables.text3dDrawable.set_function("delete", &mapTextDrawer::delete3dText);
}
