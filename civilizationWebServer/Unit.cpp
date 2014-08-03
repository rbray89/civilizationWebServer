#include "Unit.h"
#include "Player.h"

Unit* Unit::UnitList[UNIT_COUNT];
char* Unit::TextJSON = nullptr;

using namespace rapidjson;

Unit::Unit(UNIT id, char* name, TECH_ERA era, UNIT_CATEGORY category, int dieRolled, int dieBonus) :
 Id(id), Name(name), Era(era), Category(category), DieRolled(dieRolled), DieBonus(dieBonus)
{
	Unlocked = false;
	UnlockedBy = "";
	UnitList[id] = this;
}

void Unit::UnlockUnit(int owner)
{
	Unlocked = true;
	UnlockedBy = Player::GetPlayerName(owner);
}

void Unit::LockUnit()
{
	Unlocked = false;
	UnlockedBy = "";
}

bool Unit::IsUnlocked()
{
	return Unlocked;
}

void Unit::GetJSON(Document* document, Value* array)
{
	Value jsonObject(kObjectType);
	jsonObject.AddMember<int>("id", Id, document->GetAllocator());
	jsonObject.AddMember<char*>("name", Name, document->GetAllocator());
	jsonObject.AddMember<UNIT_CATEGORY>("category", Category, document->GetAllocator());
	jsonObject.AddMember<TECH_ERA>("era", Era, document->GetAllocator());
	jsonObject.AddMember<bool>("unlocked", Unlocked, document->GetAllocator());
	jsonObject.AddMember<char*>("unlockedby", UnlockedBy, document->GetAllocator());
	jsonObject.AddMember<int>("dieRolled", DieRolled, document->GetAllocator());
	jsonObject.AddMember<int>("dieBonus", DieBonus, document->GetAllocator());

	array->PushBack(jsonObject, document->GetAllocator());
}

void Unit::GetJSONArray(Document* document, Value* array)
{
	for (int i = 0; i < UNIT_COUNT; i++)
	{
		UnitList[i]->GetJSON(document, array);
	}

	document->AddMember("units", *array, document->GetAllocator());
}

char* Unit::GetUnitStatusJSON()
{
	Document document;

	document.Parse<0>("{}");
	StringBuffer ss;
	Writer<StringBuffer> writer(ss);
	Value units(kArrayType);

	GetJSONArray(&document, &units);
	
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


Unit::~Unit()
{
}
