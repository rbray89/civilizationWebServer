#include "Player.h"
#include "City.h"
#include "string.h"

Player* Player::Players[6];
int Player::PlayerCount = 0;
int Player::CurrentPlayer = 0;
char* Player::TextJSON = nullptr;

void Player::PlayersInit(const char* players[6], int playerCount)
{
	for (int i = 0; i < playerCount; i++)
	{
		Players[i] = new Player(players[i]);
	}
}

Player::Player(const char* name) : 
	AvailableHappiness(1),
	AvailableProductivity(0)
{
	Name = new char[strlen(name) + 1];
	strcpy(Name, name);
	LoggedIn = false;
	Id = PlayerCount++;
	Color = -1;
	upgrades = NONE;
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
	return i >= PlayerCount || i < 0 ? "Unknown" : Players[i]->GetName();
}

void Player::GetJSON(Document* document, Value* array)
{
	Value jsonObject(kObjectType);
	jsonObject.AddMember<char*>("name", Name, document->GetAllocator());
	jsonObject.AddMember<bool>("logged_in", LoggedIn, document->GetAllocator());
	jsonObject.AddMember<int>("id", Id, document->GetAllocator());
	jsonObject.AddMember<int>("color", Color, document->GetAllocator());
	jsonObject.AddMember<int>("happiness", AvailableHappiness, document->GetAllocator());
	jsonObject.AddMember<int>("productivity", AvailableProductivity, document->GetAllocator());
	jsonObject.AddMember<int>("total_output", City::GetPlayerTotalOutput(Id), document->GetAllocator());
	array->PushBack(jsonObject, document->GetAllocator());
}

void Player::GetJSONArray(Document* document, Value* array)
{
	for (int i = 0; i < PlayerCount; i++)
	{
		Players[i]->GetJSON(document, array);
	}
}

bool Player::LogIn(int player, int color, struct mg_connection* token)
{
	if (color == -1)
	{
		return false;
	}
	for (int i = 0; i < PlayerCount; i++)
	{
		if (i != player && Players[i]->Color == color)
		{
			return false;
		}
	}

	if (!Players[player]->LoggedIn)
	{
		Players[player]->LoggedIn = true;
		Players[player]->Token = token;
		Players[player]->Color = color;
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
			Players[i]->Color = -1;
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

void Player::DeIncrementCurrentPlayer()
{
	CurrentPlayer--;
	if (CurrentPlayer < 0)
	{
		CurrentPlayer = PlayerCount-1;
	}
}

int Player::GetCurrentPlayer()
{
	return CurrentPlayer;
}

void Player::SetCurrentPlayer(int i)
{
	CurrentPlayer = i;
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
	loginObj.AddMember<int>("color", Players[player]->Color, document.GetAllocator());
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

void Player::DeprecateUpgrade(UPGRADE upgrade, BENEFIT_TYPE type)
{
	for (int i = 0; i < PlayerCount; i++)
	{
		if (Players[i]->upgrades & static_cast<int>(upgrade))
		{
			int happyDiff = 0;
			int prodDiff = 0;
			Upgrade::GetBenefitFromType(type, happyDiff, prodDiff);

			ChangeAvailableUpgrades(i, -happyDiff, -prodDiff);
			Players[i]->upgrades = Players[i]->upgrades & ~upgrade;
		}
	}
}

void Player::PurchaseUpgrade(int player, UPGRADE upgrade, BENEFIT_TYPE type)
{
	Players[player]->upgrades = Players[player]->upgrades | upgrade;

	int happyDiff = 0;
	int prodDiff = 0;
	Upgrade::GetBenefitFromType(type, happyDiff, prodDiff);
	ChangeAvailableUpgrades(player, happyDiff, prodDiff);
}

void Player::ChangeAvailableUpgrades(int player, int happyDiff, int productivityDiff)
{
	if (player != -1)
	{
		Players[player]->AvailableHappiness += happyDiff;
		Players[player]->AvailableProductivity += productivityDiff;
	}
}

bool Player::HasAvailableHappiness(int player)
{
	return Players[player]->AvailableHappiness > 0;
}

bool Player::HasAvailableProductivity(int player)
{
	return Players[player]->AvailableProductivity > 0;
}