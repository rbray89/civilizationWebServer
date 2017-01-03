#pragma once
#include <windows.h>
#include <stdio.h>
#include "Player.h"
#include "City.h"
#include "Technology.h"

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
	
	GAME_PHASES CurrentPhase;
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
	void SendWonderStatusUpdate();
	void SendCityStatusUpdate();
	void SendUpgradeStatusUpdate();
	void SendUnitStatusUpdate();
	GameManager();
	GameManager(struct mg_server* server);
	void StartStop();
	int Start();
	int GetNextPlayer();
	void EndTurn();
	void PreviousTurn();
	void ExtendTurn();
	void PurchaseTech(int player, int tech);
	void PurchaseTechOverride(int player, int tech);
	void PurchaseUpgrade(int player, int upgrade);
	void DeprecateUpgrade(int upgrade);
	void AssignCity(int player, int city);
	void AssignCityTrade(int player, int city);
	void ToggleCityHappinessUpgrade(int city);
	void ToggleCityProductivityUpgrade(int city);
	void IncreaseCitySize(int city);
	void CreateCity(int player, RESOURCE resource, bool fertile);
	char* GetPlayerStatusJSON();
	char* GetTechStatusJSON();
	char* GetWonderStatusJSON();
	char* GetCityStatusJSON();
	char* GetUpgradeStatusJSON();
	char* GetUnitStatusJSON();
	void LoadState(char* filename);
	void SaveState(char* filename);
	~GameManager();
};

