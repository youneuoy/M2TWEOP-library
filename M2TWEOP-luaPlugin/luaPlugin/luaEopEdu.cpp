///
//![Lua logo](../Lua.png)
//M2TWEOP structures and functions. There are not many examples and descriptions here. Also note that the examples do not include many of the checks that would be required when creating modifications.
//@module LuaPlugin
//@author youneuoy
//@license GPL-3.0
#include "luaP.h"
#include "plugData.h"
#include "eopEduHelpers.h"

void luaP::initEopEdu()
{
	struct
	{
		sol::table M2TWEOPEDUTable;
	}tables;
	using namespace UnitEnums;
	
	luaState.new_enum(
		"stat_pri_attr",
		"spear", stat_pri_attr::spear,
		"light_spear", stat_pri_attr::light_spear,
		"prec", stat_pri_attr::prec,
		"ap", stat_pri_attr::ap,
		"bp", stat_pri_attr::bp,
		"area", stat_pri_attr::area,
		"fire", stat_pri_attr::fire,
		"launching", stat_pri_attr::launching,
		"thrown", stat_pri_attr::thrown,
		"short_pike", stat_pri_attr::short_pike,
		"long_pike", stat_pri_attr::long_pike,
		"spear_bonus_12", stat_pri_attr::spear_bonus_12,
		"spear_bonus_10", stat_pri_attr::spear_bonus_10,
		"spear_bonus_8", stat_pri_attr::spear_bonus_8,
		"spear_bonus_6", stat_pri_attr::spear_bonus_6,
		"spear_bonus_4", stat_pri_attr::spear_bonus_4
	);
	///M2TWEOPDU
	//@section M2TWEOPDUTable

	/***
	Basic M2TWEOPDU table. Contains descriptions of m2tweop unit types.

	@tfield addEopEduEntryFromFile addEopEduEntryFromFile
	@tfield addEopEduEntryFromEDUID addEopEduEntryFromEDUID
	@tfield getEopEduEntryByID getEopEduEntryByID Needed for change many parameters of entry.
	@tfield getDataEopDu getDataEopDu
	@tfield setEntryUnitCardTga setEntryUnitCardTga
	@tfield setEntryInfoCardTga setEntryInfoCardTga
	@tfield setEntrySoldierModel setEntrySoldierModel
	@tfield setEntryLocalizedName setEntryLocalizedName
	@tfield setEntryLocalizedDescr setEntryLocalizedDescr
	@tfield setEntryLocalizedShortDescr setEntryLocalizedShortDescr
	@table M2TWEOPDU
	*/



	tables.M2TWEOPEDUTable = luaState.create_table("M2TWEOPDU");


	/***
	Create new M2TWEOPDU entry from type description
	@function M2TWEOPDU.addEopEduEntryFromFile
	@tparam string filepath path to file with unit type description(like in export_descr_unit.txt, but only with one record and without comments)
	@tparam int eopEnryIndex Entry index, which will be assigned to a new record in DU (recommend starting from 1000, so that there is no confusion with records from EDU).
	@treturn eduEntry retEntry Usually you shouldn't use this value.
	@usage
	M2TWEOPDU.addEopEduEntryFromFile(M2TWEOP.getModPath().."/youneuoy_Data/unitTypes/myTestType.txt",1000);
	*/
	tables.M2TWEOPEDUTable.set_function("addEopEduEntryFromFile", &eopEduHelpers::addEopEduEntryFromFile);

	/***
	Create new M2TWEOPDU entry
	@function M2TWEOPDU.addEopEduEntryFromEDUID
	@tparam int baseEnryIndex Entry index number, which will be taken as the base for this DU record.
	@tparam int eopEnryIndex Entry index, which will be assigned to a new record in DU (recommend starting from 1000, so that there is no confusion with records from EDU).
	@treturn eduEntry retEntry Usually you shouldn't use this value.
	@usage
	M2TWEOPDU.addEopEduEntryFromEDUID(1,1000);
	*/
	tables.M2TWEOPEDUTable.set_function("addEopEduEntryFromEDUID", &eopEduHelpers::addEopEduEntry);

	/***
	Get eduEntry of M2TWEOPDU entry. Needed for change many parameters of entry.
	@function M2TWEOPDU.getEopEduEntryByID
	@tparam int eopEnryIndex Entry index
	@treturn eduEntry retEntry
	@usage
	local eduEntryOfEOPDU=M2TWEOPDU.getEopEduEntryByID(1000);
	eduEntryOfEOPDU.SoldierCount=20;
	eduEntryOfEOPDU.Width=1.5;
	*/
	tables.M2TWEOPEDUTable.set_function("getEopEduEntryByID", &eopEduHelpers::getEopEduEntry);
	tables.M2TWEOPEDUTable.set_function("getEduEntry", &eopEduHelpers::getEduEntry);


	/***
	Get data of M2TWEOPDU entry. You usually won't need this.
	@function M2TWEOPDU.getDataEopDu
	@tparam int eopEnryIndex Entry index
	@treturn eopDuEntry retEntry Usually you shouldn't use this value.
	@usage
	local eopDUEntry=M2TWEOPDU.getDataEopDu(1000);
	*/
	tables.M2TWEOPEDUTable.set_function("getDataEopDu", &eopEduHelpers::getDataEopDu);


	/***
	Set unit card for M2TWEOPDU entry. Requirements for the location and parameters of the image are unchanged in relation to the game.
	@function M2TWEOPDU.setEntryUnitCardTga
	@tparam int eopEnryIndex Entry index
	@tparam string newCardTga
	@usage
	M2TWEOPDU.setEntryUnitCardTga(1000,"#akavir_swordsmen.tga");
	*/
	tables.M2TWEOPEDUTable.set_function("setEntryUnitCardTga", &eopEduHelpers::setEntryUnitCardTga);

	/***
	Set unit info card for M2TWEOPDU entry. Requirements for the location and parameters of the image are unchanged in relation to the game.
	@function M2TWEOPDU.setEntryInfoCardTga
	@tparam int eopEnryIndex Entry index
	@tparam string newInfoCardTga
	@usage
	M2TWEOPDU.setEntryInfoCardTga(1000,"akavir_swordsmen_info.tga");
	*/
	tables.M2TWEOPEDUTable.set_function("setEntryInfoCardTga", &eopEduHelpers::setEntryInfoCardTga);


	/***
	Set soldier model for M2TWEOPDU entry. The required entry must be correctly recorded in game files.
	@function M2TWEOPDU.setEntrySoldierModel
	@tparam int eopEnryIndex Entry index
	@tparam string newSoldierModel
	@usage
	M2TWEOPDU.setEntrySoldierModel(1000,"Sword_and_Buckler_Men");
	*/
	tables.M2TWEOPEDUTable.set_function("setEntrySoldierModel", &eopEduHelpers::setEntrySoldierModel);


	/***
	Set armour stats for M2TWEOPDU entry.
	@function M2TWEOPDU.setEntryStatPriArmour
	@tparam int eopEnryIndex Entry index
	@tparam int armour
	@tparam int defense
	@tparam int shield
	@usage
	M2TWEOPDU.setEntryStatPriArmour(1000,5,6,7);
	*/
	tables.M2TWEOPEDUTable.set_function("setEntryStatPriArmour", &eopEduHelpers::setEntryStatPriArmour);
	tables.M2TWEOPEDUTable.set_function("setEntryAttackCharge", &eopEduHelpers::setEntryAttackCharge);
	tables.M2TWEOPEDUTable.set_function("GetEntryAttack", &eopEduHelpers::GetEntryAttack);
	tables.M2TWEOPEDUTable.set_function("GetEntryCharge", &eopEduHelpers::GetEntryCharge);
	tables.M2TWEOPEDUTable.set_function("GetArmourUpgradeLevelsNum", &eopEduHelpers::GetArmourUpgradeLevelsNum);
	tables.M2TWEOPEDUTable.set_function("SetArmourUpgradeLevelsNum", &eopEduHelpers::SetArmourUpgradeLevelsNum);
	tables.M2TWEOPEDUTable.set_function("GetArmourUpgradeLevel", &eopEduHelpers::GetArmourUpgradeLevel);
	tables.M2TWEOPEDUTable.set_function("SetArmourUpgradeLevel", &eopEduHelpers::SetArmourUpgradeLevel);
	tables.M2TWEOPEDUTable.set_function("GetArmourUpgradeModelsNum", &eopEduHelpers::GetArmourUpgradeModelsNum);
	tables.M2TWEOPEDUTable.set_function("SetArmourUpgradeModelsNum", &eopEduHelpers::SetArmourUpgradeModelsNum);
	tables.M2TWEOPEDUTable.set_function("GetArmourUpgradeModel", &eopEduHelpers::GetArmourUpgradeModel);
	tables.M2TWEOPEDUTable.set_function("SetArmourUpgradeModel", &eopEduHelpers::SetArmourUpgradeModel);
	tables.M2TWEOPEDUTable.set_function("setEntryStatPriAttribute", &eopEduHelpers::setEntryStatPriAttribute);


	/***
	Set localized name for M2TWEOPDU entry. This does not require any entries in the text folder.
	@function M2TWEOPDU.setEntryLocalizedName
	@tparam int eopEnryIndex Entry index
	@tparam string newLocalizedName
	@usage
	M2TWEOPDU.setEntryLocalizedName(1000,"Test unit");
	*/
	tables.M2TWEOPEDUTable.set_function("setEntryLocalizedName", &eopEduHelpers::setEntryLocalizedName);


	/***
	Set localized description for M2TWEOPDU entry. This does not require any entries in the text folder.
	@function M2TWEOPDU.setEntryLocalizedDescr
	@tparam int eopEnryIndex Entry index
	@tparam string newLocalizedDescr
	@usage
	M2TWEOPDU.setEntryLocalizedDescr(1000,"This is test unit description\n123321\nCreated with m2tweop");
	*/
	tables.M2TWEOPEDUTable.set_function("setEntryLocalizedDescr", &eopEduHelpers::setEntryLocalizedDescr);

	/***
	Set localized short description for M2TWEOPDU entry. This does not require any entries in the text folder.
	@function M2TWEOPDU.setEntryLocalizedShortDescr
	@tparam int eopEnryIndex Entry index
	@tparam string newLocalizedShortDescr
	@usage
	M2TWEOPDU.setEntryLocalizedShortDescr(1000,"This is test unit short description\n123321\nCreated with m2tweop");
	*/
	tables.M2TWEOPEDUTable.set_function("setEntryLocalizedShortDescr", &eopEduHelpers::setEntryLocalizedShortDescr);




}