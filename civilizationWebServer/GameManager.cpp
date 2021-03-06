#include "GameManager.h"
#include "mongoose.h"
#include "string.h"
#include <fstream>
#include "rapidjson\document.h"
#include "rapidjson\writer.h"
#include "rapidjson\stringbuffer.h"

using namespace rapidjson;

GameManager::GameManager() : CurrentPhase(MOVEMENT_PHASE)
{
}

GameManager::GameManager(struct mg_server* server) : CurrentPhase(MOVEMENT_PHASE)
{
	this->Server = server;
	Technology::InitTech();
	
	TimeRemaining = PLAYER_TURN_TIME;
	CountingDown = true;
	TextJSON = nullptr;
	Paused = true;
	Initialized = false;
	StartingPlayer = 0;
}

GameManager::~GameManager()
{
	// Delete all timers in the timer queue.
	if (!DeleteTimerQueue(hTimerQueue))
		printf("DeleteTimerQueue failed (%d)\n", GetLastError());
}

static int PostMsgToClient(struct mg_connection *conn)
{
	
	if (conn->is_websocket) {
		char* string = (char*)conn->connection_param;
		mg_websocket_write(conn, 1, string, strlen(string));
	}
	return MG_REQUEST_PROCESSED;
}

int GameManager::GetNextPlayer()
{
	if (StartingPlayer == Player::GetNextPlayer() &&
		CurrentPhase == PRODUCTION_PHASE)
	{
		return Player::GetNextPlayer(1);
	}
	else
	{
		return Player::GetNextPlayer();
	}
}

void GameManager::EndTurn()
{
	CountingDown = true;
	
	switch (CurrentPhase)
	{
	case PURCHASE_PHASE:
	case MOVEMENT_PHASE:
		Player::IncrementCurrentPlayer();
		TimeRemaining = PLAYER_TURN_TIME;
		break;
	}
	
	if (StartingPlayer == Player::GetCurrentPlayer())
	{
		switch (CurrentPhase)
		{
		case PURCHASE_PHASE:
			CurrentPhase = MOVEMENT_PHASE;
			TimeRemaining = PLAYER_TURN_TIME;
			break;
		case MOVEMENT_PHASE:
			CurrentPhase = TRADE_PHASE;
			TimeRemaining = GROUP_TURN_TIME;
			break;
		case TRADE_PHASE:
			CurrentPhase = PRODUCTION_PHASE;
			TimeRemaining = GROUP_TURN_TIME;
			break;
		case PRODUCTION_PHASE:
			CurrentPhase = PURCHASE_PHASE;
			TimeRemaining = PLAYER_TURN_TIME;
			Player::IncrementCurrentPlayer();
			StartingPlayer = Player::GetCurrentPlayer();
		}
	}
	
	HandleSecondEvent(false);
}

void GameManager::PreviousTurn()
{
	CountingDown = true;

	if (StartingPlayer == Player::GetCurrentPlayer())
	{
		switch (CurrentPhase)
		{
		case PURCHASE_PHASE:
			CurrentPhase = PRODUCTION_PHASE;
			TimeRemaining = GROUP_TURN_TIME;
			Player::DeIncrementCurrentPlayer();
			StartingPlayer = Player::GetCurrentPlayer();
			break;
		case MOVEMENT_PHASE:
			CurrentPhase = PURCHASE_PHASE;
			TimeRemaining = PLAYER_TURN_TIME;
			break;
		case TRADE_PHASE:
			CurrentPhase = MOVEMENT_PHASE;
			TimeRemaining = PLAYER_TURN_TIME;
			break;
		case PRODUCTION_PHASE:
			CurrentPhase = TRADE_PHASE;
			TimeRemaining = GROUP_TURN_TIME;
		}
	}

	switch (CurrentPhase)
	{
	case PURCHASE_PHASE:
	case MOVEMENT_PHASE:
		Player::DeIncrementCurrentPlayer();
		TimeRemaining = PLAYER_TURN_TIME;
		break;
	}

	HandleSecondEvent(false);
}

void GameManager::ExtendTurn()
{
	CountingDown = true;
	TimeRemaining += PLAYER_TURN_EXTEND;
	HandleSecondEvent(false);
}

