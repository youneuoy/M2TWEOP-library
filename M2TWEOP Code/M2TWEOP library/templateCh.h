#pragma once
#include "memWork.h"
#include <Windows.h>
using namespace std;

//template
class BasicClassTemplate
{
public:
	BasicClassTemplate();
	virtual ~BasicClassTemplate() {};
	//on/off
	virtual void Enable() {};
	virtual void Disable() {};

	bool GetState();
	void SetState(bool newState);
protected:
	MemWork* m_memory;
	DWORD	 m_adress;
	bool	 m_state : 1;

};
//without code injections
class ClassicTemplate
	: public BasicClassTemplate
{
public:
	ClassicTemplate(MemWork* mem);
	virtual ~ClassicTemplate();

	void Enable();
	void Disable();

	void SetOriginalBytes(unsigned char* bytes, UINT size);
	void SetCheatBytes(unsigned char* bytes, UINT size);

	void GetOriginalBytes();

	unsigned char* m_originalBytes;
	UINT  m_originalSize;

	unsigned char* m_cheatBytes;
	UINT  m_cheatSize;
};

//with code injects
class AATemplate
	:public ClassicTemplate
{
public:
	AATemplate(MemWork* mem);
	virtual ~AATemplate();

	void Enable(bool addRet=false);
	void Disable();
protected:
	void AllocCodeCave();
	void FreeCodeCave();
	DWORD m_codeCave;
};