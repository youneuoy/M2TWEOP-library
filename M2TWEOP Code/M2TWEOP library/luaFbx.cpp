///
//![Lua logo](../Lua.png)
//M2TWEOP structures and functions. There are not many examples and descriptions here. Also note that the examples do not include many of the checks that would be required when creating modifications.
//@module LuaPlugin
//@author youneuoy
//@license GPL-3.0
#include "luaP.h"
#include "eopFbxHelpers.h"
#include "fbxModels.h"

void luaP::initEopFbx()
{
	struct
	{
		sol::table M2TWEOPFBXTable;
	}tables;
	///M2TWEOPFBX
	//@section M2TWEOPFBXTable

	/***
	Basic M2TWEOPFBX table. Contains descriptions of m2tweop fbx feathures.

	@tfield addFbxModel addFbxModel
	@tfield deleteFbxModel deleteFbxModel
	@tfield getFbxModel getFbxModel
	@tfield addFbxObject addFbxObject
	@tfield deleteFbxObject deleteFbxObject
	@tfield getFbxObject getFbxObject
	@tfield setFbxObjectCoords setFbxObjectCoords
	@tfield getFbxObjectCoords getFbxObjectCoords
	@tfield setFbxObjectSize setFbxObjectSize
	@tfield getFbxObjectSize getFbxObjectSize
	@tfield setFbxObjectDrawState setFbxObjectDrawState
	@tfield getFbxObjectDrawState getFbxObjectDrawState
	@tfield setFbxObjectAnimState setFbxObjectAnimState
	@tfield getFbxObjectAnimState getFbxObjectAnimState
	@tfield setFbxObjectDrawType setFbxObjectDrawType
	@tfield getFbxObjectDrawType getFbxObjectDrawType

	@table M2TWEOPFBX
	*/



	tables.M2TWEOPFBXTable = luaState.create_table("M2TWEOPFBX");


	/***
	Load a new fbx model.
	@function M2TWEOPFBX.addFbxModel
	@tparam string modelPath Path to .fbx file.
	@tparam string texturePath Path to .dds file
	@tparam int modelID ID to be used for this model.

	@usage
	M2TWEOPFBX.addFbxModel(M2TWEOP.getModPath().."/eopData/testmodel.fbx",M2TWEOP.getModPath().."/eopData/testtexture.dds",1);
	*/
	tables.M2TWEOPFBXTable.set_function("addFbxModel", &fbxModels::addFbxModel);

	/***
	Delete a fbx model.
	@function M2TWEOPFBX.deleteFbxModel
	@tparam int modelID Model's ID that was specified when calling addFbxModel()

	@usage
	--1 is id here
	M2TWEOPFBX.addFbxModel(M2TWEOP.getModPath().."/eopData/testmodel.fbx",M2TWEOP.getModPath().."/eopData/testtexture.dds",1);
	M2TWEOPFBX.deleteFbxModel(1);
	*/
	tables.M2TWEOPFBXTable.set_function("deleteFbxModel", &fbxModels::deleteFbxModel);


	/***
	Get an added fbx model by it's ID.
	@function M2TWEOPFBX.getFbxModel
	@tparam int modelID Model's ID that was specified when calling addFbxModel()
	@treturn fbxModel fbx model, not it`s ID!

	@usage
	--1 is id here
	M2TWEOPFBX.addFbxModel(M2TWEOP.getModPath().."/eopData/testmodel.fbx",M2TWEOP.getModPath().."/eopData/testtexture.dds",1);
	local ourNewModel=M2TWEOPFBX.getFbxModel(1);
	*/
	tables.M2TWEOPFBXTable.set_function("getFbxModel", &fbxModels::getFbxModel);

	/***
	Create new fbx object with our previously loaded model. It can be placed on map, etc.
	@function M2TWEOPFBX.addFbxObject
	@tparam int modelID Model's ID that was specified when calling addFbxModel()
	@tparam int objectID The ID that the object should use
	@treturn fbxObject The newly created FBX object (not it's ID)

	@usage
	--1 is model id here
	M2TWEOPFBX.addFbxModel(M2TWEOP.getModPath().."/eopData/testmodel.fbx",M2TWEOP.getModPath().."/eopData/testtexture.dds",1);
	local ourNewModel=M2TWEOPFBX.getFbxModel(1);
	--2 is object id here
	local ourNewObject=M2TWEOPFBX.addFbxObject(1,2);
	*/
	tables.M2TWEOPFBXTable.set_function("addFbxObject", &fbxModels::addFbxObject);

	/***
	Delete an fbx object.
	@function M2TWEOPFBX.deleteFbxObject
	@tparam fbxObject object The created FBX object (not it's ID)

	@usage
	--1 is model id here
	M2TWEOPFBX.addFbxModel(M2TWEOP.getModPath().."/eopData/testmodel.fbx",M2TWEOP.getModPath().."/eopData/testtexture.dds",1);
	local ourNewModel=M2TWEOPFBX.getFbxModel(1);
	--2 is object id here
	local ourNewObject=M2TWEOPFBX.addFbxObject(1,2);
	M2TWEOPFBX.deleteFbxObject(ourNewObject);
	*/
	tables.M2TWEOPFBXTable.set_function("deleteFbxObject", &fbxModels::deleteFbxObject);

	/***
	Get an fbx object by it's ID.
	@function M2TWEOPFBX.getFbxObject
	@tparam objectID int The ID of the FBX object you wish to retrieve
	@treturn fbxObject The created FBX object (not it's ID)

	@usage
	--1 is model id here
	M2TWEOPFBX.addFbxModel(M2TWEOP.getModPath().."/eopData/testmodel.fbx",M2TWEOP.getModPath().."/eopData/testtexture.dds",1);
	local ourNewModel=M2TWEOPFBX.getFbxModel(1);
	--2 is object id here
	M2TWEOPFBX.addFbxObject(1,2);
	local ourNewObject=M2TWEOPFBX.getFbxObject(2);

	*/
	tables.M2TWEOPFBXTable.set_function("getFbxObject", &fbxModels::getFbxObject);


	/***
	Set an fbx object's coordinates.
	@function M2TWEOPFBX.setFbxObjectCoords
	@tparam fbxObject fbx object
	@tparam float x coord
	@tparam float y coord
	@tparam float z coord

	@usage
	--1 is model id here
	M2TWEOPFBX.addFbxModel(M2TWEOP.getModPath().."/eopData/testmodel.fbx",M2TWEOP.getModPath().."/eopData/testtexture.dds",1);
	local ourNewModel=M2TWEOPFBX.getFbxModel(1);
	--2 is object id here
	M2TWEOPFBX.addFbxObject(1,2);
	local ourNewObject=M2TWEOPFBX.getFbxObject(2);
	M2TWEOPFBX.setFbxObjectCoords(ourNewObject,1.2,123.95,1);
	*/
	tables.M2TWEOPFBXTable.set_function("setFbxObjectCoords", &eopFbxHelpers::setFbxObjectCoords);

	/***
	Get an fbx object's coordinates.
	@function M2TWEOPFBX.getFbxObjectCoords
	@tparam fbxObject fbx object
	@treturn float x coord
	@treturn float y coord
	@treturn float z coord

	@usage
	--1 is model id here
	M2TWEOPFBX.addFbxModel(M2TWEOP.getModPath().."/eopData/testmodel.fbx",M2TWEOP.getModPath().."/eopData/testtexture.dds",1);
	local ourNewModel=M2TWEOPFBX.getFbxModel(1);
	--2 is object id here
	M2TWEOPFBX.addFbxObject(1,2);
	local ourNewObject=M2TWEOPFBX.getFbxObject(2);
	M2TWEOPFBX.setFbxObjectCoords(ourNewObject,1.2,123.95,1);
	local x,y,z=M2TWEOPFBX.getFbxObjectCoords(ourNewObject);
	*/
	tables.M2TWEOPFBXTable.set_function("getFbxObjectCoords", &eopFbxHelpers::getFbxObjectCoords);

	/***
	Set an fbx object's size modifier.
	@function M2TWEOPFBX.setFbxObjectSize
	@tparam fbxObject fbx object
	@tparam float sizeModifier object size modifier

	@usage
	--1 is model id here
	M2TWEOPFBX.addFbxModel(M2TWEOP.getModPath().."/eopData/testmodel.fbx",M2TWEOP.getModPath().."/eopData/testtexture.dds",1);
	local ourNewModel=M2TWEOPFBX.getFbxModel(1);
	--2 is object id here
	M2TWEOPFBX.addFbxObject(1,2);
	local ourNewObject=M2TWEOPFBX.getFbxObject(2);
	M2TWEOPFBX.setFbxObjectCoords(ourNewObject,1.2,123.95,1);
	local x,y,z=M2TWEOPFBX.getFbxObjectCoords(ourNewObject);


	M2TWEOPFBX.setFbxObjectSize(ourNewObject,25.2);
	*/
	tables.M2TWEOPFBXTable.set_function("setFbxObjectSize", &fbxModels::setFbxObjectSize);

	/***
	Get an fbx object's size modifier.
	@function M2TWEOPFBX.getFbxObjectSize
	@tparam fbxObject fbx object
	@treturn float sizeModifier object size modifier
	@usage
	--1 is model id here
	M2TWEOPFBX.addFbxModel(M2TWEOP.getModPath().."/eopData/testmodel.fbx",M2TWEOP.getModPath().."/eopData/testtexture.dds",1);
	local ourNewModel=M2TWEOPFBX.getFbxModel(1);
	--2 is object id here
	M2TWEOPFBX.addFbxObject(1,2);
	local ourNewObject=M2TWEOPFBX.getFbxObject(2);
	M2TWEOPFBX.setFbxObjectCoords(ourNewObject,1.2,123.95,1);
	local x,y,z=M2TWEOPFBX.getFbxObjectCoords(ourNewObject);


	M2TWEOPFBX.setFbxObjectSize(ourNewObject,25.2);
	local size=M2TWEOPFBX.getFbxObjectSize(ourNewObject);
	*/
	tables.M2TWEOPFBXTable.set_function("getFbxObjectSize", &fbxModels::getFbxObjectSize);

	/***
	Set an fbx object's draw state.
	@function M2TWEOPFBX.setFbxObjectDrawState
	@tparam fbxObject fbx object
	@tparam bool is draw needed

	@usage
	--1 is model id here
	M2TWEOPFBX.addFbxModel(M2TWEOP.getModPath().."/eopData/testmodel.fbx",M2TWEOP.getModPath().."/eopData/testtexture.dds",1);
	local ourNewModel=M2TWEOPFBX.getFbxModel(1);
	--2 is object id here
	M2TWEOPFBX.addFbxObject(1,2);
	local ourNewObject=M2TWEOPFBX.getFbxObject(2);
	M2TWEOPFBX.setFbxObjectCoords(ourNewObject,1.2,123.95,1);
	local x,y,z=M2TWEOPFBX.getFbxObjectCoords(ourNewObject);


	M2TWEOPFBX.setFbxObjectSize(ourNewObject,25.2);
	local size=M2TWEOPFBX.getFbxObjectSize(ourNewObject);


	M2TWEOPFBX.setFbxObjectDrawState(ourNewObject,true);
	*/
	tables.M2TWEOPFBXTable.set_function("setFbxObjectDrawState", &fbxModels::setFbxObjectDrawState);

	/***
	Get an fbx object's draw state.
	@function M2TWEOPFBX.getFbxObjectDrawState
	@tparam fbxObject fbx object
	@treturn bool is draw needed

	@usage
	--1 is model id here
	M2TWEOPFBX.addFbxModel(M2TWEOP.getModPath().."/eopData/testmodel.fbx",M2TWEOP.getModPath().."/eopData/testtexture.dds",1);
	local ourNewModel=M2TWEOPFBX.getFbxModel(1);
	--2 is object id here
	M2TWEOPFBX.addFbxObject(1,2);
	local ourNewObject=M2TWEOPFBX.getFbxObject(2);
	M2TWEOPFBX.setFbxObjectCoords(ourNewObject,1.2,123.95,1);
	local x,y,z=M2TWEOPFBX.getFbxObjectCoords(ourNewObject);


	M2TWEOPFBX.setFbxObjectSize(ourNewObject,25.2);
	local size=M2TWEOPFBX.getFbxObjectSize(ourNewObject);


	M2TWEOPFBX.setFbxObjectDrawState(ourNewObject,true);
	local isDrawed=M2TWEOPFBX.getFbxObjectDrawState(ourNewObject);
	*/
	tables.M2TWEOPFBXTable.set_function("getFbxObjectDrawState", &fbxModels::getFbxObjectDrawState);


	/***
	Set an fbx object's animation state.
	@function M2TWEOPFBX.setFbxObjectAnimState
	@tparam fbxObject fbx object
	@tparam bool is animation needed

	@usage
	--1 is model id here
	M2TWEOPFBX.addFbxModel(M2TWEOP.getModPath().."/eopData/testmodel.fbx",M2TWEOP.getModPath().."/eopData/testtexture.dds",1);
	local ourNewModel=M2TWEOPFBX.getFbxModel(1);
	--2 is object id here
	M2TWEOPFBX.addFbxObject(1,2);
	local ourNewObject=M2TWEOPFBX.getFbxObject(2);
	M2TWEOPFBX.setFbxObjectCoords(ourNewObject,1.2,123.95,1);
	local x,y,z=M2TWEOPFBX.getFbxObjectCoords(ourNewObject);


	M2TWEOPFBX.setFbxObjectSize(ourNewObject,25.2);
	local size=M2TWEOPFBX.getFbxObjectSize(ourNewObject);


	M2TWEOPFBX.setFbxObjectDrawState(ourNewObject,true);
	local isDrawed=M2TWEOPFBX.getFbxObjectDrawState(ourNewObject);
	M2TWEOPFBX.setFbxObjectAnimState(ourNewObject,true);
	*/
	tables.M2TWEOPFBXTable.set_function("setFbxObjectAnimState", &fbxModels::setFbxObjectAnimState);

	/***
	Get an fbx object's animation state.
	@function M2TWEOPFBX.getFbxObjectAnimState
	@tparam fbxObject fbx object
	@treturn bool is animation needed

	@usage
	--1 is model id here
	M2TWEOPFBX.addFbxModel(M2TWEOP.getModPath().."/eopData/testmodel.fbx",M2TWEOP.getModPath().."/eopData/testtexture.dds",1);
	local ourNewModel=M2TWEOPFBX.getFbxModel(1);
	--2 is object id here
	M2TWEOPFBX.addFbxObject(1,2);
	local ourNewObject=M2TWEOPFBX.getFbxObject(2);
	M2TWEOPFBX.setFbxObjectCoords(ourNewObject,1.2,123.95,1);
	local x,y,z=M2TWEOPFBX.getFbxObjectCoords(ourNewObject);


	M2TWEOPFBX.setFbxObjectSize(ourNewObject,25.2);
	local size=M2TWEOPFBX.getFbxObjectSize(ourNewObject);


	M2TWEOPFBX.setFbxObjectDrawState(ourNewObject,true);
	local isDrawed=M2TWEOPFBX.getFbxObjectDrawState(ourNewObject);
	M2TWEOPFBX.setFbxObjectAnimState(ourNewObject,true);
	local isAnimationPlayed=M2TWEOPFBX.getFbxObjectAnimState(ourNewObject);
	*/
	tables.M2TWEOPFBXTable.set_function("getFbxObjectAnimState", &fbxModels::getFbxObjectAnimState);


	/***
	Set fbx object draw type, i.e. on what part of the game it is drawn.
	Can be:
	1 - Strategy Map
	2 - Tactical/Battle Map
	0 - Both
	Default value - 1

	@function M2TWEOPFBX.setFbxObjectDrawType
	@tparam fbxObject fbx object
	@tparam int current draw type

	@usage
	--1 is model id here
	M2TWEOPFBX.addFbxModel(M2TWEOP.getModPath().."/eopData/testmodel.fbx",M2TWEOP.getModPath().."/eopData/testtexture.dds",1);
	local ourNewModel=M2TWEOPFBX.getFbxModel(1);
	--2 is object id here
	M2TWEOPFBX.addFbxObject(1,2);
	local ourNewObject=M2TWEOPFBX.getFbxObject(2);
	M2TWEOPFBX.setFbxObjectCoords(ourNewObject,1.2,123.95,1);
	local x,y,z=M2TWEOPFBX.getFbxObjectCoords(ourNewObject);


	M2TWEOPFBX.setFbxObjectSize(ourNewObject,25.2);
	local size=M2TWEOPFBX.getFbxObjectSize(ourNewObject);


	M2TWEOPFBX.setFbxObjectDrawState(ourNewObject,true);
	local isDrawed=M2TWEOPFBX.getFbxObjectDrawState(ourNewObject);
	M2TWEOPFBX.setFbxObjectAnimState(ourNewObject,true);

	--get current type
	local whereWeDrawObject=M2TWEOPFBX.getFbxObjectDrawType(ourNewObject);
	--if we draw object on stratmap
	if (whereWeDrawObject==1)
	then
		--change drawing type to battle map
		M2TWEOPFBX.setFbxObjectDrawType(ourNewObject,2);
	end
	*/
	tables.M2TWEOPFBXTable.set_function("setFbxObjectDrawType", &fbxModels::setFbxObjectDrawType);
	/***
	Get fbx object draw type, i.e. on what part of game it is drawn.
	Can be:
	1 - Strategy Map
	2 - Tactical/Battle Map
	0 - Both
	Default value - 1

	@function M2TWEOPFBX.getFbxObjectDrawType
	@tparam fbxObject fbx object
	@treturn int current draw type

	@usage
	--1 is model id here
	M2TWEOPFBX.addFbxModel(M2TWEOP.getModPath().."/eopData/testmodel.fbx",M2TWEOP.getModPath().."/eopData/testtexture.dds",1);
	local ourNewModel=M2TWEOPFBX.getFbxModel(1);
	--2 is object id here
	M2TWEOPFBX.addFbxObject(1,2);
	local ourNewObject=M2TWEOPFBX.getFbxObject(2);
	M2TWEOPFBX.setFbxObjectCoords(ourNewObject,1.2,123.95,1);
	local x,y,z=M2TWEOPFBX.getFbxObjectCoords(ourNewObject);


	M2TWEOPFBX.setFbxObjectSize(ourNewObject,25.2);
	local size=M2TWEOPFBX.getFbxObjectSize(ourNewObject);


	M2TWEOPFBX.setFbxObjectDrawState(ourNewObject,true);
	local isDrawed=M2TWEOPFBX.getFbxObjectDrawState(ourNewObject);
	M2TWEOPFBX.setFbxObjectAnimState(ourNewObject,true);
	local whereWeDrawObject=M2TWEOPFBX.getFbxObjectDrawType(ourNewObject);
*/
	tables.M2TWEOPFBXTable.set_function("getFbxObjectDrawType", &fbxModels::getFbxObjectDrawType);
}
