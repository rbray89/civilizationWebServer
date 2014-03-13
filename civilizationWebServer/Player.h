#pragma once
#include "rapidjson\document.h"
#include "rapidjson\writer.h"
#include "rapidjson\stringbuffer.h"
#include "mongoose.h"

using namespace rapidjson;

class Player
{
private:
	static Player* Players[6];
	static int PlayerCount;
	static int CurrentPlayer;
	static char* TextJSON;

	char* Name;
	bool LoggedIn;
	int Id;
	struct mg_connection* Token;

	char* GetName();
	void GetJSON(Document* document, Value* array);
	Player(char* name);
public:
	static void PlayersInit(char** players, int playerCount);
	static void GetJSONArray(Document* document, Value* array);
	static char* GetPlayerName(int i);
	static void IncrementCurrentPlayer();
	static int GetCurrentPlayer();
	static int GetNextPlayer();
	static bool LogIn(int player, struct mg_connection* token);
	static bool LogOut(struct mg_connection* token);
	static char* GetLoginStatusJSON(int player, bool success);
	static void SendToAllPlayers(char* string);
	~Player();
};

