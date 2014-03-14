#pragma once
#include <windows.h>
#include <stdio.h>
#include "Player.h"
#include "TechManager.h"

#define UPDATE_BUFF_SIZE 16000
#define PLAYER_TURN_TIME 5*60
#define PLAYER_TURN_EXTEND 2*60
#define GROUP_TURN_TIME 7*60

enum GAME_PHASES
{
	PURCHASE_PHASE = 0,
	MOVEMENT_PHASE = 1,
	TRADE_PHASE = 2,
	PRODUCTION_PHASE = 3
};

class GameManager
{
private:

	struct mg_server* Server;
	TechManager* _TechManager;
	GAME_PHASES CurrentPhase = MOVEMENT_PHASE;
	int StartingPlayer;

	HANDLE hTimer;
	HANDLE hTimerQueue;
	int TimeRemaining;
	bool CountingDown;
	bool Initialized;
	bool Paused;
	char* TextJSON;

public:

	void HandleSecondEvent(bool deincrement = true);
	void SendPlayerStatusUpdate();
	void SendTechStatusUpdate();
	GameManager();
	GameManager(struct mg_server* server);
	void StartStop();
	int Start();
	int GetNextPlayer();
	void EndTurn();
	void ExtendTurn();
	void PurchaseTech(int player, int tech);
	char* GetPlayerStatusJSON();
	char* GetTechStatusJSON();
	~GameManager();
};
