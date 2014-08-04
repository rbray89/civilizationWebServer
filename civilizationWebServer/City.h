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
	GEMS,
	NUM_RESOURCES
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

	bool hasHappyUpgrade;
	bool hasProductivityUpgrade;

	bool isHappy();
	bool isProductive();
	int production();
	void GetJSON(Document* document, Value* array);

public:
	City(RESOURCE resource, int owner, bool fertile);
	City::City(RESOURCE resource, int owner, bool fertile, int traded, bool isHappy, bool isProductive, int size);
	static void Assign(int player, int city);
	static void Trade(int player, int city);
	static void SetSize(int city, int size);
	static void SetFertile(int city, int size);
	static void ToggleCityHappinessUpgrade(int city);
	static void ToggleCityProductivityUpgrade(int city);
	static void IncreaseCitySize(int city);
	static int  GetPlayerTotalOutput(int player);
	static int  GetVictoryPoints(int player);
	static void GetJSONArray(Document* document, Value* array);
	static char* GetCityStatusJSON();
	static void LoadState(Document* document);
	static void SaveState(Document* document);
	~City();
};

