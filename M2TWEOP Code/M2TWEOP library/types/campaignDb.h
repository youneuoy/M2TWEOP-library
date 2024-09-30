#pragma once
#include "realGameTypes.h"

struct campaignDbRecruitment
{
	int32_t recruitmentSlots; //0x0000
	int32_t retrainingSlots; //0x0004
	bool deplenishPoolsWithCaps; //0x0008
	char pad_0009[3]; //0x0009
	float deplenishMultiplier; //0x000C
	float deplenishOffset; //0x0010
	bool addDisbandNoCaps; //0x0014
	char pad_0015[3]; //0x0015
	int32_t percentagePoolReductionLost; //0x0018
	int32_t percentagePoolReductionOccupy; //0x001C
	int32_t percentagePoolReductionSack; //0x0020
	int32_t percentagePoolReductionExterminate; //0x0024
	int32_t maxAgentsPerTurn; //0x0028
};

struct campaignDbReligion
{
	int32_t maxWitchesPerRegion; //0x002C
	int32_t maxWitches; //0x0030
	int32_t maxHereticsPerRegion; //0x0034
	int32_t maxHeretics; //0x0038
	int32_t maxInquisitorsPerRegion; //0x003C
	int32_t maxInquisitors; //0x0040
	float maxHereticsConversionModifier; //0x0044
	float hereticConversionRateModifier; //0x0048
	float hereticConversionRateOffset; //0x004C
	float witchConversionRateOffset; //0x0050
	float inquisitorConversionRateModifier; //0x0054
	float inquisitorConversionRateOffset; //0x0058
	float priestConversionRateModifier; //0x005C
	float priestConversionRateOffset; //0x0060
	float witchChanceModifier; //0x0064
	float hereticChanceModifier; //0x0068
	float inquisitorChanceModifier; //0x006C
	int32_t minCardinalPiety; //0x0070
	float convertToHereticBaseModifier; //0x0074
	float convertToHereticUnorthodoxModifier; //0x0078
	int32_t inquisitorTurnStart; //0x007C
};

struct campaignDbBribery
{
	float baseCharacterChance; //0x0080
	float religionModifier; //0x0084
	float combinedAttributeModifier; //0x0088
	float briberAttributeDivisor; //0x008C
	float bribeeAttributeDivisor; //0x0090
	float armySizeModifier; //0x0094
	float baseSettlementChance; //0x0098
	float settlementLoyaltyModifier; //0x009C
	float settlementPopulationModifier; //0x00A0
	float factionStandingDivisor; //0x00A4
	float maxBribeChance; //0x00A8
	float minBribeChance; //0x00AC
	float bribeChanceModifier; //0x00B0
};

struct campaignDbFamilyTree
{
	int32_t maxAge; //0x00B4
	int32_t maxAgeForMarriageMale; //0x00B8
	int32_t maxAgeForMarriageForFemale; //0x00BC
	int32_t maxAgeBeforeDeath; //0x00C0
	int32_t maxAgeOfChild; //0x00C4
	int32_t oldAge; //0x00C8
	int32_t ageOfManhood; //0x00CC
	int32_t daughtersAgeOfConsent; //0x00D0
	int32_t daughtersRetirementAge; //0x00D4
	int32_t ageDifferenceMin; //0x00D8
	int32_t ageDifferenceMax; //0x00DC
	int32_t parentToChildMinAgeDiff; //0x00E0
	int32_t minAdoptionAge; //0x00E4
	int32_t maxAdoptionAge; //0x00E8
	int32_t maxAgeForConception; //0x00EC
	int32_t ageOfManhoodClose; //0x00F0
	int32_t maxNumberOfChildren; //0x00F4
};

struct campaignDbDiplomacy
{
	int32_t maxDiplomacyItems; //0x00F8
	int32_t nullMissionScore; //0x00FC
};

