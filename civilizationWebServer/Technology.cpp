#include "Technology.h"
#include <stdarg.h>
#include "string.h"
#include "GameManager.h"
#include "Wonder.h"
#include "Unit.h"

using namespace rapidjson;

int Technology::IdCount = 0;
Technology* Technology::TechTree[TECH_COUNT];
char* Technology::TextJSON;

void Technology::InitTech()
{
	TextJSON = nullptr;

	Wonder* HangingGardens = new Wonder("The Hanging Gardens", ANCIENT_ERA, NO_BENEFIT, "Two free settlers");
	Wonder* Colossus = new Wonder("The Colossus", ANCIENT_ERA, NO_BENEFIT, "+5 gold/turn during the Ancient Era");
	Wonder* GreatLighthouse = new Wonder("The Great Lighthouse", ANCIENT_ERA, NO_BENEFIT, "+5 gold/turn during the Ancient Era");
	Wonder* GreatLibrary = new Wonder("The Great Library", ANCIENT_ERA, NO_BENEFIT, "Cost for Ancient Technologies -5");
	Wonder* Oracle = new Wonder("The Oracle", ANCIENT_ERA, NO_BENEFIT, "Player sees which combat unit opponent picks each round before picking his or her own unit");
	Wonder* GreatWall = new Wonder("The Great Wall", ANCIENT_ERA, NO_BENEFIT, "Player's cities give +2 for defending units");
	Wonder* Pyramids = new Wonder("The Pyramids", ANCIENT_ERA, NO_BENEFIT, "Two free ancient cavalry units");
	Wonder* ArtOfWar = new Wonder("Sun Tsu's Art of War", MEDEVIAL_ERA, NO_BENEFIT, "Two free medieval infantry units");
	Wonder* CopernicusObservatory = new Wonder("Copernicus' Observatory", MEDEVIAL_ERA, NO_BENEFIT, "One free medieval navy");
	Wonder* LeonardosWorkshop = new Wonder("Leonardo's Workshop", MEDEVIAL_ERA, NO_BENEFIT, "Cost for Medieval Technologies -10");
	Wonder* BachsCathedral = new Wonder("J.S. Bach's Cathedral", MEDEVIAL_ERA, HAPPY, "One extra Happiness Point");
	Wonder* SistineChapel = new Wonder("The Sistine Chapel", MEDEVIAL_ERA, HAPPY, "One extra Happiness Point");
	Wonder* MagellansVoyage = new Wonder("Magellan's Voyage", GUNPOWDER_ERA, NO_BENEFIT, "Player can force one resource (1-1) trade during each trade phase");
	Wonder* NewtonsUniversity = new Wonder("Newton's University", GUNPOWDER_ERA, NO_BENEFIT, "Cost for Gunpowder/Industrial Technologies -15");
	Wonder* SmithsTradingCompany = new Wonder("Smith's Trading Company", GUNPOWDER_ERA, NO_BENEFIT, "+10 gold/turn");
	Wonder* ShakespearesTheatre = new Wonder("Shakespeare's Theatre", GUNPOWDER_ERA, HAPPY, "One extra Happiness Point");
	Wonder* TranscontinentalRailroad = new Wonder("Transcontinental Railroad", GUNPOWDER_ERA, PRODUCTIVE, "One extra Productivity Point");
	Wonder* EdisonsLab = new Wonder("Edison's Lab", MODERN_ERA, NO_BENEFIT, "Cost for Modern Technologies -20");
	Wonder* HooverDam = new Wonder("Hoover Dam", MODERN_ERA, PRODUCTIVE, "One extra Productivity Point");
	Wonder* CureForCancer = new Wonder("Cure For Cancer", MODERN_ERA, DOUBLE_HAPPY, "Two extra Happiness Points");
	Wonder* UniversalSuffrage = new Wonder("Universal Suffrage", MODERN_ERA, HAPPY, "One extra Happiness Point");
	Wonder* ManhattanProject = new Wonder("Manhattan Project", MODERN_ERA, NO_BENEFIT, "Player may choose any region on the board. All units and the city in that region are eliminated (one time only)");
	Wonder* UnitedNations = new Wonder("United Nations", MODERN_ERA, NO_BENEFIT, "Player may end the game at the end of any turn (Diplomatic)");
	Wonder* ApolloProgram = new Wonder("Apollo Program", MODERN_ERA, NO_BENEFIT, "Player may end the game at the end of any turn (Military)");
	Wonder* AlphaCentauriColonyShip = new Wonder("Alpha Centauri Colony Ship", MODERN_ERA, NO_BENEFIT, "Game ends immediately when this wonder is built (200 Gold) (Technology)");

	Upgrade* Granary = new Upgrade(GRANARY, "Granary", ANCIENT_ERA, PRODUCTIVE);
	Upgrade* Ziggurat = new Upgrade(ZIGGURAT, "Ziggurat", ANCIENT_ERA, HAPPY);
	Upgrade* Courthouse = new Upgrade(COURTHOUSE, "Courthouse", ANCIENT_ERA, HAPPY);
	Upgrade* Library = new Upgrade(LIBRARY, "Library", ANCIENT_ERA, PRODUCTIVE);
	Upgrade* Temple = new Upgrade(TEMPLE, "Temple", ANCIENT_ERA, HAPPY);
	Upgrade* Colosseum = new Upgrade(COLOSSEUM, "Colosseum", ANCIENT_ERA, HAPPY);
	Upgrade* Aqueduct = new Upgrade(AQUEDUCT, "Aqueduct", ANCIENT_ERA, PRODUCTIVE);
	Upgrade* Marketplace = new Upgrade(MARKETPLACE, "Marketplace", ANCIENT_ERA, PRODUCTIVE);
	Upgrade* Bank = new Upgrade(BANK, "Bank", MEDEVIAL_ERA, PRODUCTIVE);
	Upgrade* Castle = new Upgrade(CASTLE, "Castle", MEDEVIAL_ERA, HAPPY);
	Upgrade* University = new Upgrade(UNIVERSITY, "University", MEDEVIAL_ERA, PRODUCTIVE);
	Upgrade* Cathedral = new Upgrade(CATHEDRAL, "Cathedral", MEDEVIAL_ERA, DOUBLE_HAPPY);
	Upgrade* Capitol = new Upgrade(CAPITOL, "Capitol", GUNPOWDER_ERA, HAPPY);
	Upgrade* StockMarket = new Upgrade(STOCK_MARKET, "Stock Market", GUNPOWDER_ERA, PRODUCTIVE);
	Upgrade* Legislature = new Upgrade(LEGISLATURE, "Legislature", GUNPOWDER_ERA, HAPPY);
	Upgrade* Factory = new Upgrade(FACTORY, "Factory", GUNPOWDER_ERA, PRODUCTIVE);
	Upgrade* Railroad = new Upgrade(RAILROAD, "Railroad", GUNPOWDER_ERA, PRODUCTIVE);
	Upgrade* Hospital = new Upgrade(HOSPITAL, "Hospital", GUNPOWDER_ERA, HAPPY);
	Upgrade* HighwaySystem = new Upgrade(HIGHWAY_SYSTEM, "Highway System", MODERN_ERA, PRODUCTIVE);
	Upgrade* ManufacturingPlant = new Upgrade(MANUFACTURING_PLANT, "Manufacturing Plant", MODERN_ERA, PRODUCTIVE);
	Upgrade* ResearchLab = new Upgrade(RESEARCH_LAB, "Research Lab", MODERN_ERA, PRODUCTIVE);
	Upgrade* Airport = new Upgrade(AIRPORT, "Airport", MODERN_ERA, PRODUCTIVE);
	Upgrade* NuclearPlant = new Upgrade(NUCLEAR_POWER_PLANT, "Nuclear Plant", MODERN_ERA, PRODUCTIVE);
	Upgrade* ShoppingMall = new Upgrade(SHOPPING_MALL, "Shopping Mall", MODERN_ERA, HAPPY);
	Upgrade* TelevisionStation = new Upgrade(TELEVISION_STATION, "Television Station", MODERN_ERA, DOUBLE_HAPPY);
	Upgrade* Internet = new Upgrade(INTERNET, "Internet", MODERN_ERA, PRODUCTIVE);
	Upgrade* HealthcareComplex = new Upgrade(HEALTHCARE_COMPLEX, "Healthcare Complex", MODERN_ERA, HAPPY);
	Upgrade* FusionReactor = new Upgrade(FUSION_REACTOR, "Fusion Reactor", MODERN_ERA, DOUBLE_PRODUCTIVE);
	Upgrade* Size2Cities = new Upgrade(SIZE_2_CITIES, "Size 2 Cities", MEDEVIAL_ERA, NO_BENEFIT);
	Upgrade* Size3Cities = new Upgrade(SIZE_3_CITIES, "Size 3 Cities", GUNPOWDER_ERA, NO_BENEFIT);
	Upgrade* Size4Cities = new Upgrade(SIZE_4_CITIES, "Size 4 Cities", MODERN_ERA, NO_BENEFIT);
    
    Unit* Spearman = new Unit(SPEARMAN, "Spearman", ANCIENT_ERA, INFANTRY_TYPE, 1, 0);
    Unit* Swordsman = new Unit(SWORDSMAN, "Swordsman", ANCIENT_ERA, INFANTRY_TYPE, 1, 1);
    Unit* Chariot = new Unit(CHARIOT, "Chariot", ANCIENT_ERA, CAVALRY_TYPE, 1, 0);
    Unit* Horseman = new Unit(HORSEMAN, "Horseman", ANCIENT_ERA, CAVALRY_TYPE, 1, 1);
    Unit* Catapult = new Unit(CATAPULT, "Catapult", ANCIENT_ERA, ARTILLERY_TYPE, 1, 1);
    Unit* Galley = new Unit(GALLEY, "Galley", ANCIENT_ERA, FLEET_TYPE, 1, 0);
    
    Unit* ManAtArms = new Unit(MANATARMS, "ManAtArms", MEDEVIAL_ERA, INFANTRY_TYPE, 2, 0);
    Unit* Knight = new Unit(KNIGHT, "Knight", MEDEVIAL_ERA, CAVALRY_TYPE, 2, 0);
    Unit* Trebuchet = new Unit(TREBUCHET, "Trebuchet", MEDEVIAL_ERA, ARTILLERY_TYPE, 2, 0);
    Unit* Caravel = new Unit(CARAVEL, "Caravel", MEDEVIAL_ERA, FLEET_TYPE, 2, 0);
    Unit* Galleon = new Unit(GALLEON, "Galleon", MEDEVIAL_ERA, FLEET_TYPE, 2, 2);
    
    Unit* Musketman = new Unit(MUSKETMAN, "Musketman", GUNPOWDER_ERA, INFANTRY_TYPE, 3, 0);
    Unit* Rifleman = new Unit(RIFLEMAN, "Rifleman", GUNPOWDER_ERA, INFANTRY_TYPE, 3, 2);
    Unit* Dragoon = new Unit(DRAGOON, "Dragoon", GUNPOWDER_ERA, CAVALRY_TYPE, 3, 0);
    Unit* Cannon = new Unit(CANNON, "Cannon", GUNPOWDER_ERA, ARTILLERY_TYPE, 3, 1);
    Unit* Artillery = new Unit(ARTILLERY, "Artillery", GUNPOWDER_ERA, ARTILLERY_TYPE, 3, 3);
    Unit* Frigate = new Unit(FRIGATE, "Frigate", GUNPOWDER_ERA, FLEET_TYPE, 3, 0);
    Unit* Ironclad = new Unit(IRONCLAD, "Ironclad", GUNPOWDER_ERA, FLEET_TYPE, 3, 2);
    
    Unit* Machinegunner = new Unit(MACHINEGUNNER, "Machine Gunner", MODERN_ERA, INFANTRY_TYPE, 4, 0);
    Unit* MechanizedInfantry = new Unit(MECHANIZED_INFANTRY, "Mechanized Infantry", MODERN_ERA, INFANTRY_TYPE, 4, 2);
    Unit* Tank = new Unit(TANK, "Tank", MODERN_ERA, CAVALRY_TYPE, 4, 0);
    Unit* ModernArmor = new Unit(MODERN_ARMOR, "Modern Armor", MODERN_ERA, CAVALRY_TYPE, 4, 3);
    Unit* RocketArtillery = new Unit(ROCKET_ARTILLERY, "Rocket Artillery", MODERN_ERA, ARTILLERY_TYPE, 4, 2);
    Unit* CruiseMissles = new Unit(CRUISE_MISSLES, "Cruise Missles", MODERN_ERA, ARTILLERY_TYPE, 4, 4);
    Unit* Battleship = new Unit(BATTLESHIP, "Battleship", MODERN_ERA, FLEET_TYPE, 4, 1);
    Unit* Carrier = new Unit(CARRIER, "Carrier", MODERN_ERA, FLEET_TYPE, 4, 0);
    Unit* Biplane = new Unit(BIPLANE, "Biplane", MODERN_ERA, AIRCRAFT_TYPE, 1, 0);
    Unit* Monoplane = new Unit(MONOPLANE, "Monoplane", MODERN_ERA, AIRCRAFT_TYPE, 2, 0);
    Unit* Jet = new Unit(JET, "Jet", MODERN_ERA, AIRCRAFT_TYPE, 3, 0);
    Unit* StealthAircraft = new Unit(STEALTH_AIRCRAFT, "Stealth Aircraft", MODERN_ERA, AIRCRAFT_TYPE, 4, 0);

	Technology* Masonry = new Technology("Masonry", ANCIENT_ERA, 15, WONDER_BENEFIT);
	Masonry->SetWonders(1, HangingGardens);

	Technology* Alphabet_Writing = new Technology("Alphabet/Writing", ANCIENT_ERA, 15, SEMINAL_BENEFIT);

	Technology* Pottery_Specialization = new Technology("Pottery/Specialization", ANCIENT_ERA, 20, PRODUCTIVE_BENEFIT);
	Pottery_Specialization->SetUpgrades(1, Granary);
	Pottery_Specialization->BenefitText = "Free Granary improvement";
	Pottery_Specialization->EnablesText = "Granary";

	Technology* TheWheel = new Technology("The Wheel", ANCIENT_ERA, 25, CAVALRY_BENEFIT);
	TheWheel->SetUnits(1, Chariot);
	TheWheel->BenefitText = "One free Chariot unit";
	TheWheel->EnablesText = "Chariot";

	Technology* BronzeWorking = new Technology("Bronze Working", ANCIENT_ERA, 25, INFANTRY_BENEFIT | WONDER_BENEFIT);
	BronzeWorking->SetWonders(1, Colossus);
	BronzeWorking->SetUnits(1, Spearman);
	BronzeWorking->EnablesText = "Spearman";

	Technology* CeremonialBurial = new Technology("Ceremonial Burial", ANCIENT_ERA, 25, HAPPY_BENEFIT | WONDER_BENEFIT);
	CeremonialBurial->SetUpgrades(1, Ziggurat);
	CeremonialBurial->SetWonders(1, Pyramids);
	CeremonialBurial->EnablesText = "Ziggurat";

	Technology* HorsebackRiding = new Technology("Horseback Riding", ANCIENT_ERA, 40, CAVALRY_BENEFIT);
	HorsebackRiding->SetUnits(1, Horseman);
	HorsebackRiding->EnablesText = "Horseman";
	HorsebackRiding->BenefitText = "One free Horseman unit";
	HorsebackRiding->SetDependencies(1, TheWheel);

	Technology* CodeOfLaws = new Technology("Code of Laws", ANCIENT_ERA, 35, HAPPY_BENEFIT);
	CodeOfLaws->SetUpgrades(1, Courthouse);
	CodeOfLaws->BenefitText = "Free Courthouse improvement";
	CodeOfLaws->EnablesText = "Courthouse";
	CodeOfLaws->SetDependencies(2, Alphabet_Writing, Pottery_Specialization);

	Technology* Mathematics = new Technology("Mathematics", ANCIENT_ERA, 40, ARTILLERY_BENEFIT);
	Mathematics->SetUnits(1, Catapult);
	Mathematics->BenefitText = "One free Catapult unit";
	Mathematics->EnablesText = "Catapult";
	Mathematics->SetDependencies(2, Masonry, Alphabet_Writing);

	Technology* IronWorking = new Technology("Iron Working", ANCIENT_ERA, 40, INFANTRY_BENEFIT);
	IronWorking->SetUnits(1, Swordsman);
	IronWorking->BenefitText = "One free Swordsman unit";
	IronWorking->EnablesText = "Swordsman";
	IronWorking->SetDependencies(2, BronzeWorking, Pottery_Specialization);

	Technology* Philosophy_Literature = new Technology("Philosophy/Literature", ANCIENT_ERA, 40, PRODUCTIVE_BENEFIT | WONDER_BENEFIT);
	Philosophy_Literature->SetUpgrades(1, Library);
	Philosophy_Literature->SetWonders(1, GreatLibrary);
	Philosophy_Literature->EnablesText = "Library";
	Philosophy_Literature->SetDependencies(2, Alphabet_Writing, Pottery_Specialization);

	Technology* Mysticism = new Technology("Mysticism", ANCIENT_ERA, 40, HAPPY_BENEFIT | WONDER_BENEFIT);
	Mysticism->SetUpgrades(1, Temple);
	Mysticism->SetWonders(1, Oracle);
	Mysticism->EnablesText = "Temple";
	Mysticism->SetDependencies(2, Pottery_Specialization, CeremonialBurial);

	Technology* Trade_Mapmaking = new Technology("Trade/Mapmaking", ANCIENT_ERA, 50, TRADE_BENEFIT | FLEET_BENEFIT | WONDER_BENEFIT);
	Trade_Mapmaking->SetWonders(1, GreatLighthouse);
	Trade_Mapmaking->SetUnits(1, Galley);
	Trade_Mapmaking->EnablesText = "Galley, Trade1-sea";
	Trade_Mapmaking->SetDependencies(2, Mathematics, Pottery_Specialization);

	Technology* Currency = new Technology("Currency", ANCIENT_ERA, 50, PRODUCTIVE_BENEFIT);
	Currency->SetUpgrades(1, Marketplace);
	Currency->BenefitText = "Free Marketplace improvement";
	Currency->EnablesText = "Marketplace";
	Currency->SetDependencies(2, Mathematics, Trade_Mapmaking);

	Technology* Construction = new Technology("Construction", ANCIENT_ERA, 50, CITY_BENEFIT | PRODUCTIVE_BENEFIT | HAPPY_BENEFIT | WONDER_BENEFIT);
	Construction->SetUpgrades(3,  Aqueduct, Colosseum, Size2Cities);
	Construction->SetWonders(1, GreatWall);
	Construction->EnablesText = "Aqueduct, Colosseum, Size2 Cities";
	Construction->SetDependencies(2, IronWorking, Mathematics);

	Technology* Banking = new Technology("Banking", MEDEVIAL_ERA, 50, PRODUCTIVE_BENEFIT);
	Banking->SetUpgrades(1, Bank);
	Banking->EnablesText = "Bank";
	Banking->BenefitText = "Free Bank Improvement";
	Banking->SetDependencies(1, Currency);

	Technology* Education = new Technology("Education", MEDEVIAL_ERA, 60, PRODUCTIVE_BENEFIT | WONDER_BENEFIT);
	Education->SetUpgrades(1, University);
	Education->SetWonders(1, LeonardosWorkshop);
	Education->EnablesText = "University";
	Education->SetDependencies(2, Trade_Mapmaking, Philosophy_Literature);

	Technology* Engineering = new Technology("Engineering", MEDEVIAL_ERA, 70, ARTILLERY_BENEFIT);
	Engineering->SetUnits(1, Trebuchet);
	Engineering->EnablesText = "Trebuchet";
	Engineering->BenefitText = "One free Trebuchet unit";
	Engineering->SetDependencies(2, TheWheel, Construction);

	Technology* Theology = new Technology("Theology", MEDEVIAL_ERA, 70, HAPPY_BENEFIT | WONDER_BENEFIT);
	Theology->SetUpgrades(1, Cathedral);
	Theology->SetWonders(2, BachsCathedral, SistineChapel);
	Theology->EnablesText = "Cathedral";
	Theology->SetDependencies(2, Philosophy_Literature, Mysticism);

	Technology* Feudalism = new Technology("Feudalism", MEDEVIAL_ERA, 70, HAPPY_BENEFIT | INFANTRY_BENEFIT | WONDER_BENEFIT);
	Feudalism->SetUpgrades(1, Castle);
	Feudalism->SetWonders(1, ArtOfWar);
	Feudalism->SetUnits(1, ManAtArms);
	Feudalism->EnablesText = "Man-At-Arms, Castle";
	Feudalism->SetDependencies(2, CodeOfLaws, Construction);

	Technology* Astronomy = new Technology("Astronomy", MEDEVIAL_ERA, 60, TRADE_BENEFIT | FLEET_BENEFIT | WONDER_BENEFIT);
	Astronomy->SetWonders(1, CopernicusObservatory);
	Astronomy->SetUnits(1, Caravel);
	Astronomy->EnablesText = "Caravel, Trade2-sea";
	Astronomy->SetDependencies(2, Education, Engineering);

	Technology* PrintingPress = new Technology("Printing Press", MEDEVIAL_ERA, 40, SEMINAL_BENEFIT);
	PrintingPress->SetDependencies(2, Theology, Education);

	Technology* Chivalry = new Technology("Chivalry", MEDEVIAL_ERA, 60, CAVALRY_BENEFIT);
	Chivalry->SetUnits(1, Knight);
	Chivalry->EnablesText = "Knight";
	Chivalry->BenefitText = "One free Knight unit";
	Chivalry->SetDependencies(2, HorsebackRiding, Feudalism);

	Technology* Navigation = new Technology("Navigation", MEDEVIAL_ERA, 60, TRADE_BENEFIT | FLEET_BENEFIT);
	Navigation->SetUnits(1, Galleon);
	Navigation->EnablesText = "Galleons, Trade3";
	Navigation->BenefitText = "One free Galleon unit";
	Navigation->SetDependencies(1, Astronomy);

	Technology* Chemistry = new Technology("Chemistry", MEDEVIAL_ERA, 40, CITY_BENEFIT);
	Chemistry->SetUpgrades(1, Size3Cities);
	Chemistry->EnablesText = "Size3 Cities";
	Chemistry->BenefitText = "One free city upgrade";
	Chemistry->SetDependencies(2, PrintingPress, Education);

	Technology* Nationalism = new Technology("Nationalism", GUNPOWDER_ERA, 80, HAPPY_BENEFIT | WONDER_BENEFIT);
	Nationalism->SetUpgrades(1, Capitol);
	Nationalism->SetWonders(1, ShakespearesTheatre);
	Nationalism->EnablesText = "Capitol";
	Nationalism->SetDependencies(2, Banking, PrintingPress);

	Technology* Physics = new Technology("Physics", GUNPOWDER_ERA, 80, WONDER_BENEFIT);
	Physics->SetWonders(1, NewtonsUniversity);
	Physics->SetDependencies(2, Astronomy, Chemistry);

	Technology* Economics = new Technology("Economics", GUNPOWDER_ERA, 90, PRODUCTIVE_BENEFIT | WONDER_BENEFIT);
	Economics->SetUpgrades(1, StockMarket);
	Economics->SetWonders(1, SmithsTradingCompany);
	Economics->EnablesText = "Stock Market";
	Economics->SetDependencies(1, Nationalism);

	Technology* GunpowderWeapons = new Technology("Gunpowder Weapons", GUNPOWDER_ERA, 90, INFANTRY_BENEFIT);
	GunpowderWeapons->SetUnits(1, Musketman);
	GunpowderWeapons->EnablesText = "Musketman";
	GunpowderWeapons->BenefitText = "One free Musketman unit";
	GunpowderWeapons->SetDependencies(1, Physics);

	Technology* Cavalry = new Technology("Cavalry", GUNPOWDER_ERA, 80, CAVALRY_BENEFIT);
	Cavalry->SetUnits(1, Dragoon);
	Cavalry->EnablesText = "Dragoon";
	Cavalry->BenefitText = "One free Dragoon unit";
	Cavalry->SetDependencies(2, Chivalry, GunpowderWeapons);

	Technology* LegislativeGovernment = new Technology("Legislative Government", GUNPOWDER_ERA, 70, HAPPY_BENEFIT);
	LegislativeGovernment->SetUpgrades(1, Legislature);
	LegislativeGovernment->EnablesText = "Legislature";
	LegislativeGovernment->BenefitText = "Free Legislature improvement";
	LegislativeGovernment->SetDependencies(1, Economics);

	Technology* Metallurgy = new Technology("Metallurgy", GUNPOWDER_ERA, 100, ARTILLERY_BENEFIT | FLEET_BENEFIT | WONDER_BENEFIT);
	Metallurgy->SetWonders(1, MagellansVoyage);
	Metallurgy->SetUnits(2, Cannon, Frigate);
	Metallurgy->EnablesText = "Cannon, Frigate";
	Metallurgy->SetDependencies(2, IronWorking, GunpowderWeapons);

	Technology* Medicine = new Technology("Medicine", GUNPOWDER_ERA, 70, HAPPY_BENEFIT | CITY_BENEFIT);
	Medicine->SetUpgrades(2, Hospital, Size4Cities);
	Medicine->EnablesText = "Hospital, Size4 Cities";
	Medicine->BenefitText = "Free Hospital improvement";
	Medicine->SetDependencies(1, Metallurgy);

	Technology* SteamPower = new Technology("Steam Power", GUNPOWDER_ERA, 120, PRODUCTIVE_BENEFIT | FLEET_BENEFIT | INFANTRY_BENEFIT | TRADE_BENEFIT | WONDER_BENEFIT | SEMINAL_BENEFIT);
	SteamPower->SetUpgrades(1, Railroad);
	SteamPower->SetWonders(1, TranscontinentalRailroad);
	SteamPower->SetUnits(2, Ironclad, Rifleman);
	SteamPower->EnablesText = "Ironclad, Rifleman, Railroad, Trade4-land";
	SteamPower->SetDependencies(1, Metallurgy);

	Technology* Industrialization = new Technology("Industrialization", GUNPOWDER_ERA, 100, ARTILLERY_BENEFIT | PRODUCTIVE_BENEFIT);
	Industrialization->SetUpgrades(1, Factory);
	Industrialization->SetUnits(1, Artillery);
	Industrialization->EnablesText = "Artillery, Factory";
	Industrialization->BenefitText = "Free Factory improvement";
	Industrialization->SetDependencies(2, Economics, SteamPower);


	Technology* ScientificMethod = new Technology("Scientific Method", MODERN_ERA, 100, PRODUCTIVE_BENEFIT | WONDER_BENEFIT);
	ScientificMethod->SetUpgrades(1, ResearchLab);
	ScientificMethod->SetWonders(1, EdisonsLab);
	ScientificMethod->EnablesText = "Research Lab";
	ScientificMethod->SetDependencies(1, Medicine);

	Technology* Electricity = new Technology("Electricity", MODERN_ERA, 100, INFANTRY_BENEFIT);
	Electricity->SetUnits(1, Machinegunner);
	Electricity->EnablesText = "Machine Gunner";
	Electricity->BenefitText = "One free Machinegunner unit";
	Electricity->SetDependencies(1, SteamPower);


	Technology* Combustion = new Technology("Combustion", MODERN_ERA, 100, PRODUCTIVE_BENEFIT | FLEET_BENEFIT);
	Combustion->SetUpgrades(1, HighwaySystem);
	Combustion->SetUnits(1, Battleship);
	Combustion->EnablesText = "Highway System, Battleship";
	Combustion->BenefitText = "One free Battleship unit";
	Combustion->SetDependencies(1, Industrialization);

	Technology* Flight = new Technology("Flight", MODERN_ERA, 100, AIRCRAFT_BENEFIT | TRADE_BENEFIT);
	Flight->SetUnits(1, Biplane);
	Flight->EnablesText = "Biplane, Trade-All";
	Flight->BenefitText = "One free Biplane unit";
	Flight->SetDependencies(1, Combustion);

	Technology* Fission = new Technology("Fission", MODERN_ERA, 150, PRODUCTIVE_BENEFIT | WONDER_BENEFIT);
	Fission->SetUpgrades(1, NuclearPlant);
	Fission->SetWonders(1, ManhattanProject);
	Fission->EnablesText = "Nuclear Power Plant";
	Fission->SetDependencies(1, ScientificMethod);

	Technology* Electronics = new Technology("Electronics", MODERN_ERA, 70, WONDER_BENEFIT);
	Electronics->SetWonders(1, HooverDam);
	Electronics->SetDependencies(2, Electricity, ScientificMethod);

	Technology* MassProduction = new Technology("Mass Production", MODERN_ERA, 100, PRODUCTIVE_BENEFIT | CAVALRY_BENEFIT | WONDER_BENEFIT);
	MassProduction->SetUpgrades(1, ManufacturingPlant);
	MassProduction->SetWonders(1, UniversalSuffrage);
	MassProduction->SetUnits(1, Tank);
	MassProduction->EnablesText = "Tank, Manufacturing Plant";
	MassProduction->SetDependencies(2, Combustion, Electricity);

	Technology* Plastics = new Technology("Plastics", MODERN_ERA, 80, HAPPY_BENEFIT);
	Plastics->SetUpgrades(1, ShoppingMall);
	Plastics->EnablesText = "Shopping Mall";
	Plastics->BenefitText = "Free Shopping Mall improvement";
	Plastics->SetDependencies(1, MassProduction);

	Technology* Robotics = new Technology("Robotics", MODERN_ERA, 100, INFANTRY_BENEFIT);
	Robotics->SetUnits(1, MechanizedInfantry);
	Robotics->EnablesText = "Mechanized Infantry";
	Robotics->BenefitText = "One free Mechanized Infantry unit";
	Robotics->SetDependencies(2, Electronics, MassProduction);

	Technology* Radio_Television = new Technology("Radio/Television", MODERN_ERA, 120, HAPPY_BENEFIT | WONDER_BENEFIT);
	Radio_Television->SetUpgrades(1, TelevisionStation);
	Radio_Television->SetWonders(1, UnitedNations);
	Radio_Television->EnablesText = "Television Station";
	Radio_Television->SetDependencies(1, Electronics);

	Technology* Computers = new Technology("Computers", MODERN_ERA, 100, SEMINAL_BENEFIT | PRODUCTIVE_BENEFIT);
	Computers->SetUpgrades(1, Internet);
	Computers->EnablesText = "The Internet";
	Computers->SetDependencies(1, Electronics);

	Technology* AdvancedFlight = new Technology("Advanced Flight", MODERN_ERA, 100, AIRCRAFT_BENEFIT | PRODUCTIVE_BENEFIT | FLEET_BENEFIT);
	AdvancedFlight->SetUpgrades(1, Airport);
	AdvancedFlight->SetUnits(2, Carrier, Monoplane);
	AdvancedFlight->EnablesText = "Aircraft Carrier, Monoplane, Airport";
	AdvancedFlight->BenefitText = "One free Monoplane unit";
	AdvancedFlight->SetDependencies(2, Flight, Radio_Television);

	Technology* Genetics = new Technology("Genetics", MODERN_ERA, 100, HAPPY_BENEFIT | WONDER_BENEFIT);
	Genetics->SetUpgrades(1, HealthcareComplex);
	Genetics->SetWonders(1, CureForCancer);
	Genetics->EnablesText = "Healthcare Complex";
	Genetics->SetDependencies(2, Medicine, Computers);

	Technology* Rocketry = new Technology("Rocketry", MODERN_ERA, 120, ARTILLERY_BENEFIT | AIRCRAFT_BENEFIT);
	Rocketry->SetUnits(2, RocketArtillery, Jet);
	Rocketry->EnablesText = "Rocket Artillery, Jets";
	Rocketry->BenefitText = "One free Rocket Artillery unit";
	Rocketry->SetDependencies(1, AdvancedFlight);

	Technology* SpaceFlight = new Technology("Space Flight", MODERN_ERA, 150, ARTILLERY_BENEFIT | WONDER_BENEFIT);
	SpaceFlight->SetWonders(1, ApolloProgram);
	SpaceFlight->SetUnits(1, CruiseMissles);
	SpaceFlight->EnablesText = "Cruise Missles";
	SpaceFlight->SetDependencies(4, Plastics, Computers, Robotics, Rocketry);

	Technology* Miniaturization = new Technology("Miniaturization", MODERN_ERA, 100, CAVALRY_BENEFIT);
	Miniaturization->SetUnits(1, ModernArmor);
	Miniaturization->EnablesText = "Modern Armor";
	Miniaturization->BenefitText = "One free Modern Armour unit";
	Miniaturization->SetDependencies(1, Computers);

	Technology* SuperConductor = new Technology("Super Conductor", MODERN_ERA, 120, AIRCRAFT_BENEFIT);
	SuperConductor->SetUnits(1, StealthAircraft);
	SuperConductor->EnablesText = "Stealth Aircraft";
	SuperConductor->BenefitText = "One free Stealth Aircraft";
	SuperConductor->SetDependencies(2, Miniaturization, SpaceFlight);

	Technology* Fusion = new Technology("Fusion", MODERN_ERA, 150, PRODUCTIVE_BENEFIT | WONDER_BENEFIT);
	Fusion->SetUpgrades(1, FusionReactor);
	Fusion->SetWonders(1, AlphaCentauriColonyShip);
	Fusion->EnablesText = "Fusion Reactor";
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
	BenefitText = nullptr;
	EnablesText = nullptr;
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

void Technology::SetUpgrades(int n, ...)
{
	UpgradeCount = n;
	Upgrades = new Upgrade*[n];
	va_list args;
	va_start(args, n);
	for (int i = 0; i < n; i++)
	{
		Upgrades[i] = va_arg(args, Upgrade*);
	}
	va_end(args);
}

void Technology::SetUnits(int n, ...)
{
	UnitCount = n;
	Units = new Unit*[n];
	va_list args;
	va_start(args, n);
	for (int i = 0; i < n; i++)
	{
		Units[i] = va_arg(args, Unit*);
	}
	va_end(args);
}

void Technology::SetWonders(int n, ...)
{
	WonderCount = n;
	Wonders = new Wonder*[n];
	va_list args;
	va_start(args, n);
	for (int i = 0; i < n; i++)
	{
		Wonders[i] = va_arg(args, Wonder*);
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
	
	int previousOwner = Owner;
	Owner = player;

	for (int i = 0; i < WonderCount; i++)
	{
		Wonders[i]->SetOwner(player, previousOwner);
	}

	for (int i = 0; i < UpgradeCount; i++)
	{
		if (player == -1)
		{
			Upgrades[i]->LockUpgrade();
		}
		else
		{
			Upgrades[i]->UnlockUpgrade(player);
		}
	}

	for (int i = 0; i < UnitCount; i++)
	{
		if (player == -1)
		{
			Units[i]->LockUnit();
		}
		else
		{
			Units[i]->UnlockUnit(player);
		}
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

int Technology::GetVictoryPoints(int player)
{
	int points = 0;
	for (int i = 0; i < TECH_COUNT; i++)
	{
		if (TechTree[i]->Owner == player && TechTree[i]->Benefits & SEMINAL_BENEFIT)
		{
			points += 4;
		}
	}
	return points;
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
