#include "pch.h"
#include "templateCh.h"

#pragma region BasicClassTemplate

BasicClassTemplate::BasicClassTemplate() : m_memory(0), m_adress(0), m_state(0)
{
}

bool BasicClassTemplate::GetState()
{
	return m_state;
}

void BasicClassTemplate::SetState(bool newState)
{
	m_state = newState;
}
#pragma endregion

#pragma region ClassicTemplate


ClassicTemplate::ClassicTemplate(MemWork* mem)
	: m_originalBytes(0), m_cheatBytes(0), m_originalSize(0), m_cheatSize(0)
{
	m_memory = mem;
}

ClassicTemplate::~ClassicTemplate()
{
	if (m_originalBytes)
	{
		delete[] m_originalBytes;
		m_originalBytes = 0;
	}
	if (m_cheatBytes)
	{
		delete[] m_cheatBytes;
		m_cheatBytes = 0;
	}
}

void ClassicTemplate::Enable()
{
	if (!m_cheatBytes || !m_cheatSize || !m_adress)
		return;

	m_memory->Write(m_cheatBytes, m_adress, m_cheatSize);
	m_state = 1;
}

void ClassicTemplate::Disable()
{
	if (!m_originalBytes || !m_originalSize || !m_adress)
		return;

	m_memory->Write(m_originalBytes, m_adress, m_originalSize);
	m_state = 0;
}

void ClassicTemplate::SetOriginalBytes(unsigned char* bytes, UINT size)
{
	if (!bytes || !size)
		return;

	if (m_originalBytes)
	{
		delete[] m_originalBytes;
	}
	m_originalBytes = new unsigned char[size];

	memcpy(m_originalBytes, bytes, size);
	m_originalSize = size;
}

void ClassicTemplate::SetCheatBytes(unsigned char* bytes, UINT size)
{
	if (!bytes || !size)
		return;

	if (m_cheatBytes)
	{
		delete[] m_cheatBytes;
	}
	m_cheatBytes = new unsigned char[size];

	memcpy(m_cheatBytes, bytes, size);
	m_cheatSize = size;
}

void ClassicTemplate::GetOriginalBytes()
{
	if (!m_adress || !m_originalBytes)
		return;

	if (m_originalBytes)
	{
		delete[] m_originalBytes;
	}
	m_originalBytes = new unsigned char[m_originalSize];

	m_memory->Read(m_adress, m_originalSize);
	memcpy(m_originalBytes, m_memory->getBuffer(), m_originalSize);
}


#pragma endregion


#pragma region AATemplate



AATemplate::AATemplate(MemWork* mem) :ClassicTemplate(mem)
{
	m_memory = mem;
	m_codeCave = 0;
}


AATemplate::~AATemplate()
{
	if (m_cheatBytes)
	{
		VirtualFree((LPVOID)m_cheatBytes, 0, MEM_RELEASE);
		m_cheatBytes = 0;
	}
}



void AATemplate::Enable(bool addRet)
{
	if (!m_adress || !m_cheatBytes)
		return;
	if (!m_codeCave)
		AllocCodeCave();
	if (!m_originalBytes || !m_originalSize)
	{

		m_originalBytes = new unsigned char[20];
		m_memory->InjectCode(m_cheatBytes, m_adress, m_codeCave, m_originalBytes, (int*)&m_originalSize);
	}
	else
	{
		m_memory->InjectCode(m_cheatBytes, m_adress, m_codeCave, 0, 0);
	}

	if (addRet == true)
	{
		int size=m_memory->GetASMSize(m_cheatBytes);
		unsigned char buffer[1] = { 0xC3 };
		m_memory->Write(buffer, m_codeCave + size, 1);
	}
	m_state = 1;
}

void AATemplate::Disable()
{
	if (!m_adress || !m_originalBytes || !m_originalSize)
		return;
	m_memory->Write(m_originalBytes, m_adress, m_originalSize);
	if (m_codeCave)
		FreeCodeCave();

	m_state = 0;
}

void AATemplate::AllocCodeCave()
{
	m_codeCave = m_memory->Alloc(1024);
}

void AATemplate::FreeCodeCave()
{
	m_memory->Free(m_codeCave);
	m_codeCave = 0;
}


#pragma endregion