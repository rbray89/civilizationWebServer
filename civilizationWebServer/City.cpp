#include "City.h"
#include "Player.h"

int City::CityCount = 0;
City* City::CityList[MAX_CITY_COUNT];

#define UNIQUE_RESOURCE_BONUS_MULTIPLIER 3
#define MINIMUM_INCOME 10

// by city size
static int monopoly_bonus_incomes[] = {0, 0, 0, 20, 40, 80};

using namespace rapidjson;
char* City::TextJSON = nullptr;

City::City(RESOURCE resource, int owner, bool fertile)
{
	this->Owner = owner;
	Id = CityCount;
	CityList[Id] = this;
	CityCount++;
	Fertile = fertile;
	hasHappyUpgrade = false;
	hasProductivityUpgrade = false;
	Traded = -1;
	Size = 0;
	Resource = resource;
}

City::City(RESOURCE resource, int owner, bool fertile, int traded, bool isHappy, bool isProductive, int size)
{
	Id = CityCount;
	CityList[Id] = this;
	CityCount++;

	Resource = resource;
	Owner = owner;
	Fertile = fertile;
	this->Traded = traded;
	this->hasHappyUpgrade = isHappy;
	this->hasProductivityUpgrade = isProductive;
	this->Size = size;

	Player::ChangeAvailableUpgrades(owner,
		 Resource != GEMS && Resource != WINE && hasHappyUpgrade ? -1 : 0,
		 !fertile && hasProductivityUpgrade ? -1 : 0);
}

void City::Assign(int player, int city)
{
	// If the city was previously assigned, release the allocated
	// productivity and happiness improvements back to the previous owner
	if (CityList[city]->Owner >= 0)
	{
		Player::ChangeAvailableUpgrades(CityList[city]->Owner,
			CityList[city]->hasHappyUpgrade ? 1 : 0,
			CityList[city]->hasProductivityUpgrade ? 1 : 0);
	}

	CityList[city]->hasHappyUpgrade = false;
	CityList[city]->hasProductivityUpgrade = false;
	CityList[city]->Owner = player;
	CityList[city]->Traded = -1;
}

int City::GetPlayerTotalOutput(int player)
{
	int totalOutput = 0;
	int resourceCount[NUM_RESOURCES];
	memset(resourceCount, 0, sizeof(resourceCount));

	for (int i = 0; i < CityCount; i++)
	{
		if (CityList[i] != nullptr)
		{
			// The owner always gets the city's production
			if (CityList[i]->Owner == player)
			{
				totalOutput += CityList[i]->production();
			}

			// But only count it for monopoly/unique if
			// 1. It's traded to you (possibly by yourself), or
		    // 2. You own it and it's not traded to anyone
			if (CityList[i]->Traded == player || (CityList[i]->Traded == -1 && CityList[i]->Owner == player))
			{
				resourceCount[CityList[i]->Resource]++;
			}
		}
	}
	
	int numUniqueResources = 0;
	int monopolyBonus = 0;
	
	for (int i = NO_RESOURCE + 1; i < NUM_RESOURCES; i++)
	{
		if (resourceCount[i] > 0)
		{
			numUniqueResources++;
			monopolyBonus += monopoly_bonus_incomes[MIN(5, resourceCount[i])];
		}
	}

	return MAX(MINIMUM_INCOME, totalOutput + numUniqueResources*UNIQUE_RESOURCE_BONUS_MULTIPLIER + monopolyBonus);
}

int City::GetVictoryPoints(int player)
{
	int points = 0;

	for (int i = 0; i < CityCount; i++)
	{
		if (CityList[i] != nullptr)
		{
			// The owner always gets the city's production
			if (CityList[i]->Owner == player)
			{
				points += CityList[i]->Size + 1;
			}
		}
	}

	return points;
}

void City::Trade(int player, int city)
{
	CityList[city]->Traded = player;
}

bool City::isHappy()
{
	return (Resource == GEMS || Resource == WINE || hasHappyUpgrade);
}

bool City::isProductive()
{
	return Fertile || hasProductivityUpgrade;
}

