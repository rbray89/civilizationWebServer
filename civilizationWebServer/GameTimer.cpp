#include "GameTimer.h"
#include "mongoose.h"
#include "string.h"
#include <windows.h>
#include "rapidjson\document.h"
#include "rapidjson\writer.h"
#include "rapidjson\stringbuffer.h"

using namespace rapidjson;

char* GameTimer::Players[5];
int GameTimer::PlayerCount = 0;

GameTimer::GameTimer()
{
}

GameTimer::GameTimer(struct mg_server* server, char* players[], int playerCount)
{
	this->Server = server;
	TimeRemaining = PLAYER_TURN_TIME;
	CountingDown = true;
	PlayerCount = playerCount;
	for (int i = 0; i < playerCount; i++)
	{
		Players[i] = players[i];
	}
	Start();
}

GameTimer::~GameTimer()
{
	// Delete all timers in the timer queue.
	if (!DeleteTimerQueue(hTimerQueue))
		printf("DeleteTimerQueue failed (%d)\n", GetLastError());
}

static int PostGameTimerToClient(struct mg_connection *conn)
{
	
	if (conn->is_websocket) {
		char* string = (char*)conn->connection_param;
		mg_websocket_write(conn, 1, string, strlen(string));
	}
	return MG_REQUEST_PROCESSED;
}

void GameTimer::EndTurn()
{
	CountingDown = true;
	TimeRemaining = PLAYER_TURN_TIME;
	CurrentPlayer++;
	if (CurrentPlayer >= PlayerCount)
	{
		CurrentPlayer = 0;
	}
	HandleSecondEvent(false);
}

void GameTimer::ExtendTurn()
{
	CountingDown = true;
	TimeRemaining += PLAYER_TURN_EXTEND;
	HandleSecondEvent(false);
}

char* GameTimer::GetPlayer(int player)
{
	if (player == -1)
	{
		return "";
	}
	return Players[player];
}

void GameTimer::HandleSecondEvent(bool deincrement)
{
	static char buf[UPDATE_BUFF_SIZE];

	if (CountingDown && deincrement)
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
	gametimer.AddMember<int>("time_remaining", TimeRemaining, document.GetAllocator());
	gametimer.AddMember<char*>("player_current", Players[CurrentPlayer], document.GetAllocator());
	int nexPlayer = (CurrentPlayer+1)>=PlayerCount ? 0 : CurrentPlayer+1;
	gametimer.AddMember<char*>("player_next", Players[nexPlayer], document.GetAllocator());
	document.AddMember("game_timer", gametimer, document.GetAllocator());
		
	document.Accept(writer);
	const char* str = ss.GetString();
	memset(buf, 0, UPDATE_BUFF_SIZE);
	memcpy(buf, str, strlen(str));

	mg_iterate_over_connections(Server, PostGameTimerToClient, buf);
}

VOID CALLBACK TimerRoutine(PVOID lpParam, BOOLEAN TimerOrWaitFired)
{
	GameTimer* gameTimer = (GameTimer*)lpParam;
	gameTimer->HandleSecondEvent();
}

int GameTimer::Start()
{
	
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

