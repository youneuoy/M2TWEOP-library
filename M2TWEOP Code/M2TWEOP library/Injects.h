#pragma once
#include "TemplateCh.h"
//asm
#include <AsmJit.h>
#include <Windows.h>
using namespace std;
using namespace AsmJit;
//tests /etc
class Injects
	:public AATemplate
{
public:
	Injects(MemWork* mem);
	~Injects();

	void SetOriginalUnitCode();
	void SetlUnitCode();
};


class Age
	:public AATemplate
{
public:
	Age(MemWork* mem, LPVOID adr, int ver);
	~Age();

	void SetOriginaAgeCode();
	void SetlAgeCode();
private:
	LPVOID funcAdress;
};

class AgeVisual
	:public AATemplate
{
public:
	AgeVisual(MemWork* mem, LPVOID adr, int ver);
	~AgeVisual();

	void SetOriginaAgeCode();
	void SetlAgeCode();
private:
	LPVOID funcAdress;
};


class berserkersP
	:public AATemplate
{
public:
	berserkersP(MemWork* mem, int ver);
	~berserkersP();

	void SetOriginaBersCode();
	void SetlBersCode();
};


class toReadEDU
	:public AATemplate
{
public:
	toReadEDU(MemWork* mem, LPVOID adr, int ver);
	~toReadEDU();

	void SetOriginalEDUCode();
	void SetlEDUCode();
private:
	LPVOID funcAdress;
};


class toChangePassedTurnsCount
	:public AATemplate
{
public:
	toChangePassedTurnsCount(MemWork* mem, LPVOID adr, int ver);
	~toChangePassedTurnsCount();

	void SetOriginalTurnsCountCode();
	void SetlTurnsCountCode();
private:
	LPVOID funcAdress;
};

class toBattleResults
	:public AATemplate
{
public:
	toBattleResults(MemWork* mem, LPVOID adr, int ver);
	~toBattleResults();

	void SetOriginalBattleResCode();
	void SetlBattleResCode();
private:
	LPVOID funcAdress;
};


class toCheckLabelResults
	:public AATemplate
{
public:
	toCheckLabelResults(MemWork* mem, LPVOID adr, int ver);
	~toCheckLabelResults();

	void SetOriginalCheckLabCode();
	void SetlCheckLabCode();
private:
	LPVOID funcAdress;
};

//after read campaign or battle files
class toCreateMap
	:public AATemplate
{
public:
	toCreateMap(MemWork* mem, LPVOID adr, int ver);
	~toCreateMap();

	void SetOriginalCreateCode();
	void SetlCreateCode();
private:
	LPVOID funcAdress;

};
//after comparison in I_CompareCounter command
class toI_CompareCounter
	:public AATemplate
{
public:
	toI_CompareCounter(MemWork* mem, LPVOID adr, int ver);
	~toI_CompareCounter();

	void SetOriginaltoI_CompareCounterCode();
	void SetltoI_CompareCounterCode();
private:
	LPVOID funcAdress;

};


//to forts delete function
class toFortsDelCheck
	:public AATemplate
{
public:
	toFortsDelCheck(MemWork* mem, LPVOID adr, int ver);
	~toFortsDelCheck();

	void SetOriginalFortsDelCode();
	void SetlFortsDelCode();
private:
	LPVOID funcAdress;

};


//to read and check DiplomaticStanceFromFaction command
//try to find DiplomaticStance and write to DiplomaticStanceFromFaction structure
class toDSFromFacCh
	:public AATemplate
{
public:
	toDSFromFacCh(MemWork* mem, LPVOID adr, int ver);
	~toDSFromFacCh();

	void SetOriginalDsFromFacCode();
	void SetlDsFromFacCode();
private:
	LPVOID funcAdress;

};

//check DiplomaticStanceFromFaction command
class toDiplomaticStanceCheck
	:public AATemplate
{
public:
	toDiplomaticStanceCheck(MemWork* mem, LPVOID adr, int ver);
	~toDiplomaticStanceCheck();

	void SetOriginalDsCheckCode();
	void SetlDsCheckCode();
private:
	LPVOID funcAdress;

};

