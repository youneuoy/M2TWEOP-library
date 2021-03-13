#include "toolRoutine.h"
#include "mainUI.h"

#include "helpers.h"
namespace toolRoutine
{

	void drawTick(bool* isOpen)
	{
		helpers::updateMetrics();

		mainUI::draw(isOpen);
	}
};