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