//fix of crash with creatures in battle
class DCICrashHard
	:public AATemplate
{
public:
	DCICrashHard(MemWork* mem, LPVOID adr, int ver);
	~DCICrashHard();

	void SetOriginaDCICode();
	void SetlDCICode();
private:
	LPVOID funcAdress;
};

//spawn_army available coordinates edit
class toSpawnArmy
	:public AATemplate
{
public:
	toSpawnArmy(MemWork* mem, LPVOID adr, int ver);
	~toSpawnArmy();

	void SetOriginaSpawnCode();
	void SetlSpawnCode();
private:
	LPVOID funcAdress;
};

//spawn_army available coordinates edit
class toEndOfSpawnArmy
	:public AATemplate
{
public:
	toEndOfSpawnArmy(MemWork* mem, LPVOID adr, int ver);
	~toEndOfSpawnArmy();

	void SetOriginalEndSpawnCode();
	void SetlEndSpawnCode();
private:
	LPVOID funcAdress;
};

//spawn_army available coordinates edit
class toSpawnCharacter
	:public AATemplate
{
public:
	toSpawnCharacter(MemWork* mem, LPVOID adr, int ver);
	~toSpawnCharacter();

	void SetOriginaSpawnCode();
	void SetlSpawnCode();
private:
	LPVOID funcAdress;
};

//on the battle screen on stratmap
class toBattleStratScreen
	:public AATemplate
{
public:
	toBattleStratScreen(MemWork* mem, LPVOID adr, int ver);
	~toBattleStratScreen();

	void SetOriginaBSCode();
	void SetlBSCode();
private:
	LPVOID funcAdress;
};

//on the start of reading cas strat model
class toStartReadCas
	:public AATemplate
{
public:
	toStartReadCas(MemWork* mem, LPVOID adr, int ver);
	~toStartReadCas();

	void SetOriginaStReadCasCode();
	void SetlStReadCasCode();
private:
	LPVOID funcAdress;
};


//on the start of reading models function
class toStartReadModels
	:public AATemplate
{
public:
	toStartReadModels(MemWork* mem, LPVOID adr, int ver);
	~toStartReadModels();

	void SetOriginaStReadModelsCode();
	void SetlStReadModelsCode();
private:
	LPVOID funcAdress;
};


//on the end of reading models function
class toEndReadModels
	:public AATemplate
{
public:
	toEndReadModels(MemWork* mem, LPVOID adr, int ver);
	~toEndReadModels();

	void SetOriginaEndReadModelsCode();
	void SetlEndReadModelsCode();
private:
	LPVOID funcAdress;
};


//on the  reading resources models function
class toResourcesReadModels
	:public AATemplate
{
public:
	toResourcesReadModels(MemWork* mem, LPVOID adr, int ver);
	~toResourcesReadModels();

	void SetOriginaResModelsCode();
	void SetlResModelsCode();
private:
	LPVOID funcAdress;
};

//on the  reading resources models function
class toCharReadModels
	:public AATemplate
{
public:
	toCharReadModels(MemWork* mem, LPVOID adr, int ver);
	~toCharReadModels();

	void SetOriginalCode();
	void SetNewCode();
private:
	LPVOID funcAdress;
};



class sizeOfBattleField :
	public AATemplate
{
public:
	sizeOfBattleField(MemWork* mem, UINT32 gamever, float sizeX, float sizeY);
	~sizeOfBattleField();

	void SetOriginalSizeCode();
	void SetlSizeCode();
protected:
	float sizeX;
	float sizeY;
};


//on events
class toEvents
	:public AATemplate
{
public:
	toEvents(MemWork* mem, LPVOID adr, int ver);
	~toEvents();

	void SetOriginalEventsCode();
	void SetlEventsCode();
private:
	LPVOID funcAdress;
};

//on custom tile
class toCustomTileSelection
	:public AATemplate
{
public:
	toCustomTileSelection(MemWork* mem, LPVOID adr, int ver);
	~toCustomTileSelection();

	void SetOriginalTilesCode();
	void SetlTilesCode();
private:
	LPVOID funcAdress;
};

//on custom tiles file read
class toCustomTileFileRead
	:public AATemplate
{
public:
	toCustomTileFileRead(MemWork* mem, LPVOID adr, int ver);
	~toCustomTileFileRead();

	void SetOriginalTilesCode();
	void SetlTilesCode();
private:
	LPVOID funcAdress;
};

