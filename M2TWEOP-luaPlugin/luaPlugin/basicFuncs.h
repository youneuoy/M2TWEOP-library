#pragma once
#include "framework.h"
#include <d3d9.h>
#include <sstream>
class basicStructs
{
public:
	struct arm {
		//numbers of units
		UINT32 num;

		//army pointer
		stackStruct* army;


		//condition destroy_or_rout_enemy
		//condition destroy_or_rout_enemy
		UINT32 condDest;
		//condition capture settlement
		UINT32 condSetl;
		//defender or not
		UINT32 ifDef;
		//side in battle(0-3)
		UINT32 side;

		//deployment data offset
		DWORD deploy;

		//units
		std::vector<unit*>units;
	};

};


// functions loader
template <class T>
class ProcLoader
{
public:
	T  m_pProc;
	std::string	 m_strNameProcedure;

	//strings for compare
	char* stringAdr = nullptr;
	const char* strCmp;

	ProcLoader() {}

	BOOL Load(std::string* pczNameLibrary, std::string* pczNameProcedure, char* sCmp = nullptr)
	{
		m_strNameProcedure = *pczNameProcedure;
		m_pProc = NULL;
		strCmp = sCmp;
		HANDLE hModule;

		if (!(hModule = ::LoadLibraryA(pczNameLibrary->c_str())))
		{
			std::stringstream error;
			error << "Could not load plugin located at:\n" << pczNameProcedure->c_str() << "\n" << "Error Code: " << GetLastError();
			MessageBoxA(NULL, error.str().c_str(), "Error", NULL);
			return FALSE;
		}
		if (!(m_pProc = (T)::GetProcAddress((HMODULE)hModule, m_strNameProcedure.c_str())))
		{
			std::stringstream error2;
			error2 << "Can't load function: " << m_strNameProcedure.c_str();
			m_pProc = (T)emptyProc;
			MessageBoxA(NULL, error2.str().c_str(), "Attention", NULL);
			return FALSE;
		}
		return TRUE;
	};

	T operator *() const
	{
		return m_pProc;
	}

	static void emptyProc()
	{

	}
private:
	ProcLoader& operator = (ProcLoader&)
	{
		return *this;
	}
};

class battleFuncs
{
public:
	ProcLoader<int(__cdecl*)(DWORD)> getBattleCondCode;
	ProcLoader<void(__cdecl*)()> swUnBMapHighlight;
	ProcLoader<battleCameraStruct*(__cdecl*)()> getBattleCamCoords;
};
class technicalFuncs
{
public:
	ProcLoader<void(__cdecl*)(UNICODE_STRING**&, const char*)> createUniString;
};

class basicFuncs
{
public:

	ProcLoader<void*(__cdecl*)(const char* fontName, int weight, int isItalic)> MakeTextFont;
	ProcLoader<void(__cdecl*)(void* fontID)> DeleteTextFont;
	ProcLoader<Text3DDrawable*(__cdecl*)(void* fontID, const char* utf8Text)> MakeText;
	ProcLoader<void(__cdecl*)(Text3DDrawable* text, float scale)> ScaleText;
	ProcLoader<void(__cdecl*)(Text3DDrawable* text, unsigned char a, unsigned char r, unsigned char g, unsigned char b)> ChangeTextColor;
	ProcLoader<void(__cdecl*)(Text3DDrawable* text, float x, float y, float z)> SetTextDrawingCoords;
	ProcLoader<void(__cdecl*)(Text3DDrawable* text)> StartDrawingText;
	ProcLoader<void(__cdecl*)(Text3DDrawable* text)> StopDrawingText;
	ProcLoader<void(__cdecl*)(Text3DDrawable* text)> DrawingTextOnce;
	ProcLoader<void(__cdecl*)(Text3DDrawable* text)> Delete3dText;
	ProcLoader<void(__cdecl*)(Text3DDrawable* text, float scaleX, float scaleY, float scaleZ)> SetDimensionsTextXYZ;
	ProcLoader<void(__cdecl*)(Text3DDrawable* text, float rotX, float rotY, float rotZ)> SetRotationTextXYZ;


	ProcLoader<void(__cdecl*)(const char*, UINT32)> addModelToGame;
	ProcLoader<void(__cdecl*)(int, int, UINT32, UINT32)> setModel;
	ProcLoader<void(__cdecl*)(UINT32, int, int, float)> addCasModelToDrawList;
	ProcLoader<void(__cdecl*)(UINT32)> removeCasModelFromDrawList;

	ProcLoader<bool(__cdecl*)(const char*, const char*, char*)> callGameConsoleCommand;


