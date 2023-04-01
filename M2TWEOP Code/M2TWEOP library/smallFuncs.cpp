#include "smallFuncs.h"
#include "MemWork.h"
#include "dataOffsets.h"

#include "fastFunctsHelpers.h"
#include "MasterDefines.h"


namespace smallFuncs
{
	void* GetMainStratObject(void* baseObj)
	{
		if (baseObj == nullptr)
		{
			return baseObj;
		}

		StartMapObjectType objT = CallVFunc<4, StartMapObjectType>(baseObj);
		switch (objT)
		{
		case StartMapObjectType::FloatingGeneral:
			break;
		case StartMapObjectType::Settlement:
			break;
		case StartMapObjectType::Fort:
			break;
		case StartMapObjectType::Port:
			break;
		case StartMapObjectType::Character:
			break;
		case StartMapObjectType::RallyPointSundry:
		{
			RallyPointSundry* ral = (RallyPointSundry*)baseObj;
			if (ral->object == nullptr)
			{
				break;
			}
			return GetMainStratObject(ral->object);

			break;
		}
		default:
			break;
		}
		return baseObj;
	}
	NOINLINE EOP_EXPORT void setAncLimit(unsigned char limit)
	{

		DWORD ancillariesOffset = 0;
		if (globals::dataS.gamever == 2)//steam
		{
			ancillariesOffset = 0x005A5CAD;
		}
		else
		{
			ancillariesOffset = 0x005A57CD;
		}

		MemWork::WriteData(&limit, ancillariesOffset, 1);
	}

	NOINLINE EOP_EXPORT int getGameVersion()
	{

		if (globals::dataS.gamever == 2)//steam
		{
			return 2;
		}
		else
		{
			return 1;
		}

	}

	NOINLINE EOP_EXPORT void saveGame(const char* path)
	{
		DWORD codeOffset = 0;
		DWORD codeOffset2 = 0;
		if (globals::dataS.gamever == 2)//steam
		{
			codeOffset = 0x004350e0;
			codeOffset2 = 0x02C1B904;
		}
		else
		{
			codeOffset = 0x00434da0;
			codeOffset2 = 0x02C649BC;
		}

		UNICODE_STRING** uni = new UNICODE_STRING*;

		smallFuncs::createUniString(uni, path);

		UNICODE_STRING*** puni = &uni;

		_asm
		{
			push 0
			push puni
			mov ecx, codeOffset2
			mov eax, codeOffset
			call eax
		}

	};