//on strat models select
class toStratModelsSelect
	:public AATemplate
{
public:
	toStratModelsSelect(MemWork* mem, LPVOID adr, int ver);
	~toStratModelsSelect();

	void SetOriginalStratModelsCode();
	void SetlStratModelsCode();
private:
	LPVOID funcAdress;
};


//before load savegame
class toLoadSaveFile
	:public AATemplate
{
public:
	toLoadSaveFile(MemWork* mem, LPVOID adr, int ver);
	~toLoadSaveFile();

	void SetOriginalLoadCode();
	void SetlLoadCode();
private:
	LPVOID funcAdress;
};

//after save game
class toSaveGame
	:public AATemplate
{
public:
	toSaveGame(MemWork* mem, LPVOID adr, int ver);
	~toSaveGame();

	void SetOriginalSaveCode();
	void SetlSaveCode();
private:
	LPVOID funcAdress;
};


//new game start
class toStartNewGame
	:public AATemplate
{
public:
	toStartNewGame(MemWork* mem, LPVOID adr, int ver);
	~toStartNewGame();

	void SetOriginalStartCode();
	void SetlStartCode();
private:
	LPVOID funcAdress;
};




//load cas model
class toLoadCas
	:public AATemplate
{
public:
	toLoadCas(MemWork* mem, LPVOID adr, int ver);
	~toLoadCas();

	void SetOriginalStartCode();
	void SetlCasCode();
private:
	LPVOID funcAdress;
};



//click at stratmap
class toClickAtCoords
	:public AATemplate
{
public:
	toClickAtCoords(MemWork* mem, LPVOID adr, int ver);
	~toClickAtCoords();

	void SetOriginalClickCode();
	void SetlClickCode();
private:
	LPVOID funcAdress;
};


//select worldpkgdesc from db
class toSelectWorldFromDB
	:public AATemplate
{
public:
	toSelectWorldFromDB(MemWork* mem, LPVOID adr, int ver);
	~toSelectWorldFromDB();

	void SetOriginalSelectCode();
	void SetlSelectCode();
private:
	LPVOID funcAdress;
};



//select worldpkgdesc from db
class toBattleLoaded
	:public AATemplate
{
public:
	toBattleLoaded(MemWork* mem, LPVOID adr, int ver);
	~toBattleLoaded();

	void SetOriginalLoadCode();
	void SetlLoadCode();
private:
	LPVOID funcAdress;
};



class toReadGameDBsAtGameStart
	:public AATemplate
{
public:
	toReadGameDBsAtGameStart(MemWork* mem, LPVOID adr, int ver);
	~toReadGameDBsAtGameStart();

	void SetOriginalReadCode();
	void SetlReadCode();
private:
	LPVOID funcAdress;
};





//draw parts of stratmodels
class toDrawPartsOfStratObjects
	:public AATemplate
{
public:
	toDrawPartsOfStratObjects(MemWork* mem, LPVOID adr, int ver);
	~toDrawPartsOfStratObjects();

	void SetOriginalDrawCode();
	void SetlDrawCode();
private:
	LPVOID funcAdress;
};




//siege of settlement ended
class toEndSettlementSiege
	:public AATemplate
{
public:
	toEndSettlementSiege(MemWork* mem, LPVOID adr, int ver);
	~toEndSettlementSiege();

	void SetOriginalSiegeCode();
	void SetlSiegeCode();
private:
	LPVOID funcAdress;
};



//siege of settlement ended
class toStartSettlementSiege
	:public AATemplate
{
public:
	toStartSettlementSiege(MemWork* mem, LPVOID adr, int ver);
	~toStartSettlementSiege();

	void SetOriginalSiegeCode();
	void SetlSiegeCode();
private:
	LPVOID funcAdress;
};



class toLoadDescrBattleCharacter
	:public AATemplate
{
public:
	toLoadDescrBattleCharacter(MemWork* mem, LPVOID adr, int ver);
	~toLoadDescrBattleCharacter();

	void SetOriginalBattleCode();
	void SetlBattleCode();
private:
	LPVOID funcAdress;
};