void GameManager::HandleSecondEvent(bool deincrement)
{
	static char buf[UPDATE_BUFF_SIZE];

	if (CountingDown && deincrement && !Paused)
	{
		TimeRemaining--;
		if (TimeRemaining <= 0)
		{
			CountingDown = false;
		}
	}

	Document document;

	document.Parse<0>("{}");
	StringBuffer ss;
	Writer<StringBuffer> writer(ss);

	Value gametimer(kObjectType);
	gametimer.AddMember<int>("current_phase", CurrentPhase, document.GetAllocator());
	gametimer.AddMember<int>("time_remaining", TimeRemaining, document.GetAllocator());
	if (CurrentPhase == TRADE_PHASE || CurrentPhase == PRODUCTION_PHASE)
	{
		gametimer.AddMember<int>("player_current", -1, document.GetAllocator());
		gametimer.AddMember<int>("player_next", -1, document.GetAllocator());
	}
	else
	{
		gametimer.AddMember<int>("player_current", Player::GetCurrentPlayer(), document.GetAllocator());
		gametimer.AddMember<int>("player_next", GetNextPlayer(), document.GetAllocator());
	}
	document.AddMember("game_timer", gametimer, document.GetAllocator());
		
	document.Accept(writer);
	const char* str = ss.GetString();
	memset(buf, 0, UPDATE_BUFF_SIZE);
	memcpy(buf, str, strlen(str));

	mg_iterate_over_connections(Server, PostMsgToClient, buf);
}

void GameManager::SendPlayerStatusUpdate()
{
	static char buf[UPDATE_BUFF_SIZE];

	const char* string = GetPlayerStatusJSON();
	strcpy_s(buf, sizeof(buf), string);
	mg_iterate_over_connections(Server, PostMsgToClient, buf);
}

void GameManager::SendTechStatusUpdate()
{
	static char buf[UPDATE_BUFF_SIZE];

	const char* string = GetTechStatusJSON();
	strcpy_s(buf, sizeof(buf), string);
	mg_iterate_over_connections(Server, PostMsgToClient, buf);
}

void GameManager::SendWonderStatusUpdate()
{
	static char buf[UPDATE_BUFF_SIZE];

	const char* string = Wonder::GetWonderStatusJSON();
	strcpy_s(buf, sizeof(buf), string);
	mg_iterate_over_connections(Server, PostMsgToClient, buf);
}

void GameManager::SendCityStatusUpdate()
{
	static char buf[UPDATE_BUFF_SIZE];

	const char* string = City::GetCityStatusJSON();
	strcpy_s(buf, sizeof(buf), string);
	mg_iterate_over_connections(Server, PostMsgToClient, buf);
}

void GameManager::SendUpgradeStatusUpdate()
{
	static char buf[UPDATE_BUFF_SIZE];

	const char* string = Upgrade::GetUpgradeStatusJSON();
	strcpy_s(buf, sizeof(buf), string);
	mg_iterate_over_connections(Server, PostMsgToClient, buf);
}

void GameManager::SendUnitStatusUpdate()
{
	static char buf[UPDATE_BUFF_SIZE];

	const char* string = Unit::GetUnitStatusJSON();
	strcpy_s(buf, sizeof(buf), string);
	mg_iterate_over_connections(Server, PostMsgToClient, buf);
}

VOID CALLBACK TimerRoutine(PVOID lpParam, BOOLEAN TimerOrWaitFired)
{
	GameManager* gameTimer = (GameManager*)lpParam;
	gameTimer->HandleSecondEvent();
}

void GameManager::StartStop()
{
	if (!Initialized)
	{
		Start();
	}
	Paused = !Paused;
}

int GameManager::Start()
{
	Initialized = true;
	// Create the timer queue.
	hTimerQueue = CreateTimerQueue();
	if (NULL == hTimerQueue)
	{
		printf("CreateTimerQueue failed (%d)\n", GetLastError());
		return 2;
	}

	// Set a timer to call the timer routine in 10 seconds.
	if (!CreateTimerQueueTimer(&hTimer, hTimerQueue,
		(WAITORTIMERCALLBACK)TimerRoutine, this, 0, 1000, 0))
	{
		printf("CreateTimerQueueTimer failed (%d)\n", GetLastError());
		return 3;
	}

	return 0;
}

