#include "eduFastFuncts.h"

#include <sstream> 
namespace eduFastFuncts
{
	struct pseudoFile {
		char* _ptr;
		int _cnt;
		char* _base;
		int _flag;
		char* _file;
		char* _charbuf;
		int _bufsiz;
		char* _tmpfname;
	};

	static void prepareEduEntry(eduEntry*& entryForFilling)
	{
		int* ourInts = new int[8];
		memset(ourInts, 0, sizeof(int[8]));

		memset(entryForFilling, 0, sizeof(eduEntry));

		entryForFilling->N00000151 = ourInts;

		entryForFilling->N00000152 = &ourInts[4];

		entryForFilling->N00000153 = &ourInts[4];


		entryForFilling->Category = 0x6;
		entryForFilling->Class = 0x5;
		entryForFilling->categoryClassCombinationForAI = 0xB;
		entryForFilling->VoiceType = 0x3;


		entryForFilling->Trained = 0x3;
		entryForFilling->StatMentalDicipline = 0x5;

		entryForFilling->mountEffectClass = 0x3;
		entryForFilling->mountEffectClass2 = 0x3;
		entryForFilling->mountEffectClass3 = 0x3;

		entryForFilling->Mass = 1;
		entryForFilling->Width = (float)0.400000006;
		entryForFilling->Height = (float)1.700000048;


		entryForFilling->WeaponType = 0x5;
		entryForFilling->TechType = 0x7;
		entryForFilling->DamageType = 0x4;
		entryForFilling->SoundType = 0xA;

		entryForFilling->pad_016C[0] = 0xF;

		entryForFilling->SecWeaponType = 0x5;
		entryForFilling->SecTechType = 0x7;
		entryForFilling->SecDamageType = 0x4;

		entryForFilling->pad_019C[24] = 0xF;

		entryForFilling->pad_01BC[0] = 0xF;

		entryForFilling->EngineMissleWeaponType = 0x5;
		entryForFilling->EngineMissleTechType = 0x7;
		entryForFilling->EngineMissleDamageType = 0x4;
		entryForFilling->EngineMissleSoundType = 0xA;

		entryForFilling->pad_0258[24] = 0xF;


		entryForFilling->N00000112 = 0x5;
		entryForFilling->N00000113 = 0x7;
		entryForFilling->N00000114 = 0x4;
		entryForFilling->N00000115 = 0xA;

		entryForFilling->pad_02A0[24] = 0xF;

		entryForFilling->pad_02A0[32] = 0x6;


		entryForFilling->pad_02D0[20] = 0x5;
		entryForFilling->pad_02D0[24] = 0x7;
		entryForFilling->pad_02D0[28] = 0x4;
		entryForFilling->pad_02D0[32] = 0xA;
		entryForFilling->pad_02D0[64] = 0xF;

		entryForFilling->pad_0318[4] = 0x6;


		entryForFilling->pad_0328[24] = 0x5;
		entryForFilling->pad_0328[28] = 0x7;
		entryForFilling->pad_0328[32] = 0x4;
		entryForFilling->pad_0328[36] = 0xA;

		entryForFilling->pad_0328[68] = 0xF;

		entryForFilling->pad_0328[76] = 0x6;


		entryForFilling->Attributes6 = 0x20;
		entryForFilling->MoveSpeedMod = 0x1;

		entryForFilling->pad_03BC[20] = 0xF;
	}
	static std::string replace_all(
		const std::string& str,   // where to work
		const std::string& find,  // substitute 'find'
		const std::string& replace //      by 'replace'
	) {
		using namespace std;
		string result;
		size_t find_len = find.size();
		size_t pos, from = 0;
		while (string::npos != (pos = str.find(find, from))) {
			result.append(str, from, pos - from);
			result.append(replace);
			from = pos + find_len;
		}
		result.append(str, from, string::npos);
		return result;
	}


    int readEduFile(std::string fileName, eduEntry* entryForFilling)
    {
		std::string unitString;

		std::ifstream eduFile;
		eduFile.open(fileName);

		std::stringstream strStream;
		strStream << eduFile.rdbuf();
		unitString = strStream.str(); 
		eduFile.close();
		unitString=replace_all(unitString,"\n", " \n");
		prepareEduEntry(entryForFilling);

		pseudoFile ourF;
		ourF._ptr = (char*)unitString.c_str();
		ourF._cnt = unitString.size();
		ourF._base = (char*)(unitString.c_str()+ unitString.size());
		ourF._flag = (int)unitString.c_str();
		ourF._file = (char*)unitString.c_str();
		ourF._charbuf = (char*)unitString.c_str();
		ourF._bufsiz = unitString.size();
		ourF._tmpfname = (char*)&ourF;
		pseudoFile* pp = &ourF;

		int res = 0;

		DWORD adrFunc = codes::offsets.readEDUEntryFunc;
		_asm
		{
			push pp
			mov ecx, entryForFilling
			mov eax, adrFunc
			call eax
			mov res,eax
		}
        return res;
    }
};