	NOINLINE EOP_EXPORT void setEDUUnitsSize(signed short min, signed short max)
	{
		DWORD codeOffset = 0;
		if (globals::dataS.gamever == 2)//steam
		{
			codeOffset = 0x008efe01;
		}
		else
		{
			codeOffset = 0x008ef381;
		}

		codeOffset += 0x82C;

		MemWork::WriteData(&min, codeOffset, 2);

		codeOffset += 6;
		MemWork::WriteData(&max, codeOffset, 2);


		return;
	}
	NOINLINE EOP_EXPORT void setMaxBgSize(unsigned char size)
	{
		DWORD cmpAdr = 0;
		DWORD retAdr = 0;
		if (globals::dataS.gamever == 2)//steam
		{
			cmpAdr = 0x008ec815 + 2;
			retAdr = 0x008ec81a + 1;
		}
		else
		{
			cmpAdr = 0x008ebd95 + 2;
			retAdr = 0x008ebd9a + 1;
		}
		MemWork::WriteData(&size, cmpAdr, 1);
		MemWork::WriteData(&size, retAdr, 1);
	}
	NOINLINE EOP_EXPORT void unlockConsoleCommands()
	{
		uchar nops[2] = { 0x90,0x90 };
		DWORD cmd = 0;

		if (globals::dataS.gamever == 2)//steam
		{
			cmd = 0x00d27ae3;
		}
		else
		{
			cmd = 0x00d2d763;
		}


		//check checking code and change all jmps to nops
		for (int i = 0; i < 53; i++, cmd++)
		{
			uchar jz = 0x74;
			uchar ch;
			MemWork::ReadData(cmd, &ch, 1);
			if (ch == jz)
			{
				MemWork::WriteData(nops, cmd, 2);
			}
		}


		//unlock change_faction
		uchar nops1[6] = { 0x90,0x90,0x90,0x90,0x90,0x90 };
		if (globals::dataS.gamever == 2)//steam
		{
			cmd = 0x00d2cd2a;
		}
		else
		{
			cmd = 0x00d329aa;
		}

		MemWork::WriteData(nops1, cmd, 6);
	}
	NOINLINE EOP_EXPORT int getBattleCondCode(DWORD condObject)
	{
		if (condObject == 0)
		{
			return -1;
		}

		DWORD* objectPtr = reinterpret_cast<DWORD*>(condObject);
		DWORD vtablePtr = *objectPtr;
		if (globals::dataS.gamever == 2)//steam
		{
			switch (vtablePtr)
			{
			case 0x012ed814: return 0; break;//destroy_or_rout_enemy
			case 0x012fe30c: return 1; break;//balance_of_strength_percent
			case 0x012fe334: return 2; break;//destroy_enemy_strength_percent
			case 0x012fe2bc: return 3; break;//capture_location
			case 0x12fe9c4: return 4; break;//destroy_character
			case 0x012ed7ec: return 5; break;//capture_major_settlement
			case 0x012fe2e4: return 6; break;//capture_army_settlement
			}
		}
		else//disk version
		{
			switch (vtablePtr)
			{
			case 0x01332834: return 0; break;//destroy_or_rout_enemy
			case 0x0134332c: return 1; break;//balance_of_strength_percent
			case 0x01343354: return 2; break;//destroy_enemy_strength_percent
			case 0x013432dc: return 3; break;//capture_location
			case 0x013439e4: return 4; break;//destroy_character
			case 0x0133280c: return 5; break;//capture_major_settlement
			case 0x01343304: return 6; break;//capture_army_settlement
			}
		}


		return -1;
	}
	std::string getWinConditionS(DWORD condObject)
	{
		int conditionCode = getBattleCondCode(condObject);
		switch (conditionCode)
		{
		case 0: return "destroy_or_rout_enemy"; break;
		case 1: return "balance_of_strength_percent"; break;
		case 2: return "destroy_enemy_strength_percent"; break;
		case 3: return "capture_location"; break;
		case 4: return "destroy_character"; break;
		case 5: return "capture_major_settlement"; break;
		case 6: return "capture_army_settlement"; break;
		default: return "unknown_condition"; break;
		}
	}
	NOINLINE EOP_EXPORT void createUniString(UNICODE_STRING**& newUniStringPointer, const char* nonUniStr)
	{
		DWORD funcAdr = 0;
		if (globals::dataS.gamever == 2)//steam
		{
			funcAdr = 0x00f01eb0;
		}
		else
		{
			funcAdr = 0x00f018e0;
		}



		size_t lenS = strlen(nonUniStr);


		newUniStringPointer = (UNICODE_STRING**)new char[(lenS + 6 + 4) * 2];

		UNICODE_STRING** uniStr = newUniStringPointer;
		_asm {
			mov ecx, uniStr
			push nonUniStr
			mov eax, funcAdr
			call eax
		}

		newUniStringPointer = (UNICODE_STRING**)*uniStr;


		//string utf8TempS = nonUniStr;



		int wchars_num = MultiByteToWideChar(CP_UTF8, 0, nonUniStr, -1, NULL, 0);
		wchar_t* wstr = new wchar_t[wchars_num];
		MultiByteToWideChar(CP_UTF8, 0, nonUniStr, -1, wstr, wchars_num);

		wstring utf16line = wstr;
		// do whatever with wstr
		delete[] wstr;

		(*newUniStringPointer)->Length = (USHORT)utf16line.size();
		(*newUniStringPointer)->something2 = (USHORT)utf16line.size();

		unsigned short* ptr = (unsigned short*)&(*newUniStringPointer)->Buffer;

		for (UINT32 i = 0; i < utf16line.size(); i++)
		{

			*(ptr + i) = utf16line[i];
		}
		*(ptr + utf16line.size()) = (unsigned short)0;
	}

