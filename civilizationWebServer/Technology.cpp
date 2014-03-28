#include "Technology.h"
#include <stdarg.h>
#include "string.h"
#include "GameManager.h"
#include "Wonder.h"

using namespace rapidjson;

int Technology::IdCount = 0;
Technology* Technology::TechTree[TECH_COUNT];
char* Technology::TextJSON;

void Technology::InitTech()
{
	TextJSON = nullptr;

	Wonder* HangingGardens = new Wonder("The Hanging Gardens", ANCIENT_ERA, "Two free settlers");
	Wonder* Colossus = new Wonder("The Colossus", ANCIENT_ERA, "+5 gold/turn during the Ancient Era");
	Wonder* GreatLighthouse = new Wonder("The Great Lighthouse", ANCIENT_ERA, "+5 gold/turn during the Ancient Era");
	Wonder* GreatLibrary = new Wonder("The Great Library", ANCIENT_ERA, "Cost for Ancient Technologies -5");
	Wonder* Oracle = new Wonder("The Oracle", ANCIENT_ERA, "Player sees which combat unit opponent picks each round before picking his or her own unit");
	Wonder* GreatWall = new Wonder("The Great Wall", ANCIENT_ERA, "Player's cities give +2 for defending units");
	Wonder* Pyramids = new Wonder("The Pyramids", ANCIENT_ERA, "Two free ancient cavalry units");

	Technology* Masonry = new Technology("Masonry", ANCIENT_ERA, 15, WONDER_BENEFIT, HangingGardens);
	Technology* Alphabet_Writing = new Technology("Alphabet/Writing", ANCIENT_ERA, 15, SEMINAL_BENEFIT);
	Technology* Pottery_Specialization = new Technology("Pottery/Specialization", ANCIENT_ERA, 20, PRODUCTIVE_BENEFIT);
	Technology* TheWheel = new Technology("The Wheel", ANCIENT_ERA, 25, CAVALRY_BENEFIT);
	Technology* BronzeWorking = new Technology("Bronze Working", ANCIENT_ERA, 25, INFANTRY_BENEFIT | WONDER_BENEFIT, Colossus);
	Technology* CeremonialBurial = new Technology("Ceremonial Burial", ANCIENT_ERA, 25, HAPPY_BENEFIT | WONDER_BENEFIT, Pyramids);
	Technology* HorsebackRiding = new Technology("Horseback Riding", ANCIENT_ERA, 40, CAVALRY_BENEFIT);
	Technology* CodeOfLaws = new Technology("Code of Laws", ANCIENT_ERA, 35, HAPPY_BENEFIT);
	Technology* Mathematics = new Technology("Mathematics", ANCIENT_ERA, 40, ARTILLERY_BENEFIT);
	Technology* IronWorking = new Technology("Iron Working", ANCIENT_ERA, 40, INFANTRY_BENEFIT);
	Technology* Philosophy_Literature = new Technology("Philosophy/Literature", ANCIENT_ERA, 40, PRODUCTIVE_BENEFIT | WONDER_BENEFIT, GreatLibrary);
	Technology* Mysticism = new Technology("Mysticism", ANCIENT_ERA, 40, HAPPY_BENEFIT | WONDER_BENEFIT, Oracle);
	Technology* Trade_Mapmaking = new Technology("Trade/Mapmaking", ANCIENT_ERA, 50, TRADE_BENEFIT | FLEET_BENEFIT | WONDER_BENEFIT, GreatLighthouse);
	Technology* Currency = new Technology("Currency", ANCIENT_ERA, 50, PRODUCTIVE_BENEFIT);
	Technology* Construction = new Technology("Construction", ANCIENT_ERA, 50, CITY_BENEFIT | PRODUCTIVE_BENEFIT | HAPPY_BENEFIT | WONDER_BENEFIT, GreatWall);

	Pottery_Specialization->BenefitText = "Free Granary improvement";
	TheWheel->BenefitText = "One free Chariot unit";
	HorsebackRiding->BenefitText = "One free Horesman unit";
	CodeOfLaws->BenefitText = "Free Courthouse improvement";
	Mathematics->BenefitText = "One free Catapult unit";
	IronWorking->BenefitText = "One free Swordsman unity";
	Currency->BenefitText = "Free Marketplace improvement";

	Pottery_Specialization->EnablesText = "Granary";
	TheWheel->EnablesText = "Chariot";
	BronzeWorking->EnablesText = "Spearman";
	CeremonialBurial->EnablesText = "Ziggurat";
	HorsebackRiding->EnablesText = "Horseman";
	CodeOfLaws->EnablesText = "Courthouse";
	Mathematics->EnablesText = "Catapult";
	IronWorking->EnablesText = "Swordsman";
	Philosophy_Literature->EnablesText = "Library";
	Mysticism->EnablesText = "Temple";
	Trade_Mapmaking->EnablesText = "Galley, Trade1-sea";
	Currency->EnablesText = "Marketplace";
	Construction->EnablesText = "Aqueduct, Coloseum, Size2 Cities";

	HorsebackRiding->SetDependencies(1, TheWheel);
	CodeOfLaws->SetDependencies(2, Alphabet_Writing, Pottery_Specialization);
	Mathematics->SetDependencies(2, Masonry, Alphabet_Writing);
	IronWorking->SetDependencies(2, BronzeWorking, Pottery_Specialization);
	Philosophy_Literature->SetDependencies(2, Alphabet_Writing, Pottery_Specialization);
	Mysticism->SetDependencies(2, Pottery_Specialization, CeremonialBurial);
	Trade_Mapmaking->SetDependencies(2, Mathematics, Pottery_Specialization);
	Currency->SetDependencies(2, Mathematics, CodeOfLaws);
	Construction->SetDependencies(2, IronWorking, Mathematics);

	Wonder* ArtOfWar = new Wonder("SUn Tsu's Art of War", MEDEVIAL_ERA, "Two free medieval infantry units");
	Wonder* CopernicusObservatory = new Wonder("Copernicus' Observatory", MEDEVIAL_ERA, "One free medieval navy");
	Wonder* LeonardosWorkshop = new Wonder("Leonardo's Workshop", MEDEVIAL_ERA, "Cost for Medieval Technologies -10");
	Wonder* BachsCathedral = new Wonder("J.S. Bach's Cathedral", MEDEVIAL_ERA, "One extra Happiness Point");
	Wonder* SistineChapel = new Wonder("The Sistine Chapel", MEDEVIAL_ERA, "One extra Happines Point");

	Technology* Banking = new Technology("Banking", MEDEVIAL_ERA, 50, PRODUCTIVE_BENEFIT);
	Technology* Education = new Technology("Education", MEDEVIAL_ERA, 60, PRODUCTIVE_BENEFIT | WONDER_BENEFIT, LeonardosWorkshop);
	Technology* Engineering = new Technology("Engineering", MEDEVIAL_ERA, 70, ARTILLERY_BENEFIT);
	Technology* Theology = new Technology("Theology", MEDEVIAL_ERA, 70, HAPPY_BENEFIT | WONDER_BENEFIT, BachsCathedral, SistineChapel);
	Technology* Feudalism = new Technology("Feudalism", MEDEVIAL_ERA, 70, HAPPY_BENEFIT | INFANTRY_BENEFIT | WONDER_BENEFIT, ArtOfWar);
	Technology* Astronomy = new Technology("Astronomy", MEDEVIAL_ERA, 60, TRADE_BENEFIT | FLEET_BENEFIT | WONDER_BENEFIT, CopernicusObservatory);
	Technology* PrintingPress = new Technology("Printing Press", MEDEVIAL_ERA, 40, SEMINAL_BENEFIT);
	Technology* Chivalry = new Technology("Chivalry", MEDEVIAL_ERA, 60, CAVALRY_BENEFIT);
	Technology* Navigation = new Technology("Navigation", MEDEVIAL_ERA, 60, TRADE_BENEFIT | FLEET_BENEFIT);
	Technology* Chemistry = new Technology("Chemistry", MEDEVIAL_ERA, 40, CITY_BENEFIT);

	Banking->EnablesText = "Bank";
	Education->EnablesText = "University";
	Engineering->EnablesText = "Trebuche";
	Theology->EnablesText = "Cathedral";
	Feudalism->EnablesText = "Man-At-Arms, Castle";
	Astronomy->EnablesText = "Caravel, Trade2-sea";
	Chivalry->EnablesText = "Knight";
	Navigation->EnablesText = "Galleons, Trade3";
	Chemistry->EnablesText = "Size3 Cities";

	Banking->BenefitText = "Free Bank Improvment";
	Engineering->BenefitText = "One free Trebuche unit";
	Chivalry->BenefitText = "One free Knight unit";
	Navigation->BenefitText = "One free Galleon unit";
	Chemistry->BenefitText = "One free city upgrade";

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

	Wonder* MagellansVoyage = new Wonder("Magellan's Voyage", GUNPOWDER_ERA, "Player can force one resource (1-1) trade during each trade phase");
	Wonder* NewtonsUniversity = new Wonder("Newton's University", GUNPOWDER_ERA, "Cost for Gunpowder/Industrial Technologies -15");
	Wonder* SmithsTradingCompany = new Wonder("Smith's Trading Company", GUNPOWDER_ERA, "+10 gold/turn");
	Wonder* ShakespearesTheatre = new Wonder("Shakespeare's Theatre", GUNPOWDER_ERA, "One extra Happiness Point");
	Wonder* TranscontinentalRailroad = new Wonder("Transcontinental Railroad", GUNPOWDER_ERA, "One extra Productivity Point");

	Technology* Nationalism = new Technology("Nationalism", GUNPOWDER_ERA, 80, HAPPY_BENEFIT | WONDER_BENEFIT, ShakespearesTheatre);
	Technology* Physics = new Technology("Physics", GUNPOWDER_ERA, 80, WONDER_BENEFIT, NewtonsUniversity);
	Technology* Economics = new Technology("Economics", GUNPOWDER_ERA, 90, PRODUCTIVE_BENEFIT | WONDER_BENEFIT, SmithsTradingCompany);
	Technology* GunpowderWeapons = new Technology("Gunpowder Weapons", GUNPOWDER_ERA, 90, INFANTRY_BENEFIT);
	Technology* Cavalry = new Technology("Cavalry", GUNPOWDER_ERA, 80, CAVALRY_BENEFIT);
	Technology* LegislativeGovernment = new Technology("Legislative Government", GUNPOWDER_ERA, 70, HAPPY_BENEFIT);
	Technology* Metallurgy = new Technology("Metallurgy", GUNPOWDER_ERA, 100, ARTILLERY_BENEFIT | FLEET_BENEFIT | WONDER_BENEFIT, MagellansVoyage);
	Technology* Medicine = new Technology("Medicine", GUNPOWDER_ERA, 70, HAPPY_BENEFIT | CITY_BENEFIT);
	Technology* SteamPower = new Technology("Steam Power", GUNPOWDER_ERA, 120, PRODUCTIVE_BENEFIT | FLEET_BENEFIT | INFANTRY_BENEFIT | TRADE_BENEFIT | WONDER_BENEFIT | SEMINAL_BENEFIT, TranscontinentalRailroad);
	Technology* Industrialization = new Technology("Industrialization", GUNPOWDER_ERA, 100, ARTILLERY_BENEFIT | PRODUCTIVE_BENEFIT);

	Nationalism->EnablesText = "Capitol";
	Economics->EnablesText = "Stock Market";
	GunpowderWeapons->EnablesText = "Muketman";
	Cavalry->EnablesText = "Dragoon";
	LegislativeGovernment->EnablesText = "Legislature";
	Metallurgy->EnablesText = "Cannon, Frigate";
	Medicine->EnablesText = "Hospital, Size4 Cities";
	SteamPower->EnablesText = "Ironclad, Rifleman, Railroad, Trade4-land";
	Industrialization->EnablesText = "Artillery, Factory";

	GunpowderWeapons->BenefitText = "One free Musketman unit";
	Cavalry->BenefitText = "One free Dragoon unit";
	LegislativeGovernment->BenefitText = "Free Legilature improvement";
	Medicine->BenefitText = "Free Hospital improvement";
	Industrialization->BenefitText = "Free Factory improvement";

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

	Wonder* EdisonsLab = new Wonder("Edison's Lab", MODERN_ERA, "Cost for Modern Technologies -20");
	Wonder* HooverDam = new Wonder("Hoover Dam", MODERN_ERA, "One extra Productivity Point");
	Wonder* CureForCancer = new Wonder("Cure For Cancer", MODERN_ERA, "Two extra Happiness Points");
	Wonder* UniversalSuffrage = new Wonder("Universal Suffrage", MODERN_ERA, "One extra Happiness Point");
	Wonder* ManhattanProject = new Wonder("Manhattan Project", MODERN_ERA, "Player may choose any region on the board. All units and the city in that region are eliminated (one time only)");
	Wonder* UnitedNations = new Wonder("United Nations", MODERN_ERA, "Player may end the game at the end of any turn (Diplomatic)");
	Wonder* ApolloProgram = new Wonder("Apollo Program", MODERN_ERA, "Player may end the game at the end of any turn (Military)");
	Wonder* AlphaCentauriColonyShip = new Wonder("Alpha Centauri Colony Ship", MODERN_ERA, "Game ends immediately when this wonder is built (200 Gold) (Technology)");

	Technology* ScientificMethod = new Technology("Scientific Method", MODERN_ERA, 100, PRODUCTIVE_BENEFIT | WONDER_BENEFIT, EdisonsLab);
	Technology* Electricity = new Technology("Electricity", MODERN_ERA, 100, INFANTRY_BENEFIT);
	Technology* Combustion = new Technology("Combustion", MODERN_ERA, 100, PRODUCTIVE_BENEFIT | FLEET_BENEFIT);
	Technology* Flight = new Technology("Flight", MODERN_ERA, 100, AIRCRAFT_BENEFIT | TRADE_BENEFIT);
	Technology* Fission = new Technology("Fision", MODERN_ERA, 150, PRODUCTIVE_BENEFIT | WONDER_BENEFIT, ManhattanProject);
	Technology* Electronics = new Technology("Electronics", MODERN_ERA, 70, WONDER_BENEFIT, HooverDam);
	Technology* MassProduction = new Technology("Mass Production", MODERN_ERA, 100, PRODUCTIVE_BENEFIT | CAVALRY_BENEFIT | WONDER_BENEFIT, UniversalSuffrage);
	Technology* Plastics = new Technology("Plastics", MODERN_ERA, 80, HAPPY_BENEFIT);
	Technology* Robotics = new Technology("Robotics", MODERN_ERA, 100, INFANTRY_BENEFIT);
	Technology* Radio_Television = new Technology("Radio/Television", MODERN_ERA, 120, HAPPY_BENEFIT | WONDER_BENEFIT, UnitedNations);
	Technology* Computers = new Technology("Computers", MODERN_ERA, 100, SEMINAL_BENEFIT | PRODUCTIVE_BENEFIT);
	Technology* AdvancedFlight = new Technology("Advanced Flight", MODERN_ERA, 100, AIRCRAFT_BENEFIT | PRODUCTIVE_BENEFIT | FLEET_BENEFIT);
	Technology* Genetics = new Technology("Genetics", MODERN_ERA, 100, HAPPY_BENEFIT | WONDER_BENEFIT, CureForCancer);
	Technology* Rocketry = new Technology("Rocketry", MODERN_ERA, 120, ARTILLERY_BENEFIT | AIRCRAFT_BENEFIT);
	Technology* Miniaturization = new Technology("Miniaturization", MODERN_ERA, 100, CAVALRY_BENEFIT);
	Technology* SpaceFlight = new Technology("Space Flight", MODERN_ERA, 150, ARTILLERY_BENEFIT | WONDER_BENEFIT, ApolloProgram);
	Technology* SuperConductor = new Technology("Super Conductor", MODERN_ERA, 120, AIRCRAFT_BENEFIT);
	Technology* Fusion = new Technology("Fusion", MODERN_ERA, 150, PRODUCTIVE_BENEFIT | WONDER_BENEFIT, AlphaCentauriColonyShip);

	ScientificMethod->EnablesText = "Research Lab";
	Electricity->EnablesText = "Machine Gunner";
	Combustion->EnablesText = "Highway System";
	Flight->EnablesText = "Biplane, Trade-All";
	Fission->EnablesText = "Nuclear Power Plant";
	MassProduction->EnablesText = "Tank, Manufacturing Plant";
	Plastics->EnablesText = "Shopping Mall";
	Robotics->EnablesText = "Mechanized Infantry";
	Radio_Television->EnablesText = "Television Station";
	Computers->EnablesText = "The Internet";
	AdvancedFlight->EnablesText = "Aircraft Carrier, Monoplane, Airport";
	Genetics->EnablesText = "Healthcare Complex";
	Rocketry->EnablesText = "Rocket Artillery, Jets";
	Miniaturization->EnablesText = "Modern Armor Infantry";
	SpaceFlight->EnablesText = "Cruise Missiles";
	SuperConductor->EnablesText = "Stealth Aircraft";
	Fusion->EnablesText = "Fusion Reactor";

	Electricity->BenefitText = "One free Machinegunner unit";
	Combustion->BenefitText = "One free Batteship unit";
	Flight->BenefitText = "One free Biplane unit";
	Plastics->BenefitText = "Free Shopping Mall improvement";
	Robotics->BenefitText = "One free Mechanized Infantry unit";
	AdvancedFlight->BenefitText = "One free Monoplane unit";
	Rocketry->BenefitText = "One free Rocket Artillery unit";
	Miniaturization->BenefitText = "One free Modern Armor unit";
	SuperConductor->BenefitText = "One free Stealth Aircraft";

	ScientificMethod->SetDependencies(1, Medicine);
	Electricity->SetDependencies(1, SteamPower);
	Combustion->SetDependencies(1, Industrialization);
	Flight->SetDependencies(1, Combustion);
	Fission->SetDependencies(1, ScientificMethod);
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
	Fusion->SetDependencies(2, Fission, SuperConductor);

}

