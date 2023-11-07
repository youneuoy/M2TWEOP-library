#pragma once
#include<stdint.h>
#include<vector>

#include "MemWork.h"
class OverridedBattleCameraPatch
{
public:
	OverridedBattleCameraPatch(uint32_t address)
	{
		Address = address;
		DISASM MyDisasm = { 0 };
		MyDisasm.EIP = (UIntPtr)Address;
		int currentlen = Disasm(&MyDisasm);

		OriginalCode.resize(currentlen);
		MemWork::ReadData(Address, OriginalCode.data(), currentlen);
		for (int i = 0; i < currentlen; ++i)
		{
			PatchedCode.push_back(0x90);
		}
	}
	void UpdateState(bool isDisable)
	{
		if (isDisable == true)
		{
			Disable();
		}
		else
		{
			Enable();
		}
	}
private:
	void Enable()
	{
		if (IsEnabled == true)
		{
			return;
		}
		IsEnabled = true;

		MemWork::WriteData(PatchedCode.data(), Address, PatchedCode.size());
	}
	void Disable()
	{
		if (IsEnabled == false)
		{
			return;
		}
		IsEnabled = false;
		MemWork::WriteData(OriginalCode.data(), Address, OriginalCode.size());
	}
	bool IsEnabled = false;
	uint32_t Address = 0x0;
	std::vector<uint8_t> OriginalCode;
	std::vector<uint8_t> PatchedCode;
};	
class OverridedBattleCameraPatcher
{
public:
	void UpdateState(bool isDisable);
	void Init(int gameVersion);
private:
	std::vector<OverridedBattleCameraPatch> Patches;
};

