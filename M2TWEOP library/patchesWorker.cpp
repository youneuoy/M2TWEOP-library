#include "patchesWorker.h"

patchesWorker::patchesWorker()
{
}

patchesWorker::~patchesWorker()
{
	for (auto& patch:data.patches)
	{
		patch.second.destroy(rt);
	}
}

Assembler* patchesWorker::prepareForCoding(codePatchID patch)
{
	codePatch* cp = reinterpret_cast<codePatch*>(patch);

	return cp->prepareForCoding(rt);
}

void patchesWorker::createCode(codePatchID patch)
{
	codePatch* cp = reinterpret_cast<codePatch*>(patch);

	cp->createCode(rt);
}

void patchesWorker::setPatchedCode(codePatchID patch)
{
	codePatch* cp = reinterpret_cast<codePatch*>(patch);
	cp->setPatchedCode(rt);
}

void patchesWorker::setOriginalCode(codePatchID patch)
{
	codePatch* cp = reinterpret_cast<codePatch*>(patch);
	
	cp->setOriginalCode(rt);
}
codePatchStatus_ patchesWorker::getPatchState(codePatchID patch)
{
	codePatch* cp = reinterpret_cast<codePatch*>(patch);
	
	return cp->getState();
}

codePatchID patchesWorker::getPatch(const char* patchName)
{
	auto patch = data.patches.find(patchName);

	if (patch == data.patches.end())
	{
		return nullptr;
	}
	return &patch->second;
}

codePatchID patchesWorker::addPatch(const char* name, void* adress, codePatchFlags flags, int numOfOrigBytes)
{
	//create patch, put it to map and get ugly iterator for result checking
	auto patch=data.patches.emplace(name, codePatch(adress, flags, numOfOrigBytes));

	if (patch.second == false)
	{
		return nullptr;
	}
	return &patch.first->second;
}

void patchesWorker::deletePatch(const char* patchName)
{
	codePatchID patch = getPatch(patchName);
	if (patch == nullptr)
	{
		return;
	}
	codePatch* cp = reinterpret_cast<codePatch*>(patch);
	cp->destroy(rt);

	data.patches.erase(patchName);
}



codePatch::codePatch(void* adress, codePatchFlags flags, int numOfOrigBytes)
{
	this->flags = flags;
	originalAdress = adress;
	memHelpers::getInstructionsFromMem(adress, originalCode, numOfOrigBytes);
}

Assembler* codePatch::prepareForCoding(JitRuntime& rt)
{
	if (state != codePatchStatus_codeNotSetted)
	{
		return nullptr;
	}
	patchedCode = new CodeHolder();
	assebmler = new Assembler();
	patchedCode->init(rt.environment());

	patchedCode->attach(assebmler);

	if (flags & codePatchFlag_insertOriginalCodeAtStart)
	{
		assebmler->embed(originalCode.data(), originalCode.size());
	}
	return assebmler;
}

void codePatch::createCode(JitRuntime& rt)
{
	if (state != codePatchStatus_codeNotSetted)
	{
		return;
	}

	if (flags & codePatchFlag_insertOriginalCodeAtEnd)
	{
		assebmler->embed(originalCode.data(), originalCode.size());
	}
	assebmler->jmp((uint64_t)originalAdress+ originalCode.size());
	patchedCode->detach(assebmler);

	rt.add(&createdFunc, patchedCode);
	delete patchedCode;
	delete assebmler;
	patchedCode = nullptr;
	assebmler = nullptr;

	state = codePatchStatus_originalCode;
}

void codePatch::setPatchedCode(JitRuntime& rt)
{
	CodeHolder code;

	code.init(rt.environment());
	Assembler a(&code);
	code.relocateToBase((uint64_t)originalAdress);

	a.jmp(createdFunc);


	void* tempCode = nullptr;

	memHelpers::nopBytes((void*)originalAdress, originalCode.size());
	rt.add(&tempCode, &code);

	memHelpers::writeToMem(tempCode, (void*)originalAdress,5);

	rt.release(tempCode);

	state = codePatchStatus_patchedCode;
}

void codePatch::setOriginalCode(JitRuntime& rt)
{
	memHelpers::writeToMem(originalCode.data(), (void*)originalAdress, originalCode.size());
	state = codePatchStatus_originalCode;
}


void codePatch::destroy(JitRuntime& rt)
{
	delete patchedCode;
	delete assebmler;

	patchedCode = nullptr;
	assebmler = nullptr;
	if (createdFunc != nullptr)
	{
		rt.release(createdFunc);
	}
}

codePatchStatus_ codePatch::getState()
{
	return state;
}