	ProcLoader<int(__cdecl*)(const char* counterName, bool&)> getScriptCounter;
	ProcLoader<void(__cdecl*)(const char*, int)> setScriptCounter;


	ProcLoader<bool(__cdecl*)(int*)> isTileFree;
	ProcLoader<void(__cdecl*)(int& x, int& y)> getGameTileCoordsWithCursor;
	ProcLoader<UINT32(__cdecl*)(int x, int y)> getTileRegionID;
	ProcLoader<regionStruct* (__cdecl*)(UINT32)> getRegionByID;

	ProcLoader<settlementStruct* (__cdecl*)(int x, int y)> findSettlement;
	ProcLoader<fortStruct* (__cdecl*)(int x, int y)> findFort;

	ProcLoader<factionStruct* (__cdecl*)(int regionID)> getRegionOwner;
	ProcLoader<void(__cdecl*)(settlementStruct* sett, const char* newName)> changeSettlementName;
	ProcLoader<void(__cdecl*)(factionStruct* fac, const char* newName)> changeFactionName;
	ProcLoader<UNICODE_STRING** (__cdecl*)(factionStruct* fac)> getFactionName;
	ProcLoader<void(__cdecl*)(regionStruct* region, const char* newName)> changeRegionName;
	ProcLoader<bool(__cdecl*)(const char* condition, const eventTrigger* eventData)> condition;
	ProcLoader<void(__cdecl*)(regionStruct* region, const char* newName)> changeRebelsName;
	ProcLoader<void(__cdecl*)(const factionStruct* fac, fortStruct* fort)> deleteFort;
	ProcLoader<void(__cdecl*)(factionStruct* fac, int x, int y)> createFortXY;
	ProcLoader<DWORD(__cdecl*)(size_t amount)> allocateGameMem;
	ProcLoader<void(__cdecl*)(const general* gen)> createFort;
	ProcLoader<void(__cdecl*)(stackStruct* army)> buildWatchTower;
	ProcLoader<gameDataAllStruct* (__cdecl*)()> getGameDataAll;


	ProcLoader<void(__cdecl*)(signed short, signed short)> setEDUUnitsSize;
	ProcLoader<void(__cdecl*)(namedCharacter*, bool)> setHeir;


	ProcLoader<void(__cdecl*)(unsigned char)> setAncLimit;
	ProcLoader<void(__cdecl*)(const char*, const char*, const char*)> historicEvent;
	ProcLoader<void(__cdecl*)(const char*, const char*)> scriptCommand;
	ProcLoader<void(__cdecl*)(unsigned char)> setReligionsLimit;
	ProcLoader<void(__cdecl*)(unsigned char)> setMaxBgSize;
	ProcLoader<void(__cdecl*)()> unlockConsoleCommands;
	ProcLoader<void(__cdecl*)(int, int)> setConversionLvlFromCastle;
	ProcLoader<void(__cdecl*)(int, int)> setConversionLvlFromCity;
	ProcLoader<void(__cdecl*)(unsigned char)> setGuildCooldown;
	ProcLoader<void(__cdecl*)(const std::string&)> logStringGame;
	ProcLoader<void(__cdecl*)(general*, general*)> diplomacyCharacter;
	ProcLoader<void(__cdecl*)(general*, settlementStruct*)> diplomacySettlement;


	//get current game year
	ProcLoader<UINT32(__cdecl*)()> getYear;
	ProcLoader<void(__cdecl*)(char*, const char*)> setCryptedString;
	ProcLoader<UINT32(__cdecl*)()> getFactionsCount;
	ProcLoader<factionStruct** (__cdecl*)()> getFactionsList;

	ProcLoader<void(__cdecl*)(general*, int, int)> moveNormal;
	ProcLoader<void(__cdecl*)(general*, settlementStruct*, bool)> siegeSettlement;
	ProcLoader<void(__cdecl*)(general*, fortStruct*, bool)> siegeFort;
	ProcLoader<void(__cdecl*)(general*, general*)> attackCharacter;
	ProcLoader<void(__cdecl*)(general*, general*)> assassinate;
	ProcLoader<void(__cdecl*)(general*, general*)> marry;
	ProcLoader<void(__cdecl*)(general*, general*)> spyCharacter;
	ProcLoader<void(__cdecl*)(general*, general*)> denounce;
	ProcLoader<void(__cdecl*)(general*, general*)> bribe;
	ProcLoader<void(__cdecl*)(general*, general*)> acquire;
	ProcLoader<void(__cdecl*)(general*, factionStruct*, bool, bool)> switchCharacterFaction;
	ProcLoader<void(__cdecl*)(fortStruct*, factionStruct*, bool)> changeFortOwner;
	ProcLoader<bool(__cdecl*)(general*, int, int)> teleportCharacterClose;
	ProcLoader<coordPair*(__cdecl*)(coordPair*, int)> findValidTileNearTile;
	ProcLoader<bool(__cdecl*)(int, coordPair*)> isTileValidForCharacterType;

