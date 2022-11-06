///
//![Lua logo](../Lua.png)
//M2TWEOP structures and functions. There are not many examples and descriptions here. Also note that the examples do not include many of the checks that would be required when creating modifications.
//@module LuaPlugin
//@author youneuoy
//@license GPL-3.0
#include "luaP.h"
#include "plugData.h"


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
	Load new fbx model
	@function M2TWEOPFBX.addFbxModel
	@tparam string modelPath path to .fbx file
	@tparam string texturePath path to .dds file
	@tparam int modelID id what be used for this model

	@usage
	M2TWEOPFBX.addFbxModel(M2TWEOP.getModPath().."/eopData/testmodel.fbx",M2TWEOP.getModPath().."/eopData/testtexture.dds",1);
	*/
	tables.M2TWEOPFBXTable.set_function("addFbxModel", &eopFbxHelpers::addFbxModel);

	/***
	Delete fbx model
	@function M2TWEOPFBX.deleteFbxModel
	@tparam int modelID model id

	@usage
	--1 is id here
	M2TWEOPFBX.addFbxModel(M2TWEOP.getModPath().."/eopData/testmodel.fbx",M2TWEOP.getModPath().."/eopData/testtexture.dds",1);
	M2TWEOPFBX.deleteFbxModel(1);
	*/
	tables.M2TWEOPFBXTable.set_function("deleteFbxModel", &eopFbxHelpers::deleteFbxModel);


	/***
	Get fbx model
	@function M2TWEOPFBX.getFbxModel
	@tparam int modelID model id
	@treturn fbxModel fbx model, not it`s ID!

	@usage
	--1 is id here
	M2TWEOPFBX.addFbxModel(M2TWEOP.getModPath().."/eopData/testmodel.fbx",M2TWEOP.getModPath().."/eopData/testtexture.dds",1);
	local ourNewModel=M2TWEOPFBX.getFbxModel(1);
	*/
	tables.M2TWEOPFBXTable.set_function("getFbxModel", &eopFbxHelpers::getFbxModel);

	/***
	Create new fbx object with model. We can place object on map, etc
	@function M2TWEOPFBX.addFbxObject
	@tparam int modelID model id
	@tparam int objectID id what be used for this object
	@treturn fbxObject fbx object, not it`s ID!

	@usage
	--1 is model id here
	M2TWEOPFBX.addFbxModel(M2TWEOP.getModPath().."/eopData/testmodel.fbx",M2TWEOP.getModPath().."/eopData/testtexture.dds",1);
	local ourNewModel=M2TWEOPFBX.getFbxModel(1);
	--2 is object id here
	local ourNewObject=M2TWEOPFBX.addFbxObject(1,2);
	*/
	tables.M2TWEOPFBXTable.set_function("addFbxObject", &eopFbxHelpers::addFbxObject);

	/***
	Delete fbx object
	@function M2TWEOPFBX.deleteFbxObject
	@tparam fbxObject object fbx object, not it`s ID!

	@usage
	--1 is model id here
	M2TWEOPFBX.addFbxModel(M2TWEOP.getModPath().."/eopData/testmodel.fbx",M2TWEOP.getModPath().."/eopData/testtexture.dds",1);
	local ourNewModel=M2TWEOPFBX.getFbxModel(1);
	--2 is object id here
	local ourNewObject=M2TWEOPFBX.addFbxObject(1,2);
	M2TWEOPFBX.deleteFbxObject(ourNewObject);
	*/
	tables.M2TWEOPFBXTable.set_function("deleteFbxObject", &eopFbxHelpers::deleteFbxObject);

	/***
	Get fbx object by id
	@function M2TWEOPFBX.getFbxObject
	@tparam fbxObject object fbx object, not it`s ID!
	@treturn fbxObject fbx object, not it`s ID!

	@usage
	--1 is model id here
	M2TWEOPFBX.addFbxModel(M2TWEOP.getModPath().."/eopData/testmodel.fbx",M2TWEOP.getModPath().."/eopData/testtexture.dds",1);
	local ourNewModel=M2TWEOPFBX.getFbxModel(1);
	--2 is object id here
	M2TWEOPFBX.addFbxObject(1,2);
	local ourNewObject=M2TWEOPFBX.getFbxObject(2);

	*/
	tables.M2TWEOPFBXTable.set_function("getFbxObject", &eopFbxHelpers::getFbxObject);


	/***
	Set fbx object coordinates
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
	Get fbx object coordinates
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
	Set fbx object size modifier
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
	tables.M2TWEOPFBXTable.set_function("setFbxObjectSize", &eopFbxHelpers::setFbxObjectSize);

	/***
	Get fbx object size modifier
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
	tables.M2TWEOPFBXTable.set_function("getFbxObjectSize", &eopFbxHelpers::getFbxObjectSize);

	/***
	Set fbx object draw state
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
	tables.M2TWEOPFBXTable.set_function("setFbxObjectDrawState", &eopFbxHelpers::setFbxObjectDrawState);

	/***
	Get fbx object draw state
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
	tables.M2TWEOPFBXTable.set_function("getFbxObjectDrawState", &eopFbxHelpers::getFbxObjectDrawState);


	/***
	Set fbx object animation state
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
	tables.M2TWEOPFBXTable.set_function("setFbxObjectAnimState", &eopFbxHelpers::setFbxObjectAnimState);

	/***
	Get fbx object animation state
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
	tables.M2TWEOPFBXTable.set_function("getFbxObjectAnimState", &eopFbxHelpers::getFbxObjectAnimState);


	/***
	Set fbx object draw type. I.e. on what part of game it drawed
	Can be:
	1 - stratmap
	2 - tactmap
	0 - both maps

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
	tables.M2TWEOPFBXTable.set_function("setFbxObjectDrawType", &eopFbxHelpers::setFbxObjectDrawType);
	/***
	Get fbx object draw type. I.e. on what part of game it drawed
	Can be:
	1 - stratmap
	2 - tactmap
	0 - both maps

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
	tables.M2TWEOPFBXTable.set_function("getFbxObjectDrawType", &eopFbxHelpers::getFbxObjectDrawType);
}
