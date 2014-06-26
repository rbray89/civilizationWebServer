#include "City.h"

int City::CityCount = 0;
City* City::CityList[MAX_CITY_COUNT];

using namespace rapidjson;
char* City::TextJSON = nullptr;

City::City(RESOURCE resource, int owner, bool fertile)
{
	this->Owner = owner;
	Id = CityCount;
	CityList[Id] = this;
	CityCount++;
	Fertile = fertile;
	Happy = NONE;
	Productive = NONE;
	Traded = -1;
	Size = 0;
	Resource = resource;
}

void City::Assign(int player, int city)
{
	CityList[city]->Happy = NONE;
	CityList[city]->Productive = NONE;
	CityList[city]->Owner = player;
	CityList[city]->Traded = -1;
}

void City::Trade(int player, int city)
{
	CityList[city]->Traded = player;
}

bool City::isHappy()
{
	if (Resource == GEMS || Resource == WINE || Upgrade::isActive(Happy))
	{
		return true;
	}

	return false;
}

bool City::isProductive()
{
	if (Fertile || Upgrade::isActive(Productive))
	{
		return true;
	}
	return false;
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
	jsonObject.AddMember<UPGRADE>("happy", Happy, document->GetAllocator());
	jsonObject.AddMember<bool>("isHappy", isHappy(), document->GetAllocator());
	jsonObject.AddMember<UPGRADE>("productive", Productive, document->GetAllocator());
	jsonObject.AddMember<bool>("isProductive", isProductive(), document->GetAllocator());
	jsonObject.AddMember<int>("size", Size, document->GetAllocator());
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

City::~City()
{
}
