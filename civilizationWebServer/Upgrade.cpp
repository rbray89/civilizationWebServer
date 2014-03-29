#include "Upgrade.h"
#include "Player.h"

Upgrade* Upgrade::UpgradeList[UPGRADE_COUNT];


using namespace rapidjson;

Upgrade::Upgrade(UPGRADE id, char* name, TECH_ERA era, UPGRADE_TYPE type)
{
	Name = name;
	Id = id;
	Era = era;
	Type = type;
	int index = -1;
	while ((1 << ++index) != id){};
	UpgradeList[index] = this;
	Depricated = false;
}

void Upgrade::InitUpgrades()
{
	new Upgrade(GRANARY, "Granary", ANCIENT_ERA, PRODUCTIVE);
	new Upgrade(ZIGGURAT, "Ziggurat", ANCIENT_ERA, HAPPY);
	new Upgrade(COURTHOUSE, "Couthouse", ANCIENT_ERA, HAPPY);
	new Upgrade(LIBRARY, "Library", ANCIENT_ERA, PRODUCTIVE);
	new Upgrade(TEMPLE, "Temple", ANCIENT_ERA, HAPPY);
	new Upgrade(COLOSSEUM, "Colosseum", ANCIENT_ERA, HAPPY);
	new Upgrade(AQUEDUCT, "Aqueduct", ANCIENT_ERA, PRODUCTIVE);
	new Upgrade(MARKETPLACE, "Marketplace", ANCIENT_ERA, PRODUCTIVE);

	new Upgrade(BANK, "Bank", MEDEVIAL_ERA, PRODUCTIVE);
	new Upgrade(CASTLE, "Castle", MEDEVIAL_ERA, HAPPY);
	new Upgrade(UNIVERSITY, "University", MEDEVIAL_ERA, PRODUCTIVE);
	new Upgrade(CATHEDRAL, "Cathedral", MEDEVIAL_ERA, DOUBLE_HAPPY);

	new Upgrade(CAPITAL, "Capital", GUNPOWDER_ERA, HAPPY);
	new Upgrade(STOCK_MARKET, "Stock Market", GUNPOWDER_ERA, PRODUCTIVE);
	new Upgrade(LEGISLATURE, "Legislature", GUNPOWDER_ERA, HAPPY);
	new Upgrade(FACTORY, "Factory", GUNPOWDER_ERA, PRODUCTIVE);
	new Upgrade(RAILROAD, "Railroad", GUNPOWDER_ERA, PRODUCTIVE);
	new Upgrade(HOSPITAL, "Hospital", GUNPOWDER_ERA, HAPPY);

	new Upgrade(HIGHWAY_SYSTEM, "Highway System", MODERN_ERA, PRODUCTIVE);
	new Upgrade(MANUFACTURING_PLANT, "Manufacturing Plant", MODERN_ERA, PRODUCTIVE);
	new Upgrade(RESEARCH_LAB, "Research Lab", MODERN_ERA, PRODUCTIVE);
	new Upgrade(AIRPORT, "Airport", MODERN_ERA, PRODUCTIVE);
	new Upgrade(NUKE_PLANT, "Nuculear Plant", MODERN_ERA, PRODUCTIVE);
	new Upgrade(SHOPPING_MALL, "Shopping Mall", MODERN_ERA, HAPPY);
	new Upgrade(TELEVISION_STATION, "Television Station", MODERN_ERA, DOUBLE_HAPPY);
	new Upgrade(INTERNET, "Internet", MODERN_ERA, PRODUCTIVE);
	new Upgrade(MEDICARE_COMPLEX, "Medicare Complex", MODERN_ERA, HAPPY);
	new Upgrade(FUSION_REACTOR, "Fusion Reactor", MODERN_ERA, DOUBLE_PRODUCTIVE);
}

void Upgrade::DepricateUpgrade(UPGRADE upgrade)
{
	int index = -1;
	while ((1 << ++index) != upgrade){};
	UpgradeList[index]->Depricated = true;
	Player::DepricateUpgrade(upgrade);
}

void Upgrade::PurchaseUpgrade(int player, UPGRADE upgrade)
{
	int index = -1;
	while ((1 << ++index) != upgrade){};
	if (!UpgradeList[index]->Depricated)
	{
		Player::PurchaseUpgrade(player, upgrade);
	}
}

bool Upgrade::isActive(UPGRADE upgrade)
{
	if (upgrade == NONE)
	{
		return false;
	}
	int index = -1;
	while ((1 << ++index) != upgrade){};
	return !(UpgradeList[index]->Depricated);
}

void Upgrade::GetJSON(Document* document, Value* array)
{

	Value jsonObject(kObjectType);
	jsonObject.AddMember<int>("id", Id, document->GetAllocator());
	jsonObject.AddMember<char*>("name", Name, document->GetAllocator());
	jsonObject.AddMember<TECH_ERA>("era", Era, document->GetAllocator());
	jsonObject.AddMember<UPGRADE_TYPE>("type", Type, document->GetAllocator());
	array->PushBack(jsonObject, document->GetAllocator());
}

void Upgrade::GetJSONArray(Document* document, Value* array)
{
	for (int i = 0; i < UPGRADE_COUNT; i++)
	{
		if (!UpgradeList[i]->Depricated)
		{
			UpgradeList[i]->GetJSON(document, array);
		}
	}
}


Upgrade::~Upgrade()
{
}
