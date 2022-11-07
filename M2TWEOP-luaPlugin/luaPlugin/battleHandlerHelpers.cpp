#include "battleHandlerHelpers.h"
#include "plugData.h"
namespace battleHandlerHelpers
{
	std::string getWinConditionS(DWORD condition)
	{
		int condCode= (*(*plugData::data.funcsBattle.getBattleCondCode))(condition);

		switch (condCode)
		{
		case 0: return "destroy_or_rout_enemy"; break;
		case 1: return "balance_of_strength_percent"; break;
		case 2: return "destroy_enemy_strength_percent"; break;
		case 3: return "capture_location"; break;
		case 4: return "destroy_character"; break;
		case 5: return "capture_major_settlement"; break;
		case 6: return "capture_army_settlement"; break;
		default: return "unknown_condition"; break;			
		}
	}
};
