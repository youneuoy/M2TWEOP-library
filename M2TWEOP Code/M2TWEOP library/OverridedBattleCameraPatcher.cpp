#include "OverridedBattleCameraPatcher.h"

void OverridedBattleCameraPatcher::UpdateState(bool isDisable)
{
	for (auto& patch : Patches)
	{
		patch.UpdateState(isDisable);
	}
}

void OverridedBattleCameraPatcher::Init(int gameVersion)
{
	if (gameVersion == 2)//steam
	{
		//#TARGET_X
		Patches.push_back(OverridedBattleCameraPatch(0x008F8B78));
		Patches.push_back(OverridedBattleCameraPatch(0x008F8E38));
		Patches.push_back(OverridedBattleCameraPatch(0x008F8EB9));
		Patches.push_back(OverridedBattleCameraPatch(0x00E7EF91));
		Patches.push_back(OverridedBattleCameraPatch(0x008F6F5F));
		Patches.push_back(OverridedBattleCameraPatch(0x0095B5CB));
		Patches.push_back(OverridedBattleCameraPatch(0x0094FB90));
		Patches.push_back(OverridedBattleCameraPatch(0x0095B828));
		Patches.push_back(OverridedBattleCameraPatch(0x008F8CB6));
		Patches.push_back(OverridedBattleCameraPatch(0x008F9480));
		Patches.push_back(OverridedBattleCameraPatch(0x008F7056));
		Patches.push_back(OverridedBattleCameraPatch(0x008FAC5B));

		//#TARGET_Y
		Patches.push_back(OverridedBattleCameraPatch(0x008F8B84));
		Patches.push_back(OverridedBattleCameraPatch(0x008F8E44));
		Patches.push_back(OverridedBattleCameraPatch(0x008F8EC5));
		Patches.push_back(OverridedBattleCameraPatch(0x00E7EFA6));
		Patches.push_back(OverridedBattleCameraPatch(0x008F6F6B));
		Patches.push_back(OverridedBattleCameraPatch(0x0095B5D4));
		Patches.push_back(OverridedBattleCameraPatch(0x0094FB9B));
		Patches.push_back(OverridedBattleCameraPatch(0x0095B831));
		Patches.push_back(OverridedBattleCameraPatch(0x008F8CC0));
		Patches.push_back(OverridedBattleCameraPatch(0x008F948A));
		Patches.push_back(OverridedBattleCameraPatch(0x008F7060));
		Patches.push_back(OverridedBattleCameraPatch(0x008FAC63));

		//TARGET_Z
		Patches.push_back(OverridedBattleCameraPatch(0x008F8B7E));
		Patches.push_back(OverridedBattleCameraPatch(0x008F8E3E));
		Patches.push_back(OverridedBattleCameraPatch(0x008F8EBF));
		Patches.push_back(OverridedBattleCameraPatch(0x00E7EF9B));
		Patches.push_back(OverridedBattleCameraPatch(0x008F6F65));
		Patches.push_back(OverridedBattleCameraPatch(0x0095B5CF));
		Patches.push_back(OverridedBattleCameraPatch(0x0094FB95));
		Patches.push_back(OverridedBattleCameraPatch(0x0094FBCE));
		Patches.push_back(OverridedBattleCameraPatch(0x0094FDCD));
		Patches.push_back(OverridedBattleCameraPatch(0x0095B82C));
		Patches.push_back(OverridedBattleCameraPatch(0x008F8CBB));
		Patches.push_back(OverridedBattleCameraPatch(0x008F9485));
		Patches.push_back(OverridedBattleCameraPatch(0x008F705B));
		Patches.push_back(OverridedBattleCameraPatch(0x008FAC4E));
		Patches.push_back(OverridedBattleCameraPatch(0x0094E9BC));
		Patches.push_back(OverridedBattleCameraPatch(0x008F9055));

		//CAMERA_X
		Patches.push_back(OverridedBattleCameraPatch(0x008F8E10));
		Patches.push_back(OverridedBattleCameraPatch(0x008F8B50));
		Patches.push_back(OverridedBattleCameraPatch(0x00E7EF6A));
		Patches.push_back(OverridedBattleCameraPatch(0x0094FCDC));
		Patches.push_back(OverridedBattleCameraPatch(0x008FAC69));
		Patches.push_back(OverridedBattleCameraPatch(0x008F8C6C));
		Patches.push_back(OverridedBattleCameraPatch(0x008F9439));
		Patches.push_back(OverridedBattleCameraPatch(0x0095B40E));
		Patches.push_back(OverridedBattleCameraPatch(0x0095B7F4));
		Patches.push_back(OverridedBattleCameraPatch(0x008F8E8B));
		Patches.push_back(OverridedBattleCameraPatch(0x008F6F29));
		Patches.push_back(OverridedBattleCameraPatch(0x0095B3B0));
		Patches.push_back(OverridedBattleCameraPatch(0x0094E996));
		Patches.push_back(OverridedBattleCameraPatch(0x008F9050));

		//CAMERA_Y
		Patches.push_back(OverridedBattleCameraPatch(0x008F8E1C));
		Patches.push_back(OverridedBattleCameraPatch(0x008F8B5C));
		Patches.push_back(OverridedBattleCameraPatch(0x00E7EF7F));
		Patches.push_back(OverridedBattleCameraPatch(0x0094FCE5));
		Patches.push_back(OverridedBattleCameraPatch(0x008FAC72));
		Patches.push_back(OverridedBattleCameraPatch(0x008F8C76));
		Patches.push_back(OverridedBattleCameraPatch(0x008F9443));
		Patches.push_back(OverridedBattleCameraPatch(0x0095B429));
		Patches.push_back(OverridedBattleCameraPatch(0x0095B805));
		Patches.push_back(OverridedBattleCameraPatch(0x008F8E97));
		Patches.push_back(OverridedBattleCameraPatch(0x008F6F39));
		Patches.push_back(OverridedBattleCameraPatch(0x0095B3BB));
		Patches.push_back(OverridedBattleCameraPatch(0x0094E9DF));
		Patches.push_back(OverridedBattleCameraPatch(0x008F905A));

		//CAMERA_Z
		Patches.push_back(OverridedBattleCameraPatch(0x008F8E16));
		Patches.push_back(OverridedBattleCameraPatch(0x008F8B56));
		Patches.push_back(OverridedBattleCameraPatch(0x00E7EF74));
		Patches.push_back(OverridedBattleCameraPatch(0x0094FCE0));
		Patches.push_back(OverridedBattleCameraPatch(0x0094FD2D));
		Patches.push_back(OverridedBattleCameraPatch(0x008FAC6D));
		Patches.push_back(OverridedBattleCameraPatch(0x008F8C71));
		Patches.push_back(OverridedBattleCameraPatch(0x008F943E));
		Patches.push_back(OverridedBattleCameraPatch(0x0095B41B));
		Patches.push_back(OverridedBattleCameraPatch(0x0095B499));
		Patches.push_back(OverridedBattleCameraPatch(0x0095B7FC));
		Patches.push_back(OverridedBattleCameraPatch(0x008F8E91));
		Patches.push_back(OverridedBattleCameraPatch(0x008F6F2F));
		Patches.push_back(OverridedBattleCameraPatch(0x0095B3B5));
		Patches.push_back(OverridedBattleCameraPatch(0x008F9011));

	}
	else if (gameVersion == 1)//kingdoms
	{
		//#TARGET_X
		Patches.push_back(OverridedBattleCameraPatch(0x00941cc8));
		Patches.push_back(OverridedBattleCameraPatch(0x00941f88));
		Patches.push_back(OverridedBattleCameraPatch(0x00942009));
		Patches.push_back(OverridedBattleCameraPatch(0x00ec80e1));
		Patches.push_back(OverridedBattleCameraPatch(0x009400af));
		Patches.push_back(OverridedBattleCameraPatch(0x009a471b));
		Patches.push_back(OverridedBattleCameraPatch(0x00998ce0));
		Patches.push_back(OverridedBattleCameraPatch(0x009a4978));
		Patches.push_back(OverridedBattleCameraPatch(0x00941e06));
		Patches.push_back(OverridedBattleCameraPatch(0x009425d0));
		Patches.push_back(OverridedBattleCameraPatch(0x009401a6));
		Patches.push_back(OverridedBattleCameraPatch(0x00943dab));

		//#TARGET_Y
		Patches.push_back(OverridedBattleCameraPatch(0x00941cd4));
		Patches.push_back(OverridedBattleCameraPatch(0x00941f94));
		Patches.push_back(OverridedBattleCameraPatch(0x00942015));
		Patches.push_back(OverridedBattleCameraPatch(0x00ec80f6));
		Patches.push_back(OverridedBattleCameraPatch(0x009400bb));
		Patches.push_back(OverridedBattleCameraPatch(0x009a4724));
		Patches.push_back(OverridedBattleCameraPatch(0x00998ceb));
		Patches.push_back(OverridedBattleCameraPatch(0x009a4981));
		Patches.push_back(OverridedBattleCameraPatch(0x00941e10));
		Patches.push_back(OverridedBattleCameraPatch(0x009425da));
		Patches.push_back(OverridedBattleCameraPatch(0x009401b0));
		Patches.push_back(OverridedBattleCameraPatch(0x00943db3));

		//TARGET_Z
		Patches.push_back(OverridedBattleCameraPatch(0x00941cce));
		Patches.push_back(OverridedBattleCameraPatch(0x00941f8e));
		Patches.push_back(OverridedBattleCameraPatch(0x0094200f));
		Patches.push_back(OverridedBattleCameraPatch(0x00ec80eb));
		Patches.push_back(OverridedBattleCameraPatch(0x009400b5));
		Patches.push_back(OverridedBattleCameraPatch(0x009a471f));
		Patches.push_back(OverridedBattleCameraPatch(0x00998ce5));
		Patches.push_back(OverridedBattleCameraPatch(0x00998d1e));
		Patches.push_back(OverridedBattleCameraPatch(0x00998f1d));
		Patches.push_back(OverridedBattleCameraPatch(0x009a497c));
		Patches.push_back(OverridedBattleCameraPatch(0x00941e0b));
		Patches.push_back(OverridedBattleCameraPatch(0x009425d5));
		Patches.push_back(OverridedBattleCameraPatch(0x009401ab));
		Patches.push_back(OverridedBattleCameraPatch(0x00943d9e));
		Patches.push_back(OverridedBattleCameraPatch(0x00997b0c));
		Patches.push_back(OverridedBattleCameraPatch(0x009421a5));


		//CAMERA_X
		Patches.push_back(OverridedBattleCameraPatch(0x00941f60));
		Patches.push_back(OverridedBattleCameraPatch(0x00941ca0));
		Patches.push_back(OverridedBattleCameraPatch(0x00ec80ba));
		Patches.push_back(OverridedBattleCameraPatch(0x00998e2c));
		Patches.push_back(OverridedBattleCameraPatch(0x00943db9));
		Patches.push_back(OverridedBattleCameraPatch(0x00941dbc));
		Patches.push_back(OverridedBattleCameraPatch(0x00942589));
		Patches.push_back(OverridedBattleCameraPatch(0x009a455e));
		Patches.push_back(OverridedBattleCameraPatch(0x009a4944));
		Patches.push_back(OverridedBattleCameraPatch(0x00941fdb));
		Patches.push_back(OverridedBattleCameraPatch(0x00940079));
		Patches.push_back(OverridedBattleCameraPatch(0x009a4500));
		Patches.push_back(OverridedBattleCameraPatch(0x00997ae6));
		Patches.push_back(OverridedBattleCameraPatch(0x009421a0));

		//CAMERA_Y
		Patches.push_back(OverridedBattleCameraPatch(0x00941f6c));
		Patches.push_back(OverridedBattleCameraPatch(0x00941cac));
		Patches.push_back(OverridedBattleCameraPatch(0x00ec80cf));
		Patches.push_back(OverridedBattleCameraPatch(0x00998e35));
		Patches.push_back(OverridedBattleCameraPatch(0x00943dc2));
		Patches.push_back(OverridedBattleCameraPatch(0x00941dc6));
		Patches.push_back(OverridedBattleCameraPatch(0x00942593));
		Patches.push_back(OverridedBattleCameraPatch(0x009a4579));
		Patches.push_back(OverridedBattleCameraPatch(0x009a4955));
		Patches.push_back(OverridedBattleCameraPatch(0x00941fe7));
		Patches.push_back(OverridedBattleCameraPatch(0x00940089));
		Patches.push_back(OverridedBattleCameraPatch(0x009a450b));
		Patches.push_back(OverridedBattleCameraPatch(0x00997b2f));
		Patches.push_back(OverridedBattleCameraPatch(0x009421aa));

		//CAMERA_Z
		Patches.push_back(OverridedBattleCameraPatch(0x00941f66));
		Patches.push_back(OverridedBattleCameraPatch(0x00941ca6));
		Patches.push_back(OverridedBattleCameraPatch(0x00ec80c4));
		Patches.push_back(OverridedBattleCameraPatch(0x00998e30));
		Patches.push_back(OverridedBattleCameraPatch(0x00998e7d));
		Patches.push_back(OverridedBattleCameraPatch(0x00943dbd));
		Patches.push_back(OverridedBattleCameraPatch(0x00941dc1));
		Patches.push_back(OverridedBattleCameraPatch(0x0094258e));
		Patches.push_back(OverridedBattleCameraPatch(0x009a456b));
		Patches.push_back(OverridedBattleCameraPatch(0x009a45e9));
		Patches.push_back(OverridedBattleCameraPatch(0x009a494c));
		Patches.push_back(OverridedBattleCameraPatch(0x00941fe1));
		Patches.push_back(OverridedBattleCameraPatch(0x0094007f));
		Patches.push_back(OverridedBattleCameraPatch(0x009a4505));
		Patches.push_back(OverridedBattleCameraPatch(0x00942161));
	}
}