#include "smallFuncs.h"
#include "MemWork.h"
#include "dataOffsets.h"

#include "fastFunctsHelpers.h"

#include "utf8.h"
namespace smallFuncs
{
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
	NOINLINE EOP_EXPORT void createUniString(UNICODE_STRING** &newUniStringPointer, const char* nonUniStr)
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


		newUniStringPointer = (UNICODE_STRING**)new char[(lenS + 6) * 2];

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


	//	vector<unsigned short> utf16line;
		//utf8::utf8to16(utf8TempS.begin(), utf8TempS.end(), back_inserter(utf16line));
		unsigned short* ptr = (unsigned short*)&(*newUniStringPointer)->Buffer;

		for (int i = 0; i < utf16line.size(); i++)
		{

			*(ptr+i) = utf16line[i];
		}
		*(ptr + utf16line.size()) = (unsigned short)0;
	}

	NOINLINE EOP_EXPORT gameDataAllStruct* getGameDataAll()
	{
		gameDataAllStruct* retStruct = reinterpret_cast<gameDataAllStruct*>(dataOffsets::offsets.gameDataAllOffset);

		return retStruct;
	}
};