	ProcLoader<void(__cdecl*)(const unit*, float, float, bool)> unitMovetoPosition;
	ProcLoader<int(__cdecl*)(const unit*)> getUnitFormation;
	ProcLoader<void(__cdecl*)(unit*, float, float, int16_t, int)> placeUnit;
	ProcLoader<unit*(__cdecl*)(const char*)> getUnitByLabel;
	ProcLoader<void*(__cdecl*)(const unit*, int16_t, bool)> unitAttackClosest;
	ProcLoader<void*(__cdecl*)(const unit*, const unit*, bool)> attackUnit;
	ProcLoader<void*(__cdecl*)(const unit*)> deployStakes;
	ProcLoader<void*(__cdecl*)(const unit*)> haltUnit;
	ProcLoader<void*(__cdecl*)(const unit*, int)> changeUnitFormation;
	ProcLoader<float(__cdecl*)(float, float)> getBattleMapHeight;
	ProcLoader<void(__cdecl*)(const unit*, float, float, int, int16_t, bool)> moveToOrientation;
	ProcLoader<void(__cdecl*)(const unit*, float, float, bool)> moveRelative;
	ProcLoader<void(__cdecl*)(const unit*, const unit*, bool)> moveToMissileRange;
	ProcLoader<void(__cdecl*)(const unit*, int16_t, bool)> unitTurn;
	ProcLoader<void(__cdecl*)(const unit*)> taunt;
	ProcLoader<void(__cdecl*)(const unit*)> useSpecialAbility;
	ProcLoader<void(__cdecl*)(const unit*, buildingBattle*)> attackBuilding;
	
	ProcLoader<void(__cdecl*)(const unitGroup*, int)> changeGroupUnitFormation;
	ProcLoader<void(__cdecl*)(const unitGroup*, const unitGroup*, bool)> moveToRangeOfGroup;
	ProcLoader<void(__cdecl*)(const unitGroup*, const unit*, bool)> moveGroupToRangeOfUnit;
	ProcLoader<void(__cdecl*)(const unitGroup*, const unitGroup*, bool)> groupAttackGroup;
	ProcLoader<void(__cdecl*)(const unitGroup*)> groupHalt;
	ProcLoader<void(__cdecl*)(const unitGroup*, float, float, bool)> groupMoveFormed;
	ProcLoader<void(__cdecl*)(const unitGroup*, float, float, bool)> groupMoveUnformed;
	ProcLoader<void(__cdecl*)(const unitGroup*, float, float, bool)> groupMoveFormedRelative;
	ProcLoader<void(__cdecl*)(const unitGroup*, float, float, bool)> groupMoveUnformedRelative;
	ProcLoader<void(__cdecl*)(const unitGroup*, int16_t, bool)> groupTurn;
	


	ProcLoader<int(__cdecl*)(namedCharacter*, ancillary*)> addAncillary;
	ProcLoader<void(__cdecl*)(namedCharacter*, ancillary*)> removeAncillary;
	ProcLoader<ancillary* (__cdecl*)(char*)> findAncillary;

	ProcLoader<void(__cdecl*)(namedCharacter*, const char*, int)> addTrait;
	ProcLoader<void(__cdecl*)(namedCharacter*, const char*)> removeTrait;



	ProcLoader<void(__cdecl*)(const char*, UINT32, UINT32, const char*, const char*, const char*)> replaceTile;
	ProcLoader<void(__cdecl*)(int, int)> moveStratCameraSlow;
	ProcLoader<void(__cdecl*)(int, int)> moveStratCameraFast;
	ProcLoader<void(__cdecl*)(float)> zoomStratCamera;
	ProcLoader<void(__cdecl*)(general*, int, int)> teleportCharacter;
	ProcLoader<void(__cdecl*)(const char*)> loadSaveGame;

	//get list of armies in battle
	ProcLoader<std::vector<basicStructs::arm*>* (__cdecl*)()> getBattleArmies;


	ProcLoader< general* (__cdecl*)(const char*, factionStruct*, int, const char*, const char*, int, const char*, int, int)> createCharacter;

	ProcLoader<void(__cdecl*)(unit*)> killUnit;
	ProcLoader<void(__cdecl*)(general*)> killCharacter;
	ProcLoader<void(__cdecl*)(general*, const char*)> setCharacterModel;
	ProcLoader<void(__cdecl*)(const char*, const char*, const char*, const char*, const char*, float)> addCharacterCas;