//battle finished and we see results scroll
class toBattleStateResults
	:public AATemplate
{
public:
	toBattleStateResults(MemWork* mem, LPVOID adr, int ver);
	~toBattleStateResults();

	void SetOriginalBattleCode();
	void SetlBattleCode();
private:
	LPVOID funcAdress;
};




//we can draw here and it will eat not so many fps
class toGameFormDrawImage
	:public AATemplate
{
public:
	toGameFormDrawImage(MemWork* mem, LPVOID adr, int ver);
	~toGameFormDrawImage();

	void SetOriginalDrawCode();
	void SetlDrawCode();
private:
	LPVOID funcAdress;
};



class CastleConversionLvlSetter
	:public AATemplate
{
public:
	CastleConversionLvlSetter(MemWork* mem, LPVOID addr, int ver);
	~CastleConversionLvlSetter();

	void SetOriginalCode();
	void SetNewCode();
private:
	LPVOID funcAddress;
};


class CityConversionLvlSetter
	:public AATemplate
{
public:
	CityConversionLvlSetter(MemWork* mem, LPVOID addr, int ver);
	~CityConversionLvlSetter();

	void SetOriginalCode();
	void SetNewCode();
private:
	LPVOID funcAddress;
};


class mercenaryMovepointsGetGeneral
	:public AATemplate
{
public:
	mercenaryMovepointsGetGeneral(MemWork* mem, LPVOID addr, int ver);
	~mercenaryMovepointsGetGeneral();

	void SetOriginalCode();
	void SetNewCode();
private:
	LPVOID funcAddress;
};


class onGameInitialized
	:public AATemplate
{
public:
	onGameInitialized(MemWork* mem, LPVOID adr, int ver);
	~onGameInitialized();

	void SetOriginalCode();
	void SetNewCode();
private:
	LPVOID funcAddress;
};

class onAiTurn
	:public AATemplate
{
public:
	onAiTurn(MemWork* mem, LPVOID adr, int ver);
	~onAiTurn();

	void SetOriginalCode();
	void SetNewCode();
private:
	LPVOID funcAddress;
	DWORD otherFunc;
};

class onGameConsoleCommandFromConsole
	:public AATemplate
{
public:
	onGameConsoleCommandFromConsole(MemWork* mem, LPVOID addr, int ver);
	~onGameConsoleCommandFromConsole();

	void SetOriginalCode();
	void SetNewCode();
private:
	LPVOID funcAddress;
};

class onGameConsoleCommandFromScript
	:public AATemplate
{
public:
	onGameConsoleCommandFromScript(MemWork* mem, LPVOID addr, int ver);
	~onGameConsoleCommandFromScript();

	void SetOriginalCode();
	void SetNewCode();
private:
	LPVOID funcAddress;
};



class OnReadLogonOrLogoff
	:public AATemplate
{
public:
	OnReadLogonOrLogoff(MemWork* mem, LPVOID addr, int ver);
	~OnReadLogonOrLogoff();

	void SetOriginalCode();
	void SetNewCode();
private:
	LPVOID funcAddress;
};



class OnLoadSettlementWorldpkgdesc
	:public AATemplate
{
public:
	OnLoadSettlementWorldpkgdesc(MemWork* mem, LPVOID addr, int ver);
	~OnLoadSettlementWorldpkgdesc();

	void SetOriginalCode();
	void SetNewCode();
private:
	LPVOID funcAddress;
};

class LimitRecruitmentQueueToSlotsInj
	:public AATemplate
{
public:
	LimitRecruitmentQueueToSlotsInj(MemWork* mem, int* gameplaySettingAddr, int ver);
	~LimitRecruitmentQueueToSlotsInj();

	void SetOriginialCode();
	void SetNewCode();
private:
	int m_version;
	int* settingAddr;
};

class fortificationlevelS
	:public AATemplate
{
public:
	fortificationlevelS(MemWork* mem, LPVOID addr, int ver);
	~fortificationlevelS();

	void SetOriginialCode();
	void SetNewCode();
private:
	LPVOID funcAddress;
};

