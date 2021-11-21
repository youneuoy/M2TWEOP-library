#pragma once

#include "headersSTD.h"
#include "asmjit\asmjit.h"
#include "memHelpers.h"
using namespace asmjit;
using namespace asmjit::x86;
///
/// DO NOT MAKE INJECTS INTO CALL/JMP!
///

typedef void* codePatchID;

enum codePatchFlags_ :int
{
	codePatchFlag_none = 0,
	codePatchFlag_insertOriginalCodeAtStart = 1 << 0,
	codePatchFlag_insertOriginalCodeAtEnd = 1 << 1
};

enum codePatchStatus_:int
{
	codePatchStatus_originalCode,
	codePatchStatus_patchedCode,
	codePatchStatus_codeNotSetted
};
typedef int codePatchFlags;


class codePatch
{
public:

	codePatch(void* adress, codePatchFlags flags= codePatchFlag_none,int numOfOrigBytes=5);

	Assembler* prepareForCoding(JitRuntime& rt);
	//assembler detached here
	void createCode(JitRuntime& rt);

	void setPatchedCode(JitRuntime& rt);
	void setOriginalCode(JitRuntime& rt);
	
	void destroy(JitRuntime&rt);

	codePatchStatus_ getState();
private:
	codePatchStatus_ state = codePatchStatus_codeNotSetted;

	codePatchFlags flags;

	//used for creating code only
	Assembler* assebmler = nullptr;
	CodeHolder* patchedCode = nullptr;

	//used for store created code.
	void* createdFunc=nullptr;


	//pointer to inject place. Initialized in constructor.
	const void* originalAdress;
	//original bytes. Readed in constructor.
	std::vector<uint8_t>originalCode;
};
class patchesWorker
{
public:
	patchesWorker();
	~patchesWorker();

	Assembler* prepareForCoding(codePatchID patch);
	void createCode(codePatchID patch);
	void setPatchedCode(codePatchID patch);
	void setOriginalCode(codePatchID patch);

	codePatchStatus_ getPatchState(codePatchID patch);


	codePatchID getPatch(const char* patchName);
	codePatchID addPatch(const char*name,void* adress, codePatchFlags flags, int numOfOrigBytes = 5);
	void deletePatch(const char* patchName);
private:
	struct
	{
		struct charCmp {
			bool operator () (const char* a, const char* b) const
			{
				return strcmp(a, b) < 0;
			}
		};
		std::map<const char*, codePatch, charCmp> patches;
	}data;


	JitRuntime rt;
};

