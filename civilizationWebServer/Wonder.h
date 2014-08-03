#pragma once

#include "Common.h"
#include "rapidjson\document.h"
#include "rapidjson\writer.h"
#include "rapidjson\stringbuffer.h"

#define WONDER_COUNT 25

using namespace rapidjson;

class Wonder
{
	char* Name;
	char* Description;
	TECH_ERA Era;
	int Owner;
	int Id;
	BENEFIT_TYPE Benefit;

	static Wonder* Wonders[WONDER_COUNT];
	static int IdCount;
	static char* TextJSON;

public:
	Wonder(char* name, TECH_ERA era, BENEFIT_TYPE type, char* description);
	int GetId();
	static int GetVictoryPoints(int player);
	void SetOwner(int player, int previousOwner);
	void GetJSON(Document* document, Value* array);
	static char* GetWonderStatusJSON();
	~Wonder();
};

