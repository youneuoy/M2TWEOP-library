///
//![Lua logo](../Lua.png)
//@module LuaPlugin
//@author Fynn
//@license GPL-3.0
#include "pch.h"
#include "campaignDb.h"

#include "dataOffsets.h"
#include "luaPlugin.h"

namespace campaignHelpers
{
	campaignDbExtra* getCampaignDbExtra()
	{
		return reinterpret_cast<campaignDbExtra*>(dataOffsets::offsets.campaignDbExtra);
	}

	campaignDb* getCampaignDb()
	{
		return reinterpret_cast<campaignDb*>(dataOffsets::offsets.campaignDb);
	}
}

void luaPlugin::initCampaignDb()
{
	struct
	{
		sol::usertype<campaignDbRecruitment> campaignDbRecruitment;
		sol::usertype<campaignDbReligion> campaignDbReligion;
		sol::usertype<campaignDbBribery> campaignDbBribery;
		sol::usertype<campaignDbFamilyTree> campaignDbFamilyTree;
		sol::usertype<campaignDbDiplomacy> campaignDbDiplomacy;
		sol::usertype<campaignDbDisplay> campaignDbDisplay;
		sol::usertype<campaignDbRansom> campaignDbRansom;
		sol::usertype<campaignDbAutoresolve> campaignDbAutoresolve;
		sol::usertype<campaignDbSettlement> campaignDbSettlement;
		sol::usertype<campaignDbRevolt> campaignDbRevolt;
		sol::usertype<campaignDbHorde> campaignDbHorde;
		sol::usertype<campaignDbMerchants> campaignDbMerchants;
		sol::usertype<campaignDbAgents> campaignDbAgents;
		sol::usertype<campaignDbCrusades> campaignDbCrusades;
		sol::usertype<campaignDbAi> campaignDbAi;
		sol::usertype<campaignDbMisc> campaignDbMisc;
		sol::usertype<campaignDb> campaignDb;
		sol::usertype<campaignDbExtra> campaignDbExtra;
	}typeAll;
	
	///Campaign DB
	//@section Campaign DB

	/***
	Basic campaignDb Recruitment table

	@tfield int recruitmentSlots
	@tfield int retrainingSlots
	@tfield bool deplenishPoolsWithCaps
	@tfield float deplenishMultiplier
	@tfield float deplenishOffset
	@tfield bool addDisbandNoCaps
	@tfield int percentagePoolReductionLost
	@tfield int percentagePoolReductionOccupy
	@tfield int percentagePoolReductionSack
	@tfield int percentagePoolReductionExterminate
	@tfield int maxAgentsPerTurn

	@table campaignDbRecruitment
	*/
	
	typeAll.campaignDbRecruitment = luaState.new_usertype<campaignDbRecruitment>("campaignDbRecruitment");
	typeAll.campaignDbRecruitment.set("recruitmentSlots", &campaignDbRecruitment::recruitmentSlots);
	typeAll.campaignDbRecruitment.set("retrainingSlots", &campaignDbRecruitment::retrainingSlots);
	typeAll.campaignDbRecruitment.set("deplenishPoolsWithCaps", &campaignDbRecruitment::deplenishPoolsWithCaps);
	typeAll.campaignDbRecruitment.set("deplenishMultiplier", &campaignDbRecruitment::deplenishMultiplier);
	typeAll.campaignDbRecruitment.set("deplenishOffset", &campaignDbRecruitment::deplenishOffset);
	typeAll.campaignDbRecruitment.set("addDisbandNoCaps", &campaignDbRecruitment::addDisbandNoCaps);
	typeAll.campaignDbRecruitment.set("percentagePoolReductionLost", &campaignDbRecruitment::percentagePoolReductionLost);
	typeAll.campaignDbRecruitment.set("percentagePoolReductionOccupy", &campaignDbRecruitment::percentagePoolReductionOccupy);
	typeAll.campaignDbRecruitment.set("percentagePoolReductionSack", &campaignDbRecruitment::percentagePoolReductionSack);
	typeAll.campaignDbRecruitment.set("percentagePoolReductionExterminate", &campaignDbRecruitment::percentagePoolReductionExterminate);
	typeAll.campaignDbRecruitment.set("maxAgentsPerTurn", &campaignDbRecruitment::maxAgentsPerTurn);

	/***
	Basic campaignDb Religion table

	@tfield int maxWitchesPerRegion
	@tfield int maxWitches
	@tfield int maxHereticsPerRegion
	@tfield int maxHeretics
	@tfield int maxInquisitorsPerRegion
	@tfield int maxInquisitors
	@tfield float maxHereticsConversionModifier
	@tfield float hereticConversionRateModifier
	@tfield float hereticConversionRateOffset
	@tfield float witchConversionRateOffset
	@tfield float inquisitorConversionRateModifier
	@tfield float inquisitorConversionRateOffset
	@tfield float priestConversionRateModifier
	@tfield float priestConversionRateOffset
	@tfield float witchChanceModifier
	@tfield float hereticChanceModifier
	@tfield float inquisitorChanceModifier
	@tfield int minCardinalPiety
	@tfield float convertToHereticBaseModifier
	@tfield float convertToHereticUnorthodoxModifier
	@tfield int inquisitorTurnStart

	@table campaignDbReligion
	*/
	typeAll.campaignDbReligion = luaState.new_usertype<campaignDbReligion>("campaignDbReligion");
	typeAll.campaignDbReligion.set("maxWitchesPerRegion", &campaignDbReligion::maxWitchesPerRegion);
	typeAll.campaignDbReligion.set("maxWitches", &campaignDbReligion::maxWitches);
	typeAll.campaignDbReligion.set("maxHereticsPerRegion", &campaignDbReligion::maxHereticsPerRegion);
	typeAll.campaignDbReligion.set("maxHeretics", &campaignDbReligion::maxHeretics);
	typeAll.campaignDbReligion.set("maxInquisitorsPerRegion", &campaignDbReligion::maxInquisitorsPerRegion);
	typeAll.campaignDbReligion.set("maxInquisitors", &campaignDbReligion::maxInquisitors);
	typeAll.campaignDbReligion.set("maxHereticsConversionModifier", &campaignDbReligion::maxHereticsConversionModifier);
	typeAll.campaignDbReligion.set("hereticConversionRateModifier", &campaignDbReligion::hereticConversionRateModifier);
	typeAll.campaignDbReligion.set("hereticConversionRateOffset", &campaignDbReligion::hereticConversionRateOffset);
	typeAll.campaignDbReligion.set("witchConversionRateOffset", &campaignDbReligion::witchConversionRateOffset);
	typeAll.campaignDbReligion.set("inquisitorConversionRateModifier", &campaignDbReligion::inquisitorConversionRateModifier);
	typeAll.campaignDbReligion.set("inquisitorConversionRateOffset", &campaignDbReligion::inquisitorConversionRateOffset);
	typeAll.campaignDbReligion.set("priestConversionRateModifier", &campaignDbReligion::priestConversionRateModifier);
	typeAll.campaignDbReligion.set("priestConversionRateOffset", &campaignDbReligion::priestConversionRateOffset);
	typeAll.campaignDbReligion.set("witchChanceModifier", &campaignDbReligion::witchChanceModifier);
	typeAll.campaignDbReligion.set("hereticChanceModifier", &campaignDbReligion::hereticChanceModifier);
	typeAll.campaignDbReligion.set("inquisitorChanceModifier", &campaignDbReligion::inquisitorChanceModifier);
	typeAll.campaignDbReligion.set("minCardinalPiety", &campaignDbReligion::minCardinalPiety);
	typeAll.campaignDbReligion.set("convertToHereticBaseModifier", &campaignDbReligion::convertToHereticBaseModifier);
	typeAll.campaignDbReligion.set("convertToHereticUnorthodoxModifier", &campaignDbReligion::convertToHereticUnorthodoxModifier);
	typeAll.campaignDbReligion.set("inquisitorTurnStart", &campaignDbReligion::inquisitorTurnStart);
	typeAll.campaignDbReligion.set("inquisitorTurnStart", &campaignDbReligion::inquisitorTurnStart);

	/***
	Basic campaignDb Bribery table

	@tfield float baseCharacterChance
	@tfield float religionModifier
	@tfield float combinedAttributeModifier
	@tfield float briberAttributeDivisor
	@tfield float bribeeAttributeDivisor
	@tfield float armySizeModifier
	@tfield float baseSettlementChance
	@tfield float settlementLoyaltyModifier
	@tfield float settlementPopulationModifier
	@tfield float factionStandingDivisor
	@tfield float maxBribeChance
	@tfield float minBribeChance
	@tfield float bribeChanceModifier

	@table campaignDbBribery
	*/
	typeAll.campaignDbBribery = luaState.new_usertype<campaignDbBribery>("campaignDbBribery");
	typeAll.campaignDbBribery.set("baseCharacterChance", &campaignDbBribery::baseCharacterChance);
	typeAll.campaignDbBribery.set("religionModifier", &campaignDbBribery::religionModifier);
	typeAll.campaignDbBribery.set("combinedAttributeModifier", &campaignDbBribery::combinedAttributeModifier);
	typeAll.campaignDbBribery.set("briberAttributeDivisor", &campaignDbBribery::briberAttributeDivisor);
	typeAll.campaignDbBribery.set("bribeeAttributeDivisor", &campaignDbBribery::bribeeAttributeDivisor);
	typeAll.campaignDbBribery.set("armySizeModifier", &campaignDbBribery::armySizeModifier);
	typeAll.campaignDbBribery.set("baseSettlementChance", &campaignDbBribery::baseSettlementChance);
	typeAll.campaignDbBribery.set("settlementLoyaltyModifier", &campaignDbBribery::settlementLoyaltyModifier);
	typeAll.campaignDbBribery.set("settlementPopulationModifier", &campaignDbBribery::settlementPopulationModifier);
	typeAll.campaignDbBribery.set("factionStandingDivisor", &campaignDbBribery::factionStandingDivisor);
	typeAll.campaignDbBribery.set("maxBribeChance", &campaignDbBribery::maxBribeChance);
	typeAll.campaignDbBribery.set("minBribeChance", &campaignDbBribery::minBribeChance);
	typeAll.campaignDbBribery.set("bribeChanceModifier", &campaignDbBribery::bribeChanceModifier);

	/***
	Basic campaignDb Family Tree table

	@tfield int maxAge
	@tfield int maxAgeForMarriageMale
	@tfield int maxAgeForMarriageForFemale
	@tfield int maxAgeBeforeDeath
	@tfield int maxAgeOfChild
	@tfield int oldAge
	@tfield int ageOfManhood
	@tfield int daughtersAgeOfConsent
	@tfield int daughtersRetirementAge
	@tfield int ageDifferenceMin
	@tfield int ageDifferenceMax
	@tfield int parentToChildMinAgeDiff
	@tfield int minAdoptionAge
	@tfield int maxAdoptionAge
	@tfield int maxAgeForConception
	@tfield int ageOfManhoodClose
	@tfield int maxNumberOfChildren

	@table campaignDbFamilyTree
	*/
	typeAll.campaignDbFamilyTree = luaState.new_usertype<campaignDbFamilyTree>("campaignDbFamilyTree");
	typeAll.campaignDbFamilyTree.set("maxAge", &campaignDbFamilyTree::maxAge);
	typeAll.campaignDbFamilyTree.set("maxAgeForMarriageMale", &campaignDbFamilyTree::maxAgeForMarriageMale);
	typeAll.campaignDbFamilyTree.set("maxAgeForMarriageForFemale", &campaignDbFamilyTree::maxAgeForMarriageForFemale);
	typeAll.campaignDbFamilyTree.set("maxAgeBeforeDeath", &campaignDbFamilyTree::maxAgeBeforeDeath);
	typeAll.campaignDbFamilyTree.set("maxAgeOfChild", &campaignDbFamilyTree::maxAgeOfChild);
	typeAll.campaignDbFamilyTree.set("oldAge", &campaignDbFamilyTree::oldAge);
	typeAll.campaignDbFamilyTree.set("ageOfManhood", &campaignDbFamilyTree::ageOfManhood);
	typeAll.campaignDbFamilyTree.set("daughtersAgeOfConsent", &campaignDbFamilyTree::daughtersAgeOfConsent);
	typeAll.campaignDbFamilyTree.set("daughtersRetirementAge", &campaignDbFamilyTree::daughtersRetirementAge);
	typeAll.campaignDbFamilyTree.set("ageDifferenceMin", &campaignDbFamilyTree::ageDifferenceMin);
	typeAll.campaignDbFamilyTree.set("ageDifferenceMax", &campaignDbFamilyTree::ageDifferenceMax);
	typeAll.campaignDbFamilyTree.set("parentToChildMinAgeDiff", &campaignDbFamilyTree::parentToChildMinAgeDiff);
	typeAll.campaignDbFamilyTree.set("minAdoptionAge", &campaignDbFamilyTree::minAdoptionAge);
	typeAll.campaignDbFamilyTree.set("maxAdoptionAge", &campaignDbFamilyTree::maxAdoptionAge);
	typeAll.campaignDbFamilyTree.set("maxAgeForConception", &campaignDbFamilyTree::maxAgeForConception);
	typeAll.campaignDbFamilyTree.set("ageOfManhoodClose", &campaignDbFamilyTree::ageOfManhoodClose);
	typeAll.campaignDbFamilyTree.set("maxNumberOfChildren", &campaignDbFamilyTree::maxNumberOfChildren);

	/***
	Basic campaignDb Diplomacy table

	@tfield int maxDiplomacyItems
	@tfield int nullMissionScore

	@table campaignDbDiplomacy
	*/
	typeAll.campaignDbDiplomacy = luaState.new_usertype<campaignDbDiplomacy>("campaignDbDiplomacy");
	typeAll.campaignDbDiplomacy.set("maxDiplomacyItems", &campaignDbDiplomacy::maxDiplomacyItems);
	typeAll.campaignDbDiplomacy.set("nullMissionScore", &campaignDbDiplomacy::nullMissionScore);

	/***
	Basic campaignDb Display table

	@tfield float characterSelectionRadius
	@tfield float characterSelectionHeight
	@tfield float characterSelectionHeightCrouching
	@tfield float diplomacyScrollHeight
	@tfield float factionStandingMin
	@tfield float factionStandingMax
	@tfield bool UseOrigRebelFactionModels
	@tfield int hudTabTextOffset
	@tfield bool useFactionCreatorSettModels
	@tfield int standardSoldierLimit
	@tfield int standardSoldierLevelScale
	@tfield bool clearBattleModelsOnNewFaction

	@table campaignDbDisplay
	*/
	typeAll.campaignDbDisplay = luaState.new_usertype<campaignDbDisplay>("campaignDbDisplay");
	typeAll.campaignDbDisplay.set("characterSelectionRadius", &campaignDbDisplay::characterSelectionRadius);
	typeAll.campaignDbDisplay.set("characterSelectionHeight", &campaignDbDisplay::characterSelectionHeight);
	typeAll.campaignDbDisplay.set("characterSelectionHeightCrouching", &campaignDbDisplay::characterSelectionHeightCrouching);
	typeAll.campaignDbDisplay.set("diplomacyScrollHeight", &campaignDbDisplay::diplomacyScrollHeight);
	typeAll.campaignDbDisplay.set("factionStandingMin", &campaignDbDisplay::factionStandingMin);
	typeAll.campaignDbDisplay.set("factionStandingMax", &campaignDbDisplay::factionStandingMax);
	typeAll.campaignDbDisplay.set("UseOrigRebelFactionModels", &campaignDbDisplay::UseOrigRebelFactionModels);
	typeAll.campaignDbDisplay.set("hudTabTextOffset", &campaignDbDisplay::hudTabTextOffset);
	typeAll.campaignDbDisplay.set("useFactionCreatorSettModels", &campaignDbDisplay::useFactionCreatorSettModels);
	typeAll.campaignDbDisplay.set("standardSoldierLimit", &campaignDbDisplay::standardSoldierLimit);
	typeAll.campaignDbDisplay.set("standardSoldierLevelScale", &campaignDbDisplay::standardSoldierLevelScale);
	typeAll.campaignDbDisplay.set("clearBattleModelsOnNewFaction", &campaignDbDisplay::clearBattleModelsOnNewFaction);

	/***
	Basic campaignDb Ransom table

	@tfield float captorReleaseChanceBase
	@tfield float captorReleaseChanceChivMod
	@tfield float captorRansomChanceBase
	@tfield float captorRansomChanceChivMod
	@tfield float captorRansomChanceTmMod
	@tfield float captiveRansomChanceBase
	@tfield float captiveRansomChanceChivMod
	@tfield float captiveRansomChanceTmMod
	@tfield float captiveRansomChanceMsmMod

	@table campaignDbRansom
	*/
	typeAll.campaignDbRansom = luaState.new_usertype<campaignDbRansom>("campaignDbRansom");
	typeAll.campaignDbRansom.set("captorReleaseChanceBase", &campaignDbRansom::captorReleaseChanceBase);
	typeAll.campaignDbRansom.set("captorReleaseChanceChivMod", &campaignDbRansom::captorReleaseChanceChivMod);
	typeAll.campaignDbRansom.set("captorRansomChanceBase", &campaignDbRansom::captorRansomChanceBase);
	typeAll.campaignDbRansom.set("captorRansomChanceChivMod", &campaignDbRansom::captorRansomChanceChivMod);
	typeAll.campaignDbRansom.set("captorRansomChanceTmMod", &campaignDbRansom::captorRansomChanceTmMod);
	typeAll.campaignDbRansom.set("captiveRansomChanceBase", &campaignDbRansom::captiveRansomChanceBase);
	typeAll.campaignDbRansom.set("captiveRansomChanceChivMod", &campaignDbRansom::captiveRansomChanceChivMod);
	typeAll.campaignDbRansom.set("captiveRansomChanceTmMod", &campaignDbRansom::captiveRansomChanceTmMod);
	typeAll.campaignDbRansom.set("captiveRansomChanceMsmMod", &campaignDbRansom::captiveRansomChanceMsmMod);

	/***
	Basic campaignDb Autoresolve table

	@tfield float minCapturePercent
	@tfield float maxCapturePercent
	@tfield float lopsidedThresh
	@tfield float lopsidedHnMod
	@tfield int separationMissileAdd
	@tfield float navalSinkModifier
	@tfield float navalSinkOffset
	@tfield float navalSinkMax
	@tfield float sallyAtDefDrawDivisor
	@tfield bool useNewSettAutoResolve
	@tfield int gateDefenceNumOilAttacks
	@tfield float gateDefenceStrengthOilBase
	@tfield int gateDefenceNumArrowAttacks
	@tfield float gateDefenceStrengthArrowBase
	@tfield float gateDefenceStrengthArrowLevelModifier
	@tfield int gateDefenceNumDefaultAttacks
	@tfield float gateDefenceStrengthDefaultBase
	@tfield float gateDefenceStrengthDefaultLevelModifier
	@tfield int settDefenceNumArrowAttacks
	@tfield float settDefenceStrengthArrowBase
	@tfield float settDefenceStrengthArrowModifier
	@tfield float settDefenceStrengthDefaultBase
	@tfield float settDefenceStrengthDefaultModifier
	@tfield float displayStrengthOil
	@tfield float displayStrengthArrow
	@tfield float displayStrengthDefault

	@table campaignDbAutoresolve
	*/
	typeAll.campaignDbAutoresolve = luaState.new_usertype<campaignDbAutoresolve>("campaignDbAutoresolve");
	typeAll.campaignDbAutoresolve.set("minCapturePercent", &campaignDbAutoresolve::minCapturePercent);
	typeAll.campaignDbAutoresolve.set("maxCapturePercent", &campaignDbAutoresolve::maxCapturePercent);
	typeAll.campaignDbAutoresolve.set("lopsidedThresh", &campaignDbAutoresolve::lopsidedThresh);
	typeAll.campaignDbAutoresolve.set("lopsidedHnMod", &campaignDbAutoresolve::lopsidedHnMod);
	typeAll.campaignDbAutoresolve.set("separationMissileAdd", &campaignDbAutoresolve::separationMissileAdd);
	typeAll.campaignDbAutoresolve.set("navalSinkModifier", &campaignDbAutoresolve::navalSinkModifier);
	typeAll.campaignDbAutoresolve.set("navalSinkOffset", &campaignDbAutoresolve::navalSinkOffset);
	typeAll.campaignDbAutoresolve.set("navalSinkMax", &campaignDbAutoresolve::navalSinkMax);
	typeAll.campaignDbAutoresolve.set("sallyAtDefDrawDivisor", &campaignDbAutoresolve::sallyAtDefDrawDivisor);
	typeAll.campaignDbAutoresolve.set("useNewSettAutoResolve", &campaignDbAutoresolve::useNewSettAutoResolve);
	typeAll.campaignDbAutoresolve.set("gateDefenceNumOilAttacks", &campaignDbAutoresolve::gateDefenceNumOilAttacks);
	typeAll.campaignDbAutoresolve.set("gateDefenceStrengthOilBase", &campaignDbAutoresolve::gateDefenceStrengthOilBase);
	typeAll.campaignDbAutoresolve.set("gateDefenceNumArrowAttacks", &campaignDbAutoresolve::gateDefenceNumArrowAttacks);
	typeAll.campaignDbAutoresolve.set("gateDefenceStrengthArrowBase", &campaignDbAutoresolve::gateDefenceStrengthArrowBase);
	typeAll.campaignDbAutoresolve.set("gateDefenceStrengthArrowLevelModifier", &campaignDbAutoresolve::gateDefenceStrengthArrowLevelModifier);
	typeAll.campaignDbAutoresolve.set("gateDefenceNumDefaultAttacks", &campaignDbAutoresolve::gateDefenceNumDefaultAttacks);
	typeAll.campaignDbAutoresolve.set("gateDefenceStrengthDefaultBase", &campaignDbAutoresolve::gateDefenceStrengthDefaultBase);
	typeAll.campaignDbAutoresolve.set("gateDefenceStrengthDefaultLevelModifier", &campaignDbAutoresolve::gateDefenceStrengthDefaultLevelModifier);
	typeAll.campaignDbAutoresolve.set("settDefenceNumArrowAttacks", &campaignDbAutoresolve::settDefenceNumArrowAttacks);
	typeAll.campaignDbAutoresolve.set("settDefenceStrengthArrowBase", &campaignDbAutoresolve::settDefenceStrengthArrowBase);
	typeAll.campaignDbAutoresolve.set("settDefenceStrengthArrowModifier", &campaignDbAutoresolve::settDefenceStrengthArrowModifier);
	typeAll.campaignDbAutoresolve.set("settDefenceStrengthDefaultBase", &campaignDbAutoresolve::settDefenceStrengthDefaultBase);
	typeAll.campaignDbAutoresolve.set("settDefenceStrengthDefaultModifier", &campaignDbAutoresolve::settDefenceStrengthDefaultModifier);
	typeAll.campaignDbAutoresolve.set("displayStrengthOil", &campaignDbAutoresolve::displayStrengthOil);
	typeAll.campaignDbAutoresolve.set("displayStrengthArrow", &campaignDbAutoresolve::displayStrengthArrow);
	typeAll.campaignDbAutoresolve.set("displayStrengthDefault", &campaignDbAutoresolve::displayStrengthDefault);

	/***
	Basic campaignDb Settlement table

	@tfield float sackMoneyModifier
	@tfield float exterminateMoneyModifier
	@tfield float chivSpfModifier
	@tfield float chivSofModifier
	@tfield float dreadSofModifier
	@tfield float pietyCorruptionModifier
	@tfield float pietyAdminSifModifier
	@tfield float portToPortMpMin
	@tfield float heresyUnrestNodifier
	@tfield float religionUnrestModifier
	@tfield int siegeGearRequiredForCityLevel
	@tfield int noTowersOnlyForCityLevel
	@tfield int minTurnKeepRebelGarrison
	@tfield bool destroyEmptyForts
	@tfield bool canBuildForts
	@tfield float raceGameCostsModifier
	@tfield float altRelAlliedModifier
	@tfield float altRelGovModifierBase
	@tfield float altRelGovCoefficient

	@table campaignDbSettlement
	*/
	typeAll.campaignDbSettlement = luaState.new_usertype<campaignDbSettlement>("campaignDbSettlement");
	typeAll.campaignDbSettlement.set("sackMoneyModifier", &campaignDbSettlement::sackMoneyModifier);
	typeAll.campaignDbSettlement.set("exterminateMoneyModifier", &campaignDbSettlement::exterminateMoneyModifier);
	typeAll.campaignDbSettlement.set("chivSpfModifier", &campaignDbSettlement::chivSpfModifier);
	typeAll.campaignDbSettlement.set("chivSofModifier", &campaignDbSettlement::chivSofModifier);
	typeAll.campaignDbSettlement.set("dreadSofModifier", &campaignDbSettlement::dreadSofModifier);
	typeAll.campaignDbSettlement.set("pietyCorruptionModifier", &campaignDbSettlement::pietyCorruptionModifier);
	typeAll.campaignDbSettlement.set("pietyAdminSifModifier", &campaignDbSettlement::pietyAdminSifModifier);
	typeAll.campaignDbSettlement.set("portToPortMpMin", &campaignDbSettlement::portToPortMpMin);
	typeAll.campaignDbSettlement.set("heresyUnrestNodifier", &campaignDbSettlement::heresyUnrestNodifier);
	typeAll.campaignDbSettlement.set("religionUnrestModifier", &campaignDbSettlement::religionUnrestModifier);
	typeAll.campaignDbSettlement.set("siegeGearRequiredForCityLevel", &campaignDbSettlement::siegeGearRequiredForCityLevel);
	typeAll.campaignDbSettlement.set("noTowersOnlyForCityLevel", &campaignDbSettlement::noTowersOnlyForCityLevel);
	typeAll.campaignDbSettlement.set("minTurnKeepRebelGarrison", &campaignDbSettlement::minTurnKeepRebelGarrison);
	typeAll.campaignDbSettlement.set("destroyEmptyForts", &campaignDbSettlement::destroyEmptyForts);
	typeAll.campaignDbSettlement.set("canBuildForts", &campaignDbSettlement::canBuildForts);
	typeAll.campaignDbSettlement.set("raceGameCostsModifier", &campaignDbSettlement::raceGameCostsModifier);
	typeAll.campaignDbSettlement.set("altRelAlliedModifier", &campaignDbSettlement::altRelAlliedModifier);
	typeAll.campaignDbSettlement.set("altRelGovModifierBase", &campaignDbSettlement::altRelGovModifierBase);
	typeAll.campaignDbSettlement.set("altRelGovCoefficient", &campaignDbSettlement::altRelGovCoefficient);

	/***
	Basic campaignDb Revolt table

	@tfield float endTurnModifier
	@tfield float excommunicatedModifier
	@tfield float newLeaderModifier
	@tfield float maxEffectiveLoyalty
	@tfield float rebelRegionModifier
	@tfield float shadowRegionModifier
	@tfield float rebelBorderModifier
	@tfield float shadowBorderModifier
	@tfield float numUnitsModifier
	@tfield float captainModifier
	@tfield float minRevoltChance
	@tfield float maxRevoltChance
	@tfield float aiRevoltModifier
	@tfield float shadowAuthorityModifier
	@tfield float shadowAuthorityModifierSett

	@table campaignDbRevolt
	*/
	typeAll.campaignDbRevolt = luaState.new_usertype<campaignDbRevolt>("campaignDbRevolt");
	typeAll.campaignDbRevolt.set("endTurnModifier", &campaignDbRevolt::endTurnModifier);
	typeAll.campaignDbRevolt.set("excommunicatedModifier", &campaignDbRevolt::excommunicatedModifier);
	typeAll.campaignDbRevolt.set("newLeaderModifier", &campaignDbRevolt::newLeaderModifier);
	typeAll.campaignDbRevolt.set("maxEffectiveLoyalty", &campaignDbRevolt::maxEffectiveLoyalty);
	typeAll.campaignDbRevolt.set("rebelRegionModifier", &campaignDbRevolt::rebelRegionModifier);
	typeAll.campaignDbRevolt.set("shadowRegionModifier", &campaignDbRevolt::shadowRegionModifier);
	typeAll.campaignDbRevolt.set("rebelBorderModifier", &campaignDbRevolt::rebelBorderModifier);
	typeAll.campaignDbRevolt.set("shadowBorderModifier", &campaignDbRevolt::shadowBorderModifier);
	typeAll.campaignDbRevolt.set("numUnitsModifier", &campaignDbRevolt::numUnitsModifier);
	typeAll.campaignDbRevolt.set("captainModifier", &campaignDbRevolt::captainModifier);
	typeAll.campaignDbRevolt.set("minRevoltChance", &campaignDbRevolt::minRevoltChance);
	typeAll.campaignDbRevolt.set("maxRevoltChance", &campaignDbRevolt::maxRevoltChance);
	typeAll.campaignDbRevolt.set("aiRevoltModifier", &campaignDbRevolt::aiRevoltModifier);
	typeAll.campaignDbRevolt.set("shadowAuthorityModifier", &campaignDbRevolt::shadowAuthorityModifier);
	typeAll.campaignDbRevolt.set("shadowAuthorityModifierSett", &campaignDbRevolt::shadowAuthorityModifierSett);

	/***
	Basic campaignDb Horde table

	@tfield int endTargetFactionBonus
	@tfield int startTargetFactionBonus
	@tfield int farmingLevelBonus
	@tfield int sharedTargetBonus
	@tfield int disbandingHordeBonus
	@tfield int hordeStartingRegionBonus
	@tfield int hordeTargetResourceBonus

	@table campaignDbHorde
	*/
	typeAll.campaignDbHorde = luaState.new_usertype<campaignDbHorde>("campaignDbHorde");
	typeAll.campaignDbHorde.set("endTargetFactionBonus", &campaignDbHorde::endTargetFactionBonus);
	typeAll.campaignDbHorde.set("startTargetFactionBonus", &campaignDbHorde::startTargetFactionBonus);
	typeAll.campaignDbHorde.set("farmingLevelBonus", &campaignDbHorde::farmingLevelBonus);
	typeAll.campaignDbHorde.set("sharedTargetBonus", &campaignDbHorde::sharedTargetBonus);
	typeAll.campaignDbHorde.set("disbandingHordeBonus", &campaignDbHorde::disbandingHordeBonus);
	typeAll.campaignDbHorde.set("hordeStartingRegionBonus", &campaignDbHorde::hordeStartingRegionBonus);
	typeAll.campaignDbHorde.set("hordeTargetResourceBonus", &campaignDbHorde::hordeTargetResourceBonus);

	/***
	Basic campaignDb Merchants table

	@tfield float baseIncomeModifier
	@tfield float tradeBonusOffset

	@table campaignDbMerchants
	*/
	typeAll.campaignDbMerchants = luaState.new_usertype<campaignDbMerchants>("campaignDbMerchants");
	typeAll.campaignDbMerchants.set("baseIncomeModifier", &campaignDbMerchants::baseIncomeModifier);
	typeAll.campaignDbMerchants.set("tradeBonusOffset", &campaignDbMerchants::tradeBonusOffset);

	/***
	Basic campaignDb Agents table

	@tfield float denounceInquisitorBaseChance
	@tfield float DenouncePriestBaseChance
	@tfield float denounceAttackModifier
	@tfield float denounceDefenceModifier
	@tfield int denounceChanceMax
	@tfield float assassinateBaseChance
	@tfield float assassinateAttackModifier
	@tfield float assassinateDefenceModifier
	@tfield float assassinatePublicModifier
	@tfield float assassinatePersonalModifier
	@tfield float assassinateCounterSpyModifier
	@tfield float assassinateAgentModifier
	@tfield float assassinateOwnRegionModifier
	@tfield float assassinateAssassinateAttrModifier
	@tfield int assassinateChanceMin
	@tfield int assassinateChanceMax
	@tfield float denounceHereticAttemptModifier
	@tfield float denounceCharacterAttemptModifier
	@tfield float acquisitionBaseChance
	@tfield float acquisitionLevelModifier
	@tfield float acquisitionAttackTradeRightsModifier
	@tfield float acquisitionDefenceTradeRightsModifier
	@tfield int acquisitionChanceMin
	@tfield int acquisitionChanceMax
	@tfield float inquisitorCrtHeresyDivisor
	@tfield float inquisitorCrtPfpModifier
	@tfield float inquisitorCrtPfpModifierMin
	@tfield float inquisitorCrtPfpModifierMax
	@tfield float inquisitorCrtChanceMax
	@tfield float spyBaseChance
	@tfield float spyLevelModifier
	@tfield float notSpyLevelModifier
	@tfield float spyPublicModifier
	@tfield float spyCounterSpyModifier
	@tfield float spyDistanceModifier
	@tfield float spySecretAgentTargetModifier
	@tfield float spySedentaryTurnsModifier
	@tfield float spyAllianceModifier
	@tfield float spyTargetEngagedModifier
	@tfield float spyInSettlementModifier
	@tfield float spyWatchtowerModifier
	@tfield float spyInOwnRegionModifier
	@tfield int spyChanceMin
	@tfield int spyChanceMax

	@table campaignDbAgents
	*/
	typeAll.campaignDbAgents = luaState.new_usertype<campaignDbAgents>("campaignDbAgents");
	typeAll.campaignDbAgents.set("denounceInquisitorBaseChance", &campaignDbAgents::denounceInquisitorBaseChance);
	typeAll.campaignDbAgents.set("DenouncePriestBaseChance", &campaignDbAgents::DenouncePriestBaseChance);
	typeAll.campaignDbAgents.set("denounceAttackModifier", &campaignDbAgents::denounceAttackModifier);
	typeAll.campaignDbAgents.set("denounceDefenceModifier", &campaignDbAgents::denounceDefenceModifier);
	typeAll.campaignDbAgents.set("denounceChanceMax", &campaignDbAgents::denounceChanceMax);
	typeAll.campaignDbAgents.set("assassinateBaseChance", &campaignDbAgents::assassinateBaseChance);
	typeAll.campaignDbAgents.set("assassinateAttackModifier", &campaignDbAgents::assassinateAttackModifier);
	typeAll.campaignDbAgents.set("assassinateDefenceModifier", &campaignDbAgents::assassinateDefenceModifier);
	typeAll.campaignDbAgents.set("assassinatePublicModifier", &campaignDbAgents::assassinatePublicModifier);
	typeAll.campaignDbAgents.set("assassinatePersonalModifier", &campaignDbAgents::assassinatePersonalModifier);
	typeAll.campaignDbAgents.set("assassinateCounterSpyModifier", &campaignDbAgents::assassinateCounterSpyModifier);
	typeAll.campaignDbAgents.set("assassinateAgentModifier", &campaignDbAgents::assassinateAgentModifier);
	typeAll.campaignDbAgents.set("assassinateOwnRegionModifier", &campaignDbAgents::assassinateOwnRegionModifier);
	typeAll.campaignDbAgents.set("assassinateAssassinateAttrModifier", &campaignDbAgents::assassinateAssassinateAttrModifier);
	typeAll.campaignDbAgents.set("assassinateChanceMin", &campaignDbAgents::assassinateChanceMin);
	typeAll.campaignDbAgents.set("assassinateChanceMax", &campaignDbAgents::assassinateChanceMax);
	typeAll.campaignDbAgents.set("denounceHereticAttemptModifier", &campaignDbAgents::denounceHereticAttemptModifier);
	typeAll.campaignDbAgents.set("denounceCharacterAttemptModifier", &campaignDbAgents::denounceCharacterAttemptModifier);
	typeAll.campaignDbAgents.set("acquisitionBaseChance", &campaignDbAgents::acquisitionBaseChance);
	typeAll.campaignDbAgents.set("acquisitionLevelModifier", &campaignDbAgents::acquisitionLevelModifier);
	typeAll.campaignDbAgents.set("acquisitionAttackTradeRightsModifier", &campaignDbAgents::acquisitionAttackTradeRightsModifier);
	typeAll.campaignDbAgents.set("acquisitionDefenceTradeRightsModifier", &campaignDbAgents::acquisitionDefenceTradeRightsModifier);
	typeAll.campaignDbAgents.set("acquisitionChanceMin", &campaignDbAgents::acquisitionChanceMin);
	typeAll.campaignDbAgents.set("acquisitionChanceMax", &campaignDbAgents::acquisitionChanceMax);
	typeAll.campaignDbAgents.set("inquisitorCrtHeresyDivisor", &campaignDbAgents::inquisitorCrtHeresyDivisor);
	typeAll.campaignDbAgents.set("inquisitorCrtPfpModifier", &campaignDbAgents::inquisitorCrtPfpModifier);
	typeAll.campaignDbAgents.set("inquisitorCrtPfpModifierMin", &campaignDbAgents::inquisitorCrtPfpModifierMin);
	typeAll.campaignDbAgents.set("inquisitorCrtPfpModifierMax", &campaignDbAgents::inquisitorCrtPfpModifierMax);
	typeAll.campaignDbAgents.set("inquisitorCrtChanceMax", &campaignDbAgents::inquisitorCrtChanceMax);
	typeAll.campaignDbAgents.set("spyBaseChance", &campaignDbAgents::spyBaseChance);
	typeAll.campaignDbAgents.set("spyLevelModifier", &campaignDbAgents::spyLevelModifier);
	typeAll.campaignDbAgents.set("notSpyLevelModifier", &campaignDbAgents::notSpyLevelModifier);
	typeAll.campaignDbAgents.set("spyPublicModifier", &campaignDbAgents::spyPublicModifier);
	typeAll.campaignDbAgents.set("spyCounterSpyModifier", &campaignDbAgents::spyCounterSpyModifier);
	typeAll.campaignDbAgents.set("spyDistanceModifier", &campaignDbAgents::spyDistanceModifier);
	typeAll.campaignDbAgents.set("spySecretAgentTargetModifier", &campaignDbAgents::spySecretAgentTargetModifier);
	typeAll.campaignDbAgents.set("spySedentaryTurnsModifier", &campaignDbAgents::spySedentaryTurnsModifier);
	typeAll.campaignDbAgents.set("spyAllianceModifier", &campaignDbAgents::spyAllianceModifier);
	typeAll.campaignDbAgents.set("spyTargetEngagedModifier", &campaignDbAgents::spyTargetEngagedModifier);
	typeAll.campaignDbAgents.set("spyInSettlementModifier", &campaignDbAgents::spyInSettlementModifier);
	typeAll.campaignDbAgents.set("spyWatchtowerModifier", &campaignDbAgents::spyWatchtowerModifier);
	typeAll.campaignDbAgents.set("spyInOwnRegionModifier", &campaignDbAgents::spyInOwnRegionModifier);
	typeAll.campaignDbAgents.set("spyChanceMin", &campaignDbAgents::spyChanceMin);
	typeAll.campaignDbAgents.set("spyChanceMax", &campaignDbAgents::spyChanceMax);

	/***
	Basic campaignDb Crusades table

	@tfield int requiredJihadPiety
	@tfield float maxDisbandProgress
	@tfield float nearTargetNoDisbandDistance
	@tfield int disbandProgressWindow
	@tfield int crusadeCalledStartTurn
	@tfield int jihadCalledStartTurn
	@tfield float movementPointsModifier

	@table campaignDbCrusades
	*/
	typeAll.campaignDbCrusades = luaState.new_usertype<campaignDbCrusades>("campaignDbCrusades");
	typeAll.campaignDbCrusades.set("requiredJihadPiety", &campaignDbCrusades::requiredJihadPiety);
	typeAll.campaignDbCrusades.set("maxDisbandProgress", &campaignDbCrusades::maxDisbandProgress);
	typeAll.campaignDbCrusades.set("nearTargetNoDisbandDistance", &campaignDbCrusades::nearTargetNoDisbandDistance);
	typeAll.campaignDbCrusades.set("disbandProgressWindow", &campaignDbCrusades::disbandProgressWindow);
	typeAll.campaignDbCrusades.set("crusadeCalledStartTurn", &campaignDbCrusades::crusadeCalledStartTurn);
	typeAll.campaignDbCrusades.set("jihadCalledStartTurn", &campaignDbCrusades::jihadCalledStartTurn);
	typeAll.campaignDbCrusades.set("movementPointsModifier", &campaignDbCrusades::movementPointsModifier);

	/***
	Basic campaignDb Ai table

	@tfield float priestReligionMin
	@tfield float priestReligionMax
	@tfield float priestHeresyMin
	@tfield float priestHeresyMax
	@tfield float priestReligionExport
	@tfield float priestMaxProdTurns
	@tfield int merchantMinSurvivalAcquire
	@tfield float attStrModifier
	@tfield float siegeAttStrModifier
	@tfield float crusadeAttStrModifier
	@tfield float sallyAttStrModifier
	@tfield float ambushAttStrModifier
	@tfield float strLimitWeak
	@tfield float strLimitStrong

	@table campaignDbAi
	*/
	typeAll.campaignDbAi = luaState.new_usertype<campaignDbAi>("campaignDbAi");
	typeAll.campaignDbAi.set("priestReligionMin", &campaignDbAi::priestReligionMin);
	typeAll.campaignDbAi.set("priestReligionMax", &campaignDbAi::priestReligionMax);
	typeAll.campaignDbAi.set("priestHeresyMin", &campaignDbAi::priestHeresyMin);
	typeAll.campaignDbAi.set("priestHeresyMax", &campaignDbAi::priestHeresyMax);
	typeAll.campaignDbAi.set("priestReligionExport", &campaignDbAi::priestReligionExport);
	typeAll.campaignDbAi.set("priestMaxProdTurns", &campaignDbAi::priestMaxProdTurns);
	typeAll.campaignDbAi.set("merchantMinSurvivalAcquire", &campaignDbAi::merchantMinSurvivalAcquire);
	typeAll.campaignDbAi.set("attStrModifier", &campaignDbAi::attStrModifier);
	typeAll.campaignDbAi.set("siegeAttStrModifier", &campaignDbAi::siegeAttStrModifier);
	typeAll.campaignDbAi.set("crusadeAttStrModifier", &campaignDbAi::crusadeAttStrModifier);
	typeAll.campaignDbAi.set("sallyAttStrModifier", &campaignDbAi::sallyAttStrModifier);
	typeAll.campaignDbAi.set("ambushAttStrModifier", &campaignDbAi::ambushAttStrModifier);
	typeAll.campaignDbAi.set("strLimitWeak", &campaignDbAi::strLimitWeak);
	typeAll.campaignDbAi.set("strLimitStrong", &campaignDbAi::strLimitStrong);

	/***
	Basic campaignDb Misc table

	@tfield int fortDevastationDistance
	@tfield int armyDevastationDistance
	@tfield int fortDevastationModifier
	@tfield int armyDevastationModifier
	@tfield bool allowEnemyForts
	@tfield int siegeMovementPointsModifier
	@tfield int cavalryMovementPointsModifier

	@table campaignDbMisc
	*/
	typeAll.campaignDbMisc = luaState.new_usertype<campaignDbMisc>("campaignDbMisc");
	typeAll.campaignDbMisc.set("fortDevastationDistance", &campaignDbMisc::fortDevastationDistance);
	typeAll.campaignDbMisc.set("armyDevastationDistance", &campaignDbMisc::armyDevastationDistance);
	typeAll.campaignDbMisc.set("fortDevastationModifier", &campaignDbMisc::fortDevastationModifier);
	typeAll.campaignDbMisc.set("armyDevastationModifier", &campaignDbMisc::armyDevastationModifier);
	typeAll.campaignDbMisc.set("allowEnemyForts", &campaignDbMisc::allowEnemyForts);
	typeAll.campaignDbMisc.set("siegeMovementPointsModifier", &campaignDbMisc::siegeMovementPointsModifier);
	typeAll.campaignDbMisc.set("cavalryMovementPointsModifier", &campaignDbMisc::cavalryMovementPointsModifier);

	/***
	Basic campaignDb table

	@tfield campaignDbRecruitment recruitment
	@tfield campaignDbReligion religion
	@tfield campaignDbBribery bribery
	@tfield campaignDbFamilyTree familyTree
	@tfield campaignDbDiplomacy diplomacy
	@tfield campaignDbDisplay display
	@tfield campaignDbRansom ransom
	@tfield campaignDbAutoresolve autoResolve
	@tfield campaignDbSettlement settlement
	@tfield campaignDbRevolt revolt
	@tfield campaignDbHorde horde
	@tfield campaignDbMerchants merchants
	@tfield campaignDbAgents agents
	@tfield campaignDbCrusades crusades
	@tfield campaignDbAi ai
	@tfield campaignDbMisc misc

	@table campaignDb
	*/
	typeAll.campaignDb = luaState.new_usertype<campaignDb>("campaignDb");
	typeAll.campaignDb.set("recruitment", &campaignDb::campaignDbRecruitment);
	typeAll.campaignDb.set("religion", &campaignDb::campaignDbReligion);
	typeAll.campaignDb.set("bribery", &campaignDb::campaignDbBribery);
	typeAll.campaignDb.set("familyTree", &campaignDb::campaignDbFamilyTree);
	typeAll.campaignDb.set("diplomacy", &campaignDb::campaignDbDiplomacy);
	typeAll.campaignDb.set("display", &campaignDb::campaignDbDisplay);
	typeAll.campaignDb.set("ransom", &campaignDb::campaignDbRansom);
	typeAll.campaignDb.set("autoResolve", &campaignDb::campaignDbAutoresolve);
	typeAll.campaignDb.set("settlement", &campaignDb::campaignDbSettlement);
	typeAll.campaignDb.set("revolt", &campaignDb::campaignDbRevolt);
	typeAll.campaignDb.set("horde", &campaignDb::campaignDbHorde);
	typeAll.campaignDb.set("merchants", &campaignDb::campaignDbMerchants);
	typeAll.campaignDb.set("agents", &campaignDb::campaignDbAgents);
	typeAll.campaignDb.set("crusades", &campaignDb::campaignDbCrusades);
	typeAll.campaignDb.set("ai", &campaignDb::campaignDbAi);
	typeAll.campaignDb.set("misc", &campaignDb::campaignDbMisc);

	/***
	Basic campaignDb Extra table (these options are just in another place in memory)

	@tfield bool clearPoolsWithCaps
	@tfield bool addInitialWithCaps
	@tfield bool forceClampToMax
	@tfield float witchConversionRateModifier
	@tfield bool inquisitorTargetCrusades
	@tfield float foundingConversionDefaultRate
	@tfield float ownerConversionDefaultRate
	@tfield float neighbourNormaliseWeight
	@tfield float governorConversionRateOffset
	@tfield float governorConversionRateModifier
	@tfield float spyConversionRateOffset
	@tfield float spyConversionRateModifier
	@tfield float spyConversionRateForeignModifier
	@tfield bool bribeToFamilyTree
	@tfield bool enemiesRejectGifts
	@tfield bool useBalanceOwed
	@tfield bool recruitmentSortSimple
	@tfield bool keepOriginalHereticPortraits
	@tfield bool altSettOrderColors
	@tfield bool separateGamesRaces
	@tfield int chivalryDisplayThreshold
	@tfield bool captiveRansomForSlave
	@tfield bool switchableDefenceExposed
	@tfield float gateDefenceStrengthOilLevelModifier
	@tfield int settDefenceStrengthNumDefaultAttacks
	@tfield int siegeGearRequiredForCastleLevel
	@tfield int noTowersOnlyForCastleLevel
	@tfield int fortFortificationLevel
	@tfield bool alternativeReligiousUnrest
	@tfield bool revoltAdditionalArmies
	@tfield bool revoltCrusadingArmies
	@tfield bool agentsCanHide
	@tfield int denounceChanceMin
	@tfield float inquisitorCrtChanceMin
	@tfield bool inquisitorTargetLeaders
	@tfield bool inquisitorTargetHeirs
	@tfield bool spyRescaleChance
	@tfield bool allowResourceForts
	@tfield bool enableHotseatMessages
	@tfield bool enableBananaRepublicCheat
	@tfield bool enableUnitAccentOverrides

	@table campaignDbExtra
	*/

	typeAll.campaignDbExtra = luaState.new_usertype<campaignDbExtra>("campaignDbExtra");
	typeAll.campaignDbExtra.set("clearPoolsWithCaps", &campaignDbExtra::clearPoolsWithCaps);
	typeAll.campaignDbExtra.set("addInitialWithCaps", &campaignDbExtra::addInitialWithCaps);
	typeAll.campaignDbExtra.set("forceClampToMax", &campaignDbExtra::forceClampToMax);
	typeAll.campaignDbExtra.set("witchConversionRateModifier", &campaignDbExtra::witchConversionRateModifier);
	typeAll.campaignDbExtra.set("inquisitorTargetCrusades", &campaignDbExtra::inquisitorTargetCrusades);
	typeAll.campaignDbExtra.set("foundingConversionDefaultRate", &campaignDbExtra::foundingConversionDefaultRate);
	typeAll.campaignDbExtra.set("ownerConversionDefaultRate", &campaignDbExtra::ownerConversionDefaultRate);
	typeAll.campaignDbExtra.set("neighbourNormaliseWeight", &campaignDbExtra::neighbourNormaliseWeight);
	typeAll.campaignDbExtra.set("governorConversionRateOffset", &campaignDbExtra::governorConversionRateOffset);
	typeAll.campaignDbExtra.set("governorConversionRateModifier", &campaignDbExtra::governorConversionRateModifier);
	typeAll.campaignDbExtra.set("spyConversionRateOffset", &campaignDbExtra::spyConversionRateOffset);
	typeAll.campaignDbExtra.set("spyConversionRateModifier", &campaignDbExtra::spyConversionRateModifier);
	typeAll.campaignDbExtra.set("spyConversionRateForeignModifier", &campaignDbExtra::spyConversionRateForeignModifier);
	typeAll.campaignDbExtra.set("bribeToFamilyTree", &campaignDbExtra::bribeToFamilyTree);
	typeAll.campaignDbExtra.set("enemiesRejectGifts", &campaignDbExtra::enemiesRejectGifts);
	typeAll.campaignDbExtra.set("useBalanceOwed", &campaignDbExtra::useBalanceOwed);
	typeAll.campaignDbExtra.set("recruitmentSortSimple", &campaignDbExtra::recruitmentSortSimple);
	typeAll.campaignDbExtra.set("keepOriginalHereticPortraits", &campaignDbExtra::keepOriginalHereticPortraits);
	typeAll.campaignDbExtra.set("altSettOrderColors", &campaignDbExtra::altSettOrderColors);
	typeAll.campaignDbExtra.set("separateGamesRaces", &campaignDbExtra::separateGamesRaces);
	typeAll.campaignDbExtra.set("chivalryDisplayThreshold", &campaignDbExtra::chivalryDisplayThreshold);
	typeAll.campaignDbExtra.set("captiveRansomForSlave", &campaignDbExtra::captiveRansomForSlave);
	typeAll.campaignDbExtra.set("switchableDefenceExposed", &campaignDbExtra::switchableDefenceExposed);
	typeAll.campaignDbExtra.set("gateDefenceStrengthOilLevelModifier", &campaignDbExtra::gateDefenceStrengthOilLevelModifier);
	typeAll.campaignDbExtra.set("settDefenceStrengthNumDefaultAttacks", &campaignDbExtra::settDefenceStrengthNumDefaultAttacks);
	typeAll.campaignDbExtra.set("siegeGearRequiredForCastleLevel", &campaignDbExtra::siegeGearRequiredForCastleLevel);
	typeAll.campaignDbExtra.set("noTowersOnlyForCastleLevel", &campaignDbExtra::noTowersOnlyForCastleLevel);
	typeAll.campaignDbExtra.set("fortFortificationLevel", &campaignDbExtra::fortFortificationLevel);
	typeAll.campaignDbExtra.set("alternativeReligiousUnrest", &campaignDbExtra::alternativeReligiousUnrest);
	typeAll.campaignDbExtra.set("revoltAdditionalArmies", &campaignDbExtra::revoltAdditionalArmies);
	typeAll.campaignDbExtra.set("revoltCrusadingArmies", &campaignDbExtra::revoltCrusadingArmies);
	typeAll.campaignDbExtra.set("agentsCanHide", &campaignDbExtra::agentsCanHide);
	typeAll.campaignDbExtra.set("denounceChanceMin", &campaignDbExtra::denounceChanceMin);
	typeAll.campaignDbExtra.set("inquisitorCrtChanceMin", &campaignDbExtra::inquisitorCrtChanceMin);
	typeAll.campaignDbExtra.set("inquisitorTargetLeaders", &campaignDbExtra::inquisitorTargetLeaders);
	typeAll.campaignDbExtra.set("inquisitorTargetHeirs", &campaignDbExtra::inquisitorTargetHeirs);
	typeAll.campaignDbExtra.set("spyRescaleChance", &campaignDbExtra::spyRescaleChance);
	typeAll.campaignDbExtra.set("allowResourceForts", &campaignDbExtra::allowResourceForts);
	typeAll.campaignDbExtra.set("enableHotseatMessages", &campaignDbExtra::enableHotseatMessages);
	typeAll.campaignDbExtra.set("enableBananaRepublicCheat", &campaignDbExtra::enableBananaRepublicCheat);
	typeAll.campaignDbExtra.set("enableUnitAccentOverrides", &campaignDbExtra::enableUnitAccentOverrides);
}
