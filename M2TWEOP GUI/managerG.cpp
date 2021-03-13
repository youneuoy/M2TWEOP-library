#include "managerG.h"
#include "helpers.h"

#include "managerGHelpers.h"
namespace managerG
{
	void init()
	{
		helpers::updateMetrics();
		loadTextures();
		loadFonts();
	}
};