void City::ToggleCityHappinessUpgrade(int city)
{
	if (CityList[city]->Resource != GEMS && CityList[city]->Resource != WINE)
	{
		int availHappyDiff = 0;
		if (!CityList[city]->hasHappyUpgrade)
		{
			if (Player::HasAvailableHappiness(CityList[city]->Owner))
			{
				CityList[city]->hasHappyUpgrade = true;
				availHappyDiff = -1;
			}
		}
		else
		{
			CityList[city]->hasHappyUpgrade = false;
			availHappyDiff = 1;
		}

		Player::ChangeAvailableUpgrades(CityList[city]->Owner, availHappyDiff, 0);
	}
}

void City::ToggleCityProductivityUpgrade(int city)
{
	if (!CityList[city]->Fertile)
	{
		int availProdDiff = 0;
		if (!CityList[city]->hasProductivityUpgrade)
		{
			if (Player::HasAvailableProductivity(CityList[city]->Owner))
			{
				CityList[city]->hasProductivityUpgrade = true;
				availProdDiff = -1;
			}
		}
		else
		{
			CityList[city]->hasProductivityUpgrade = false;
			availProdDiff = 1;
		}

		Player::ChangeAvailableUpgrades(CityList[city]->Owner, 0, availProdDiff);
	}
}

void City::IncreaseCitySize(int city)
{
	bool isUpgradeAllowed = false;
	switch(CityList[city]->Size + 1)
	{
	case 1:
		isUpgradeAllowed = Upgrade::GetUpgrade(SIZE_2_CITIES)->IsUnlocked();
		break;
	case 2:
		isUpgradeAllowed = Upgrade::GetUpgrade(SIZE_3_CITIES)->IsUnlocked();
		break;
	case 3:
		isUpgradeAllowed = Upgrade::GetUpgrade(SIZE_4_CITIES)->IsUnlocked();
		break;
	default:
		break;
	}

	if (isUpgradeAllowed)
	{
		CityList[city]->Size++;
	}
}


int City::production()
{
	int product = 0;
	if (Resource == NONE)
	{
		if (isHappy())
		{
			product = Size + 2;
		}
		else
		{
			product = Size;
		}
	}
	else
	{
		if (isHappy())
		{
			product = 4 + (Size * 2);
		}
		else
		{
			product = 2 + (Size * 2);
		}
	}

	if (isProductive())
	{
		product *= 2;
	}

	return product;
}

void City::GetJSON(Document* document, Value* array)
{

	Value jsonObject(kObjectType);
	jsonObject.AddMember<int>("id", Id, document->GetAllocator());
	jsonObject.AddMember<int>("owner", Owner, document->GetAllocator());
	jsonObject.AddMember<int>("traded", Traded, document->GetAllocator());
	jsonObject.AddMember<bool>("isHappy", isHappy(), document->GetAllocator());
	jsonObject.AddMember<bool>("isProductive", isProductive(), document->GetAllocator());
	jsonObject.AddMember<int>("size", Size, document->GetAllocator());
	jsonObject.AddMember<int>("isFertile", Fertile, document->GetAllocator());
	jsonObject.AddMember<RESOURCE>("resource", Resource, document->GetAllocator());
	jsonObject.AddMember<int>("production", production(), document->GetAllocator());
	array->PushBack(jsonObject, document->GetAllocator());
}

void City::GetJSONArray(Document* document, Value* array)
{

	for (int i = 0; i < CityCount; i++)
	{
		if (CityList[i] != nullptr)
		{
			CityList[i]->GetJSON(document, array);
		}
	}
	document->AddMember("cities", *array, document->GetAllocator());
}

char* City::GetCityStatusJSON()
{
	Document document;

	document.Parse<0>("{}");
	StringBuffer ss;
	Writer<StringBuffer> writer(ss);
	Value cities(kArrayType);

	GetJSONArray(&document, &cities);
	
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


void City::LoadState(Document* document)
{
	const Value& cityArray = (*document)["cities"];
	for (SizeType i = 0; i < cityArray.Size(); i++)
	{
		new City((RESOURCE) cityArray[SizeType(i)]["resource"].GetInt(),
								cityArray[SizeType(i)]["owner"].GetInt(),
								cityArray[SizeType(i)]["isFertile"].GetInt(),
								cityArray[SizeType(i)]["traded"].GetInt(),
								cityArray[SizeType(i)]["isHappy"].GetBool(),
								cityArray[SizeType(i)]["isProductive"].GetBool(),
								cityArray[SizeType(i)]["size"].GetInt());
	}
}

void City::SaveState(Document* document)
{
	Value cityArray(kArrayType);

	City::GetJSONArray(document, &cityArray);
}

City::~City()
{
}