char* Technology::GetTechStatusJSON()
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
	TextJSON = new char[strLen + 1];
	memset(TextJSON, 0, strLen + 1);
	memcpy(TextJSON, str, strLen);

	return TextJSON;
}

void Technology::Purchase(int tech, int player)
{
	TechTree[tech]->Purchase(player);
}


Technology::Technology()
{
	Owner = -1;
	DependsOn = nullptr;
}

Technology::Technology(char* name, TECH_ERA era, int cost, TECH_BENEFITS benefits, Wonder* wonder1, Wonder* wonder2)
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
	BenefitText = nullptr;
	EnablesText = nullptr;
	IdCount++;
	if (wonder1 != nullptr && wonder2 != nullptr)
	{
		WonderCount = 2;
		Wonders = new Wonder*[WonderCount];
		Wonders[0] = wonder1;
		Wonders[1] = wonder2;
	}
	else if (wonder1 != nullptr)
	{
		WonderCount = 1;
		Wonders = new Wonder*[WonderCount];
		Wonders[0] = wonder1;
	}
	else
	{
		WonderCount = 0;
		Wonders = nullptr;
	}
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

void Technology::Purchase(int player)
{
	Owner = player;
	for (int i = 0; i < WonderCount; i++)
	{
		Wonders[i]->SetOwner(player);
	}
}

