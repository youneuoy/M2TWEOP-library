#include "toolRoutine.h"
#include "mainUI.h"

#include "helpers.h"
namespace toolRoutine
{
	int drawTick(bool* isOpen)
	{
		helpers::updateMetrics();

		return mainUI::draw(isOpen);
	}
};