	NOINLINE EOP_EXPORT gameDataAllStruct* getGameDataAll()
	{
		gameDataAllStruct* retStruct = reinterpret_cast<gameDataAllStruct*>(dataOffsets::offsets.gameDataAllOffset);

		return retStruct;
	}

	NOINLINE EOP_EXPORT void changeSettlementName(settlementStruct* sett, const char* newName)
	{

		UNICODE_STRING** nameMem = new UNICODE_STRING*;
		sett->localizedName = nameMem;

		smallFuncs::createUniString(sett->localizedName, newName);
	}

	NOINLINE EOP_EXPORT void changeFactionName(factionStruct* fac, const char* newName)
	{

		UNICODE_STRING** nameMem = new UNICODE_STRING*;
		fac->localizedName = nameMem;

		smallFuncs::createUniString(fac->localizedName, newName);
	}

	NOINLINE EOP_EXPORT UNICODE_STRING** getFactionName(factionStruct* fac)
	{

		std::string facname = fac->factSmDescr->facName;
		std::transform(facname.begin(), facname.end(), facname.begin(), ::toupper);
		UNICODE_STRING** nameMem = new UNICODE_STRING*;
		smallFuncs::createUniString(nameMem, facname.c_str());
		UNICODE_STRING*** nameMem2 = &nameMem;
		DWORD funcAddr = codes::offsets.getStringFromTable;
		DWORD stringTable = dataOffsets::offsets.stringTable;
		_asm
		{
			push nameMem2
			mov ecx, stringTable
			mov ecx, [ecx]
			mov eax, funcAddr
			call eax
			mov nameMem2, eax
		}

		return *nameMem2;
	}

	NOINLINE EOP_EXPORT void historicEvent(const char* name, const char* title, const char* description)
	{
		DWORD funcAddr = codes::offsets.historicEventFunc;

		UNICODE_STRING** titleUni = new UNICODE_STRING*;
		smallFuncs::createUniString(titleUni, title);

		UNICODE_STRING** bodyUni = new UNICODE_STRING*;
		smallFuncs::createUniString(bodyUni, description);

		UNICODE_STRING*** titleUnip = &titleUni;
		UNICODE_STRING*** bodyUnip = &bodyUni;


		_asm
		{
			push 0x3FFFFFFF
			push 0x0
			push 0x0
			push 0xFFFFFFFF
			push 0xFFFFFFFF
			push name
			push bodyUnip
			push titleUnip
			mov eax, funcAddr
			call eax
		}

	}

	DWORD getScriptCommandByName(const char* cmdName)
	{
		DWORD func1 = codes::offsets.scriptCommandOne;
		DWORD func2 = codes::offsets.scriptCommandTwo;
		DWORD result = 0x0;
		DWORD cmdNamePtr = (DWORD)&cmdName;

		_asm {
			mov eax, func1
			call eax
			mov ecx, eax
			push cmdNamePtr
			mov eax, func2
			call eax
			mov result, eax
		}
		return result;
	}

	struct fakeTextInput
	{
	public:
		char* textBuffer; //0x0000
		uint32_t byteSize; //0x0004
		char* endString; //0x0008
		struct UNICODE_STRING** unicodePointerPointer; //0x000C
		char* currRead; //0x0010
		char* currLine; //0x0014
		uint32_t lineNumber; //0x0018
		int32_t N1814981889; //0x001C
		int32_t N0; //0x0020
		DWORD classPointer; //0x0024
	}; //Size: 0x0028

