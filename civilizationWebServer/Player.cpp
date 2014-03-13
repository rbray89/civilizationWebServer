#include "Player.h"
#include "string.h"

Player* Player::Players[6];
int Player::PlayerCount = 0;
int Player::CurrentPlayer = 0;
char* Player::TextJSON = nullptr;

void Player::PlayersInit(char** players, int playerCount)
{
	for (int i = 0; i < playerCount; i++)
	{
		Players[i] = new Player(players[i]);
	}
}

Player::Player(char* name)
{
	Name = new char[strlen(name) + 1];
	strcpy(Name, name);
	LoggedIn = false;
	Id = PlayerCount++;
}

Player::~Player()
{
}

char* Player::GetName()
{
	return Name;
}

char* Player::GetPlayerName(int i)
{
	return i >= PlayerCount || i < 0 ? "" : Players[i]->GetName();
}

void Player::GetJSON(Document* document, Value* array)
{
	Value jsonObject(kObjectType);
	jsonObject.AddMember<char*>("name", Name, document->GetAllocator());
	jsonObject.AddMember<bool>("logged_in", LoggedIn, document->GetAllocator());
	jsonObject.AddMember<int>("id", Id, document->GetAllocator());
	array->PushBack(jsonObject, document->GetAllocator());
}

void Player::GetJSONArray(Document* document, Value* array)
{
	for (int i = 0; i < PlayerCount; i++)
	{
		Players[i]->GetJSON(document, array);
	}
}

bool Player::LogIn(int player, struct mg_connection* token)
{
	if (!Players[player]->LoggedIn)
	{
		Players[player]->LoggedIn = true;
		Players[player]->Token = token;
		return true;
	}
	return false;
}

bool Player::LogOut(struct mg_connection* token)
{
	for (int i = 0; i < PlayerCount; i++)
	{
		if (Players[i]->LoggedIn && Players[i]->Token == token)
		{
			Players[i]->LoggedIn = false;
			Players[i]->Token = nullptr;
			return true;
		}
	}
	printf("could not find conn!");
	return false;
}

void Player::IncrementCurrentPlayer()
{
	CurrentPlayer++;
	if (CurrentPlayer >= PlayerCount)
	{
		CurrentPlayer = 0;
	}
}

int Player::GetCurrentPlayer()
{
	return CurrentPlayer;
}

int Player::GetNextPlayer(int skip)
{
	return (CurrentPlayer + 1 + skip) % PlayerCount;
}

char* Player::GetLoginStatusJSON(int player, bool success)
{
	Document document;

	document.Parse<0>("{}");
	StringBuffer ss;
	Writer<StringBuffer> writer(ss);
	Value loginObj(kObjectType);

	loginObj.AddMember<int>("player", player, document.GetAllocator());
	loginObj.AddMember<bool>("verified", success, document.GetAllocator());

	document.AddMember("login", loginObj, document.GetAllocator());

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

void Player::SendToAllPlayers(char* string)
{
	for (int i = 0; i < PlayerCount; i++)
	{
		mg_websocket_write(Players[i]->Token, 1, string, strlen(string)); 
	}
}