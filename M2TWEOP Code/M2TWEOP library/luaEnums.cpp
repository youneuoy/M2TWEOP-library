///
//![Lua logo](../Lua.png)
//M2TWEOP structures and functions. There are not many examples and descriptions here. Also note that the examples do not include many of the checks that would be required when creating modifications.
//@module LuaPlugin
//@author youneuoy
//@license GPL-3.0
#include "pch.h"
#include "luaPlugin.h"
#include "gameHelpers.h"
#include "unit.h"
#include "campaign.h"

void luaPlugin::initLuaEnums()
{
	using namespace campaignEnums;
	
	///EOP Enums
	//@section dipRelType

	/***
	Enum with a list of types of diplomatic relations.

	@tfield int war
	@tfield int peace
	@tfield int alliance
	@tfield int suzerain
	@tfield int trade (Doesn't work with trade rights agreements set at game start)

	@usage
	local campaign = M2TW.campaign
	local fac1 = campaign:getFactionByOrder(0)
	local fac2 = campaign:getFactionByOrder(1)
	local isInWar = campaign:checkDipStance(dipRelType.war, fac1, fac2)

	@table dipRelType
	*/
	luaState.new_enum(
		"dipRelType",
		"war", dipRelEnum::war,
		"peace", dipRelEnum::peace,
		"alliance", dipRelEnum::alliance,
		"suzerain", dipRelEnum::suzerain,
		"trade", dipRelEnum::trade
	);

	
	/***
	Enum with a list of types of unit battle properties.

	@tfield int guardMode
	@tfield int fireAtWill
	@tfield int skirmish

	@table unitBattleProperties
	*/
	luaState.new_enum(
		"unitBattleProperties",
		"guardMode", guardMode,
		"fireAtWill", fireAtWill,
		"skirmish", skirmish
	);

	
	/***
	Enum with a list of types of building capabilities.

	@tfield int population_growth_bonus
	@tfield int population_loyalty_bonus
	@tfield int population_health_bonus
	@tfield int trade_base_income_bonus
	@tfield int trade_level_bonus
	@tfield int trade_fleet
	@tfield int taxable_income_bonus
	@tfield int mine_resource
	@tfield int farming_level
	@tfield int road_level
	@tfield int gate_strength
	@tfield int gate_defences
	@tfield int wall_level
	@tfield int tower_level
	@tfield int armour
	@tfield int stage_games
	@tfield int stage_races
	@tfield int fire_risk
	@tfield int weapon_melee_simple
	@tfield int weapon_melee_blade
	@tfield int weapon_missile_mechanical
	@tfield int weapon_missile_gunpowder
	@tfield int weapon_artillery_mechanical
	@tfield int weapon_artillery_gunpowder
	@tfield int weapon_naval_gunpowder
	@tfield int upgrade_bodyguard
	@tfield int recruits_morale_bonus
	@tfield int recruits_exp_bonus
	@tfield int happiness_bonus
	@tfield int law_bonus
	@tfield int construction_cost_bonus_military
	@tfield int construction_cost_bonus_religious
	@tfield int construction_cost_bonus_defensive
	@tfield int construction_cost_bonus_other
	@tfield int construction_time_bonus_military
	@tfield int construction_time_bonus_religious
	@tfield int construction_time_bonus_defensive
	@tfield int construction_time_bonus_other
	@tfield int construction_cost_bonus_wooden
	@tfield int construction_cost_bonus_stone
	@tfield int construction_time_bonus_wooden
	@tfield int construction_time_bonus_stone
	@tfield int free_upkeep
	@tfield int pope_approval
	@tfield int pope_disapproval
	@tfield int religion_level
	@tfield int amplify_religion_level
	@tfield int archer_bonus
	@tfield int cavalry_bonus
	@tfield int heavy_cavalry_bonus
	@tfield int gun_bonus
	@tfield int navy_bonus
	@tfield int recruitment_cost_bonus_naval
	@tfield int retrain_cost_bonus
	@tfield int weapon_projectile
	@tfield int income_bonus
	@tfield int recruitment_slots

	@table buildingCapability
	*/
	luaState.new_enum(
		"buildingCapability",
		"population_growth_bonus", 0,
		"population_loyalty_bonus", 1,
		"population_health_bonus", 2,
		"trade_base_income_bonus", 3,
		"trade_level_bonus", 4,
		"trade_fleet", 5,
		"taxable_income_bonus", 6,
		"mine_resource", 7,
		"farming_level", 8,
		"road_level", 9,
		"gate_strength", 10,
		"gate_defences", 11,
		"wall_level", 12,
		"tower_level", 13,
		"armour", 14,
		"stage_games", 15,
		"stage_races", 16,
		"fire_risk", 17,
		"weapon_melee_simple", 18,
		"weapon_melee_blade", 19,
		"weapon_missile_mechanical", 20,
		"weapon_missile_gunpowder", 21,
		"weapon_artillery_mechanical", 22,
		"weapon_artillery_gunpowder", 23,
		"weapon_naval_gunpowder", 24,
		"upgrade_bodyguard", 25,
		"recruits_morale_bonus", 26,
		"recruits_exp_bonus", 27,
		"happiness_bonus", 28,
		"law_bonus", 29,
		"construction_cost_bonus_military", 30,
		"construction_cost_bonus_religious", 31,
		"construction_cost_bonus_defensive", 32,
		"construction_cost_bonus_other", 33,
		"construction_time_bonus_military", 34,
		"construction_time_bonus_religious", 35,
		"construction_time_bonus_defensive", 36,
		"construction_time_bonus_other", 37,
		"construction_cost_bonus_wooden", 38,
		"construction_cost_bonus_stone", 39,
		"construction_time_bonus_wooden", 40,
		"construction_time_bonus_stone", 41,
		"free_upkeep", 42,
		"pope_approval", 43,
		"pope_disapproval", 44,
		"religion_level", 45,
		"amplify_religion_level", 46,
		"archer_bonus", 47,
		"cavalry_bonus", 48,
		"heavy_cavalry_bonus", 49,
		"gun_bonus", 50,
		"navy_bonus", 51,
		"recruitment_cost_bonus_naval", 52,
		"retrain_cost_bonus", 53,
		"weapon_projectile", 54,
		"income_bonus", 55,
		"recruitment_slots", 56
	);

	
	/***
	Enum with a list of character types.

	@tfield int spy
	@tfield int assassin
	@tfield int diplomat
	@tfield int admiral
	@tfield int merchant
	@tfield int priest
	@tfield int general
	@tfield int named_character
	@tfield int princess
	@tfield int heretic
	@tfield int witch
	@tfield int inquisitor
	@tfield int pope

	@table characterType
	*/
	luaState.new_enum(
		"characterType",
		"spy", 0,
		"assassin", 1,
		"diplomat", 2,
		"admiral", 3,
		"merchant", 4,
		"priest", 5,
		"general", 6,
		"named_character", 7,
		"princess", 8,
		"heretic", 9,
		"witch", 10,
		"inquisitor", 11,
		"pope", 13
	);

	
	/***
	Enum with a character death types.

	@tfield int alive
	@tfield int diedNatural
	@tfield int diedInDisaster
	@tfield int diedInBattle
	@tfield int assassinated
	@tfield int executed
	@tfield int executedPrisoner
	@tfield int poisoned
	@tfield int plague
	@tfield int starvation
	@tfield int captainRemoval
	@tfield int removedForRetraining
	@tfield int incompatibleFaction
	@tfield int bribed
	@tfield int disbanded
	@tfield int sunk
	@tfield int married
	@tfield int acquired
	@tfield int retired
	@tfield int disgraced
	@tfield int denounced

	@table deathType
	*/
	luaState.new_enum(
		"deathType",
		"alive", 0,
		"diedNatural", 1,
		"diedInDisaster", 2,
		"diedInBattle", 3,
		"assassinated", 4,
		"executed", 5,
		"executedPrisoner", 6,
		"poisoned", 7,
		"plague", 8,
		"starvation", 9,
		"captainRemoval", 10,
		"removedForRetraining", 11,
		"incompatibleFaction", 12,
		"bribed", 13,
		"disbanded", 14,
		"sunk", 15,
		"married", 16,
		"acquired", 17,
		"retired", 18,
		"disgraced", 19,
		"denounced", 20
	);

	
	/***
	Enum with a list of army sort types.

	@tfield int eduType
	@tfield int category
	@tfield int class
	@tfield int soldierCount
	@tfield int experience
	@tfield int categoryClass
	@tfield int aiUnitValue

	@table sortType
	*/
	luaState.new_enum(
		"sortType",
		"eduType", 1,
		"category", 2,
		"class", 3,
		"soldierCount", 4,
		"experience", 5,
		"categoryClass", 6,
		"aiUnitValue", 7
	);

	
	/***
	Enum with a list of GTA AI plans.

	@tfield int doNothing
	@tfield int attackAll
	@tfield int defend
	@tfield int defendFeature
	@tfield int hide
	@tfield int ambush
	@tfield int scout
	@tfield int withdraw
	@tfield int attackSettlement
	@tfield int defendSettlement
	@tfield int sallyOut

	@table aiPlan
	*/
	luaState.new_enum(
		"aiPlan",
		"doNothing", 0,
		"attackAll", 1,
		"defend", 2,
		"defendFeature", 3,
		"hide", 4,
		"ambush", 5,
		"scout", 6,
		"withdraw", 7,
		"attackSettlement", 8,
		"defendSettlement", 9,
		"sallyOut", 10
	);

	
	/***
	Enum with a list of GTA AI objectives.

	@tfield int invalid
	@tfield int moveToPoint
	@tfield int attackEnemyBattleGroup
	@tfield int defendTerrainHill
	@tfield int defendTerrainForest
	@tfield int defendTerrainArea
	@tfield int defendCrossing
	@tfield int assaultCrossing
	@tfield int defendLine
	@tfield int scout
	@tfield int withdraw
	@tfield int defendSettlement
	@tfield int supportDefendSettlement
	@tfield int attackSettlement
	@tfield int skirmish
	@tfield int bombard
	@tfield int attackModel
	@tfield int sallyOut
	@tfield int ambush

	@table aiObjective
	*/
	luaState.new_enum(
		"aiObjective",
		"invalid", 0,
		"moveToPoint", 1,
		"attackEnemyBattleGroup", 2,
		"defendTerrainHill", 3,
		"defendTerrainForest", 4,
		"defendTerrainArea", 5,
		"defendCrossing", 6,
		"assaultCrossing", 7,
		"defendLine", 8,
		"scout", 9,
		"withdraw", 10,
		"defendSettlement", 11,
		"supportDefendSettlement", 12,
		"attackSettlement", 13,
		"skirmish", 14,
		"bombard", 15,
		"attackModel", 16,
		"sallyOut", 17,
		"ambush", 18
	);

	
	/***
	Enum with a list of resources.

	@tfield int gold
	@tfield int silver
	@tfield int fish
	@tfield int furs
	@tfield int grain
	@tfield int timber
	@tfield int iron
	@tfield int ivory
	@tfield int wine
	@tfield int slaves
	@tfield int chocolate
	@tfield int marble
	@tfield int textiles
	@tfield int dyes
	@tfield int tobacco
	@tfield int silk
	@tfield int sugar
	@tfield int sulfur
	@tfield int tin
	@tfield int spices
	@tfield int cotton
	@tfield int amber
	@tfield int coal
	@tfield int wool
	@tfield int elephants
	@tfield int camels
	@tfield int dogs
	@tfield int generic

	@table resourceType
	*/
	luaState.new_enum(
		"resourceType",
		"gold", 0,
		"silver", 1,
		"fish", 2,
		"furs", 3,
		"grain", 4,
		"timber", 5,
		"iron", 6,
		"ivory", 7,
		"wine", 8,
		"slaves", 9,
		"chocolate", 10,
		"marble", 11,
		"textiles", 12,
		"dyes", 13,
		"tobacco", 14,
		"silk", 15,
		"sugar", 16,
		"sulfur", 17,
		"tin", 18,
		"spices", 19,
		"cotton", 20,
		"amber", 21,
		"coal", 22,
		"wool", 23,
		"elephants", 24,
		"camels", 25,
		"dogs", 26,
		"generic", 27
	);

	
	/***
	Enum of unit morale status.

	@tfield int berserk
	@tfield int impetuous
	@tfield int high
	@tfield int firm
	@tfield int shaken
	@tfield int wavering
	@tfield int routing

	@table moraleStatus
	*/
	luaState.new_enum(
		"moraleStatus",
		"berserk", 0,
		"impetuous", 1,
		"high", 2,
		"firm", 3,
		"shaken", 4,
		"wavering", 5,
		"routing", 6
	);

	
	/***
	Enum of unit discipline.

	@tfield int berserker
	@tfield int impetuous
	@tfield int low
	@tfield int normal
	@tfield int disciplined

	@table unitDiscipline
	*/
	luaState.new_enum(
		"unitDiscipline",
		"berserker", 0,
		"impetuous", 1,
		"low", 2,
		"normal", 3,
		"disciplined", 4
	);

	
	/***
	Enum of unit training.

	@tfield int untrained
	@tfield int trained
	@tfield int highly_trained

	@table unitTraining
	*/
	luaState.new_enum(
		"unitTraining",
		"untrained", 0,
		"trained", 1,
		"highly_trained", 2
	);

	
	/***
	Enum of unit combat status.

	@tfield int notInCombat
	@tfield int victoryCertain
	@tfield int victoryAlmostCertain
	@tfield int victoryDistinct
	@tfield int balanced
	@tfield int defeatDistinct
	@tfield int defeatAlmostCertain
	@tfield int defeatCertain

	@table combatStatus
	*/
	luaState.new_enum(
		"combatStatus",
		"notInCombat", 0,
		"victoryCertain", 1,
		"victoryAlmostCertain", 2,
		"victoryDistinct", 3,
		"balanced", 4,
		"defeatDistinct", 5,
		"defeatAlmostCertain", 6,
		"defeatCertain", 7
	);

	
	/***
	Enum of battle success types.

	@tfield int close
	@tfield int average
	@tfield int clear
	@tfield int crushing

	@table battleSuccess
	*/
	luaState.new_enum(
		"battleSuccess",
		"close", 0,
		"average", 1,
		"clear", 2,
		"crushing", 3
	);

	
	/***
	Enum of unit mount class.

	@tfield int horse
	@tfield int camel
	@tfield int elephant
	@tfield int infantry

	@table mountClass
	*/
	luaState.new_enum(
		"mountClass",
		"horse", 0,
		"camel", 1,
		"elephant", 2,
		"infantry", 3
	);
	
	/***
	Enum of tile visibility status.

	@tfield int notVisibleInitial
	@tfield int notVisible
	@tfield int wasVisible
	@tfield int visible

	@table tileVisibility
	*/
	luaState.new_enum(
		"tileVisibility",
		"notVisibleInitial", 0,
		"notVisible", 1,
		"wasVisible", 2,
		"visible", 3
	);
	
	/***
	Enum of construction types.

	@tfield int upgrade
	@tfield int construction
	@tfield int repair
	@tfield int demolition
	@tfield int convert
	@tfield int none

	@table constructionType
	*/
	luaState.new_enum(
		"constructionType",
		"upgrade", 0,
		"construction", 1,
		"repair", 2,
		"demolition", 3,
		"convert", 4,
		"none", 5
	);

	
	/***
	Enum of battle types.

	@tfield int ambushSuccess
	@tfield int ambushFail
	@tfield int open
	@tfield int siege
	@tfield int sally
	@tfield int naval
	@tfield int withdrawal
	@tfield int meetEnemy

	@table battleType
	*/
	luaState.new_enum(
		"battleType",
		"ambushSuccess", 0,
		"ambushFail", 1,
		"open", 2,
		"siege", 3,
		"sally", 4,
		"naval", 5,
		"withdrawal", 6,
		"meetEnemy", 7
	);

	
	/***
	Enum of battle states.

	@tfield int notInBattle
	@tfield int preBattle
	@tfield int delay
	@tfield int deployment
	@tfield int deploymentPlayer2
	@tfield int conflict
	@tfield int victoryScroll
	@tfield int pursuit

	@table battleState
	*/
	luaState.new_enum(
		"battleState",
		"notInBattle", 0,
		"preBattle", 1,
		"delay", 2,
		"deployment", 3,
		"deploymentPlayer2", 4,
		"conflict", 5,
		"victoryScroll", 6,
		"pursuit", 7
	);
	
	/***
	Enum of unit status.

	@tfield int idle
	@tfield int hiding
	@tfield int taunting
	@tfield int celebrating
	@tfield int ready
	@tfield int reforming
	@tfield int moving
	@tfield int withdrawing
	@tfield int missilesFiring
	@tfield int missilesReloading
	@tfield int charging
	@tfield int fighting
	@tfield int pursuing
	@tfield int routing
	@tfield int fightingBacksToTheWalls
	@tfield int runningAmok
	@tfield int berserk
	@tfield int rallying
	@tfield int dead
	@tfield int leavingBattle
	@tfield int enteringBattle
	@tfield int leftBattle
	@tfield int bracing
	@tfield int infighting

	@table unitStatus
	*/
	luaState.new_enum(
		"unitStatus",
		"idle", 0,
		"hiding", 1,
		"taunting", 2,
		"celebrating", 3,
		"ready", 4,
		"reforming", 5,
		"moving", 6,
		"withdrawing", 7,
		"missilesFiring", 8,
		"missilesReloading", 9,
		"charging", 10,
		"fighting", 11,
		"pursuing", 12,
		"routing", 13,
		"fightingBacksToTheWalls", 14,
		"runningAmok", 15,
		"berserk", 16,
		"rallying", 17,
		"dead", 18,
		"leavingBattle", 19,
		"enteringBattle", 20,
		"leftBattle", 21,
		"bracing", 22,
		"infighting", 23
	);
	
	/***
	Enum of unit actions.

	@tfield int changeFormation
	@tfield int changeFormationClose
	@tfield int changeFormationWidth
	@tfield int changeSpeed
	@tfield int changeMeleeState
	@tfield int setupEngine
	@tfield int collectEngine
	@tfield int dropEngine
	@tfield int hide
	@tfield int move
	@tfield int moveIntoPlayableArea
	@tfield int turn
	@tfield int attackUnit
	@tfield int attackBuilding
	@tfield int attackEngine
	@tfield int withdraw
	@tfield int feignRout
	@tfield int useAbility
	@tfield int halt
	@tfield int idle

	@table unitAction
	*/
	luaState.new_enum(
		"unitAction",
		"changeFormation", 0,
		"changeFormationClose", 1,
		"changeFormationWidth", 2,
		"changeSpeed", 3,
		"changeMeleeState", 4,
		"setupEngine", 5,
		"collectEngine", 6,
		"dropEngine", 7,
		"hide", 8,
		"move", 9,
		"moveIntoPlayableArea", 10,
		"turn", 11,
		"attackUnit", 12,
		"attackBuilding", 13,
		"attackEngine", 14,
		"withdraw", 15,
		"feignRout", 16,
		"useAbility", 17,
		"halt", 18,
		"idle", 19
	);

	/***
	Enum of character attributes

	@tfield int command
	@tfield int influence increases command radius
	@tfield int subterfuge
	@tfield int loyalty
	@tfield int charm princess only
	@tfield int finance merchant only
	@tfield int piety
	@tfield int magic witch only, doesnt really do anything
	@tfield int chivalry
	@tfield int authority
	@tfield int level
	@tfield int unorthodoxy influences chance for priest to become heretic
	@tfield int heresyImmunity value over 1 makes it impossible for priest to become heretic
	@tfield int assassination
	@tfield int sabotage
	@tfield int eligibility increases college of cardinals eligibility
	@tfield int purity used for catholic mission scoring
	@tfield int violence used for cease fire / break alliance mission scoring
	@tfield int disposition useless
	@tfield int boldness useless
	@tfield int generosity useless
	@tfield int management useless
	@tfield int bodyguardSize
	@tfield int troopMorale
	@tfield int movementPoints 5% per point
	@tfield int attack
	@tfield int defence
	@tfield int siegeAttack
	@tfield int siegeDefence
	@tfield int ambush
	@tfield int navalCommand
	@tfield int siegeEngineering
	@tfield int nightBattle
	@tfield int personalSecurity increases bodyguard size
	@tfield int publicSecurity increases counterspy ability as governor
	@tfield int bribery divided by campaign db briber attr divisor
	@tfield int briberyResistance divided by campaign db bribee attr divisor
	@tfield int electability useless
	@tfield int lineOfSight
	@tfield int trainingUnits -5% cost per point
	@tfield int trainingAgents -5% cost per point
	@tfield int construction -5% cost per point
	@tfield int trading 1% per point
	@tfield int localPopularity
	@tfield int footInTheDoor useless
	@tfield int farming
	@tfield int mining
	@tfield int taxCollection
	@tfield int fertility
	@tfield int cavalryCommand
	@tfield int infantryCommand
	@tfield int gunpowderCommand
	@tfield int artilleryCommand
	@tfield int startReligionCombat
	@tfield int combatVReligion0
	@tfield int combatVReligion1
	@tfield int combatVReligion2
	@tfield int combatVReligion3
	@tfield int combatVReligion4
	@tfield int combatVReligion5
	@tfield int combatVReligion6
	@tfield int combatVReligion7
	@tfield int combatVReligion8
	@tfield int combatVReligion9
	@tfield int endReligionCombat
	@tfield int startFactionCombat
	@tfield int combatVFaction0
	@tfield int combatVFaction1
	@tfield int combatVFaction2
	@tfield int combatVFaction3
	@tfield int combatVFaction4
	@tfield int combatVFaction5
	@tfield int combatVFaction6
	@tfield int combatVFaction7
	@tfield int combatVFaction8
	@tfield int combatVFaction9
	@tfield int combatVFaction10
	@tfield int combatVFaction11
	@tfield int combatVFaction12
	@tfield int combatVFaction13
	@tfield int combatVFaction14
	@tfield int combatVFaction15
	@tfield int combatVFaction16
	@tfield int combatVFaction17
	@tfield int combatVFaction18
	@tfield int combatVFaction19
	@tfield int combatVFaction20
	@tfield int combatVFaction21
	@tfield int combatVFaction22
	@tfield int combatVFaction23
	@tfield int combatVFaction24
	@tfield int combatVFaction25
	@tfield int combatVFaction26
	@tfield int combatVFaction27
	@tfield int combatVFaction28
	@tfield int combatVFaction29
	@tfield int combatVFaction30
	@tfield int combatVFaction31
	@tfield int endFactionCombat
	@tfield int health
	@tfield int squalor
	@tfield int unrest
	@tfield int law
	@tfield int looting
	@tfield int bodyguardValour
	@tfield int hitPoints
	@tfield int trainingAnimalUnits
	@tfield int battleSurgery
	@tfield int attributeNum
	
	@table characterAttr
	*/
	luaState.new_enum(
		"characterAttr",
		"command", 0,
		"influence", 1,
		"subterfuge", 2,
		"loyalty", 3,
		"charm", 4,
		"finance", 5,
		"piety", 6,
		"magic", 7,
		"chivalry", 8,
		"authority", 9,
		"level", 10,
		"unorthodoxy", 11,
		"heresyImmunity", 12,
		"assassination", 13,
		"sabotage", 14,
		"eligibility", 15,
		"purity", 16,
		"violence", 17,
		"disposition", 18,
		"boldness", 19,
		"generosity", 20,
		"management", 21,
		"bodyguardSize", 22,
		"troopMorale", 23,
		"movementPoints", 24,
		"attack", 25,
		"defence", 26,
		"siegeAttack", 27,
		"siegeDefence", 28,
		"ambush", 29,
		"navalCommand", 30,
		"siegeEngineering", 31,
		"nightBattle", 32,
		"personalSecurity", 33,
		"publicSecurity", 34,
		"bribery", 35,
		"briberyResistance", 36,
		"electability", 37,
		"lineOfSight", 38,
		"trainingUnits", 39,
		"trainingAgents", 40,
		"construction", 41,
		"trading", 42,
		"localPopularity", 43,
		"footInTheDoor", 44,
		"farming", 45,
		"mining", 46,
		"taxCollection", 47,
		"fertility", 48,
		"cavalryCommand", 49,
		"infantryCommand", 50,
		"gunpowderCommand", 51,
		"artilleryCommand", 52,
		"startReligionCombat", 53,
		"combatVReligion0", 53,
		"combatVReligion1", 54,
		"combatVReligion2", 55,
		"combatVReligion3", 56,
		"combatVReligion4", 57,
		"combatVReligion5", 58,
		"combatVReligion6", 59,
		"combatVReligion7", 60,
		"combatVReligion8", 61,
		"combatVReligion9", 62,
		"endReligionCombat", 62,
		"startFactionCombat", 63,
		"combatVFaction0", 63,
		"combatVFaction1", 64,
		"combatVFaction2", 65,
		"combatVFaction3", 66,
		"combatVFaction4", 67,
		"combatVFaction5", 68,
		"combatVFaction6", 69,
		"combatVFaction7", 70,
		"combatVFaction8", 71,
		"combatVFaction9", 72,
		"combatVFaction10", 73,
		"combatVFaction11", 74,
		"combatVFaction12", 75,
		"combatVFaction13", 76,
		"combatVFaction14", 77,
		"combatVFaction15", 78,
		"combatVFaction16", 79,
		"combatVFaction17", 80,
		"combatVFaction18", 81,
		"combatVFaction19", 82,
		"combatVFaction20", 83,
		"combatVFaction21", 84,
		"combatVFaction22", 85,
		"combatVFaction23", 86,
		"combatVFaction24", 87,
		"combatVFaction25", 88,
		"combatVFaction26", 89,
		"combatVFaction27", 90,
		"combatVFaction28", 91,
		"combatVFaction29", 92,
		"combatVFaction30", 93,
		"combatVFaction31", 94,
		"endFactionCombat", 94,
		"health", 95,
		"squalor", 96,
		"unrest", 97,
		"law", 98,
		"looting", 99,
		"bodyguardValour", 100,
		"hitPoints", 101,
		"trainingAnimalUnits", 102,
		"battleSurgery", 103,
		"attributeNum", 104
	);
	
	/***
	Enum of unit classes

	@tfield int heavy
	@tfield int light
	@tfield int skirmish
	@tfield int spearmen
	@tfield int missile

	@table unitClass
	*/
	luaState.new_enum(
		"unitClass",
		"heavy",0,
		"light",1,
		"skirmish",2,
		"spearmen",3,
		"missile",4
	);
	
	/***
	Enum of unit categories

	@tfield int infantry
	@tfield int cavalry
	@tfield int siege
	@tfield int non_combatant
	@tfield int ship
	@tfield int handler

	@table unitCategory
	*/
	luaState.new_enum(
		"unitCategory",
		"infantry", 0,
		"cavalry", 1,
		"siege", 2,
		"non_combatant", 3,
		"ship", 4,
		"handler", 5
	);

	
	/***
	Enum of unit categories and class combo

	@tfield int nonCombatant
	@tfield int lightInfantry
	@tfield int heavyInfantry
	@tfield int spearmenInfantry
	@tfield int missileInfantry
	@tfield int lightCavalry
	@tfield int heavyCavalry
	@tfield int missileCavalry
	@tfield int siegeWeapon
	@tfield int animalHandler
	@tfield int battleShip

	@table unitCategoryClass
	*/
	luaState.new_enum(
		"unitCategoryClass",
		"nonCombatant", 0,
		"lightInfantry", 1,
		"heavyInfantry", 2,
		"spearmenInfantry", 3,
		"missileInfantry", 4,
		"lightCavalry", 5,
		"heavyCavalry", 6,
		"missileCavalry", 7,
		"siegeWeapon", 8,
		"animalHandler", 9,
		"battleShip", 10
	);

	
	/***
	Enum of automated settlement policy types

	@tfield int balanced
	@tfield int financial
	@tfield int military
	@tfield int growth
	@tfield int cultural
	@tfield int noPolicy

	@table managePolicy
	*/
	luaState.new_enum(
		"managePolicy",
		"balanced", 0,
		"financial", 1,
		"military", 2,
		"growth", 3,
		"cultural", 4,
		"noPolicy", 5
	);
	
	/***
	Enum of strategy pathfinding types

	@tfield int avoidZoc
	@tfield int ignoreZoc
	@tfield int direct
	@tfield int militaryAccess
	@tfield int landingPoints
	@tfield int region
	@tfield int strategic

	@table searchType
	*/
	luaState.new_enum(
		"searchType",
		"avoidZoc", 0,
		"ignoreZoc", 1,
		"direct", 2,
		"militaryAccess", 3,
		"landingPoints", 4,
		"region", 5,
		"strategic", 6
	);
	
	/***
	Enum of production controller extra bias

	@tfield int frontier
	@tfield int conflict
	@tfield int frontline
	@tfield int abandon
	@tfield int increaseGarrison
	@tfield int trade
	@tfield int tax
	@tfield int spies
	@tfield int assassins
	@tfield int merchants
	@tfield int priests
	@tfield int diplomats
	@tfield int ships
	@tfield int qualityUnits
	@tfield int buildStrengthLimit

	@table productionBias
	*/
	luaState.new_enum(
		"productionBias",
		"frontier", 0,
		"conflict", 1,
		"frontline", 2,
		"abandon", 3,
		"increaseGarrison", 4,
		"trade", 5,
		"tax", 6,
		"spies", 7,
		"assassins", 8,
		"merchants", 9,
		"priests", 10,
		"diplomats", 11,
		"ships", 12,
		"qualityUnits", 13,
		"buildStrengthLimit", 14
	);

	
	/***
	Enum of formation types

	@tfield int horde
	@tfield int column
	@tfield int square
	@tfield int wedge
	@tfield int squareHollow
	@tfield int phalanx
	@tfield int schiltrom
	@tfield int shieldWall
	@tfield int wall
	@tfield int movingThrough

	@table formationType
	*/
	luaState.new_enum(
		"formationType",
		"horde", 0,
		"column", 1,
		"square", 2,
		"wedge", 3,
		"squareHollow", 4,
		"phalanx", 5,
		"schiltrom", 6,
		"shieldWall", 7,
		"wall", 8,
		"movingThrough", 9
	);

	
	/***
	Enum of battle building types

	@tfield int ambient
	@tfield int gate
	@tfield int tower
	@tfield int wall

	@table battleBuildingType
	*/
	luaState.new_enum(
		"battleBuildingType",
		"ambient", 1,
		"gate", 3,
		"tower", 9,
		"wall", 10
	);

	
	/***
	Enum of precipitation types

	@tfield int none
	@tfield int rain
	@tfield int hail 
	@tfield int snow
	@tfield int dust
	@tfield int invalid

	@table rainType
	*/
	luaState.new_enum(
		"rainType",
		"none", 0,
		"rain", 1,
		"hail", 2,
		"snow", 3,
		"dust", 4,
		"invalid", 5
	);
	
	/***
	Enum of precipitation levels

	@tfield int dry
	@tfield int drizzle
	@tfield int light 
	@tfield int heavy
	@tfield int torrential
	@tfield int invalid

	@table rainLevel
	*/
	luaState.new_enum(
		"rainLevel",
		"dry", 0,
		"drizzle", 1,
		"light", 2,
		"heavy", 3,
		"torrential", 4,
		"invalid", 5
	);
	
	/***
	Enum of battle ground types

	@tfield int grassShort
	@tfield int grassLong
	@tfield int sand
	@tfield int rock
	@tfield int forestDense
	@tfield int scrubDense
	@tfield int swamp
	@tfield int mud
	@tfield int mudRoad
	@tfield int stoneRoad
	@tfield int water
	@tfield int ice
	@tfield int snow
	@tfield int wood
	@tfield int dirt
	@tfield int unknown

	@table physicalGroundType
	*/
	luaState.new_enum(
		"physicalGroundType",
		"grassShort", 0,
		"grassLong", 1,
		"sand", 2,
		"rock", 3,
		"forestDense", 4,
		"scrubDense", 5,
		"swamp", 6,
		"mud", 7,
		"mudRoad", 8,
		"stoneRoad", 9,
		"water", 10,
		"ice", 11,
		"snow", 12,
		"wood", 13,
		"dirt", 14,
		"unknown", 15
	);
	
	/***
	Enum of campaign ground types

	@tfield int lowFertility
	@tfield int mediumFertility
	@tfield int highFertility
	@tfield int wilderness
	@tfield int highMountains
	@tfield int lowMountains
	@tfield int hills
	@tfield int denseForest
	@tfield int woodland
	@tfield int swamp
	@tfield int ocean
	@tfield int deepSea
	@tfield int shallowSea
	@tfield int coast
	@tfield int impassableLand
	@tfield int impassableSea

	@table strategyGroundType
	*/
	luaState.new_enum(
		"strategyGroundType",
		"lowFertility", 0,
		"mediumFertility", 1,
		"highFertility", 2,
		"wilderness", 3,
		"highMountains", 4,
		"lowMountains", 5,
		"hills", 6,
		"denseForest", 7,
		"woodland", 8,
		"swamp", 9,
		"ocean", 10,
		"deepSea", 11,
		"shallowSea", 12,
		"coast", 13,
		"impassableLand", 14,
		"impassableSea", 15
	);
	
	/***
	Enum of engine types

	@tfield int catapult
	@tfield int trebuchet
	@tfield int ballista
	@tfield int bombard
	@tfield int grandBombard
	@tfield int hugeBombard
	@tfield int culverin
	@tfield int basilisk
	@tfield int cannon
	@tfield int mortar
	@tfield int scorpion
	@tfield int serpentine
	@tfield int rocketLauncher
	@tfield int ribault
	@tfield int monsterRibault
	@tfield int mangonel
	@tfield int tower
	@tfield int ram
	@tfield int ladder
	@tfield int holy_cart

	@table engineType
	*/
	luaState.new_enum(
		"engineType",
		"catapult", 0,
		"trebuchet", 1,
		"ballista", 2,
		"bombard", 3,
		"grandBombard", 4,
		"hugeBombard", 5,
		"culverin", 6,
		"basilisk", 7,
		"cannon", 8,
		"mortar", 9,
		"scorpion", 10,
		"serpentine", 11,
		"rocketLauncher", 12,
		"ribault", 13,
		"monsterRibault", 14,
		"mangonel", 15,
		"tower", 17,
		"ram", 18,
		"ladder", 19,
		"holy_cart", 20
	);
}