	NOINLINE EOP_EXPORT void scriptCommand(const char* command, const char* args)
	{
		DWORD scriptClass = getScriptCommandByName(command);
		if (scriptClass == 0x0)
		{
			return;
		}
		char* fullCommand = new char[strlen(command) + strlen(args) + 2];
		strcpy(fullCommand, command);
		strcat(fullCommand, " ");
		strcat(fullCommand, args);
		fakeTextInput* fakeText = new fakeTextInput;
		std::string scriptPath = "data/world/maps/campaign/imperial_campaign/campaign_script.txt";
		fakeText->unicodePointerPointer = new UNICODE_STRING*;
		smallFuncs::createUniString(fakeText->unicodePointerPointer, scriptPath.c_str());
		fakeText->textBuffer = fullCommand;
		fakeText->byteSize = strlen(command) + (int8_t)0x4;
		size_t len = strlen(fullCommand);
		char* endAddress = fullCommand + len;
		fakeText->endString = endAddress;
		fakeText->currRead = fullCommand;
		fakeText->currLine = fullCommand;
		fakeText->lineNumber = 1;
		fakeText->N1814981889 = 1814981889;
		fakeText->N0 = 0;
		DWORD classPointer = 0x0;
		_asm
		{
			mov eax, scriptClass
			mov eax, [eax]
			mov classPointer, eax
		}
		fakeText->classPointer = classPointer;
		DWORD funcAddr = scriptClass + (int8_t)0x4;
		DWORD scriptObject = 0x0;
		_asm
		{
			push fakeText
			mov eax, funcAddr
			mov eax, [eax]
			call eax
			mov scriptObject, eax
			add esp, 0x4
		}

		if (scriptObject == 0x0)
		{
			return;
		}

		_asm
		{
			mov ecx, scriptObject
			mov eax, [ecx]
			mov eax, [eax]
			call eax
		}
	}

	NOINLINE EOP_EXPORT void changeRegionName(regionStruct* region, const char* newName)
	{

		UNICODE_STRING** nameMem = new UNICODE_STRING*;
		region->localizedRegionName = nameMem;

		smallFuncs::createUniString(region->localizedRegionName, newName);
	}

	NOINLINE EOP_EXPORT void changeRebelsName(regionStruct* region, const char* newName)
	{

		UNICODE_STRING** nameMem = new UNICODE_STRING*;
		region->localizedRebelsName = nameMem;

		smallFuncs::createUniString(region->localizedRebelsName, newName);
	}

