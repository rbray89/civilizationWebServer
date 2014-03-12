#pragma once
#include <windows.h>
#include <stdio.h>

#define UPDATE_BUFF_SIZE 500
#define PLAYER_TURN_TIME 5*60
#define PLAYER_TURN_EXTEND 2*60
#define TIMER_JSON_TEMPLATE "{ \"game_timer\": { \"time_remaining\":\"0\"}}"

class GameTimer
{
private:

	static char* Players[5];
	static int PlayerCount;

	struct mg_server* Server;
	int CurrentPlayer = 0;
	HANDLE hTimer;
	HANDLE hTimerQueue;
	int TimeRemaining;
	bool CountingDown;
	
public:
	static char* GetPlayer(int player);

	void HandleSecondEvent(bool deincrement = true);
	GameTimer();
	GameTimer(struct mg_server* server, char* players[], int playerCount);
	int Start();
	void EndTurn();
	void ExtendTurn();
	~GameTimer();
};