struct campaignDbDisplay
{
	float characterSelectionRadius; //0x0100
	float characterSelectionHeight; //0x0104
	float characterSelectionHeightCrouching; //0x0108
	float diplomacyScrollHeight; //0x010C
	float factionStandingMin; //0x0110
	float factionStandingMax; //0x0114
	bool UseOrigRebelFactionModels; //0x0118
	char pad_0119[3]; //0x0119
	int32_t hudTabTextOffset; //0x011C
	bool useFactionCreatorSettModels; //0x0120
	char pad_0121[3]; //0x0121
	int32_t standardSoldierLimit; //0x0124
	int32_t standardSoldierLevelScale; //0x0128
	bool clearBattleModelsOnNewFaction; //0x012C
	char pad_012D[3]; //0x012D
};

struct campaignDbRansom
{
	float captorReleaseChanceBase; //0x0130
	float captorReleaseChanceChivMod; //0x0134
	float captorRansomChanceBase; //0x0138
	float captorRansomChanceChivMod; //0x013C
	float captorRansomChanceTmMod; //0x0140
	float captiveRansomChanceBase; //0x0144
	float captiveRansomChanceChivMod; //0x0148
	float captiveRansomChanceTmMod; //0x014C
	float captiveRansomChanceMsmMod; //0x0150
};

struct campaignDbAutoresolve
{
	float minCapturePercent; //0x0154
	float maxCapturePercent; //0x0158
	float lopsidedThresh; //0x015C
	float lopsidedHnMod; //0x0160
	int32_t separationMissileAdd; //0x0164
	float navalSinkModifier; //0x0168
	float navalSinkOffset; //0x016C
	float navalSinkMax; //0x0170
	float sallyAtDefDrawDivisor; //0x0174
	bool useNewSettAutoResolve; //0x0178
	char pad_0179[3]; //0x0179
	int32_t gateDefenceNumOilAttacks; //0x017C
	float gateDefenceStrengthOilBase; //0x0180
	int32_t gateDefenceNumArrowAttacks; //0x0184
	float gateDefenceStrengthArrowBase; //0x0188
	float gateDefenceStrengthArrowLevelModifier; //0x018C
	int32_t gateDefenceNumDefaultAttacks; //0x0190
	float gateDefenceStrengthDefaultBase; //0x0194
	float gateDefenceStrengthDefaultLevelModifier; //0x0198
	int32_t settDefenceNumArrowAttacks; //0x019C
	float settDefenceStrengthArrowBase; //0x01A0
	float settDefenceStrengthArrowModifier; //0x01A4
	float settDefenceStrengthDefaultBase; //0x01A8
	float settDefenceStrengthDefaultModifier; //0x01AC
	float displayStrengthOil; //0x01B0
	float displayStrengthArrow; //0x01B4
	float displayStrengthDefault; //0x01B8
};
struct campaignDbSettlement
{
	float sackMoneyModifier; //0x01BC
	float exterminateMoneyModifier; //0x01C0
	float chivSpfModifier; //0x01C4
	float chivSofModifier; //0x01C8
	float dreadSofModifier; //0x01CC
	float pietyCorruptionModifier; //0x01D0
	float pietyAdminSifModifier; //0x01D4
	float portToPortMpMin; //0x01D8
	float heresyUnrestNodifier; //0x01DC
	float religionUnrestModifier; //0x01E0
	int32_t siegeGearRequiredForCityLevel; //0x01E4
	int32_t noTowersOnlyForCityLevel; //0x01E8
	int32_t minTurnKeepRebelGarrison; //0x01EC
	bool destroyEmptyForts; //0x01F0
	bool canBuildForts; //0x01F1
	char pad_01F2[2]; //0x01F2
	float raceGameCostsModifier; //0x01F4
	float altRelAlliedModifier; //0x01F8
	float altRelGovModifierBase; //0x01FC
	float altRelGovCoefficient; //0x0200
};

struct campaignDbRevolt
{
	float endTurnModifier; //0x0204
	float excommunicatedModifier; //0x0208
	float newLeaderModifier; //0x020C
	float maxEffectiveLoyalty; //0x0210
	float rebelRegionModifier; //0x0214
	float shadowRegionModifier; //0x0218
	float rebelBorderModifier; //0x021C
	float shadowBorderModifier; //0x0220
	float numUnitsModifier; //0x0224
	float captainModifier; //0x0228
	float minRevoltChance; //0x022C
	float maxRevoltChance; //0x0230
	float aiRevoltModifier; //0x0234
	float shadowAuthorityModifier; //0x0238
	float shadowAuthorityModifierSett; //0x023C
};

