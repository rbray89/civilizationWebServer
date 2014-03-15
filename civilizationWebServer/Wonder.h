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

	static Wonder* Wonders[WONDER_COUNT];
	static int IdCount;
	static char* TextJSON;

public:
	Wonder(char* name, TECH_ERA era, char* description);
	int GetId();
	void SetOwner(int player);
	void GetJSON(Document* document, Value* array);
	static char* GetWonderStatusJSON();
	~Wonder();
};

