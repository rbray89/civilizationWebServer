#pragma once

#define MIN(x,y) ((x) < (y) ? (x) : (y))
#define MAX(x,y) ((x) > (y) ? (x) : (y))

enum TECH_ERA
{
	ANCIENT_ERA,
	MEDEVIAL_ERA,
	GUNPOWDER_ERA,
	MODERN_ERA
};

enum BENEFIT_TYPE
{
	// These values should not change
	NO_BENEFIT = -1,
	HAPPY = 0,
	PRODUCTIVE = 1,
	DOUBLE_HAPPY = 2,
	DOUBLE_PRODUCTIVE = 3,
};