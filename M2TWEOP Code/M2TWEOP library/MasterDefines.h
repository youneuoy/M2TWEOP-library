#pragma once
enum class GroundType
{
	low_fertility = 0,
	medium_fertility = 1,
	high_fertility = 2,
	wilderness = 3,
	high_moutains = 4,
	low_moutains = 5,
	hills = 6,
	dense_forest = 7,
	woodland = 8,
	swamp = 9,
	ocean = 10,
	deep_sea = 11,
	shallow_sea = 12,
	coast = 13,
	impassable_land = 14,
	impassable_sea = 15,
};

enum class StartMapObjectType :int
{
	FloatingGeneral = 0x23,
	Settlement = 0x1D,
	Fort = 0x1E,
	Port = 0x1F,
	Character = 0x1C,
	RallyPointSundry = 0x22
};