class OnSaveEDUStringS
	:public AATemplate
{
public:
	OnSaveEDUStringS(MemWork* mem, LPVOID addr, int ver);
	~OnSaveEDUStringS();

	void SetOriginialCode();
	void SetNewCode();
private:
	LPVOID funcAddress;
};
class OnSaveEDUStringS2
	:public AATemplate
{
public:
	OnSaveEDUStringS2(MemWork* mem, LPVOID addr, int ver);
	~OnSaveEDUStringS2();

	void SetOriginialCode();
	void SetNewCode();
private:
	LPVOID funcAddress;
};

class OnCreateUnit
	:public AATemplate
{
public:
	OnCreateUnit(MemWork* mem, LPVOID addr, int ver);
	~OnCreateUnit();

	void SetOriginialCode();
	void SetNewCode();
private:
	LPVOID funcAddress;
};

class OnFindUnit
	:public AATemplate
{
public:
	OnFindUnit(MemWork* mem, LPVOID addr, int ver);
	~OnFindUnit();

	void SetOriginialCode();
	void SetNewCode();
private:
	LPVOID funcAddress;
};

class OnCreateMercUnit
	:public AATemplate
{
public:
	OnCreateMercUnit(MemWork* mem, LPVOID addr, int ver);
	~OnCreateMercUnit();

	void SetOriginialCode();
	void SetNewCode();
private:
	LPVOID funcAddress;
	DWORD otherFunc;
};

class OnCreateUnitWrapper
	:public AATemplate
{
public:
	OnCreateUnitWrapper(MemWork* mem, LPVOID addr, int ver);
	~OnCreateUnitWrapper();

	void SetOriginialCode();
	void SetNewCode();
private:
	LPVOID funcAddress;
};

class OnGetRecruitPoolUnitEntry
	:public AATemplate
{
public:
	OnGetRecruitPoolUnitEntry(MemWork* mem, LPVOID addr, int ver);
	~OnGetRecruitPoolUnitEntry();

	void SetOriginialCode();
	void SetNewCode();
private:
	LPVOID funcAddress;
};

class OnGetRecruitPoolUnitEntry2
	:public AATemplate
{
public:
	OnGetRecruitPoolUnitEntry2(MemWork* mem, LPVOID addr, int ver);
	~OnGetRecruitPoolUnitEntry2();

	void SetOriginialCode();
	void SetNewCode();
private:
	LPVOID funcAddress;
};

class OnFindUnitStrings
	:public AATemplate
{
public:
	OnFindUnitStrings(MemWork* mem, LPVOID addr, int ver);
	~OnFindUnitStrings();

	void SetOriginialCode();
	void SetNewCode();
private:
	LPVOID funcAddress;
};

class OnFindUnitStrings2
	:public AATemplate
{
public:
	OnFindUnitStrings2(MemWork* mem, LPVOID addr, int ver);
	~OnFindUnitStrings2();

	void SetOriginialCode();
	void SetNewCode();
private:
	LPVOID funcAddress;
};

class OnCreateMercUnitCheck
	:public AATemplate
{
public:
	OnCreateMercUnitCheck(MemWork* mem, LPVOID addr, int ver);
	~OnCreateMercUnitCheck();

	void SetOriginialCode();
	void SetNewCode();
private:
	LPVOID funcAddress;
};

class OnQuickSave
	:public AATemplate
{
public:
	OnQuickSave(MemWork* mem, LPVOID addr, int ver);
	~OnQuickSave();

	void SetOriginialCode();
	void SetNewCode();
private:
	LPVOID funcAddress;
};


class OnAutoSave
	:public AATemplate
{
public:
	OnAutoSave(MemWork* mem, LPVOID addr, int ver);
	~OnAutoSave();

	void SetOriginialCode();
	void SetNewCode();
private:
	LPVOID funcAddress;
};


class blockLaunchWithoutEop
	:public AATemplate
{
public:
	blockLaunchWithoutEop(MemWork* mem, int ver);
	~blockLaunchWithoutEop();

	void SetOriginialCode();
	void SetNewCode();
};


