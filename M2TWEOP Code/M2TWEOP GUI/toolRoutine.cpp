#include "toolRoutine.h"
#include "mainUI.h"
#include "redistUI.h"

#include "helpers.h"

namespace toolRoutine
{
	void tryJustStartMod()
	{
		mainUI::tryJustStartMod();
	}
	int __stdcall drawTick(bool* isOpen)
	{
		helpers::updateMetrics();

		return mainUI::draw(isOpen);
	}
	int __stdcall drawRedistTick(bool* isOpen)
	{
		helpers::updateMetrics();

		return redistUI::draw(isOpen);
	}
};