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
	Technology* HorsebackRiding = new Technology("Horseback Riding", ANCIENT_ERA, 40, CAVALRY_BENEFIT);
	Technology* CodeOfLaws = new Technology("Code of Laws", ANCIENT_ERA, 35, HAPPY_BENEFIT);
	Technology* Mathematics =new Technology("Mathematics", ANCIENT_ERA, 40, ARTILLERY_BENEFIT);
	Technology* IronWorking = new Technology("Iron Working", ANCIENT_ERA, 40, INFANTRY_BENEFIT);
	Technology* Philosophy_Literature = new Technology("Philosophy/Literature", ANCIENT_ERA, 40, PRODUCTIVE_BENEFIT|WONDER_BENEFIT);
	Technology* Mysticism = new Technology("Mysticism", ANCIENT_ERA, 40, HAPPY_BENEFIT|WONDER_BENEFIT);
	Technology* Trade_Mapmaking = new Technology("Trade/Mapmaking", ANCIENT_ERA, 50, TRADE_BENEFIT|FLEET_BENEFIT|WONDER_BENEFIT);
	Technology* Currency = new Technology("Currency", ANCIENT_ERA, 50, PRODUCTIVE_BENEFIT);
	Technology* Construction = new Technology("Construction", ANCIENT_ERA, 50, CITY_BENEFIT|PRODUCTIVE_BENEFIT|HAPPY_BENEFIT|WONDER_BENEFIT);

	HorsebackRiding->SetDependencies(1, TheWheel);
	CodeOfLaws->SetDependencies(2, Alphabet_Writing, Pottery_Specialization);
	Mathematics->SetDependencies(2, Masonry, Alphabet_Writing);
	IronWorking->SetDependencies(2, BronzeWorking, Pottery_Specialization);
	Philosophy_Literature->SetDependencies(2, Alphabet_Writing, Pottery_Specialization);
	Mysticism->SetDependencies(2, Pottery_Specialization, CeremonialBurial);
	Trade_Mapmaking->SetDependencies(2, Mathematics, Pottery_Specialization);
	Currency->SetDependencies(2, Mathematics, CodeOfLaws);
	Construction->SetDependencies(2, IronWorking, Mathematics);

	Technology* Banking = new Technology("Banking", MEDEVIAL_ERA, 50, PRODUCTIVE_BENEFIT);
	Technology* Education = new Technology("Education", MEDEVIAL_ERA, 60, PRODUCTIVE_BENEFIT | WONDER_BENEFIT);
	Technology* Engineering = new Technology("Engineering", MEDEVIAL_ERA, 70, ARTILLERY_BENEFIT);
	Technology* Theology = new Technology("Theology", MEDEVIAL_ERA, 70, HAPPY_BENEFIT | WONDER_BENEFIT);
	Technology* Feudalism = new Technology("Feudalism", MEDEVIAL_ERA, 70, HAPPY_BENEFIT | INFANTRY_BENEFIT | WONDER_BENEFIT);
	Technology* Astronomy = new Technology("Astronomy", MEDEVIAL_ERA, 60, TRADE_BENEFIT | FLEET_BENEFIT | WONDER_BENEFIT);
	Technology* PrintingPress = new Technology("Printing Press", MEDEVIAL_ERA, 40, SEMINAL_BENEFIT);
	Technology* Chivalry = new Technology("Chivalry", MEDEVIAL_ERA, 60, INFANTRY_BENEFIT);
	Technology* Navigation = new Technology("Navigation", MEDEVIAL_ERA, 60, TRADE_BENEFIT | FLEET_BENEFIT);
	Technology* Chemistry = new Technology("Chemistry", MEDEVIAL_ERA, 40, CITY_BENEFIT);
	
	Banking->SetDependencies(1, Currency);
	Education->SetDependencies(2, Trade_Mapmaking, Philosophy_Literature);
	Engineering->SetDependencies(2, TheWheel, Construction);
	Theology->SetDependencies(2, Philosophy_Literature, Mysticism);
	Feudalism->SetDependencies(2, CodeOfLaws, Construction);
	Astronomy->SetDependencies(2, Education, Engineering);
	PrintingPress->SetDependencies(2, Theology, Education);
	Chivalry->SetDependencies(2, HorsebackRiding, Feudalism);
	Navigation->SetDependencies(1, Astronomy);
	Chemistry->SetDependencies(2, PrintingPress, Education);

	Technology* Nationalism = new Technology("Nationalism", GUNPOWDER_ERA, 80, HAPPY_BENEFIT|WONDER_BENEFIT);
	Technology* Physics = new Technology("Physics", GUNPOWDER_ERA, 80, WONDER_BENEFIT);
	Technology* Economics = new Technology("Economics", GUNPOWDER_ERA, 90, PRODUCTIVE_BENEFIT | WONDER_BENEFIT);
	Technology* GunpowderWeapons = new Technology("Gunpowder Weapons", GUNPOWDER_ERA, 90, INFANTRY_BENEFIT);
	Technology* Cavalry = new Technology("Cavalry", GUNPOWDER_ERA, 80, CAVALRY_BENEFIT);
	Technology* LegislativeGovernment = new Technology("Legislative Government", GUNPOWDER_ERA, 70, HAPPY_BENEFIT);
	Technology* Metallurgy = new Technology("Metallurgy", GUNPOWDER_ERA, 100, ARTILLERY_BENEFIT | FLEET_BENEFIT | WONDER_BENEFIT);
	Technology* Medicine = new Technology("Medicine", GUNPOWDER_ERA, 70, HAPPY_BENEFIT | CITY_BENEFIT);
	Technology* SteamPower = new Technology("Steam Power", GUNPOWDER_ERA, 120, PRODUCTIVE_BENEFIT|FLEET_BENEFIT|INFANTRY_BENEFIT|TRADE_BENEFIT|WONDER_BENEFIT|SEMINAL_BENEFIT);
	Technology* Industrialization = new Technology("Industrialization", GUNPOWDER_ERA, 100, ARTILLERY_BENEFIT|PRODUCTIVE_BENEFIT);

	Nationalism->SetDependencies(2, Banking, PrintingPress);
	Physics->SetDependencies(2, Astronomy, Chemistry);
	Economics->SetDependencies(1, Nationalism);
	GunpowderWeapons->SetDependencies(1, Physics);
	Cavalry->SetDependencies(2, Chivalry, GunpowderWeapons);
	LegislativeGovernment->SetDependencies(1, Economics);
	Metallurgy->SetDependencies(2, IronWorking, GunpowderWeapons);
	Medicine->SetDependencies(1, Metallurgy);
	SteamPower->SetDependencies(1, Metallurgy);
	Industrialization->SetDependencies(2, Economics, SteamPower);

	Technology* ScientificMethod = new Technology("Scientific Method", MODERN_ERA, 100, PRODUCTIVE_BENEFIT| WONDER_BENEFIT);
	Technology* Electricity = new Technology("Electricity", MODERN_ERA, 100, INFANTRY_BENEFIT);
	Technology* Combustion = new Technology("Combustion", MODERN_ERA, 100, PRODUCTIVE_BENEFIT|FLEET_BENEFIT);
	Technology* Flight = new Technology("Flight", MODERN_ERA, 100, AIRCRAFT_BENEFIT | TRADE_BENEFIT);
	Technology* Fision = new Technology("Fision", MODERN_ERA, 150, PRODUCTIVE_BENEFIT | WONDER_BENEFIT);
	Technology* Electronics = new Technology("Electronics", MODERN_ERA, 70, WONDER_BENEFIT);
	Technology* MassProduction = new Technology("Mass Production", MODERN_ERA, 100, PRODUCTIVE_BENEFIT | CAVALRY_BENEFIT| WONDER_BENEFIT);
	Technology* Plastics = new Technology("Plastics", MODERN_ERA, 80, HAPPY_BENEFIT);
	Technology* Robotics = new Technology("Robotics", MODERN_ERA, 100, INFANTRY_BENEFIT);
	Technology* Radio_Television = new Technology("Radio/Television", MODERN_ERA, 120, HAPPY_BENEFIT | WONDER_BENEFIT);
	Technology* Computers = new Technology("Computers", MODERN_ERA, 100, SEMINAL_BENEFIT| PRODUCTIVE_BENEFIT| WONDER_BENEFIT);
	Technology* AdvancedFlight = new Technology("Advanced Flight", MODERN_ERA, 100, AIRCRAFT_BENEFIT|PRODUCTIVE_BENEFIT|FLEET_BENEFIT);
	Technology* Genetics = new Technology("Genetics", MODERN_ERA, 100, HAPPY_BENEFIT| WONDER_BENEFIT);
	Technology* Rocketry = new Technology("Rocketry", MODERN_ERA, 120, ARTILLERY_BENEFIT|AIRCRAFT_BENEFIT);
	Technology* Miniaturization = new Technology("Miniaturization", MODERN_ERA, 100, CAVALRY_BENEFIT);
	Technology* SpaceFlight = new Technology("Space Flight", MODERN_ERA, 150, ARTILLERY_BENEFIT | WONDER_BENEFIT);
	Technology* SuperConductor = new Technology("Super Conductor", MODERN_ERA, 120, AIRCRAFT_BENEFIT);
	Technology* Fusion = new Technology("Fusion", MODERN_ERA, 150, PRODUCTIVE_BENEFIT|WONDER_BENEFIT);

	ScientificMethod->SetDependencies(1, Medicine);
	Electricity->SetDependencies(1, SteamPower);
	Combustion->SetDependencies(1, Industrialization);
	Flight->SetDependencies(1, Combustion);
	Fision->SetDependencies(1, ScientificMethod);
	Electronics->SetDependencies(2, Electricity, ScientificMethod);
	MassProduction->SetDependencies(2, Combustion, Electricity);
	Plastics->SetDependencies(1, MassProduction);
	Robotics->SetDependencies(2, Electronics, MassProduction);
	Radio_Television->SetDependencies(1, Electronics);
	Computers->SetDependencies(1, Electronics);
	AdvancedFlight->SetDependencies(2, Flight, Radio_Television);
	Genetics->SetDependencies(2, Medicine, Computers);
	Rocketry->SetDependencies(1, AdvancedFlight);
	Miniaturization->SetDependencies(1, Computers);
	SpaceFlight->SetDependencies(2, Plastics, Computers);
	SuperConductor->SetDependencies(2, Miniaturization, SpaceFlight);
	Fusion->SetDependencies(2, Fision, SuperConductor);

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

void TechManager::Purchase(int tech, int player)
{
	TechTree[tech]->Purchase(player);
}

TechManager::~TechManager()
{
}