void Technology::GetJSON(Document* document, Value* array, bool minimal)
{

	Value jsonObject(kObjectType);
	jsonObject.AddMember<int>("id", Id, document->GetAllocator());
	jsonObject.AddMember<int>("owner", Owner, document->GetAllocator());
	if (!minimal)
	{

		jsonObject.AddMember<char*>("name", Name, document->GetAllocator());
		jsonObject.AddMember<TECH_ERA>("era", Era, document->GetAllocator());
		jsonObject.AddMember<int>("cost", Cost, document->GetAllocator());

		jsonObject.AddMember<TECH_BENEFITS>("benefits", Benefits, document->GetAllocator());
		if (BenefitText != nullptr)
		{
			jsonObject.AddMember<char*>("benefit_text", BenefitText, document->GetAllocator());
		}
		else
		{
			Value nullVal(kNullType);
			jsonObject.AddMember("benefit_text", nullVal, document->GetAllocator());
		}
		if (EnablesText != nullptr)
		{
			jsonObject.AddMember<char*>("enables_text", EnablesText, document->GetAllocator());
		}
		else
		{
			Value nullVal(kNullType);
			jsonObject.AddMember("enables_text", nullVal, document->GetAllocator());
		}
		Value wonders(kArrayType);
		for (int i = 0; i < WonderCount; i++)
		{
			wonders.PushBack<int>(Wonders[i]->GetId(), document->GetAllocator());
		}
		jsonObject.AddMember("wonders", wonders, document->GetAllocator());
		Value dependencies(kArrayType);
		for (int i = 0; i < DepCount; i++)
		{
			dependencies.PushBack<int>(DependsOn[i]->Id, document->GetAllocator());
		}
		jsonObject.AddMember("dependencies", dependencies, document->GetAllocator());
	}
	array->PushBack(jsonObject, document->GetAllocator());
}

void Technology::LoadState(Document* document)
{
	const Value& techArray = (*document)["technologies"];
	for (SizeType i = 0; i < techArray.Size(); i++)
	{
		int id = techArray[SizeType(i)]["id"].GetInt();
		int owner = techArray[SizeType(i)]["owner"].GetInt();
		TechTree[id]->Purchase(owner);
	}
}

void Technology::SaveState(Document* document)
{
	Value technologies(kArrayType);

	for (int i = 0; i < TECH_COUNT; i++)
	{
		TechTree[i]->GetJSON(document, &technologies, true);
	}
	document->AddMember("technologies", technologies, document->GetAllocator());
}

Technology::~Technology()
{
	delete Name;
	if (DependsOn != nullptr)
	{
		delete DependsOn;
	}
}