	ProcLoader<void(__cdecl*)(unit*, int)> setSoldiersCount;
	ProcLoader<void(__cdecl*)(unit*, int, int, int, int)> setUnitParams;
	ProcLoader<void(__cdecl*)(unit*, float)> setUnitMovepoints;

	ProcLoader<stackStruct* (__cdecl*)(general*)> createArmy;
	ProcLoader<stackStruct* (__cdecl*)(settlementStruct*)> createArmyInSettlement;
	ProcLoader<unit* (__cdecl*)(const char*, int, int, int, int, int)> createUnitN;
	ProcLoader<unit* (__cdecl*)(int, int, int, int, int, int)> createUnitIdx;
	ProcLoader<unit* (__cdecl*)(int, int, int, int, int, int)> createUnitEDB;

	ProcLoader<int(__cdecl*)(stackStruct*, unit*)> addUnitToArmy;
	ProcLoader<void(__cdecl*)(general*, unit*)> setBodyguard;


	ProcLoader<void(__cdecl*)(settlementStruct*, factionStruct*, bool)> setSettlementOwner;
	ProcLoader<void(__cdecl*)(settlementStruct*, const char*, bool)> destroyBuilding;
	ProcLoader<void(__cdecl*)(settlementStruct*, const char*)> createBuilding;
	ProcLoader<void(__cdecl*)(general*)> sendOffMap;
	ProcLoader<bool(__cdecl*)(stackStruct*, portBuildingStruct*)> blockadePort;

	
	ProcLoader<void(__cdecl*)(general*, fortStruct*)> diplomacyFort;
	ProcLoader<void(__cdecl*)(general*, fortStruct*)> bribeFort;
	ProcLoader<void(__cdecl*)(general*, fortStruct*)> spyFort;
	ProcLoader<void(__cdecl*)(general*, settlementStruct*)> bribeSettlement;
	ProcLoader<void(__cdecl*)(general*, settlementStruct*)> spySettlement;
	ProcLoader<void(__cdecl*)(general*, settlementStruct*)> sabotageSettlement;


	ProcLoader<void(__cdecl*)(general*, int, int, int)> setCharacterType;
	
	ProcLoader<float(__cdecl*)(int, int, int, int)> GetMovepointsForReachNearTile;

	ProcLoader<int(__cdecl*)()> getGameVersion;
	ProcLoader<void(__cdecl*)()> toggleDeveloperMode;
	ProcLoader<void(__cdecl*)(const char*)> saveGame;
	ProcLoader<void(__cdecl*)(stackStruct*, stackStruct*)> mergeArmies;
	ProcLoader<stackStruct*(__cdecl*)(
		factionStruct*,
		const char*,
		const char*,
		int,
		const char*,
		const char*,
		int,
		int,
		int,
		bool,
		int,
		int,
		int,
		int,
		int)> spawnArmy;

	//graphics
	ProcLoader<LPDIRECT3DTEXTURE9(__cdecl*)(const char*, int*, int*)> loadTexture;
	ProcLoader<IDirect3DTexture9*(__cdecl*)(mapImage*,IDirect3DTexture9*, int, int)> updateRegionColors;
	ProcLoader<void(__cdecl*)(LPDIRECT3DTEXTURE9)> unloadTexture;


	ProcLoader<void(__cdecl*)(unsigned int)> setBuildingChainLimit;

	ProcLoader<guild* (_cdecl*)(unsigned char)> getGuild;


	ProcLoader<int(_cdecl*)()> GetUnitSize;

	//build functions
	ProcLoader<edbEntry* (__cdecl*)(edbEntry*, int)>addEopBuildEntry;
	ProcLoader<edbEntry* (__cdecl*)(int)>getEopBuildEntry;
	ProcLoader<void(__cdecl*)(edbEntry*, const char*, int, int)>setBuildingPic;
	ProcLoader<void(__cdecl*)(edbEntry*, const char*, int, int)>setBuildingPicConstructed;
	ProcLoader<void(__cdecl*)(edbEntry*, const char*, int, int)>setBuildingPicConstruction;
	ProcLoader<void(__cdecl*)(edbEntry*, const char*, int, int)>setBuildingLocalizedName;
	ProcLoader<void(__cdecl*)(edbEntry*, const char*, int, int)>setBuildingLocalizedDescr;
	ProcLoader<void(__cdecl*)(edbEntry*, const char*, int, int)>setBuildingLocalizedDescrShort;
	ProcLoader<void(__cdecl*)(edbEntry*, int, int, int16_t, bool)>addBuildingCapability;
	ProcLoader<void(__cdecl*)(edbEntry*, int, int)>removeBuildingCapability;
	ProcLoader<BuildingLvlCapability* (__cdecl*)(edbEntry*, int, int)>getBuildingCapability;
	ProcLoader<int(__cdecl*)(edbEntry*, int)>getBuildingCapabilityNum;
	ProcLoader<void(__cdecl*)(edbEntry*, int, int, float, float, float, int32_t, const char*)>addBuildingPool;
	ProcLoader<void(__cdecl*)(edbEntry*, int, int)>removeBuildingPool;
	ProcLoader<recruitPool* (__cdecl*)(edbEntry*, int, int)>getBuildingPool;
	ProcLoader<int(__cdecl*)(edbEntry*, int)>getBuildingPoolNum;
	ProcLoader<void(__cdecl*)(settlementStruct*, int, int)>createEOPBuilding;
	ProcLoader<edbEntry* (__cdecl*)(const char*)>getBuildingByName;

};


