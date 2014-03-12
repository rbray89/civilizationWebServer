#include "TechManager.h"
#include "rapidjson\document.h"
#include "rapidjson\writer.h"
#include "rapidjson\stringbuffer.h"

using namespace rapidjson;

TechManager::TechManager()
{
	TextJSON = nullptr;

	Technology* Masonry = new Technology("Masonry", ANCIENT_ERA, 15, WONDER_BENEFIT);
	Technology* Alphabet_Writing = new Technology("Alphabet/Writing", ANCIENT_ERA, 15, SEMINAL_BENEFIT);
	Technology* Pottery_Specialization = new Technology("Pottery/Specialization", ANCIENT_ERA, 20, PRODUCTIVE_BENEFIT);
	Technology* TheWheel = new Technology("The Wheel", ANCIENT_ERA, 25, CAVALRY_BENEFIT);
	Technology* BronzeWorking = new Technology("Bronze Working", ANCIENT_ERA, 25, INFANTRY_BENEFIT | WONDER_BENEFIT);
	Technology* CeremonialBurial = new Technology("Ceremonial Burial", ANCIENT_ERA, 25, HAPPY_BENEFIT | WONDER_BENEFIT);
	Technology* CodeOfLaws = new Technology("Code of Laws", ANCIENT_ERA, 35, HAPPY_BENEFIT);
	Technology* HorsebackRiding = new Technology("Horseback Riding", ANCIENT_ERA, 40, CAVALRY_BENEFIT);
	Technology* Mathematics =new Technology("Mathematics", ANCIENT_ERA, 40, ARTILLERY_BENEFIT);
	Technology* IronWorking = new Technology("Iron Working", ANCIENT_ERA, 40, INFANTRY_BENEFIT);
	Technology* Philosophy_Literature = new Technology("Philosophy/Literature", ANCIENT_ERA, 40, PRODUCTIVE_BENEFIT|WONDER_BENEFIT);
	Technology* Mysticism = new Technology("Mysticism", ANCIENT_ERA, 40, HAPPY_BENEFIT|WONDER_BENEFIT);
	Technology* Trade_Mapmaking = new Technology("Trade/Mapmaking", ANCIENT_ERA, 50, TRADE_BENEFIT|FLEET_BENEFIT|WONDER_BENEFIT);
	Technology* Construction = new Technology("Construction", ANCIENT_ERA, 50, CITY_BENEFIT|PRODUCTIVE_BENEFIT|HAPPY_BENEFIT|WONDER_BENEFIT);
	Technology* Currency = new Technology("Currency", ANCIENT_ERA, 50, PRODUCTIVE_BENEFIT);

	HorsebackRiding->SetDependencies(1, TheWheel);
	Mathematics->SetDependencies(2, Masonry, Alphabet_Writing);
	Trade_Mapmaking->SetDependencies(2, Mathematics, Pottery_Specialization);
	IronWorking->SetDependencies(2, BronzeWorking, Pottery_Specialization);
	Construction->SetDependencies(2, IronWorking, Mathematics);
	CodeOfLaws->SetDependencies(2, Alphabet_Writing, Pottery_Specialization);
	Currency->SetDependencies(2, Mathematics, CodeOfLaws);
	Philosophy_Literature->SetDependencies(2, Alphabet_Writing, Pottery_Specialization);
	Mysticism->SetDependencies(2, Pottery_Specialization, CeremonialBurial);

	TechTree = Technology::GetTechTree();
}

char* TechManager::GetTechStatusJSON()
{
	Document document;

	document.Parse<0>("{}");
	StringBuffer ss;
	Writer<StringBuffer> writer(ss);
	Value technologies(kArrayType);

	for (int i = 0; i < TECH_COUNT; i++)
	{
		TechTree[i]->GetJSON(&document, &technologies);
	}
	document.AddMember("technologies", technologies, document.GetAllocator());

	document.Accept(writer);
	const char* str = ss.GetString();
	int strLen = strlen(str);

	if (TextJSON != nullptr)
	{
		delete TextJSON;
	}
	TextJSON = new char[strLen+1];
	memset(TextJSON, 0, strLen+1);
	memcpy(TextJSON, str, strLen);

	return TextJSON;
}

TechManager::~TechManager()
{
}
