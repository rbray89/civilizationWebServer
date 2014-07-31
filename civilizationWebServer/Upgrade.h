#pragma once
#include "Common.h"
#include "rapidjson\document.h"
#include "rapidjson\writer.h"
#include "rapidjson\stringbuffer.h"

using namespace rapidjson;

enum UPGRADE
{
	NONE = 0,
	ZIGGURAT = (1 << 0),
	GRANARY = (1 << 1),
	COURTHOUSE = (1 << 2),
	LIBRARY = (1 << 3),
	TEMPLE = (1 << 4),
	COLOSSEUM = (1 << 5),
	AQUEDUCT = (1 << 6),
	MARKETPLACE = (1 << 7),
	BANK = (1 << 8),
	CASTLE = (1 << 9),
	UNIVERSITY = (1 << 10),
	CATHEDRAL = (1 << 11),
	CAPITOL = (1 << 12),
	STOCK_MARKET = (1 << 13),
	LEGISLATURE = (1 << 14),
	FACTORY = (1 << 15),
	RAILROAD = (1 << 16),
	HOSPITAL = (1 << 17),
	HIGHWAY_SYSTEM = (1 << 18),
	MANUFACTURING_PLANT = (1 << 19),
	RESEARCH_LAB = (1 << 20),
	AIRPORT = (1 << 21),
	NUCLEAR_POWER_PLANT = (1 << 22),
	SHOPPING_MALL = (1 << 23),
	TELEVISION_STATION = (1 << 24),
	INTERNET = (1 << 25),
	HEALTHCARE_COMPLEX = (1 << 26),
	FUSION_REACTOR = (1 << 27),
	SIZE_2_CITIES = (1 << 28),
	SIZE_3_CITIES = (1 << 29),
	SIZE_4_CITIES = (1 << 30),
};

#define UPGRADE_COUNT 31

inline UPGRADE operator|(UPGRADE a, UPGRADE b)
{
	return static_cast<UPGRADE>(static_cast<int>(a) | static_cast<int>(b));
}

inline UPGRADE operator&(UPGRADE a, int b)
{
	return static_cast<UPGRADE>(static_cast<int>(a) & static_cast<int>(b));
}

class Upgrade
{
	static Upgrade* UpgradeList[UPGRADE_COUNT];
	static bool UpgradePurchased[6][UPGRADE_COUNT];
	static char* TextJSON;

	TECH_ERA Era;
	char* Name;
	char* UnlockedBy;
	UPGRADE Id;
	BENEFIT_TYPE Type;
	bool Deprecated;
	bool Unlocked;
	int upgradeIndex;

public:

	Upgrade(UPGRADE id, char* name, TECH_ERA era, BENEFIT_TYPE Type);
	static void DeprecateUpgrade(UPGRADE upgrade);
	static void PurchaseUpgrade(int player, UPGRADE upgrade);
	static bool isActive(UPGRADE upgrade);
	void GetJSON(Document* document, Value* array);
	static void GetJSONArray(Document* document, Value* array);
	static char* Upgrade::GetUpgradeStatusJSON();
	static void GetBenefitFromType(BENEFIT_TYPE type, int& happyDiff, int& prodDiff);
	static Upgrade* Upgrade::GetUpgrade(UPGRADE upgrade);
	void LockUpgrade();
	void UnlockUpgrade(int owner);
	bool IsUnlocked();
	~Upgrade();
};