class eopEduFuncs
{
public:
	//edu functions
	ProcLoader<eduEntry* (__cdecl*)(int, int)> addEopEduEntry;
	ProcLoader<eduEntry* (__cdecl*)(const char*, int)> addEopEduEntryFromFile;
	ProcLoader<eduEntry* (__cdecl*)(int)> getEopEduEntry;
	ProcLoader<eduEntry* (__cdecl*)(int)> getEduEntry;
	ProcLoader<eduEntry* (__cdecl*)(const char*)> getEduEntryByType;
	ProcLoader<int(__cdecl*)(const char*)> getEduIndexByType;
	ProcLoader<int(__cdecl*)(int)> getDataEopEdu;


	ProcLoader<void(__cdecl*)(int, const char*)> setEntryUnitCardTga;
	ProcLoader<void(__cdecl*)(int, const char*)> setEntryInfoCardTga;
	ProcLoader<void(__cdecl*)(int, const char*)> setEntrySoldierModel;

	ProcLoader<void(__cdecl*)(int, const char*)> setEntryLocalizedName;
	ProcLoader<void(__cdecl*)(int, const char*)> setEntryLocalizedDescr;
	ProcLoader<void(__cdecl*)(int, const char*)> setEntryLocalizedShortDescr;
};


class eopFbxFuncs
{
public:
	//fbx functions
	ProcLoader<bool(__cdecl*)(const char*, const char*, int)> addFbxModel;
	ProcLoader<bool(__cdecl*)(int)> deleteFbxModel;
	ProcLoader<void* (__cdecl*)(int)> getFbxModel;

	ProcLoader<void* (__cdecl*)(int, int)> addFbxObject;
	ProcLoader<bool(__cdecl*)(void*)> deleteFbxObject;
	ProcLoader< void* (__cdecl*)(int)> getFbxObject;


	ProcLoader< void(__cdecl*)(void*, float*)> setFbxObjectCoords;
	ProcLoader< float* (__cdecl*)(void*)> getFbxObjectCoords;


	ProcLoader< void(__cdecl*)(void*, float)> setFbxObjectSize;
	ProcLoader< float(__cdecl*)(void*)> getFbxObjectSize;

	ProcLoader< void(__cdecl*)(void*, bool)> setFbxObjectDrawState;
	ProcLoader< bool(__cdecl*)(void*)> getFbxObjectDrawState;


	ProcLoader< void(__cdecl*)(void*, bool)> setFbxObjectAnimState;
	ProcLoader< bool(__cdecl*)(void*)> getFbxObjectAnimState;


	ProcLoader< void(__cdecl*)(void*, int)> setFbxObjectDrawType;
	ProcLoader< int(__cdecl*)(void*)> getFbxObjectDrawType;




};
class eopSoundsFuncs
{
public:
	//Sounds functions
	ProcLoader<void* (__cdecl*)(const char*)> createEOPSound;
	ProcLoader<void(__cdecl*)(void*)> deleteEOPSound;

	ProcLoader<void(__cdecl*)(void*)> playEOPSound;
	ProcLoader<void(__cdecl*)(void*, unsigned int)> setEOPSoundOffset;
	ProcLoader<unsigned int(__cdecl*)(void*)> getEOPSoundOffset;
	ProcLoader<void(__cdecl*)(void*)> pauseEOPSound;
	ProcLoader<void(__cdecl*)(void*)> stopEOPSound;

};
class gameUIFuncs
{
public:
	//Game ui functions
	ProcLoader<uiElement* (__cdecl*)(const char*)> getUiElement;
	ProcLoader<void(__cdecl*)(uiElement*)> useUiElement;
};