void GameManager::PurchaseTech(int player, int tech)
{
	if (player == Player::GetCurrentPlayer() && CurrentPhase == PURCHASE_PHASE)
	{
		Technology::Purchase(tech, player);
	}
}

void GameManager::PurchaseUpgrade(int player, int upgrade)
{
	if (player == Player::GetCurrentPlayer() && CurrentPhase == PURCHASE_PHASE)
	{
		Upgrade::PurchaseUpgrade(player, (UPGRADE) upgrade);
	}
}

void GameManager::DeprecateUpgrade(int upgrade)
{
	Upgrade::DeprecateUpgrade((UPGRADE) upgrade);
}

void GameManager::PurchaseTechOverride(int player, int tech)
{
	Technology::Purchase(tech, player);
}

void GameManager::AssignCity(int player, int city)
{
	City::Assign(player, city);
}

void GameManager::AssignCityTrade(int player, int city)
{
	City::Trade(player, city);
}

void GameManager::ToggleCityHappinessUpgrade(int city)
{
	City::ToggleCityHappinessUpgrade(city);
}

void GameManager::ToggleCityProductivityUpgrade(int city)
{
	City::ToggleCityProductivityUpgrade(city);
}

void GameManager::IncreaseCitySize(int city)
{
	City::IncreaseCitySize(city);
}

void GameManager::CreateCity(int player, RESOURCE resource, bool fertile)
{
	new City(resource, player, fertile);
}

char* GameManager::GetPlayerStatusJSON()
{
	Document document;

	document.Parse<0>("{}");
	StringBuffer ss;
	Writer<StringBuffer> writer(ss);
	Value players(kArrayType);

	Player::GetJSONArray(&document, &players);

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

char* GameManager::GetTechStatusJSON()
{
	return Technology::GetTechStatusJSON();
}

char* GameManager::GetWonderStatusJSON()
{
	return Wonder::GetWonderStatusJSON();
}

char* GameManager::GetCityStatusJSON()
{
	return City::GetCityStatusJSON();
}

char* GameManager::GetUpgradeStatusJSON()
{
	return Upgrade::GetUpgradeStatusJSON();
}

char* GameManager::GetUnitStatusJSON()
{
	return Unit::GetUnitStatusJSON();
}

void GameManager::LoadState(char* filename)
{
	std::ifstream in(filename, std::ios::in | std::ios::binary);
	std::string contents;
	if (in)
	{
		in.seekg(0, std::ios::end);
		contents.resize((unsigned int) in.tellg());
		in.seekg(0, std::ios::beg);
		in.read(&contents[0], contents.size());
		in.close();
	}
	else
	{
		return;
	}
	Document document;
	if (document.Parse<0>(contents.c_str()).HasParseError() || !document.IsObject())
	{
		return;
	}
	Player::SetCurrentPlayer(document["gameState"]["currentPlayer"].GetInt());
	StartingPlayer = document["gameState"]["turnStartingPlayer"].GetInt();
	CurrentPhase = (GAME_PHASES)document["gameState"]["currentPhase"].GetInt();
	Player::LoadState(&document);
	Technology::LoadState(&document);
	Upgrade::LoadState(&document);
	City::LoadState(&document);
	HandleSecondEvent(false);
}

void GameManager::SaveState(char* filename)
{
	Document document;

	document.Parse<0>("{}");
	StringBuffer ss;
	Writer<StringBuffer> writer(ss);

	Value gameState(kObjectType);
	gameState.AddMember<int>("currentPlayer", Player::GetCurrentPlayer(), document.GetAllocator());
	gameState.AddMember<int>("turnStartingPlayer", StartingPlayer, document.GetAllocator());
	gameState.AddMember<GAME_PHASES>("currentPhase", CurrentPhase, document.GetAllocator());
	document.AddMember("gameState", gameState, document.GetAllocator());
	Player::SaveState(&document);
	Technology::SaveState(&document);
	Upgrade::SaveState(&document);
	City::SaveState(&document);	
	document.Accept(writer);
	const char* str = ss.GetString();

	std::ofstream out(filename, std::ios::out | std::ios::binary);
	std::string contents;
	if (out)
	{
		out.write(str, strlen(str)+1);
		out.close();
	}
	
	
}