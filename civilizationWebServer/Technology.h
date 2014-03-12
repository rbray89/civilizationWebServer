#pragma once

#include "rapidjson\document.h"
#include "rapidjson\writer.h"
#include "rapidjson\stringbuffer.h"

//#define TECH_COUNT 53
#define TECH_COUNT 15
	
using namespace rapidjson;

enum TECH_ERA
{
	ANCIENT_ERA,
	MEDIVIAL_ERA,
	GUNPOWDER_ERA,
	MODERN_ERA
};

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
	Value* jsonObject;

	static Technology* TechTree[TECH_COUNT];
	static int IdCount;

public:
	static Technology** GetTechTree();

	Technology();
	Technology(char* name, TECH_ERA era, int cost);
	(const char*) GetName();
	int GetCost();
	void GetJSON(Document* document, Value* array);
	void SetDependencies(int n, ...);
	~Technology();
};

