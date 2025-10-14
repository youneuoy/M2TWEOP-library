#pragma once
#include "templateCh.h"
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


class onUnloadCampaign
	:public AATemplate
{
public:
	onUnloadCampaign(MemWork* mem, LPVOID adr, int ver);
	~onUnloadCampaign();

	void SetOriginalCode();
	void SetNewCode();
private:
	LPVOID funcAddress;
};


class onGetUnitByLabel
	:public AATemplate
{
public:
	onGetUnitByLabel(MemWork* mem, LPVOID adr, int ver);
	~onGetUnitByLabel();

	void SetOriginalCode();
	void SetNewCode();
private:
	LPVOID funcAddress;
	DWORD otherFunc;
};


class onGetGroupByLabel
	:public AATemplate
{
public:
	onGetGroupByLabel(MemWork* mem, LPVOID adr, int ver);
	~onGetGroupByLabel();

	void SetOriginalCode();
	void SetNewCode();
private:
	LPVOID funcAddress;
	DWORD otherFunc;
};


class onAddCultureToArray
	:public AATemplate
{
public:
	onAddCultureToArray(MemWork* mem, LPVOID adr, int ver);
	~onAddCultureToArray();

	void SetOriginalCode();
	void SetNewCode();
private:
	LPVOID funcAddress;
};


class onCreatePortraitDb
	:public AATemplate
{
public:
	onCreatePortraitDb(MemWork* mem, LPVOID adr, int ver);
	~onCreatePortraitDb();

	void SetOriginalCode();
	void SetNewCode();
private:
	LPVOID funcAddress;
};

//008A472D
class onGetBrowserPicConstructed
	:public AATemplate
{
public:
	onGetBrowserPicConstructed(MemWork* mem, LPVOID adr, int ver);
	~onGetBrowserPicConstructed();

	void SetOriginalCode();
	void SetNewCode();
private:
	LPVOID funcAddress;
};

class onGetBrowserPicConstructed2
	:public AATemplate
{
public:
	onGetBrowserPicConstructed2(MemWork* mem, LPVOID adr, int ver);
	~onGetBrowserPicConstructed2();

	void SetOriginalCode();
	void SetNewCode();
private:
	LPVOID funcAddress;
};

//008A472D
class onGetBrowserPicConstruction
	:public AATemplate
{
public:
	onGetBrowserPicConstruction(MemWork* mem, LPVOID adr, int ver);
	~onGetBrowserPicConstruction();

	void SetOriginalCode();
	void SetNewCode();
private:
	LPVOID funcAddress;
};

class onGetBuildingPic
	:public AATemplate
{
public:
	onGetBuildingPic(MemWork* mem, LPVOID adr, int ver);
	~onGetBuildingPic();

	void SetOriginalCode();
	void SetNewCode();
private:
	LPVOID funcAddress;
};

class onGetBuildingPic2
	:public AATemplate
{
public:
	onGetBuildingPic2(MemWork* mem, LPVOID adr, int ver);
	~onGetBuildingPic2();

	void SetOriginalCode();
	void SetNewCode();
private:
	LPVOID funcAddress;
};

class onGetGuildOfferPic
	:public AATemplate
{
public:
	onGetGuildOfferPic(MemWork* mem, LPVOID adr, int ver);
	~onGetGuildOfferPic();

	void SetOriginalCode();
	void SetNewCode();
private:
	LPVOID funcAddress;
};

class onGetPortraitDbEntry
	:public AATemplate
{
public:
	onGetPortraitDbEntry(MemWork* mem, LPVOID adr, int ver);
	~onGetPortraitDbEntry();

	void SetOriginalCode();
	void SetNewCode();
private:
	LPVOID funcAddress;
};

class onParseBuildingPics
	:public AATemplate
{
public:
	onParseBuildingPics(MemWork* mem, LPVOID adr, int ver);
	~onParseBuildingPics();

	void SetOriginalCode();
	void SetNewCode();
private:
	LPVOID funcAddress;
};

class onParseBuildingLevelPics
	:public AATemplate
{
public:
	onParseBuildingLevelPics(MemWork* mem, LPVOID adr, int ver);
	~onParseBuildingLevelPics();

	void SetOriginalCode();
	void SetNewCode();
private:
	LPVOID funcAddress;
};

class onParseEventBodies
	:public AATemplate
{
public:
	onParseEventBodies(MemWork* mem, LPVOID adr, int ver);
	~onParseEventBodies();

	void SetOriginalCode();
	void SetNewCode();
private:
	LPVOID funcAddress;
};

class onGetCultureEndTurnSound
	:public AATemplate
{
public:
	onGetCultureEndTurnSound(MemWork* mem, LPVOID adr, int ver);
	~onGetCultureEndTurnSound();

	void SetOriginalCode();
	void SetNewCode();
private:
	LPVOID funcAddress;
};

class onGetBuildingPicConstruction
	:public AATemplate
{
public:
	onGetBuildingPicConstruction(MemWork* mem, LPVOID adr, int ver);
	~onGetBuildingPicConstruction();

	void SetOriginalCode();
	void SetNewCode();
private:
	LPVOID funcAddress;
};

class onGetBuildingPicConstructed
	:public AATemplate
{
public:
	onGetBuildingPicConstructed(MemWork* mem, LPVOID adr, int ver);
	~onGetBuildingPicConstructed();

	void SetOriginalCode();
	void SetNewCode();
private:
	LPVOID funcAddress;
};


class onSearchUnitType
	:public AATemplate
{
public:
	onSearchUnitType(MemWork* mem, LPVOID adr, int ver);
	~onSearchUnitType();

	void SetOriginalCode();
	void SetNewCode();
private:
	LPVOID funcAddress;
};


