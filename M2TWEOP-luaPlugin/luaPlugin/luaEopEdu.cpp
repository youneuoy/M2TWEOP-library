///
//![Lua logo](../Lua.png)
//M2TWEOP structures and functions. There are not many examples and descriptions here. Also note that the examples do not include many of the checks that would be required when creating modifications.
//@module LuaPlugin
//@author youneuoy
//@license GPL-3.0
#include "luaP.h"
#include "plugData.h"
#include "eopEduHelpers.h"
#include "buildingStructHelpers.h"

void luaP::initEopEdu()
{
	struct
	{
		sol::table M2TWEOPEDUTable;
		sol::table EDB;
	}tables;

	struct
	{
		sol::usertype<edbEntry>edbEntry;
		sol::usertype<BuildingLvlCapability>capability;
		sol::usertype<recruitPool>recruitpool;
	}types;
	using namespace UnitEnums;

	luaState.new_enum(
		"attackAttr",
		"", attackAttr::nothing,
		"spear", attackAttr::spear,
		"light_spear", attackAttr::light_spear,
		"prec", attackAttr::prec,
		"ap", attackAttr::ap,
		"bp", attackAttr::bp,
		"area", attackAttr::area,
		"fire", attackAttr::fire,
		"launching", attackAttr::launching,
		"thrown", attackAttr::thrown,
		"short_pike", attackAttr::short_pike,
		"long_pike", attackAttr::long_pike,
		"spear_bonus_12", attackAttr::spear_bonus_12,
		"spear_bonus_10", attackAttr::spear_bonus_10,
		"spear_bonus_8", attackAttr::spear_bonus_8,
		"spear_bonus_6", attackAttr::spear_bonus_6,
		"spear_bonus_4", attackAttr::spear_bonus_4
	);

	luaState.new_enum(
		"eduStat",
		"", eduStat::none,
		"armour", eduStat::armour,
		"defense", eduStat::defense,
		"shield", eduStat::shield,
		"attack", eduStat::attack,
		"charge", eduStat::charge
	);
	///M2TWEOPDU
	//@section M2TWEOPDUTable

	/***
	Basic M2TWEOPDU table. Contains descriptions of M2TWEOP unit types.

	@tfield addEopEduEntryFromFile addEopEduEntryFromFile
	@tfield addEopEduEntryFromEDUID addEopEduEntryFromEDUID
	@tfield getEopEduEntryByID getEopEduEntryByID Needed for change many parameters of entry.
	@tfield getDataEopDu getDataEopDu
	@tfield getEduEntry getEduEntry
	@tfield getEduEntryByType getEduEntryByType
	@tfield getEduIndexByType getEduIndexByType
	@tfield setEntryUnitCardTga setEntryUnitCardTga (only for eopdu units added by file!)
	@tfield setEntryInfoCardTga setEntryInfoCardTga (only for eopdu units added by file!)
	@tfield setEntrySoldierModel setEntrySoldierModel
	@tfield getArmourUpgradeLevelsNum getArmourUpgradeLevelsNum
	@tfield setArmourUpgradeLevelsNum setArmourUpgradeLevelsNum (only for eopdu units added by file!)
	@tfield getArmourUpgradeLevel getArmourUpgradeLevel
	@tfield setArmourUpgradeLevel setArmourUpgradeLevel (only for eopdu units added by file!)
	@tfield getArmourUpgradeModelsNum getArmourUpgradeModelsNum
	@tfield setArmourUpgradeModelsNum setArmourUpgradeModelsNum (only for eopdu units added by file!)
	@tfield getArmourUpgradeModel getArmourUpgradeModel
	@tfield setArmourUpgradeModel setArmourUpgradeModel (only for eopdu units added by file!)
	@tfield setEntryAttackAttribute setEntryAttackAttribute
	@tfield getEntryAttackAttribute getEntryAttackAttribute
	@tfield setEntryStat setEntryStat
	@tfield getEntryStat getEntryStat
	@tfield setEntryLocalizedName setEntryLocalizedName
	@tfield setEntryLocalizedDescr setEntryLocalizedDescr
	@tfield setEntryLocalizedShortDescr setEntryLocalizedShortDescr
	@table M2TWEOPDU
	*/



	tables.M2TWEOPEDUTable = luaState.create_table("M2TWEOPDU");


	/***
	Create new M2TWEOPDU entry from a file describing it.
	@function M2TWEOPDU.addEopEduEntryFromFile
	@tparam string filepath path to file with unit type description(like in export\_descr\_unit.txt, but only with one record and without comments)
	@tparam int eopEnryIndex Entry index, which will be assigned to a new record in DU (recommend starting from 1000, so that there is no confusion with records from EDU).
	@treturn eduEntry retEntry Usually you shouldn't use this value.
	@usage
	M2TWEOPDU.addEopEduEntryFromFile(M2TWEOP.getModPath().."/youneuoy_Data/unitTypes/myTestType.txt",1000);
	*/
	tables.M2TWEOPEDUTable.set_function("addEopEduEntryFromFile", &eopEduHelpers::addEopEduEntryFromFile);

	/***
	Create new M2TWEOPDU entry.
	@function M2TWEOPDU.addEopEduEntryFromEDUID
	@tparam int baseEnryIndex Entry index number, which will be taken as the base for this DU record.
	@tparam int eopEnryIndex Entry index, which will be assigned to a new record in DU (recommend starting from 1000, so that there is no confusion with records from EDU).
	@treturn eduEntry retEntry Usually you shouldn't use this value.
	@usage
	M2TWEOPDU.addEopEduEntryFromEDUID(1,1000);
	*/
	tables.M2TWEOPEDUTable.set_function("addEopEduEntryFromEDUID", &eopEduHelpers::addEopEduEntry);

	/***
	Get eduEntry of a M2TWEOPDU entry. Needed to change many parameters of the entry.
	@function M2TWEOPDU.getEopEduEntryByID
	@tparam int eopEnryIndex Entry index in M2TWEOPDU.
	@treturn eduEntry retEntry
	@usage
	local eduEntryOfEOPDU=M2TWEOPDU.getEopEduEntryByID(1000);
	eduEntryOfEOPDU.SoldierCount=20;
	eduEntryOfEOPDU.Width=1.5;
	*/
	tables.M2TWEOPEDUTable.set_function("getEopEduEntryByID", &eopEduHelpers::getEopEduEntry);

	/***
	Get eduEntry by index. Needed to change many parameters of the entry.
	@function M2TWEOPDU.getEduEntry
	@tparam int EnryIndex Entry index (Values lower then 500 look for edu entry, values over 500 look for EOP edu entry).
	@treturn eduEntry retEntry
	@usage
	local eduEntry=M2TWEOPDU.getEduEntry(5);
	eduEntry.SoldierCount=20;
	eduEntry.Width=1.5;
	*/
	tables.M2TWEOPEDUTable.set_function("getEduEntry", &eopEduHelpers::getEduEntry);

	/***
	Get eduEntry by edu type name. Needed to change many parameters of the entry.
	@function M2TWEOPDU.getEduEntryByType
	@tparam string type Unit type as in export_descr_unit.
	@treturn eduEntry retEntry
	@usage
	local eduEntry=M2TWEOPDU.getEduEntryByType("Peasants");
	eduEntry.SoldierCount=20;
	eduEntry.Width=1.5;
	*/
	tables.M2TWEOPEDUTable.set_function("getEduEntryByType", &eopEduHelpers::getEduEntryByType);

	/***
	Get edu index by edu type name. Needed to use many edu functions.
	@function M2TWEOPDU.getEduIndexByType
	@tparam string type Unit type as in export_descr_unit.
	@treturn int eduindex
	@usage
	local eduindex=M2TWEOPDU.getEduIndexByType("Peasants");
	M2TWEOPDU.setEntryStat(eduindex, eduStat.armour, 5, 1);
	*/
	tables.M2TWEOPEDUTable.set_function("getEduIndexByType", &eopEduHelpers::getEduIndexByType);


	/***
	Get data of a M2TWEOPDU entry. You usually won't need this.
	@function M2TWEOPDU.getDataEopDu
	@tparam int eopEnryIndex Entry index in M2TWEOPDU.
	@treturn eopDuEntry retEntry Usually you shouldn't use this value.
	@usage
	local eopDUEntry=M2TWEOPDU.getDataEopDu(1000);
	*/
	tables.M2TWEOPEDUTable.set_function("getDataEopDu", &eopEduHelpers::getDataEopDu);


	/***
	Set unit card for a M2TWEOPDU entry. Requirements for the location and parameters of the image are unchanged in relation to the game (only for eopdu units added by file!).
	@function M2TWEOPDU.setEntryUnitCardTga
	@tparam int eopEnryIndex Entry index in M2TWEOPDU.
	@tparam string newCardTga
	@usage
	M2TWEOPDU.setEntryUnitCardTga(1000,"#akavir_swordsmen.tga");
	*/
	tables.M2TWEOPEDUTable.set_function("setEntryUnitCardTga", &eopEduHelpers::setEntryUnitCardTga);

	/***
	Set unit info card for M2TWEOPDU entry. Requirements for the location and parameters of the image are unchanged in relation to the game (only for eopdu units added by file!).
	@function M2TWEOPDU.setEntryInfoCardTga
	@tparam int eopEnryIndex Entry index in M2TWEOPDU.
	@tparam string newInfoCardTga
	@usage
	M2TWEOPDU.setEntryInfoCardTga(1000,"akavir_swordsmen_info.tga");
	*/
	tables.M2TWEOPEDUTable.set_function("setEntryInfoCardTga", &eopEduHelpers::setEntryInfoCardTga);


	/***
	Set unit info card for a M2TWEOPDU entry. Requirements for the location and parameters of the image are unchanged in relation to the game.
	@function M2TWEOPDU.setEntrySoldierModel
	@tparam int eopEnryIndex Entry index in M2TWEOPDU.
	@tparam string newSoldierModel
	@usage
	M2TWEOPDU.setEntrySoldierModel(1000,"Sword_and_Buckler_Men");
	*/
	tables.M2TWEOPEDUTable.set_function("setEntrySoldierModel", &eopEduHelpers::setEntrySoldierModel);

	/***
	Get the amount of numbers in the armour_upg_levels line in export_descr_unit.
	@function M2TWEOPDU.getArmourUpgradeLevelsNum
	@tparam int index Entry index (Values lower then 500 look for edu entry, values over 500 look for EOP edu entry).
	@treturn int ArmourUpgradeLevelsNum
	@usage
	M2TWEOPDU.getArmourUpgradeLevelsNum(1000);
	*/
	tables.M2TWEOPEDUTable.set_function("getArmourUpgradeLevelsNum", &eopEduHelpers::getArmourUpgradeLevelsNum);


	/***
	Set the amount of armour_upg_levels, if you increase the amount of levels the last number entry will be repeated (only for eopdu units added by file!).
	@function M2TWEOPDU.setArmourUpgradeLevelsNum
	@tparam int index Entry index (Values lower then 500 look for edu entry, values over 500 look for EOP edu entry).
	@tparam int amount
	@usage
	M2TWEOPDU.setArmourUpgradeLevelsNum(1000, 3);
	*/
	tables.M2TWEOPEDUTable.set_function("setArmourUpgradeLevelsNum", &eopEduHelpers::setArmourUpgradeLevelsNum);


	/***
	Get armour upgrade level number at specified index.
	@function M2TWEOPDU.getArmourUpgradeLevel
	@tparam int index Entry index (Values lower then 500 look for edu entry, values over 500 look for EOP edu entry).
	@tparam int levelidx
	@treturn int level
	@usage
	M2TWEOPDU.getArmourUpgradeLevel(1000, 0);
	*/
	tables.M2TWEOPEDUTable.set_function("getArmourUpgradeLevel", &eopEduHelpers::getArmourUpgradeLevel);

	/***
	Set armour upgrade level number at specified index (only for eopdu units added by file!).
	@function M2TWEOPDU.setArmourUpgradeLevel
	@tparam int index Entry index (Values lower then 500 look for edu entry, values over 500 look for EOP edu entry).
	@tparam int levelidx
	@tparam int newlevel
	@usage
	M2TWEOPDU.setArmourUpgradeLevel(1000, 1, 4);
	*/
	tables.M2TWEOPEDUTable.set_function("setArmourUpgradeLevel", &eopEduHelpers::setArmourUpgradeLevel);

	/***
	Get the amount of models in the armour_upg_models line in export_descr_unit.
	@function M2TWEOPDU.getArmourUpgradeLevelsNum
	@tparam int index Entry index (Values lower then 500 look for edu entry, values over 500 look for EOP edu entry).
	@treturn int ArmourUpgradeLevelsNum
	@usage
	M2TWEOPDU.getArmourUpgradeModelsNum(1000);
	*/
	tables.M2TWEOPEDUTable.set_function("getArmourUpgradeModelsNum", &eopEduHelpers::getArmourUpgradeModelsNum);

	/***
	Set the amount of armour_upg_levels, if you increase the amount of models the last model entry will be repeated (only for eopdu units added by file!).
	@function M2TWEOPDU.setArmourUpgradeModelsNum
	@tparam int index Entry index (Values lower then 500 look for edu entry, values over 500 look for EOP edu entry).
	@tparam int amount Maximum: 4
	@usage
	M2TWEOPDU.setArmourUpgradeModelsNum(1000, 3);
	*/
	tables.M2TWEOPEDUTable.set_function("setArmourUpgradeModelsNum", &eopEduHelpers::setArmourUpgradeModelsNum);

	/***
	Get armour upgrade level number at specified index.
	@function M2TWEOPDU.getArmourUpgradeModel
	@tparam int index Entry index (Values lower then 500 look for edu entry, values over 500 look for EOP edu entry).
	@tparam int levelidx
	@treturn string modelName
	@usage
	M2TWEOPDU.getArmourUpgradeModel(1000, 0);
	*/
	tables.M2TWEOPEDUTable.set_function("getArmourUpgradeModel", &eopEduHelpers::getArmourUpgradeModel);

	/***
	Set the unit model at specified index (only for eopdu units added by file!).
	@function M2TWEOPDU.setArmourUpgradeModel
	@tparam int index Entry index (Values lower then 500 look for edu entry, values over 500 look for EOP edu entry).
	@tparam int levelidx
	@tparam string modelName
	@usage
	M2TWEOPDU.setArmourUpgradeModel(1000, 1, 4);
	*/
	tables.M2TWEOPEDUTable.set_function("setArmourUpgradeModel", &eopEduHelpers::setArmourUpgradeModel);

	/***
	Set a primary or secondary attack attribute of an edu entry.
	@function M2TWEOPDU.setEntryAttackAttribute
	@tparam int index Entry index (Values lower then 500 look for edu entry, values over 500 look for EOP edu entry).
	@tparam int attribute Use the attackAttr enum: attackAttr.spear, attackAttr.light\_spear, attackAttr.prec, attackAttr.ap, attackAttr.bp, attackAttr.area, attackAttr.fire, attackAttr.launching, attackAttr.thrown, attackAttr.short\_pike, attackAttr.long\_pike, attackAttr.spear\_bonus\_12, attackAttr.spear\_bonus\_10, attackAttr.spear\_bonus\_8, attackAttr.spear\_bonus\_6, attackAttr.spear\_bonus\_4.
	@tparam boolean enable
	@tparam int sec 1 = primary, 2 = secondary.
	@usage
	M2TWEOPDU.setEntryAttackAttribute(1000, attackAttr.ap, true, 1);
	*/
	tables.M2TWEOPEDUTable.set_function("setEntryAttackAttribute", &eopEduHelpers::setEntryAttackAttribute);

	/***
	Get a primary or secondary attack attribute from an edu entry.
	@function M2TWEOPDU.getEntryAttackAttribute
	@tparam int index Entry index (Values lower then 500 look for edu entry, values over 500 look for EOP edu entry).
	@tparam int attribute Use the attackAttr enum: attackAttr.spear, attackAttr.light\_spear, attackAttr.prec, attackAttr.ap, attackAttr.bp, attackAttr.area, attackAttr.fire, attackAttr.launching, attackAttr.thrown, attackAttr.short\_pike, attackAttr.long\_pike, attackAttr.spear\_bonus\_12, attackAttr.spear\_bonus\_10, attackAttr.spear\_bonus\_8, attackAttr.spear\_bonus\_6, attackAttr.spear\_bonus\_4.
	@tparam int sec 1 = primary, 2 = secondary.
	@treturn boolean hasAttackAttribute
	@usage
	M2TWEOPDU.getEntryAttackAttribute(1000, attackAttr.ap, 1);
	*/
	tables.M2TWEOPEDUTable.set_function("getEntryAttackAttribute", &eopEduHelpers::getEntryAttackAttribute);

	/***
	Set any of the basic unit stats of an edu entry.
	@function M2TWEOPDU.setEntryStat
	@tparam int index Entry index (Values lower then 500 look for edu entry, values over 500 look for EOP edu entry).
	@tparam int eduStat Use the eduStat enum: eduStat.armour, eduStat.defense, eduStat.shield, eduStat.attack, eduStat.charge.
	@tparam int value
	@tparam int sec 1 = primary, 2 = secondary.
	@usage
	M2TWEOPDU.setEntryStat(1000, attackAttr.attack, 1);
	*/
	tables.M2TWEOPEDUTable.set_function("setEntryStat", &eopEduHelpers::setEntryStat);

	/***
	Get any of the basic unit stats of an edu entry.
	@function M2TWEOPDU.getEntryStat
	@tparam int index Entry index (Values lower then 500 look for edu entry, values over 500 look for EOP edu entry).
	@tparam int eduStat Use the eduStat enum: eduStat.armour, eduStat.defense, eduStat.shield, eduStat.attack, eduStat.charge.
	@tparam int sec 1 = primary, 2 = secondary.
	@treturn int unitStat
	@usage
	M2TWEOPDU.getEntryStat(1000, attackAttr.attack, 1);
	*/
	tables.M2TWEOPEDUTable.set_function("getEntryStat", &eopEduHelpers::getEntryStat);


	/***
	Set localized name for a M2TWEOPDU entry. This does not require any entries in the text folder.
	@function M2TWEOPDU.setEntryLocalizedName
	@tparam int eopEnryIndex Entry index in M2TWEOPDU.
	@tparam string newLocalizedName
	@usage
	M2TWEOPDU.setEntryLocalizedName(1000,"Test unit");
	*/
	tables.M2TWEOPEDUTable.set_function("setEntryLocalizedName", &eopEduHelpers::setEntryLocalizedName);
	//tables.M2TWEOPEDUTable.set_function("addUnitToRQ", &eopEduHelpers::addUnitToRQ); comment out not good implementation


	/***
	Set localized description for M2TWEOPDU entry. This does not require any entries in the text folder.
	@function M2TWEOPDU.setEntryLocalizedDescr
	@tparam int eopEnryIndex Entry index in M2TWEOPDU.
	@tparam string newLocalizedDescr
	@usage
	M2TWEOPDU.setEntryLocalizedDescr(1000,"This is test unit description\n123321\nCreated with m2tweop");
	*/
	tables.M2TWEOPEDUTable.set_function("setEntryLocalizedDescr", &eopEduHelpers::setEntryLocalizedDescr);

	/***
	Set localized short description for M2TWEOPDU entry. This does not require any entries in the text folder.
	@function M2TWEOPDU.setEntryLocalizedShortDescr
	@tparam int eopEnryIndex Entry index in M2TWEOPDU.
	@tparam string newLocalizedShortDescr
	@usage
	M2TWEOPDU.setEntryLocalizedShortDescr(1000,"This is test unit short description\n123321\nCreated with m2tweop");
	*/
	tables.M2TWEOPEDUTable.set_function("setEntryLocalizedShortDescr", &eopEduHelpers::setEntryLocalizedShortDescr);


	///edbEntry
	//@section edbEntry Table

	/***
	Basic edbEntry table.

	@tfield int buildingID
	@tfield int classification
	@tfield int isCoreBuilding
	@tfield int isPort
	@tfield int isCoreBuilding2
	@tfield int hasReligion
	@tfield int religionID
	@tfield int isHinterland
	@tfield int isFarm
	@tfield int buildingLevelCount

	@table edbEntry
	*/

	types.edbEntry = luaState.new_usertype<edbEntry>("edbEntry");
	types.edbEntry.set("buildingID", &edbEntry::buildingID);
	types.edbEntry.set("classification", &edbEntry::classification);
	types.edbEntry.set("isCoreBuilding", &edbEntry::isCoreBuilding);
	types.edbEntry.set("isPort", &edbEntry::isPort);
	types.edbEntry.set("isCoreBuilding2", &edbEntry::isCoreBuilding2);
	types.edbEntry.set("hasReligion", &edbEntry::hasReligion);
	types.edbEntry.set("religionID", &edbEntry::religionID);
	types.edbEntry.set("isHinterland", &edbEntry::isHinterland);
	types.edbEntry.set("isFarm", &edbEntry::isFarm);
	types.edbEntry.set("buildingLevelCount", &edbEntry::buildingLevelCount);

	///capability
	//@section capability Table

	/***
	Basic capability table.

	@tfield int capabilityType
	@tfield int capabilityLvl
	@tfield int bonus
	@tfield int capabilityID

	@table capability
	*/
	types.capability = luaState.new_usertype<BuildingLvlCapability>("capability");
	types.capability.set("capabilityType", &BuildingLvlCapability::capabilityType);
	types.capability.set("capabilityLvl", &BuildingLvlCapability::capabilityLvl);
	types.capability.set("bonus", &BuildingLvlCapability::bonus);
	types.capability.set("capabilityID", &BuildingLvlCapability::capabilityID);

	///recruitpool
	//@section recruitpool Table

	/***
	Basic recruitpool table.

	@tfield int capabilityType
	@tfield int capabilityLvlorExp Difference is for agents
	@tfield int unitID
	@tfield float initialSize
	@tfield float gainPerTurn
	@tfield float maxSize

	@table recruitpool
	*/
	types.recruitpool = luaState.new_usertype<recruitPool>("recruitpool");
	types.recruitpool.set("capabilityType", &recruitPool::capabilityType);
	types.recruitpool.set("capabilityLvlorExp", &recruitPool::capabilityLvlorExp);
	types.recruitpool.set("unitID", &recruitPool::unitID);
	types.recruitpool.set("initialSize", &recruitPool::initialSize);
	types.recruitpool.set("gainPerTurn", &recruitPool::gainPerTurn);
	types.recruitpool.set("maxSize", &recruitPool::maxSize);

	///EDB
	//@section EDB Table

	/***
	Basic EDB table.

	@tfield addEopBuildEntry addEopBuildEntry
	@tfield getEopBuildEntry getEopBuildEntry
	@tfield setBuildingPic setBuildingPic
	@tfield setBuildingPicConstructed setBuildingPicConstructed
	@tfield setBuildingPicConstruction setBuildingPicConstruction
	@tfield setBuildingLocalizedName setBuildingLocalizedName
	@tfield setBuildingLocalizedDescr setBuildingLocalizedDescr
	@tfield setBuildingLocalizedDescrShort setBuildingLocalizedDescrShort
	@tfield addBuildingCapability addBuildingCapability
	@tfield removeBuildingCapability removeBuildingCapability
	@tfield getBuildingCapability getBuildingCapability
	@tfield getBuildingCapabilityNum getBuildingCapabilityNum
	@tfield addBuildingPool addBuildingPool
	@tfield removeBuildingPool removeBuildingPool
	@tfield getBuildingPool getBuildingPool
	@tfield getBuildingPoolNum getBuildingPoolNum
	@tfield getBuildingByName getBuildingByName

	@table EDB
	*/
	tables.EDB = luaState.create_table("EDB");

	/***
	Create new EOP Building entry
	@function EDB.addEopBuildEntry
	@tparam edbEntry edbEntry Old entry.
	@tparam int newIndex New index of new entry.
	@treturn edbEntry eopentry.
	@usage
	-- Basic Example
	oldBuilding = EDB.getBuildingByName("market")
	newBuilding = EDB.addEopBuildEntry(oldBuilding,150);

	-- Full example
	local oldBuilding = EDB.getBuildingByName("market");
	EDB.addEopBuildEntry(oldBuilding, 0);
	local eopBuilding = EDB.getEopBuildEntry(0);

	-- Set pictures, names and descriptions by culture and faction
	for c = 0, 6 do --every culture
		EDB.setBuildingPic(eopBuilding, 'some path to pic', 0, c)
		EDB.setBuildingPicConstructed(eopBuilding,'some path to pic', 0, c)
	end
	for f = 0, 30 do --every faction
		EDB.setBuildingLocalizedName(eopBuilding, 'some name', 0, f)
		EDB.setBuildingLocalizedDescr(eopBuilding, 'some description', 0, f)
	end

	-- Add in an income bonus of 500
	EDB.addBuildingCapability(eopBuilding, 0, 55, 500, true)

	-- Add a recruit pool
	EDB.addBuildingPool(eopBuilding, 0, 55, 1, 0.1, 2, 0);

	-- Create a dummy building and get it
	sett:createBuilding("market");; --just assuming you have got a sett with some loop or function

	-- Set the existing building in the settlement to be the EOP building we just created
	local dummyBuilding = sett:getBuilding(5)
	dummyBuilding.edbEntry = eopBuilding
	*/
	tables.EDB.set_function("addEopBuildEntry", &buildingStructHelpers::addEopBuildEntry);

	/***
	Get EOP Building entry.
	@function EDB.getEopBuildEntry
	@tparam int index Index of eop entry.
	@treturn edbEntry eopentry.
	@usage
	building = EDB.getEopBuildEntry(150);
	*/
	tables.EDB.set_function("getEopBuildEntry", &buildingStructHelpers::getEopBuildEntry);

	/***
	Set picture of building.
	@function EDB.setBuildingPic
	@tparam edbEntry edbEntry Entry to set.
	@tparam string newPic Path to new pic.
	@tparam int level Building level to set pic for.
	@tparam int culture ID of the culture to set the pic for.
	@usage
	building = EDB.getBuildingByName("market")
	EDB.setBuildingPic(building, modPath .. mp_path_mods .. "data/ui/northern_european/buildings/#northern_european_vintner.tga", 0, 4);
	*/
	tables.EDB.set_function("setBuildingPic", &buildingStructHelpers::setBuildingPic);

	/***
	Set constructed picture of building.
	@function EDB.setBuildingPicConstructed
	@tparam edbEntry edbEntry Entry to set.
	@tparam string newPic Path to new pic.
	@tparam int level Building level to set pic for.
	@tparam int culture ID of the culture to set the pic for.
	@usage
	building = EDB.getBuildingByName("market")
	EDB.setBuildingPicConstructed(building, modPath .. mp_path_mods .. "data/ui/northern_european/buildings/#northern_european_vintner.tga", 0, 4);
	*/
	tables.EDB.set_function("setBuildingPicConstructed", &buildingStructHelpers::setBuildingPicConstructed);

	/***
	Set construction picture of building.
	@function EDB.setBuildingPicConstruction
	@tparam edbEntry edbEntry Entry to set.
	@tparam string newPic Path to new pic.
	@tparam int level Building level to set pic for.
	@tparam int culture ID of the culture to set the pic for.
	@usage
	building = EDB.getBuildingByName("market")
	EDB.setBuildingPicConstruction(building, modPath .. mp_path_mods .. "data/ui/northern_european/buildings/#northern_european_vintner.tga", 0, 4);
	*/
	tables.EDB.set_function("setBuildingPicConstruction", &buildingStructHelpers::setBuildingPicConstruction);

	/***
	Set name of a building.
	@function EDB.setBuildingLocalizedName
	@tparam edbEntry edbEntry Entry to set.
	@tparam string newName New name.
	@tparam int level Building level.
	@tparam int facnum Faction ID of the faction to set it for (dipNum).
	@usage
	building = EDB.getBuildingByName("market")
	EDB.setBuildingLocalizedName(building, modPath .. mp_path_mods .. "data/ui/northern_european/buildings/#northern_european_vintner.tga", 0, 4);
	*/
	tables.EDB.set_function("setBuildingLocalizedName", &buildingStructHelpers::setBuildingLocalizedName);

	/***
	Set description of a building.
	@function EDB.setBuildingLocalizedDescr
	@tparam edbEntry edbEntry Entry to set.
	@tparam string newName New description.
	@tparam int level Building level.
	@tparam int facnum Faction ID of the faction to set it for (dipNum).
	@usage
	building = EDB.getBuildingByName("market")
	EDB.setBuildingLocalizedDescr(building, modPath .. mp_path_mods .. "data/ui/northern_european/buildings/#northern_european_vintner.tga", 0, 4);
	*/
	tables.EDB.set_function("setBuildingLocalizedDescr", &buildingStructHelpers::setBuildingLocalizedDescr);

	/***
	Set short description of a building.
	@function EDB.setBuildingLocalizedDescrShort
	@tparam edbEntry edbEntry Entry to set.
	@tparam string newName New short description.
	@tparam int level Building level.
	@tparam int facnum Faction ID of the faction to set it for (dipNum).
	@usage
	building = EDB.getBuildingByName("market")
	EDB.setBuildingLocalizedDescrShort(building, modPath .. mp_path_mods .. "data/ui/northern_european/buildings/#northern_european_vintner.tga", 0, 4);
	*/
	tables.EDB.set_function("setBuildingLocalizedDescrShort", &buildingStructHelpers::setBuildingLocalizedDescrShort);

	/***
	Add a capability to a building.
	@function EDB.addBuildingCapability
	@tparam edbEntry edbEntry Entry to set.
	@tparam int level Building level.
	@tparam int capability ID of capability to set.
	@tparam int value Value to set.
	@tparam bool bonus Is it bonus or not.
	@usage
	-- Building Capability Enums
	population_growth_bonus = 0,
    population_loyalty_bonus = 1,
    population_health_bonus = 2,
    trade_base_income_bonus = 3,
    trade_level_bonus = 4,
    trade_fleet = 5,
    taxable_income_bonus = 6,
    mine_resource = 7,
    farming_level = 8,
    road_level = 9,
    gate_strength = 10,
    gate_defences = 11,
    wall_level = 12,
    tower_level = 13,
    armour = 14,
    stage_games = 15,
    stage_races = 16,
    fire_risk = 17,
    weapon_melee_simple = 18,
    weapon_melee_blade = 19,
    weapon_missile_mechanical = 20,
    weapon_missile_gunpowder = 21,
    weapon_artillery_mechanical = 22,
    weapon_artillery_gunpowder = 23,
    weapon_naval_gunpowder = 24,
    upgrade_bodyguard = 25,
    recruits_morale_bonus = 26,
    recruits_exp_bonus = 27,
    happiness_bonus = 28,
    law_bonus = 29,
    construction_cost_bonus_military = 30,
    construction_cost_bonus_religious = 31,
    construction_cost_bonus_defensive = 32,
    construction_cost_bonus_other = 33,
    construction_time_bonus_military = 34,
    construction_time_bonus_religious = 35,
    construction_time_bonus_defensive = 36,
    construction_time_bonus_other = 37,
    construction_cost_bonus_wooden = 38,
    construction_cost_bonus_stone = 39,
    construction_time_bonus_wooden = 40,
    construction_time_bonus_stone = 41,
    free_upkeep = 42,
    pope_approval = 43,
    pope_disapproval = 44,
    religion_level = 45,
    amplify_religion_level = 46,
    archer_bonus = 47,
    cavalry_bonus = 48,
    heavy_cavalry_bonus = 49,
    gun_bonus = 50,
    navy_bonus = 51,
    recruitment_cost_bonus_naval = 52,
    retrain_cost_bonus = 53,
    weapon_projectile = 54,
    income_bonus = 55,
    recruitment_slots = 56

	building = EDB.getBuildingByName("market")

	-- Add a population growth bonus to the market building
	EDB.addBuildingCapability(building, 0, 0, 5, true);

	-- Add a 500 income bonus to the market building
	EDB.addBuildingCapability(building, 0, 55, 500, true)
	*/
	tables.EDB.set_function("addBuildingCapability", &buildingStructHelpers::addBuildingCapability);

	/***
	Remove a capability from a building.
	@function EDB.removeBuildingCapability
	@tparam edbEntry edbEntry Entry to set.
	@tparam int level Building level.
	@tparam int index Which capability to remove (In order of iterating).
	@usage
	building = EDB.getBuildingByName("market")
	EDB.removeBuildingCapability(building, 0, 3);
	*/
	tables.EDB.set_function("removeBuildingCapability", &buildingStructHelpers::removeBuildingCapability);

	/***

	Get capability from a building at an index.
	@function EDB.getBuildingCapability
	@tparam edbEntry edbEntry Entry to set.
	@tparam int level Building level.
	@tparam int index.
	@treturn capability capability.
	@usage
	building = EDB.getBuildingByName("market")
	cap = EDB.getBuildingCapability(building, 0, 3);
	*/
	tables.EDB.set_function("getBuildingCapability", &buildingStructHelpers::getBuildingCapability);


	/***

	Get capability amount from a building.
	@function EDB.getBuildingCapabilityNum
	@tparam edbEntry edbEntry Entry to set.
	@tparam int level Building level.
	@treturn int capabilityNum.
	@usage
	building = EDB.getBuildingByName("market")
	EDB.getBuildingCapabilityNum(building, 0);
	*/
	tables.EDB.set_function("getBuildingCapabilityNum", &buildingStructHelpers::getBuildingCapabilityNum);

	/***
	Add a recruitment pool to a building.
	@function EDB.addBuildingPool
	@tparam edbEntry edbEntry Entry to set.
	@tparam int level Building level.
	@tparam int eduIndex edu index of unit to add (EOP units not supported!).
	@tparam float initialSize Initial pool.
	@tparam float gainPerTurn Replenishment per turn.
	@tparam float maxSize Maximum size.
	@tparam int exp Initial experience.
	@usage
	building = EDB.getBuildingByName("market")
	EDB.addBuildingPool(building, 0, 55, 1, 0.1, 2, 0);
	*/
	tables.EDB.set_function("addBuildingPool", &buildingStructHelpers::addBuildingPool);


	/***
	Remove a recruitment pool from a building.
	@function EDB.removeBuildingPool
	@tparam edbEntry edbEntry Entry to set.
	@tparam int level Building level.
	@tparam int index Which pool to remove (In order of iterating).
	@usage
	building = EDB.getBuildingByName("market")
	EDB.removeBuildingPool(building, 0, 3);
	*/
	tables.EDB.set_function("removeBuildingPool", &buildingStructHelpers::removeBuildingPool);


	/***
	Get a recruitment pool from a building by index.
	@function EDB.getBuildingPool
	@tparam edbEntry edbEntry Entry to set.
	@tparam int level Building level.
	@tparam int index Which pool to get (In order of iterating).
	@treturn recruitpool pool.
	@usage
	building = EDB.getBuildingByName("market")
	EDB.getBuildingPool(building, 0, 3);
	*/
	tables.EDB.set_function("getBuildingPool", &buildingStructHelpers::getBuildingPool);


	/***
	Get a recruitment pool count.
	@function EDB.getBuildingPoolNum
	@tparam edbEntry edbEntry Entry to set.
	@tparam int level Building level.
	@treturn int poolNum.
	@usage
	building = EDB.getBuildingByName("market")
	EDB.getBuildingPoolNum(building, 0);
	*/
	tables.EDB.set_function("getBuildingPoolNum", &buildingStructHelpers::getBuildingPoolNum);


	//tables.EDB.set_function("createEOPBuilding", &buildingStructHelpers::createEOPBuilding);


	/***
	Get a building edb entry by name.
	@function EDB.getBuildingByName
	@tparam string buildingname
	@treturn edbEntry entry
	@usage
	building = EDB.getBuildingByName("market")
	*/
	tables.EDB.set_function("getBuildingByName", &buildingStructHelpers::getBuildingByName);
}