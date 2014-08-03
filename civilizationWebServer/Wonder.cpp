#include "Wonder.h"
#include "Upgrade.h"
#include "Player.h"

int Wonder::IdCount = 0;
Wonder* Wonder::Wonders[WONDER_COUNT];
char* Wonder::TextJSON;

Wonder::Wonder(char* name, TECH_ERA era, BENEFIT_TYPE benefit, char* description)
{
	Name = name;
	Era = era;
	Description = description;
	Id = IdCount;
	Benefit = benefit;
	Owner = -1;
	Wonders[IdCount++] = this;
}

int Wonder::GetId()
{
	return Id;
}

void Wonder::SetOwner(int player, int previousOwner)
{
	Owner = player;

	// Increment happy/prod counters if necessary
	int happyDiff = 0;
	int prodDiff = 0;
	Upgrade::GetBenefitFromType(Benefit, happyDiff, prodDiff);
	Player::ChangeAvailableUpgrades(player, happyDiff, prodDiff);
	Player::ChangeAvailableUpgrades(previousOwner, -happyDiff, -prodDiff);
}

int Wonder::GetVictoryPoints(int player)
{
	int points = 0;

	for (int i = 0; i < WONDER_COUNT; i++)
	{
		if (Wonders[i]->Owner == player)
		{
			points += 2;
		}
	}

	return points;
}

void Wonder::GetJSON(Document* document, Value* array)
{

	Value jsonObject(kObjectType);
	jsonObject.AddMember<int>("id", Id, document->GetAllocator());
	jsonObject.AddMember<char*>("name", Name, document->GetAllocator());
	jsonObject.AddMember<TECH_ERA>("era", Era, document->GetAllocator());
	jsonObject.AddMember<int>("owner", Owner, document->GetAllocator());
	jsonObject.AddMember<char*>("description", Description, document->GetAllocator());
	array->PushBack(jsonObject, document->GetAllocator());
}

char* Wonder::GetWonderStatusJSON()
{
	Document document;

	document.Parse<0>("{}");
	StringBuffer ss;
	Writer<StringBuffer> writer(ss);
	Value technologies(kArrayType);

	for (int i = 0; i < WONDER_COUNT; i++)
	{
		Wonders[i]->GetJSON(&document, &technologies);
	}
	document.AddMember("wonders", technologies, document.GetAllocator());

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

Wonder::~Wonder()
{
}