class onAssignGateAttacker
	:public AATemplate
{
public:
	onAssignGateAttacker(MemWork* mem, LPVOID adr, int ver);
	~onAssignGateAttacker();

	void SetOriginalCode();
	void SetNewCode();
private:
	LPVOID funcAddress;
};

class onAttackGate
	:public AATemplate
{
public:
	onAttackGate(MemWork* mem, LPVOID adr, int ver);
	~onAttackGate();

	void SetOriginalCode();
	void SetNewCode();
private:
	LPVOID funcAddress;
};

class onAttackGate2
	:public AATemplate
{
public:
	onAttackGate2(MemWork* mem, LPVOID adr, int ver);
	~onAttackGate2();

	void SetOriginalCode();
	void SetNewCode();
private:
	LPVOID funcAddress;
};

class onReadBuildingPool
	:public AATemplate
{
public:
	onReadBuildingPool(MemWork* mem, LPVOID adr, int ver);
	~onReadBuildingPool();

	void SetOriginalCode();
	void SetNewCode();
private:
	LPVOID funcAddress;
};

class onReadMercenaryPool
	:public AATemplate
{
public:
	onReadMercenaryPool(MemWork* mem, LPVOID adr, int ver);
	~onReadMercenaryPool();

	void SetOriginalCode();
	void SetNewCode();
private:
	LPVOID funcAddress;
};

class onReadDescrStrat
	:public AATemplate
{
public:
	onReadDescrStrat(MemWork* mem, LPVOID adr, int ver);
	~onReadDescrStrat();

	void SetOriginalCode();
	void SetNewCode();
private:
	LPVOID funcAddress;
};

class onSpawnBrigands
	:public AATemplate
{
public:
	onSpawnBrigands(MemWork* mem, LPVOID adr, int ver);
	~onSpawnBrigands();

	void SetOriginalCode();
	void SetNewCode();
private:
	LPVOID funcAddress;
};

class onSpawnBrigands2
	:public AATemplate
{
public:
	onSpawnBrigands2(MemWork* mem, LPVOID adr, int ver);
	~onSpawnBrigands2();

	void SetOriginalCode();
	void SetNewCode();
private:
	LPVOID funcAddress;
};

class onCustomBattleCost
	:public AATemplate
{
public:
	onCustomBattleCost(MemWork* mem, LPVOID adr, int ver);
	~onCustomBattleCost();

	void SetOriginalCode();
	void SetNewCode();
private:
	LPVOID funcAddress;
};

class spawnUnitsSettGift
	:public AATemplate
{
public:
	spawnUnitsSettGift(MemWork* mem, LPVOID adr, int ver);
	~spawnUnitsSettGift();

	void SetOriginalCode();
	void SetNewCode();
private:
	LPVOID funcAddress;
};

class spawnUnitsSettGift2
	:public AATemplate
{
public:
	spawnUnitsSettGift2(MemWork* mem, LPVOID adr, int ver);
	~spawnUnitsSettGift2();

	void SetOriginalCode();
	void SetNewCode();
private:
	LPVOID funcAddress;
};

class spawnUnitsSettGift3
	:public AATemplate
{
public:
	spawnUnitsSettGift3(MemWork* mem, LPVOID adr, int ver);
	~spawnUnitsSettGift3();

	void SetOriginalCode();
	void SetNewCode();
private:
	LPVOID funcAddress;
};

class spawnUnitsSettGift4
	:public AATemplate
{
public:
	spawnUnitsSettGift4(MemWork* mem, LPVOID adr, int ver);
	~spawnUnitsSettGift4();

	void SetOriginalCode();
	void SetNewCode();
private:
	LPVOID funcAddress;
};

class onReadDescrRebel
	:public AATemplate
{
public:
	onReadDescrRebel(MemWork* mem, LPVOID adr, int ver);
	~onReadDescrRebel();

	void SetOriginalCode();
	void SetNewCode();
private:
	LPVOID funcAddress;
};

class onCalcUnitStatsWpn
	:public AATemplate
{
public:
	onCalcUnitStatsWpn(MemWork* mem, LPVOID adr, int ver);
	~onCalcUnitStatsWpn() = default;

	void SetNewCode();
private:
	LPVOID funcAddress;
};

class onCalcUnitValueWpn
	:public AATemplate
{
public:
	onCalcUnitValueWpn(MemWork* mem, LPVOID adr, int ver);
	~onCalcUnitValueWpn() = default;

	void SetNewCode();
private:
	LPVOID funcAddress;
};

class onGetUnitStatsUi1
	:public AATemplate
{
public:
	onGetUnitStatsUi1(MemWork* mem, LPVOID adr, int ver);
	~onGetUnitStatsUi1() = default;

	void SetNewCode();
private:
	LPVOID funcAddress;
};

class onGetUnitStatsUi2
	:public AATemplate
{
public:
	onGetUnitStatsUi2(MemWork* mem, LPVOID adr, int ver);
	~onGetUnitStatsUi2() = default;

	void SetNewCode();
private:
	LPVOID funcAddress;
};

class onCustomBattleUnitCards
	:public AATemplate
{
public:
	onCustomBattleUnitCards(MemWork* mem, LPVOID adr, int ver);
	~onCustomBattleUnitCards();

	void SetOriginalCode();
	void SetNewCode();
private:
	LPVOID funcAddress;
};

class onCustomBattleUnits
	:public AATemplate
{
public:
	onCustomBattleUnits(MemWork* mem, LPVOID adr, int ver);
	~onCustomBattleUnits();

	void SetOriginalCode();
	void SetNewCode();
private:
	LPVOID funcAddress;
};


