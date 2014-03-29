#pragma once

#include "Upgrade.h"
#include "rapidjson\document.h"
#include "rapidjson\writer.h"
#include "rapidjson\stringbuffer.h"

#define MAX_CITY_COUNT 100

enum RESOURCE
{
	NO_RESOURCE,
	HORSES,
	SPICES,
	RARE_METALS,
	IRON,
	OIL,
	COAL,
	WINE,
	GEMS
};

using namespace rapidjson;

class City
{
	static int CityCount;
	static City* CityList[MAX_CITY_COUNT];
	static char* TextJSON;

	bool Fertile;
	int Size;
	RESOURCE Resource;

	int Owner;
	int Traded;
	int Id;

	UPGRADE Happy;
	UPGRADE Productive;

	bool isHappy();
	bool isProductive();
	int production();
	void GetJSON(Document* document, Value* array);

public:
	City(RESOURCE resource, int owner, bool fertile);
	static void Assign(int player, int city);
	static void SetSize(int city, int size);
	static void SetFertile(int city, int size);
	static void GetJSONArray(Document* document, Value* array);
	static char* GetCityStatusJSON();
	~City();
};