/*
class OntryFindTypeIdInListRecruitPoolEDB
	:public AATemplate
{
public:
	OntryFindTypeIdInListRecruitPoolEDB(MemWork* mem, LPVOID addr, int ver);
	~OntryFindTypeIdInListRecruitPoolEDB();

	void SetOriginialCode();
	void SetNewCode();
private:
	LPVOID funcAddress;
};


class OnrecruitPoolFillFromFile
	:public AATemplate
{
public:
	OnrecruitPoolFillFromFile(MemWork* mem, LPVOID addr, int ver);
	~OnrecruitPoolFillFromFile();

	void SetOriginialCode();
	void SetNewCode();
private:
	LPVOID funcAddress;
};
class OnrecruitPoolFillFromFile2
	:public AATemplate
{
public:
	OnrecruitPoolFillFromFile2(MemWork* mem, LPVOID addr, int ver);
	~OnrecruitPoolFillFromFile2();

	void SetOriginialCode();
	void SetNewCode();
private:
	LPVOID funcAddress;
};
*/



//backspace patch
class OnStopCharacter
	:public AATemplate
{
public:
	OnStopCharacter(MemWork* mem, LPVOID addr, int ver);
	~OnStopCharacter();

	void SetOriginialCode();
	void SetNewCode();
private:
	LPVOID funcAddress;
};

class OnMoveRecruitQueue
	:public AATemplate
{
public:
	OnMoveRecruitQueue(MemWork* mem, LPVOID addr, int ver);
	~OnMoveRecruitQueue();
	void SetOriginialCode();
	void SetNewCode();
private:
	LPVOID funcAddress;
};

//PathCashe crash fix
class OnPathCasheCrashPlace
	:public AATemplate
{
public:
	OnPathCasheCrashPlace(MemWork* mem, LPVOID addr, int ver, LPVOID cbObj);
	~OnPathCasheCrashPlace();
	void SetOriginalCode();
	void SetNewCode();
private:
	LPVOID funcAddress;
	LPVOID callbackObject;
};

//create eop unit
class recruitEOPunit
	:public AATemplate
{
public:
	recruitEOPunit(MemWork* mem, LPVOID addr, int ver);
	~recruitEOPunit();

	void SetOriginialCode();
	void SetNewCode();
private:
	LPVOID funcAddress;
};

//create eop unit
class recruitEOPunit2
	:public AATemplate
{
public:
	recruitEOPunit2(MemWork* mem, LPVOID addr, int ver);
	~recruitEOPunit2();

	void SetOriginialCode();
	void SetNewCode();
private:
	LPVOID funcAddress;
};

//create eop unit
class loadRecruitQueue
	:public AATemplate
{
public:
	loadRecruitQueue(MemWork* mem, LPVOID addr, int ver);
	~loadRecruitQueue();

	void SetOriginialCode();
	void SetNewCode();
private:
	LPVOID funcAddress;
};

//create eop unit
class loadRecruitQueue2
	:public AATemplate
{
public:
	loadRecruitQueue2(MemWork* mem, LPVOID addr, int ver);
	~loadRecruitQueue2();

	void SetOriginialCode();
	void SetNewCode();
private:
	LPVOID funcAddress;
};

//create eop unit
class recruitEOPMercunit
	:public AATemplate
{
public:
	recruitEOPMercunit(MemWork* mem, LPVOID addr, int ver);
	~recruitEOPMercunit();

	void SetOriginialCode();
	void SetNewCode();
private:
	LPVOID funcAddress;
};

class toSelectForDrawPortsCas
	:public AATemplate
{
public:
	toSelectForDrawPortsCas(MemWork* mem, LPVOID addr, int ver);
	~toSelectForDrawPortsCas();

	void SetOriginialCode();
	void SetNewCode();
private:
	LPVOID funcAddress;
};

class toStartOfDrawFunction
	:public AATemplate
{
public:
	toStartOfDrawFunction(MemWork* mem, LPVOID addr, int ver);
	~toStartOfDrawFunction();

	void SetOriginialCode();
	void SetNewCode();
private:
	LPVOID funcAddress;
};



class toRetreat
	:public AATemplate
{
public:
	toRetreat(MemWork* mem, LPVOID addr, int ver);
	~toRetreat();

	void SetOriginialCode();
	void SetNewCode();
private:
	LPVOID funcAddress;
};

class sizeOfBattleField :
	public AATemplate
{
public:
	sizeOfBattleField(MemWork* mem, UINT32 gamever, float sizeX, float sizeY);
	~sizeOfBattleField();

	void SetOriginalSizeCode();
	void SetlSizeCode();
protected:
	UINT32 gamever;
	float sizeX;
	float sizeY;
};