struct campaignDbHorde
{
	int32_t endTargetFactionBonus; //0x0240
	int32_t startTargetFactionBonus; //0x0244
	int32_t farmingLevelBonus; //0x0248
	int32_t sharedTargetBonus; //0x024C
	int32_t disbandingHordeBonus; //0x0250
	int32_t hordeStartingRegionBonus; //0x0254
	int32_t hordeTargetResourceBonus; //0x0258
};

struct campaignDbMerchants
{
	float baseIncomeModifier; //0x025C
	float tradeBonusOffset; //0x0260
};

struct campaignDbAgents
{
	float denounceInquisitorBaseChance; //0x0264
	float DenouncePriestBaseChance; //0x0268
	float denounceAttackModifier; //0x026C
	float denounceDefenceModifier; //0x0270
	int32_t denounceChanceMax; //0x0274
	float assassinateBaseChance; //0x0278
	float assassinateAttackModifier; //0x027C
	float assassinateDefenceModifier; //0x0280
	float assassinatePublicModifier; //0x0284
	float assassinatePersonalModifier; //0x0288
	float assassinateCounterSpyModifier; //0x028C
	float assassinateAgentModifier; //0x0290
	float assassinateOwnRegionModifier; //0x0294
	float assassinateAssassinateAttrModifier; //0x0298
	int32_t assassinateChanceMin; //0x029C
	int32_t assassinateChanceMax; //0x02A0
	float denounceHereticAttemptModifier; //0x02A4
	float denounceCharacterAttemptModifier; //0x02A8
	float acquisitionBaseChance; //0x02AC
	float acquisitionLevelModifier; //0x02B0
	float acquisitionAttackTradeRightsModifier; //0x02B4
	float acquisitionDefenceTradeRightsModifier; //0x02B8
	int32_t acquisitionChanceMin; //0x02BC
	int32_t acquisitionChanceMax; //0x02C0
	float inquisitorCrtHeresyDivisor; //0x02C4
	float inquisitorCrtPfpModifier; //0x02C8
	float inquisitorCrtPfpModifierMin; //0x02CC
	float inquisitorCrtPfpModifierMax; //0x02D0
	float inquisitorCrtChanceMax; //0x02D4
	float spyBaseChance; //0x02D8
	float spyLevelModifier; //0x02DC
	float notSpyLevelModifier; //0x02E0
	float spyPublicModifier; //0x02E4
	float spyCounterSpyModifier; //0x02E8
	float spyDistanceModifier; //0x02EC
	float spySecretAgentTargetModifier; //0x02F0
	float spySedentaryTurnsModifier; //0x02F4
	float spyAllianceModifier; //0x02F8
	float spyTargetEngagedModifier; //0x02FC
	float spyInSettlementModifier; //0x0300
	float spyWatchtowerModifier; //0x0304
	float spyInOwnRegionModifier; //0x0308
	int32_t spyChanceMin; //0x030C
	int32_t spyChanceMax; //0x0310
};

struct campaignDbCrusades
{
	int32_t requiredJihadPiety; //0x0314
	float maxDisbandProgress; //0x0318
	float nearTargetNoDisbandDistance; //0x031C
	int32_t disbandProgressWindow; //0x0320
	int32_t crusadeCalledStartTurn; //0x0324
	int32_t jihadCalledStartTurn; //0x0328
	float movementPointsModifier; //0x032C
};

struct campaignDbAi
{
	float priestReligionMin; //0x0330
	float priestReligionMax; //0x0334
	float priestHeresyMin; //0x0338
	float priestHeresyMax; //0x033C
	float priestReligionExport; //0x0340
	float priestMaxProdTurns; //0x0344
	int32_t merchantMinSurvivalAcquire; //0x0348
	float attStrModifier; //0x034C
	float siegeAttStrModifier; //0x0350
	float crusadeAttStrModifier; //0x0354
	float sallyAttStrModifier; //0x0358
	float ambushAttStrModifier; //0x035C
	float strLimitWeak; //0x0360
	float strLimitStrong; //0x0364
};