	bool highlightOn = false;
	NOINLINE EOP_EXPORT void swUnBMapHighlight()
	{
		static unsigned char highLightOn = 0x8f;
		static unsigned char highLightOff = 0x8e;

		DWORD codeAdr = 0;
		if (globals::dataS.gamever == 2)//steam
		{
			codeAdr = 0x009e1a64;
		}
		else
		{
			codeAdr = 0x009e0ec4;
		}

		if (!highlightOn)
		{
			MemWork::WriteData(&highLightOn, codeAdr + 0x1, 1);
			highlightOn = true;
		}
		else
		{
			MemWork::WriteData(&highLightOff, codeAdr + 0x1, 1);
			highlightOn = false;
		}


		return;
	}
	NOINLINE EOP_EXPORT void setReligionsLimit(unsigned char limit)
	{
		DWORD codeAdr = 0;
		if (globals::dataS.gamever == 2)//steam
		{
			codeAdr = 0x0411E0E;
		}
		else
		{
			codeAdr = 0x0411C0E;
		}

		MemWork::WriteData(&limit, codeAdr, 1);

		return;
	}
	NOINLINE EOP_EXPORT bool isTileFree(int* xy)
	{
		DWORD funcAdr = 0;
		bool retZ = false;
		if (globals::dataS.gamever == 2)//steam
		{
			funcAdr = 0x004c9220;
		}
		else
		{
			funcAdr = 0x004c8c70;
		}

		_asm {
			push xy
			mov eax, funcAdr
			call eax
			mov retZ, al
		}

		return retZ;
	}
	struct counterS
	{
		counterS* something1;
		counterS* something2;
		counterS* something3;
		char* counterName;
		int nameCrypt;
		int counterValue;
	};
	struct countersObjectS
	{
		void* something;
		void* testCounterSValue;
	};
	NOINLINE EOP_EXPORT int getScriptCounter(const char* counterName, bool& isFinded)
	{
		countersObjectS* eventsObject = 0;
		counterS* retS = 0;
		int retValue = 0;
		if (globals::dataS.gamever == 2)//steam
		{
			eventsObject = (countersObjectS*)0x016A7A58;
		}
		else
		{
			eventsObject = (countersObjectS*)0x016F0BF0;
		}
		DWORD funcAdr = 0;

		if (globals::dataS.gamever == 2)//steam
		{
			funcAdr = 0x00489760;
		}
		else
		{
			funcAdr = 0x00489360;
		}

		char** cryptS = fastFunctsHelpers::makeCryptedString(counterName);
		_asm {
			push cryptS
			mov ecx, eventsObject
			mov eax, funcAdr
			call eax
			mov retS, eax
		}


		if (retS == eventsObject->testCounterSValue)
		{
			isFinded = false;
		}
		else
		{

			if (retS != nullptr)
			{
				if (retS->nameCrypt == (int)cryptS[1])
				{
					isFinded = true;
					return retS->counterValue;
				}
				else isFinded = false;
			}
		}


		return 0;
	}
	NOINLINE EOP_EXPORT void setScriptCounter(const char* counterName, int counterValue)
	{
		DWORD eventsObject;
		if (globals::dataS.gamever == 2)//steam
		{
			eventsObject = 0x016A7A30;
		}
		else
		{
			eventsObject = 0x16F0BC8;
		}

		DWORD funcAdr = 0;


		if (globals::dataS.gamever == 2)//steam
		{
			funcAdr = 0x0048cce0;
		}
		else
		{
			funcAdr = 0x0048c8e0;
		}

		char** cryptS = fastFunctsHelpers::makeCryptedString(counterName);
		_asm {
			push counterValue
			push cryptS
			mov ecx, eventsObject
			mov eax, funcAdr
			call eax
		}


		return;
	}

	NOINLINE EOP_EXPORT void setBuildingChainLimit(unsigned int limit)
	{
		DWORD codeAdr = 0;

		if (globals::dataS.gamever == 2)//steam
		{
			codeAdr = 0x008AC174;
		}
		else
		{
			codeAdr = 0x008AB794;
		}


		MemWork::WriteData(&limit, codeAdr, 4);
		return;
	}

	NOINLINE EOP_EXPORT void setGuildCooldown(unsigned char turns)
	{
		DWORD codeAdr = 0;

		if (globals::dataS.gamever == 2)//steam
		{
			codeAdr = 0x004F5D7B;
		}
		else
		{
			codeAdr = 0x004F57AB;
		}

		MemWork::WriteData(&turns, codeAdr, 1);

	}

	NOINLINE EOP_EXPORT int GetUnitSize()
	{
		return *dataOffsets::offsets.gameUnit_size;
	}

	float GetMinimumPossibleMovepointsForArmy(stackStruct* army)
	{
		if (army == nullptr)
		{
			return 0;
		}

		typedef float (__thiscall* GetUnitFullMovePointsF)(unit* un);

		GetUnitFullMovePointsF getUnitFullMovePointsF = nullptr;
		if (globals::dataS.gamever == 2)//steam
		{
			getUnitFullMovePointsF = (GetUnitFullMovePointsF)0x00742b10;
		}
		else
		{
			getUnitFullMovePointsF = (GetUnitFullMovePointsF)0x00742380;
		}
		if (army->numOfUnits < 1)
		{
			return 0;
		}
		float minMp = getUnitFullMovePointsF(army->units[0]);

		for (int i = 1; i < army->numOfUnits; ++i)
		{
			float unitFullMp = getUnitFullMovePointsF(army->units[i]);
			if (unitFullMp < minMp)
			{
				minMp = unitFullMp;
			}
		}
		return minMp;
	}

	float GetDistanceInTiles(int x, int y, int destX, int destY)
	{
		int dx = x - destX;
		int dy = y - destY;
		return (float)sqrt((double)(dx * dx) + (double)(dy * dy));
	}

};
