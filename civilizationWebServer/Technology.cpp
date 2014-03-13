#include "Technology.h"
#include <stdarg.h>
#include "string.h"
#include "GameManager.h"

using namespace rapidjson;

int Technology::IdCount = 0;
Technology* Technology::TechTree[TECH_COUNT];

Technology::Technology()
{
	Owner = -1;
	DependsOn = nullptr;
}

Technology::Technology(char* name, TECH_ERA era, int cost, TECH_BENEFITS benefits)
{
	Owner = -1;
	DepCount = 0;
	DependsOn = nullptr;
	jsonObject = nullptr;
	Name = new char[strlen(name)+1];
	memcpy(Name, name, strlen(name)+1);
	Era = era;
	TechTree[IdCount] = this;
	Id = IdCount;
	Cost = cost;
	Benefits = benefits;
	IdCount++;
}

void Technology::SetDependencies(int n, ...)
{
	DepCount = n;
	DependsOn = new Technology*[n];
	va_list args;
	va_start(args, n);
	for (int i = 0; i < n; i++)
	{
		DependsOn[i] = va_arg(args, Technology*);
	}
	va_end(args);
}

Technology** Technology::GetTechTree()
{
	return TechTree;
}

const char* Technology::GetName()
{
	return Name;
}

int Technology::GetCost()
{
	return Cost;
}

void Technology::GetJSON(Document* document, Value* array)
{

	Value jsonObject(kObjectType);
	jsonObject.AddMember<int>("id", Id, document->GetAllocator());
	jsonObject.AddMember<char*>("name", Name, document->GetAllocator());
	jsonObject.AddMember<TECH_ERA>("era", Era, document->GetAllocator());
	jsonObject.AddMember<int>("cost", Cost, document->GetAllocator());
	jsonObject.AddMember<char*>("owner", Player::GetPlayerName(Owner), document->GetAllocator());
	jsonObject.AddMember<TECH_BENEFITS>("benefits", Benefits, document->GetAllocator());
	Value dependencies(kArrayType);
	for (int i = 0; i < DepCount; i++)
	{
		dependencies.PushBack<int>(DependsOn[i]->Id, document->GetAllocator());
	}
	jsonObject.AddMember("dependencies",dependencies, document->GetAllocator());
	array->PushBack(jsonObject, document->GetAllocator());
}

Technology::~Technology()
{
	delete Name;
	if (DependsOn != nullptr)
	{
		delete DependsOn;
	}
}
