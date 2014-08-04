#include "Upgrade.h"
#include "Player.h"

Upgrade* Upgrade::UpgradeList[UPGRADE_COUNT];
bool Upgrade::UpgradePurchased[6][UPGRADE_COUNT];
char* Upgrade::TextJSON = nullptr;

using namespace rapidjson;

Upgrade::Upgrade(UPGRADE id, char* name, TECH_ERA era, BENEFIT_TYPE type)
{
	Name = name;
	Id = id;
	Era = era;
	Type = type;
	Unlocked = false;
	UnlockedBy = "";
	upgradeIndex = -1;
	while ((1 << ++upgradeIndex) != id){};
	UpgradeList[upgradeIndex] = this;
	Deprecated = false;
	memset(UpgradePurchased, 0, sizeof(UpgradePurchased));
}

Upgrade* Upgrade::GetUpgrade(UPGRADE upgrade)
{
	int index = -1;
	while ((1 << ++index) != upgrade){};
	return UpgradeList[index];
}

void Upgrade::UnlockUpgrade(int owner)
{
	Unlocked = true;
	UnlockedBy = Player::GetPlayerName(owner);
}

void Upgrade::LockUpgrade()
{
	Unlocked = false;
	UnlockedBy = "";
}

bool Upgrade::IsUnlocked()
{
	return Unlocked;
}

void Upgrade::DeprecateUpgrade(UPGRADE upgrade)
{
	int index = -1;
	while ((1 << ++index) != upgrade){};
	UpgradeList[index]->Deprecated = true;
	Player::DeprecateUpgrade(upgrade, UpgradeList[index]->Type);
}

void Upgrade::PurchaseUpgrade(int player, UPGRADE upgrade)
{
	int index = -1;
	while ((1 << ++index) != upgrade){};
	if (!UpgradeList[index]->Deprecated)
	{
		Player::PurchaseUpgrade(player, upgrade, UpgradeList[index]->Type);
	}
	UpgradePurchased[player][index] = true;
}

static const char* purchased_ids[] = {"purchased_0", "purchased_1", "purchased_2", "purchased_3", "purchased_4", "purchased_5"};
void Upgrade::GetJSON(Document* document, Value* array)
{

	Value jsonObject(kObjectType);
	jsonObject.AddMember<int>("id", Id, document->GetAllocator());
	jsonObject.AddMember<char*>("name", Name, document->GetAllocator());
	jsonObject.AddMember<BENEFIT_TYPE>("type", Type, document->GetAllocator());
	jsonObject.AddMember<TECH_ERA>("era", Era, document->GetAllocator());
	jsonObject.AddMember<bool>("unlocked", Unlocked, document->GetAllocator());
	jsonObject.AddMember<char*>("unlockedby", UnlockedBy, document->GetAllocator());
	jsonObject.AddMember<int>("upgrade_index", upgradeIndex, document->GetAllocator());
	jsonObject.AddMember<bool>("deprecated", Deprecated, document->GetAllocator());

	// Send purchased indicator for each player
	for (int i = 0; i < 6; i++) // TODO
	{
		jsonObject.AddMember<bool>(purchased_ids[i], UpgradePurchased[i][upgradeIndex], document->GetAllocator());
	}

	array->PushBack(jsonObject, document->GetAllocator());
}

void Upgrade::GetJSONArray(Document* document, Value* array)
{
	for (int i = 0; i < UPGRADE_COUNT; i++)
	{
		UpgradeList[i]->GetJSON(document, array);
	}

	document->AddMember("upgrades", *array, document->GetAllocator());
}
void Upgrade::LoadState(Document* document)
{
	const Value& upgradeArray = (*document)["upgrades"];
	for (SizeType i = 0; i < upgradeArray.Size(); i++)
	{
		int upgradeIndex = upgradeArray[SizeType(i)]["upgrade_index"].GetInt();;
		UpgradeList[upgradeIndex]->Deprecated = upgradeArray[SizeType(i)]["deprecated"].GetBool();
	}
}

void Upgrade::SaveState(Document* document)
{
	Value upgradeArray(kArrayType);

	Upgrade::GetJSONArray(document, &upgradeArray);
}

char* Upgrade::GetUpgradeStatusJSON()
{
	Document document;

	document.Parse<0>("{}");
	StringBuffer ss;
	Writer<StringBuffer> writer(ss);
	Value upgrades(kArrayType);

	GetJSONArray(&document, &upgrades);
	
	document.Accept(writer);
	const char* str = ss.GetString();
	int strLen = strlen(str);

	if (TextJSON != nullptr)
	{
		delete TextJSON;
	}
	TextJSON = new char[strLen + 1];
	memset(TextJSON, 0, strLen + 1);
	memcpy(TextJSON, str, strLen);

	return TextJSON;
}

void Upgrade::GetBenefitFromType(BENEFIT_TYPE type, int& happyDiff, int& prodDiff)
{
	happyDiff = 0;
	prodDiff = 0;
	switch(type)
	{
	case NO_BENEFIT:
		break;
	case HAPPY:
		happyDiff = 1;
		break;
	case PRODUCTIVE:
		prodDiff = 1;
		break;
	case DOUBLE_HAPPY:
		happyDiff = 2;
		break;
	case DOUBLE_PRODUCTIVE:
		prodDiff = 2;
		break;
	}
}


Upgrade::~Upgrade()
{
}