struct campaignDbMisc
{
	int32_t fortDevastationDistance; //0x0368
	int32_t armyDevastationDistance; //0x036C
	float fortDevastationModifier; //0x0370
	float armyDevastationModifier; //0x0374
	bool allowEnemyForts; //0x0378
	char pad_0379[3]; //0x0379
	float siegeMovementPointsModifier; //0x037C
	float cavalryMovementPointsModifier; //0x0380
};

struct campaignDb
{
	campaignDbRecruitment campaignDbRecruitment;
	campaignDbReligion campaignDbReligion;
	campaignDbBribery campaignDbBribery;
	campaignDbFamilyTree campaignDbFamilyTree;
	campaignDbDiplomacy campaignDbDiplomacy;
	campaignDbDisplay campaignDbDisplay;
	campaignDbRansom campaignDbRansom;
	campaignDbAutoresolve campaignDbAutoresolve;
	campaignDbSettlement campaignDbSettlement;
	campaignDbRevolt campaignDbRevolt;
	campaignDbHorde campaignDbHorde;
	campaignDbMerchants campaignDbMerchants;
	campaignDbAgents campaignDbAgents;
	campaignDbCrusades campaignDbCrusades;
	campaignDbAi campaignDbAi;
	campaignDbMisc campaignDbMisc;
}; //Size: 0x0580

struct campaignDbExtra
{
	bool clearPoolsWithCaps; //0x0000
	bool addInitialWithCaps; //0x0001
	bool forceClampToMax; //0x0002
	char pad_0003[1]; //0x0003
	float witchConversionRateModifier; //0x0004
	bool inquisitorTargetCrusades; //0x0008
	char pad_0009[3]; //0x0009
	float foundingConversionDefaultRate; //0x000C
	float ownerConversionDefaultRate; //0x0010
	float neighbourNormaliseWeight; //0x0014
	float governorConversionRateOffset; //0x0018
	float governorConversionRateModifier; //0x001C
	float spyConversionRateOffset; //0x0020
	float spyConversionRateModifier; //0x0024
	float spyConversionRateForeignModifier; //0x0028
	bool bribeToFamilyTree; //0x002C
	bool enemiesRejectGifts; //0x002D
	bool useBalanceOwed; //0x002E
	bool recruitmentSortSimple; //0x002F
	bool keepOriginalHereticPortraits; //0x0030
	bool altSettOrderColors; //0x0031
	bool separateGamesRaces; //0x0032
	char pad_0033[1]; //0x0033
	int32_t chivalryDisplayThreshold; //0x0034
	bool captiveRansomForSlave; //0x0038
	bool switchableDefenceExposed; //0x0039
	char pad_003A[2]; //0x003A
	float gateDefenceStrengthOilLevelModifier; //0x003C
	int32_t settDefenceStrengthNumDefaultAttacks; //0x0040
	int32_t siegeGearRequiredForCastleLevel; //0x0044
	int32_t noTowersOnlyForCastleLevel; //0x0048
	int32_t fortFortificationLevel; //0x004C
	bool alternativeReligiousUnrest; //0x0050
	bool revoltAdditionalArmies; //0x0051
	bool revoltCrusadingArmies; //0x0052
	bool agentsCanHide; //0x0053
	int32_t denounceChanceMin; //0x0054
	float inquisitorCrtChanceMin; //0x0058
	bool inquisitorTargetLeaders; //0x005C
	bool inquisitorTargetHeirs; //0x005D
	bool spyRescaleChance; //0x005E
	bool allowResourceForts; //0x005F
	bool enableHotseatMessages; //0x0060
	bool enableBananaRepublicCheat; //0x0061
	bool enableUnitAccentOverrides; //0x0062
	char pad_0063[1]; //0x0063
}; //Size: 0x0064

namespace campaignHelpers
{
	campaignDbExtra* getCampaignDbExtra();
	campaignDb* getCampaignDb();
}