class onBuildingUnitAttributes
	:public AATemplate
{
public:
	onBuildingUnitAttributes(MemWork* mem, LPVOID adr, int ver);
	~onBuildingUnitAttributes();

	void SetOriginalCode();
	void SetNewCode();
private:
	LPVOID funcAddress;
};


class onBuildingUnitAttributes2
	:public AATemplate
{
public:
	onBuildingUnitAttributes2(MemWork* mem, LPVOID adr, int ver);
	~onBuildingUnitAttributes2();

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

class onSetExtentsTexture
	:public AATemplate
{
public:
	onSetExtentsTexture(MemWork* mem, LPVOID addr, int ver);
	~onSetExtentsTexture();

	void SetOriginalCode();
	void SetNewCode();
private:
	LPVOID funcAddress;
};

class onSetExtentsTextureBorder
	:public AATemplate
{
public:
	onSetExtentsTextureBorder(MemWork* mem, LPVOID addr, int ver);
	~onSetExtentsTextureBorder();

	void SetOriginalCode();
	void SetNewCode();
private:
	LPVOID funcAddress;
};

class onSetExtentsTextureZOC
	:public AATemplate
{
public:
	onSetExtentsTextureZOC(MemWork* mem, LPVOID addr, int ver);
	~onSetExtentsTextureZOC();

	void SetOriginalCode();
	void SetNewCode();
private:
	LPVOID funcAddress;
};

class onSetExtentsTextureZOCAlpha
	:public AATemplate
{
public:
	onSetExtentsTextureZOCAlpha(MemWork* mem, LPVOID addr, int ver);
	~onSetExtentsTextureZOCAlpha();

	void SetOriginalCode();
	void SetNewCode();
private:
	LPVOID funcAddress;
};

class onCreateProductionController
	:public AATemplate
{
public:
	onCreateProductionController(MemWork* mem, LPVOID addr, int ver);
	~onCreateProductionController();

	void SetOriginalCode();
	void SetNewCode();
private:
	LPVOID funcAddress;
};

class onDecideNeighbours
	:public AATemplate
{
public:
	onDecideNeighbours(MemWork* mem, LPVOID addr, int ver);
	~onDecideNeighbours();

	void SetOriginalCode();
	void SetNewCode();
private:
	LPVOID funcAddress;
};

class onTransferSettlement
	:public AATemplate
{
public:
	onTransferSettlement(MemWork* mem, LPVOID addr, int ver);
	void SetOriginalCode(){}
	void SetNewCode();
private:
	LPVOID funcAddress;
};

class onTransferSettlementPort
	:public AATemplate
{
public:
	onTransferSettlementPort(MemWork* mem, LPVOID addr, int ver);
	void SetOriginalCode(){}
	void SetNewCode();
private:
	LPVOID funcAddress;
};

class onCheckConstructionItem
	:public AATemplate
{
public:
	onCheckConstructionItem(MemWork* mem, LPVOID addr, int ver);
	void SetOriginalCode(){}
	void SetNewCode();
private:
	LPVOID funcAddress;
};

class onInitGsd
	:public AATemplate
{
public:
	onInitGsd(MemWork* mem, LPVOID addr, int ver);
	void SetOriginalCode(){}
	void SetNewCode();
private:
	LPVOID funcAddress;
};

class onInitGsd2
	:public AATemplate
{
public:
	onInitGsd2(MemWork* mem, LPVOID addr, int ver);
	void SetOriginalCode(){}
	void SetNewCode();
private:
	LPVOID funcAddress;
};

class onCreateTakeResidenceObjective
	:public AATemplate
{
public:
	onCreateTakeResidenceObjective(MemWork* mem, LPVOID addr, int ver);
	void SetOriginalCode(){}
	void SetNewCode();
private:
	LPVOID funcAddress;
};

class onAttachRegionSettlement
	:public AATemplate
{
public:
	onAttachRegionSettlement(MemWork* mem, LPVOID addr, int ver);
	void SetOriginalCode(){}
	void SetNewCode();
private:
	LPVOID funcAddress;
};

class onCalculateSettlement
	:public AATemplate
{
public:
	onCalculateSettlement(MemWork* mem, LPVOID addr, int ver);
	void SetOriginalCode(){}
	void SetNewCode();
private:
	LPVOID funcAddress;
};

class onScoreBestCapital
	:public AATemplate
{
public:
	onScoreBestCapital(MemWork* mem, LPVOID addr, int ver);
	void SetOriginalCode(){}
	void SetNewCode();
private:
	LPVOID funcAddress;
};

class onEvalAttObjective
	:public AATemplate
{
public:
	onEvalAttObjective(MemWork* mem, LPVOID addr, int ver);
	void SetOriginalCode(){}
	void SetNewCode();
private:
	LPVOID funcAddress;
};

class onEvalAttObjective2
	:public AATemplate
{
public:
	onEvalAttObjective2(MemWork* mem, LPVOID addr, int ver);
	void SetOriginalCode(){}
	void SetNewCode();
private:
	LPVOID funcAddress;
};

class onEvalAttObjective3
	:public AATemplate
{
public:
	onEvalAttObjective3(MemWork* mem, LPVOID addr, int ver);
	void SetOriginalCode(){}
	void SetNewCode();
private:
	LPVOID funcAddress;
};

class onEvalAttObjective4
	:public AATemplate
{
public:
	onEvalAttObjective4(MemWork* mem, LPVOID addr, int ver);
	void SetOriginalCode(){}
	void SetNewCode();
private:
	LPVOID funcAddress;
};

class onAssessRequiredStrength
	:public AATemplate
{
public:
	onAssessRequiredStrength(MemWork* mem, LPVOID addr, int ver);
	void SetOriginalCode(){}
	void SetNewCode();
private:
	LPVOID funcAddress;
};

class onCalcGarrisonStr
	:public AATemplate
{
public:
	onCalcGarrisonStr(MemWork* mem, LPVOID addr, int ver);
	void SetOriginalCode(){}
	void SetNewCode();
private:
	LPVOID funcAddress;
};

class onEvalAttObjective5
	:public AATemplate
{
public:
	onEvalAttObjective5(MemWork* mem, LPVOID addr, int ver);
	void SetOriginalCode(){}
	void SetNewCode();
private:
	LPVOID funcAddress;
};

class onPredictedStats
	:public AATemplate
{
public:
	onPredictedStats(MemWork* mem, LPVOID addr, int ver);
	void SetOriginalCode(){}
	void SetNewCode();
private:
	LPVOID funcAddress;
};

class onCalculateMiningIncome
	:public AATemplate
{
public:
	onCalculateMiningIncome(MemWork* mem, LPVOID addr, int ver);
	void SetOriginalCode(){}
	void SetNewCode();
private:
	LPVOID funcAddress;
};

class onGetUnitCard
	:public AATemplate
{
public:
	onGetUnitCard(MemWork* mem, LPVOID addr, int ver);
	void SetOriginalCode(){}
	void SetNewCode();
private:
	LPVOID funcAddress;
};

class onGetUnitInfoCard
	:public AATemplate
{
public:
	onGetUnitInfoCard(MemWork* mem, LPVOID addr, int ver);
	void SetOriginalCode(){}
	void SetNewCode();
private:
	LPVOID funcAddress;
};

class onUpdateControllerAlloc
	:public AATemplate
{
public:
	onUpdateControllerAlloc(MemWork* mem, LPVOID addr, int ver);
	void SetOriginalCode(){}
	void SetNewCode();
private:
	LPVOID funcAddress;
};

class onRegionGroupStuff
	:public AATemplate
{
public:
	onRegionGroupStuff(MemWork* mem, LPVOID addr, int ver);
	void SetOriginalCode(){}
	void SetNewCode();
private:
	LPVOID funcAddress;
};

class onGetSupportingArmies
	:public AATemplate
{
public:
	onGetSupportingArmies(MemWork* mem, LPVOID addr, int ver);
	void SetOriginalCode(){}
	void SetNewCode();
private:
	LPVOID funcAddress;
};

class onGetSupportingArmies2
	:public AATemplate
{
public:
	onGetSupportingArmies2(MemWork* mem, LPVOID addr, int ver);
	void SetOriginalCode(){}
	void SetNewCode();
private:
	LPVOID funcAddress;
};

class onDrawBanner
	:public AATemplate
{
public:
	onDrawBanner(MemWork* mem, LPVOID addr, int ver);
	void SetOriginalCode(){}
	void SetNewCode();
private:
	LPVOID funcAddress;
};

class onGetRebelSymbol
	:public AATemplate
{
public:
	onGetRebelSymbol(MemWork* mem, LPVOID addr, int ver);
	void SetOriginalCode(){}
	void SetNewCode();
private:
	LPVOID funcAddress;
};

class onSetupBattleFromStrat
	:public AATemplate
{
public:
	onSetupBattleFromStrat(MemWork* mem, LPVOID addr, int ver);
	void SetOriginalCode(){}
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



class OnCalculateUnitValue
	:public AATemplate
{
public:
	OnCalculateUnitValue(MemWork* mem, LPVOID addr, int ver);
	~OnCalculateUnitValue();

	void SetOriginalCode();
	void SetNewCode();
private:
	LPVOID funcAddress;
	DWORD otherFunc;
};

class onPopActionMem
	:public AATemplate
{
public:
	onPopActionMem(MemWork* mem, LPVOID addr, int ver);
	~onPopActionMem() = default;
	
	void SetNewCode();
private:
	LPVOID funcAddress;
};

class onWriteSoldiersToStrat
	:public AATemplate
{
public:
	onWriteSoldiersToStrat(MemWork* mem, LPVOID addr, int ver);
	~onWriteSoldiersToStrat() = default;
	
	void SetNewCode();
private:
	LPVOID funcAddress;
};

class onArmyDecimate
	:public AATemplate
{
public:
	onArmyDecimate(MemWork* mem, LPVOID addr, int ver);
	~onArmyDecimate() = default;
	
	void SetNewCode();
private:
	LPVOID funcAddress;
};

class onUnitMerge
	:public AATemplate
{
public:
	onUnitMerge(MemWork* mem, LPVOID addr, int ver);
	~onUnitMerge() = default;
	
	void SetNewCode();
private:
	LPVOID funcAddress;
};

class onSetSettlementModel
	:public AATemplate
{
public:
	onSetSettlementModel(MemWork* mem, LPVOID addr, int ver);
	~onSetSettlementModel() = default;
	
	void SetNewCode();
private:
	LPVOID funcAddress;
};

class onRemoveFromUnitQueue
	:public AATemplate
{
public:
	onRemoveFromUnitQueue(MemWork* mem, LPVOID addr, int ver);
	~onRemoveFromUnitQueue() = default;
	
	void SetNewCode();
private:
	LPVOID funcAddress;
};

class onAddSettlementToDiplomacyScroll
	:public AATemplate
{
public:
	onAddSettlementToDiplomacyScroll(MemWork* mem, LPVOID addr, int ver);
	~onAddSettlementToDiplomacyScroll() = default;
	
	void SetNewCode();
private:
	LPVOID funcAddress;
};

class onPartialConstructionPush
	:public AATemplate
{
public:
	onPartialConstructionPush(MemWork* mem, LPVOID addr, int ver);
	~onPartialConstructionPush() = default;
	
	void SetNewCode();
private:
	LPVOID funcAddress;
};

class onOffMapModelThing
	:public AATemplate
{
public:
	onOffMapModelThing(MemWork* mem, LPVOID addr, int ver);
	~onOffMapModelThing() = default;
	
	void SetNewCode();
private:
	LPVOID funcAddress;
};

class onOffMapModelThing2
	:public AATemplate
{
public:
	onOffMapModelThing2(MemWork* mem, LPVOID addr, int ver);
	~onOffMapModelThing2() = default;
	
	void SetNewCode();
private:
	LPVOID funcAddress;
};

class onAddBuilding
	:public AATemplate
{
public:
	onAddBuilding(MemWork* mem, LPVOID addr, int ver);
	~onAddBuilding() = default;
	
	void SetNewCode();
private:
	LPVOID funcAddress;
};

class onRemoveBuilding
	:public AATemplate
{
public:
	onRemoveBuilding(MemWork* mem, LPVOID addr, int ver);
	~onRemoveBuilding() = default;
	
	void SetNewCode();
private:
	LPVOID funcAddress;
};

class onGetBuildingById
	:public AATemplate
{
public:
	onGetBuildingById(MemWork* mem, LPVOID addr, int ver);
	~onGetBuildingById() = default;
	
	void SetNewCode();
private:
	LPVOID funcAddress;
};

class onConflictTest
	:public AATemplate
{
public:
	onConflictTest(MemWork* mem, LPVOID addr, int ver);
	~onConflictTest() = default;
	
	void SetNewCode();
private:
	LPVOID funcAddress;
};

class onConflictTest2
	:public AATemplate
{
public:
	onConflictTest2(MemWork* mem, LPVOID addr, int ver);
	~onConflictTest2() = default;
	
	void SetNewCode();
private:
	LPVOID funcAddress;
};

class onCheckBuildUpgrade
	:public AATemplate
{
public:
	onCheckBuildUpgrade(MemWork* mem, LPVOID addr, int ver);
	~onCheckBuildUpgrade() = default;
	
	void SetNewCode();
private:
	LPVOID funcAddress;
};

class onCheckBuildUpgrade2
	:public AATemplate
{
public:
	onCheckBuildUpgrade2(MemWork* mem, LPVOID addr, int ver);
	~onCheckBuildUpgrade2() = default;
	
	void SetNewCode();
private:
	LPVOID funcAddress;
};

class onCheckSettHasBuilding
	:public AATemplate
{
public:
	onCheckSettHasBuilding(MemWork* mem, LPVOID addr, int ver);
	~onCheckSettHasBuilding() = default;
	
	void SetNewCode();
private:
	LPVOID funcAddress;
};

class getPossibleConstructions
	:public AATemplate
{
public:
	getPossibleConstructions(MemWork* mem, LPVOID addr, int ver);
	~getPossibleConstructions() = default;
	
	void SetNewCode();
private:
	LPVOID funcAddress;
};

class onSetCqFlags
	:public AATemplate
{
public:
	onSetCqFlags(MemWork* mem, LPVOID addr, int ver);
	~onSetCqFlags() = default;
	
	void SetNewCode();
private:
	LPVOID funcAddress;
};

class onSetCqFlags2
	:public AATemplate
{
public:
	onSetCqFlags2(MemWork* mem, LPVOID addr, int ver);
	~onSetCqFlags2() = default;
	
	void SetNewCode();
private:
	LPVOID funcAddress;
};

class onRemoveCqFlags
	:public AATemplate
{
public:
	onRemoveCqFlags(MemWork* mem, LPVOID addr, int ver);
	~onRemoveCqFlags() = default;
	
	void SetNewCode();
private:
	LPVOID funcAddress;
};

class onRemoveCqFlags2
	:public AATemplate
{
public:
	onRemoveCqFlags2(MemWork* mem, LPVOID addr, int ver);
	~onRemoveCqFlags2() = default;
	
	void SetNewCode();
private:
	LPVOID funcAddress;
};

class onDamageCoreBuilding
	:public AATemplate
{
public:
	onDamageCoreBuilding(MemWork* mem, LPVOID addr, int ver);
	~onDamageCoreBuilding() = default;
	
	void SetNewCode();
private:
	LPVOID funcAddress;
};

class onDamageCoreBuilding2
	:public AATemplate
{
public:
	onDamageCoreBuilding2(MemWork* mem, LPVOID addr, int ver);
	~onDamageCoreBuilding2() = default;
	
	void SetNewCode();
private:
	LPVOID funcAddress;
};

class onDamageCoreBuilding3
	:public AATemplate
{
public:
	onDamageCoreBuilding3(MemWork* mem, LPVOID addr, int ver);
	~onDamageCoreBuilding3() = default;
	
	void SetNewCode();
private:
	LPVOID funcAddress;
};

class onGetWatchTowerRange
	:public AATemplate
{
public:
	onGetWatchTowerRange(MemWork* mem, LPVOID addr, int ver);
	~onGetWatchTowerRange() = default;
	
	void SetNewCode();
private:
	LPVOID funcAddress;
};

class onCheckGarrison
	:public AATemplate
{
public:
	onCheckGarrison(MemWork* mem, LPVOID addr, int ver);
	~onCheckGarrison() = default;
	
	void SetNewCode();
private:
	LPVOID funcAddress;
};

class onValidateGarrison
	:public AATemplate
{
public:
	onValidateGarrison(MemWork* mem, LPVOID addr, int ver);
	~onValidateGarrison() = default;
	
	void SetNewCode();
private:
	LPVOID funcAddress;
};

class onGeneralAssaultAction
	:public AATemplate
{
public:
	onGeneralAssaultAction(MemWork* mem, LPVOID addr, int ver);
	~onGeneralAssaultAction() = default;
	
	void SetNewCode();
private:
	LPVOID funcAddress;
};

class onAddBuildingCapsAfterConstruction
	:public AATemplate
{
public:
	onAddBuildingCapsAfterConstruction(MemWork* mem, LPVOID addr, int ver);
	~onAddBuildingCapsAfterConstruction() = default;
	
	void SetNewCode();
private:
	LPVOID funcAddress;
};

class onAddBuildingCapsAfterConstruction2
	:public AATemplate
{
public:
	onAddBuildingCapsAfterConstruction2(MemWork* mem, LPVOID addr, int ver);
	~onAddBuildingCapsAfterConstruction2() = default;
	
	void SetNewCode();
private:
	LPVOID funcAddress;
};

class onAddBuildingCapsAfterConstruction3
	:public AATemplate
{
public:
	onAddBuildingCapsAfterConstruction3(MemWork* mem, LPVOID addr, int ver);
	~onAddBuildingCapsAfterConstruction3() = default;
	
	void SetNewCode();
private:
	LPVOID funcAddress;
};

class onAddBuildingCapsAfterConstruction4
	:public AATemplate
{
public:
	onAddBuildingCapsAfterConstruction4(MemWork* mem, LPVOID addr, int ver);
	~onAddBuildingCapsAfterConstruction4() = default;
	
	void SetNewCode();
private:
	LPVOID funcAddress;
};

class onAddBuildingCapsAfterConstruction5
	:public AATemplate
{
public:
	onAddBuildingCapsAfterConstruction5(MemWork* mem, LPVOID addr, int ver);
	~onAddBuildingCapsAfterConstruction5() = default;
	
	void SetNewCode();
private:
	LPVOID funcAddress;
};

class onGetDefensesBuilding
	:public AATemplate
{
public:
	onGetDefensesBuilding(MemWork* mem, LPVOID addr, int ver);
	~onGetDefensesBuilding() = default;
	
	void SetNewCode();
private:
	LPVOID funcAddress;
};

class onSetKhakiText
	:public AATemplate
{
public:
	onSetKhakiText(MemWork* mem, LPVOID addr, int ver);
	~onSetKhakiText() = default;
	
	void SetNewCode();
private:
	LPVOID funcAddress;
};

class onSetKhakiText2
	:public AATemplate
{
public:
	onSetKhakiText2(MemWork* mem, LPVOID addr, int ver);
	~onSetKhakiText2() = default;
	
	void SetNewCode();
private:
	LPVOID funcAddress;
};

class onSetKhakiText3
	:public AATemplate
{
public:
	onSetKhakiText3(MemWork* mem, LPVOID addr, int ver);
	~onSetKhakiText3() = default;
	
	void SetNewCode();
private:
	LPVOID funcAddress;
};

class onSetKhakiText4
	:public AATemplate
{
public:
	onSetKhakiText4(MemWork* mem, LPVOID addr, int ver);
	~onSetKhakiText4() = default;
	
	void SetNewCode();
private:
	LPVOID funcAddress;
};

class onSetKhakiText5
	:public AATemplate
{
public:
	onSetKhakiText5(MemWork* mem, LPVOID addr, int ver);
	~onSetKhakiText5() = default;
	
	void SetNewCode();
private:
	LPVOID funcAddress;
};

class onSetRegionHiddenResources
	:public AATemplate
{
public:
	onSetRegionHiddenResources(MemWork* mem, LPVOID addr, int ver);
	~onSetRegionHiddenResources() = default;
	
	void SetNewCode();
private:
	LPVOID funcAddress;
};

class onSetRegionHiddenResources2
	:public AATemplate
{
public:
	onSetRegionHiddenResources2(MemWork* mem, LPVOID addr, int ver);
	~onSetRegionHiddenResources2() = default;
	
	void SetNewCode();
private:
	LPVOID funcAddress;
};

class onHiddenResourceConditionParse
	:public AATemplate
{
public:
	onHiddenResourceConditionParse(MemWork* mem, LPVOID addr, int ver);
	~onHiddenResourceConditionParse() = default;
	
	void SetNewCode();
private:
	LPVOID funcAddress;
};

class onHiddenResourceCheck
	:public AATemplate
{
public:
	onHiddenResourceCheck(MemWork* mem, LPVOID addr, int ver);
	~onHiddenResourceCheck() = default;
	
	void SetNewCode();
private:
	LPVOID funcAddress;
};

class onCreateMarriageOption
	:public AATemplate
{
public:
	onCreateMarriageOption(MemWork* mem, LPVOID addr, int ver);
	~onCreateMarriageOption() = default;
	
	void SetNewCode();
private:
	LPVOID funcAddress;
};

class onEnemyClicked
	:public AATemplate
{
public:
	onEnemyClicked(MemWork* mem, LPVOID addr, int ver);
	~onEnemyClicked() = default;
	
	void SetNewCode();
private:
	LPVOID funcAddress;
};

class onCampaignTick
	:public AATemplate
{
public:
	onCampaignTick(MemWork* mem, LPVOID addr, int ver);
	~onCampaignTick() = default;
	
	void SetNewCode();
private:
	LPVOID funcAddress;
};

class onBattleTick
	:public AATemplate
{
public:
	onBattleTick(MemWork* mem, LPVOID addr, int ver);
	~onBattleTick() = default;
	
	void SetNewCode();
private:
	LPVOID funcAddress;
};

class onCalculateLTGD
	:public AATemplate
{
public:
	onCalculateLTGD(MemWork* mem, LPVOID addr, int ver);
	~onCalculateLTGD() = default;
	
	void SetNewCode();
private:
	LPVOID funcAddress;
};

class onStartProductionTurn
	:public AATemplate
{
public:
	onStartProductionTurn(MemWork* mem, LPVOID addr, int ver);
	~onStartProductionTurn() = default;
	
	void SetNewCode();
private:
	LPVOID funcAddress;
};

class onSetBuildPolicies
	:public AATemplate
{
public:
	onSetBuildPolicies(MemWork* mem, LPVOID addr, int ver);
	~onSetBuildPolicies() = default;
	
	void SetNewCode();
private:
	LPVOID funcAddress;
};

class onUpdateProdControllers
	:public AATemplate
{
public:
	onUpdateProdControllers(MemWork* mem, LPVOID addr, int ver);
	~onUpdateProdControllers() = default;
	
	void SetNewCode();
private:
	LPVOID funcAddress;
};

class onSetProdPriorities
	:public AATemplate
{
public:
	onSetProdPriorities(MemWork* mem, LPVOID addr, int ver);
	~onSetProdPriorities() = default;
	
	void SetNewCode();
private:
	LPVOID funcAddress;
};

class onGetTrueBuildingCapabilities
	:public AATemplate
{
public:
	onGetTrueBuildingCapabilities(MemWork* mem, LPVOID addr, int ver);
	~onGetTrueBuildingCapabilities() = default;
	
	void SetNewCode();
private:
	LPVOID funcAddress;
};

class onCheckMountedEngineValid
	:public AATemplate
{
public:
	onCheckMountedEngineValid(MemWork* mem, LPVOID addr, int ver);
	~onCheckMountedEngineValid() = default;
	
	void SetNewCode();
private:
	LPVOID funcAddress;
};

class onGetMountedEngine
	:public AATemplate
{
public:
	onGetMountedEngine(MemWork* mem, LPVOID addr, int ver);
	~onGetMountedEngine() = default;
	
	void SetNewCode();
private:
	LPVOID funcAddress;
};

class onParseMountedEngines
	:public AATemplate
{
public:
	onParseMountedEngines(MemWork* mem, LPVOID addr, int ver);
	~onParseMountedEngines() = default;
	
	void SetNewCode();
private:
	LPVOID funcAddress;
};

class onParseEdu
	:public AATemplate
{
public:
	onParseEdu(MemWork* mem, LPVOID addr, int ver);
	~onParseEdu() = default;
	
	void SetNewCode();
private:
	LPVOID funcAddress;
};

class onApplyKillChance
	:public AATemplate
{
public:
	onApplyKillChance(MemWork* mem, LPVOID addr, int ver);
	~onApplyKillChance() = default;
	
	void SetNewCode();
private:
	LPVOID funcAddress;
};

class onRemoveKillChance
	:public AATemplate
{
public:
	onRemoveKillChance(MemWork* mem, LPVOID addr, int ver);
	~onRemoveKillChance() = default;
	
	void SetNewCode();
private:
	LPVOID funcAddress;
};

class onInitUnitBattle
	:public AATemplate
{
public:
	onInitUnitBattle(MemWork* mem, LPVOID addr, int ver);
	~onInitUnitBattle() = default;
	
	void SetNewCode();
private:
	LPVOID funcAddress;
};

class onCreateUnitKillChance
	:public AATemplate
{
public:
	onCreateUnitKillChance(MemWork* mem, LPVOID addr, int ver);
	~onCreateUnitKillChance() = default;
	
	void SetNewCode();
private:
	LPVOID funcAddress;
};

class onCreateMarriageOption2
	:public AATemplate
{
public:
	onCreateMarriageOption2(MemWork* mem, LPVOID addr, int ver);
	~onCreateMarriageOption2() = default;
	
	void SetNewCode();
private:
	LPVOID funcAddress;
};

class onCreateBirthOption
	:public AATemplate
{
public:
	onCreateBirthOption(MemWork* mem, LPVOID addr, int ver);
	~onCreateBirthOption() = default;
	
	void SetNewCode();
private:
	LPVOID funcAddress;
};

class onCreateBirthOption2
	:public AATemplate
{
public:
	onCreateBirthOption2(MemWork* mem, LPVOID addr, int ver);
	~onCreateBirthOption2() = default;
	
	void SetNewCode();
private:
	LPVOID funcAddress;
};

class onLoadModelRigid
	:public AATemplate
{
public:
	onLoadModelRigid(MemWork* mem, LPVOID addr, int ver);
	~onLoadModelRigid() = default;
	
	void SetNewCode();
private:
	LPVOID funcAddress;
};

class onUnloadModels
	:public AATemplate
{
public:
	onUnloadModels(MemWork* mem, LPVOID addr, int ver);
	~onUnloadModels() = default;
	
	void SetNewCode();
private:
	LPVOID funcAddress;
};

class onLoadBuilding
	:public AATemplate
{
public:
	onLoadBuilding(MemWork* mem, LPVOID addr, int ver);
	~onLoadBuilding() = default;
	
	void SetNewCode();
private:
	LPVOID funcAddress;
};

class onHiddenResourceCheck2
	:public AATemplate
{
public:
	onHiddenResourceCheck2(MemWork* mem, LPVOID addr, int ver);
	~onHiddenResourceCheck2() = default;
	
	void SetNewCode();
private:
	LPVOID funcAddress;
};

class onReadHiddenResources
	:public AATemplate
{
public:
	onReadHiddenResources(MemWork* mem, LPVOID addr, int ver);
	~onReadHiddenResources() = default;
	
	void SetNewCode();
private:
	LPVOID funcAddress;
};


class onIsBuildingTypePresentAtMinLevel
	:public AATemplate
{
public:
	onIsBuildingTypePresentAtMinLevel(MemWork* mem, LPVOID addr, int ver);
	~onIsBuildingTypePresentAtMinLevel() = default;
	
	void SetNewCode();
private:
	LPVOID funcAddress;
};

class onGetBuildingByIdConst
	:public AATemplate
{
public:
	onGetBuildingByIdConst(MemWork* mem, LPVOID addr, int ver);
	~onGetBuildingByIdConst() = default;
	
	void SetNewCode();
private:
	LPVOID funcAddress;
};

class onDecideMissionTarget
	:public AATemplate
{
public:
	onDecideMissionTarget(MemWork* mem, LPVOID addr, int ver);
	~onDecideMissionTarget() = default;
	
	void SetNewCode();
private:
	LPVOID funcAddress;
};

class onCanWithdrawPreBattle
	:public AATemplate
{
public:
	onCanWithdrawPreBattle(MemWork* mem, LPVOID addr, int ver);
	~onCanWithdrawPreBattle() = default;
	
	void SetNewCode();
private:
	LPVOID funcAddress;
};

class onCalculateCommand
	:public AATemplate
{
public:
	onCalculateCommand(MemWork* mem, LPVOID addr, int ver);
	~onCalculateCommand() = default;
	
	void SetNewCode();
private:
	LPVOID funcAddress;
};

class onSetArmyGeneralsUnit
	:public AATemplate
{
public:
	onSetArmyGeneralsUnit(MemWork* mem, LPVOID addr, int ver);
	~onSetArmyGeneralsUnit() = default;
	
	void SetNewCode();
private:
	LPVOID funcAddress;
};

class OnCalculateUnitValue2
	:public AATemplate
{
public:
	OnCalculateUnitValue2(MemWork* mem, LPVOID addr, int ver);
	~OnCalculateUnitValue2();

	void SetOriginalCode();
	void SetNewCode();
private:
	LPVOID funcAddress;
	DWORD otherFunc;
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

class OnCreateUnit2
	:public AATemplate
{
public:
	OnCreateUnit2(MemWork* mem, LPVOID addr, int ver);
	~OnCreateUnit2();

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

class OnUnitInfo
	:public AATemplate
{
public:
	OnUnitInfo(MemWork* mem, LPVOID addr, int ver);
	~OnUnitInfo();

	void SetOriginialCode();
	void SetNewCode();
private:
	LPVOID funcAddress;
};

class onEvaluateUnit
	:public AATemplate
{
public:
	onEvaluateUnit(MemWork* mem, LPVOID addr, int ver);
	~onEvaluateUnit();

	void SetOriginialCode();
	void SetNewCode();
private:
	LPVOID funcAddress;
};

class onEvaluateUnit2
	:public AATemplate
{
public:
	onEvaluateUnit2(MemWork* mem, LPVOID addr, int ver);
	~onEvaluateUnit2();

	void SetOriginialCode();
	void SetNewCode();
private:
	LPVOID funcAddress;
};

class OnReligionCombatBonus
	:public AATemplate
{
public:
	OnReligionCombatBonus(MemWork* mem, LPVOID addr, int ver);
	~OnReligionCombatBonus();

	void SetOriginialCode();
	void SetNewCode();
private:
	LPVOID funcAddress;
};

class OnBattleArmiesBug
	:public AATemplate
{
public:
	OnBattleArmiesBug(MemWork* mem, LPVOID addr, int ver);
	~OnBattleArmiesBug();

	void SetOriginialCode();
	void SetNewCode();
private:
	LPVOID funcAddress;
};


class onBattleRams
	:public AATemplate
{
public:
	onBattleRams(MemWork* mem, LPVOID addr, int ver);
	~onBattleRams();

	void SetOriginialCode();
	void SetNewCode();
private:
	LPVOID funcAddress;
};


class onPreBattlePlacement
	:public AATemplate
{
public:
	onPreBattlePlacement(MemWork* mem, LPVOID addr, int ver);
	~onPreBattlePlacement();

	void SetOriginialCode();
	void SetNewCode();
private:
	LPVOID funcAddress;
};


class onPreBattlePlacement2
	:public AATemplate
{
public:
	onPreBattlePlacement2(MemWork* mem, LPVOID addr, int ver);
	~onPreBattlePlacement2();

	void SetOriginialCode();
	void SetNewCode();
private:
	LPVOID funcAddress;
};


class onDecideRamAttacks
	:public AATemplate
{
public:
	onDecideRamAttacks(MemWork* mem, LPVOID addr, int ver);
	~onDecideRamAttacks();

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

class onNewGameLoaded
	:public AATemplate
{
public:
	onNewGameLoaded(MemWork* mem, LPVOID addr, int ver);
	~onNewGameLoaded();

	void SetOriginalCode();
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


//Faction Symbol Select
class toFactionSymbolSelect
	:public AATemplate
{
public:
	toFactionSymbolSelect(MemWork* mem, LPVOID adr, int ver);
	~toFactionSymbolSelect();

	void SetOriginalFactionSymbolSelect();
	void SetNewFactionSymbolSelect();
private:
	LPVOID funcAdress;
};










