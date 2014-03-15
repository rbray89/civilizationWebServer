#include "GameManager.h"
#include "mongoose.h"
#include "string.h"
#include <windows.h>
#include "rapidjson\document.h"
#include "rapidjson\writer.h"
#include "rapidjson\stringbuffer.h"

using namespace rapidjson;

GameManager::GameManager()
{
}

GameManager::GameManager(struct mg_server* server)
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
	strcpy(buf, string);
	mg_iterate_over_connections(Server, PostMsgToClient, buf);
}

void GameManager::SendTechStatusUpdate()
{
	static char buf[UPDATE_BUFF_SIZE];

	const char* string = GetTechStatusJSON();
	strcpy(buf, string);
	mg_iterate_over_connections(Server, PostMsgToClient, buf);
}

void GameManager::SendWonderStatusUpdate()
{
	static char buf[UPDATE_BUFF_SIZE];

	const char* string = Wonder::GetWonderStatusJSON();
	strcpy(buf, string);
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

char* GameManager::GetPlayerStatusJSON()
{
	Document document;

	document.Parse<0>("{}");
	StringBuffer ss;
	Writer<StringBuffer> writer(ss);
	Value players(kArrayType);

	Player::GetJSONArray(&document, &players);
	document.AddMember("players", players, document.GetAllocator());

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
