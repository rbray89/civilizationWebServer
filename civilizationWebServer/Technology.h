#pragma once

#include "rapidjson\document.h"
#include "rapidjson\writer.h"
#include "rapidjson\stringbuffer.h"

#define TECH_COUNT 53
	
using namespace rapidjson;

enum TECH_ERA
{
	ANCIENT_ERA,
	MEDEVIAL_ERA,
	GUNPOWDER_ERA,
	MODERN_ERA
};

enum TECH_BENEFITS
{
	WONDER_BENEFIT = (1 << 0),
	SEMINAL_BENEFIT = (1 << 1),
	PRODUCTIVE_BENEFIT = (1 << 2),
	HAPPY_BENEFIT = (1 << 3),
	CITY_BENEFIT = (1 << 4),
	TRADE_BENEFIT = (1 << 5),
	INFANTRY_BENEFIT = (1 << 6),
	CAVALRY_BENEFIT = (1 << 7),
	ARTILLERY_BENEFIT = (1 << 8),
	FLEET_BENEFIT = (1 << 9),
	AIRCRAFT_BENEFIT = (1 << 10)
};
inline TECH_BENEFITS operator|(TECH_BENEFITS a, TECH_BENEFITS b)
{
	return static_cast<TECH_BENEFITS>(static_cast<int>(a) | static_cast<int>(b));
}


class Technology
{
private:
	char* Name;
	TECH_ERA Era;
	int Cost;
	int Owner;
	int DepCount;
	Technology** DependsOn;
	int Id;
	TECH_BENEFITS Benefits;
	Value* jsonObject;

	static Technology* TechTree[TECH_COUNT];
	static int IdCount;

public:
	static Technology** GetTechTree();

	Technology();
	Technology(char* name, TECH_ERA era, int cost, TECH_BENEFITS benefits);
	(const char*) GetName();
	int GetCost();
	void Purchase(int player);
	void GetJSON(Document* document, Value* array);
	void SetDependencies(int n, ...);
	~Technology();
};

