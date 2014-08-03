#pragma once
#include "Common.h"
#include "rapidjson\document.h"
#include "rapidjson\writer.h"
#include "rapidjson\stringbuffer.h"

using namespace rapidjson;

enum UNIT
{
	SPEARMAN = 0,
	SWORDSMAN,
	CHARIOT,
	HORSEMAN,
	CATAPULT,
	GALLEY,

	MANATARMS,
	KNIGHT,
	TREBUCHET,
	CARAVEL,
	GALLEON,

	MUSKETMAN,
	RIFLEMAN,
	DRAGOON,
	CANNON,
	ARTILLERY,
	FRIGATE,
	IRONCLAD,

	MACHINEGUNNER,
	MECHANIZED_INFANTRY,
	TANK,
	MODERN_ARMOR,
	ROCKET_ARTILLERY,
	CRUISE_MISSLES,
	BATTLESHIP,
	CARRIER,
	BIPLANE,
	MONOPLANE,
	JET,
	STEALTH_AIRCRAFT,

	UNIT_COUNT
};

enum UNIT_CATEGORY
{
	INFANTRY_TYPE = 0,
	CAVALRY_TYPE,
	ARTILLERY_TYPE,
	FLEET_TYPE,
	AIRCRAFT_TYPE
};

class Unit
{
	static Unit* UnitList[UNIT_COUNT];
	static char* TextJSON;

	TECH_ERA Era;
	char* Name;
	char* UnlockedBy;
	UNIT Id;
    int DieRolled;
    int DieBonus;
	UNIT_CATEGORY Category;
	bool Unlocked;

public:

	Unit(UNIT id, char* name, TECH_ERA era, UNIT_CATEGORY Type, int dieRolled, int dieBonus);
	void GetJSON(Document* document, Value* array);
	static void GetJSONArray(Document* document, Value* array);
	static char* Unit::GetUnitStatusJSON();
	static Unit* Unit::GetUnit(UNIT unit);
	void LockUnit();
	void UnlockUnit(int owner);
	bool IsUnlocked();
	~Unit();
};

