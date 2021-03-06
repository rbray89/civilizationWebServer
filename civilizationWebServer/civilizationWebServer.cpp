// civilizationWebServer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <string.h>
#include <iostream>
#include <fstream>
#include "mongoose.h"
#include "GameManager.h"
#include "rapidjson\document.h"
#include "rapidjson\writer.h"
#include "rapidjson\stringbuffer.h"

using namespace rapidjson;
using namespace std;


char* filename;
GameManager* _GameManager;

static void msg_handler(char* msg, struct mg_connection *conn)
{
	printf("%s\n\n", msg);
	Document document;
	if (document.Parse<0>(msg).HasParseError() || !document.IsObject())
	{
		return;
	}
	if (document.HasMember("command"))
	{
		if (strcmp(document["command"]["cmd"].GetString(), "get_player_status") == 0)
		{
			char* string = _GameManager->GetPlayerStatusJSON();
			mg_websocket_write(conn, 1, string, strlen(string));
		}
		else if (strcmp(document["command"]["cmd"].GetString(), "get_tech_status") == 0)
		{
			char* string = _GameManager->GetWonderStatusJSON();
			mg_websocket_write(conn, 1, string, strlen(string));
			string = _GameManager->GetTechStatusJSON();
			mg_websocket_write(conn, 1, string, strlen(string));
		}
		else if (strcmp(document["command"]["cmd"].GetString(), "get_city_status") == 0)
		{
			char* string = _GameManager->GetCityStatusJSON();
			mg_websocket_write(conn, 1, string, strlen(string));
		}
		else if (strcmp(document["command"]["cmd"].GetString(), "get_upgrade_status") == 0)
		{
			char* string = _GameManager->GetUpgradeStatusJSON();
			mg_websocket_write(conn, 1, string, strlen(string));
		}
		else if (strcmp(document["command"]["cmd"].GetString(), "get_unit_status") == 0)
		{
			char* string = _GameManager->GetUnitStatusJSON();
			mg_websocket_write(conn, 1, string, strlen(string));
		}
		else if (strcmp(document["command"]["cmd"].GetString(), "assign_city_trade") == 0)
		{
			int player = document["command"]["args"]["traded"].GetInt();
			int city = document["command"]["args"]["id"].GetInt();
			_GameManager->AssignCityTrade(player, city);
			_GameManager->SendCityStatusUpdate();
			_GameManager->SendPlayerStatusUpdate();
		}
		else if (strcmp(document["command"]["cmd"].GetString(), "toggle_city_happiness") == 0)
		{
			int city = document["command"]["args"]["id"].GetInt();
			_GameManager->ToggleCityHappinessUpgrade(city);
			_GameManager->SendCityStatusUpdate();
			_GameManager->SendPlayerStatusUpdate();
		}
		else if (strcmp(document["command"]["cmd"].GetString(), "toggle_city_productivity") == 0)
		{
			int city = document["command"]["args"]["id"].GetInt();
			_GameManager->ToggleCityProductivityUpgrade(city);
			_GameManager->SendCityStatusUpdate();
			_GameManager->SendPlayerStatusUpdate();
		}
		else if (strcmp(document["command"]["cmd"].GetString(), "increase_city_size") == 0)
		{
			int city = document["command"]["args"]["id"].GetInt();
			_GameManager->IncreaseCitySize(city);
			_GameManager->SendCityStatusUpdate();
			_GameManager->SendPlayerStatusUpdate();
		}

		if (strcmp(conn->uri, "/ws-manager") == 0)
		{
			if (strcmp(document["command"]["cmd"].GetString(), "start_stop") == 0)
			{
				_GameManager->StartStop();
			}

			if (strcmp(document["command"]["cmd"].GetString(), "init_players") == 0)
			{
				const Value& players = document["command"]["args"];
				const char* playerNames[6];
				for (unsigned int i = 0; i < players.Size(); i++)
				{
					playerNames[i] = players[i].GetString();
				}
				Player::PlayersInit(playerNames, players.Size());

				_GameManager->SendPlayerStatusUpdate();
				_GameManager->SendWonderStatusUpdate();
				_GameManager->SendTechStatusUpdate();
				_GameManager->SendCityStatusUpdate();
				_GameManager->SendUpgradeStatusUpdate();
				_GameManager->SendUnitStatusUpdate();
				
			}
			
			if (strcmp(document["command"]["cmd"].GetString(), "assign_tech") == 0)
			{
				int player = document["command"]["player"].GetInt();
				int tech = document["command"]["args"].GetInt();
				_GameManager->PurchaseTechOverride(player, tech);
				_GameManager->SendWonderStatusUpdate();
				_GameManager->SendTechStatusUpdate();
				_GameManager->SendUpgradeStatusUpdate();
				_GameManager->SendPlayerStatusUpdate();
				_GameManager->SendUnitStatusUpdate();
			}
			if (strcmp(document["command"]["cmd"].GetString(), "assign_city") == 0)
			{
				int player = document["command"]["player"].GetInt();
				int city = document["command"]["args"].GetInt();
				_GameManager->AssignCity(player, city);
				_GameManager->SendCityStatusUpdate();
				_GameManager->SendPlayerStatusUpdate();
			}
			if (strcmp(document["command"]["cmd"].GetString(), "create_city") == 0)
			{
				int player = document["command"]["player"].GetInt();
				RESOURCE resource = static_cast<RESOURCE>(document["command"]["args"]["resource"].GetInt());
				bool fertile = static_cast<bool>(document["command"]["args"]["fertile"].GetBool());
				_GameManager->CreateCity(player, resource, fertile);
				_GameManager->SendCityStatusUpdate();
				_GameManager->SendPlayerStatusUpdate();
			}
			if (strcmp(document["command"]["cmd"].GetString(), "deprecate_upgrade") == 0)
			{
				int upgrade = document["command"]["args"].GetInt();
				_GameManager->DeprecateUpgrade(upgrade);
				_GameManager->SendUpgradeStatusUpdate();
				_GameManager->SendPlayerStatusUpdate();
			}
			if (strcmp(document["command"]["cmd"].GetString(), "end_turn") == 0)
			{
				_GameManager->EndTurn();
			}
			if (strcmp(document["command"]["cmd"].GetString(), "previous_turn") == 0)
			{
				_GameManager->PreviousTurn();
			}
		}
		else
		{
			if (strcmp(document["command"]["cmd"].GetString(), "login") == 0)
			{
				int player = document["command"]["args"]["player"].GetInt();
				int color = document["command"]["args"]["color"].GetInt();
				bool success = Player::LogIn(player, color, conn);
				_GameManager->SendPlayerStatusUpdate();
				_GameManager->HandleSecondEvent(false);
				char* string = Player::GetLoginStatusJSON(player, success);
				mg_websocket_write(conn, 1, string, strlen(string));
			}
			else if (strcmp(document["command"]["cmd"].GetString(), "buy_time") == 0)
			{
				_GameManager->ExtendTurn();
			}
			else if (strcmp(document["command"]["cmd"].GetString(), "end_turn") == 0)
			{
				_GameManager->EndTurn();
			}
			else if (strcmp(document["command"]["cmd"].GetString(), "purchase_tech") == 0)
			{
				int player = document["command"]["player"].GetInt();
				int tech = document["command"]["args"].GetInt();
				_GameManager->PurchaseTech(player, tech);
				_GameManager->SendWonderStatusUpdate();
				_GameManager->SendTechStatusUpdate();
				_GameManager->SendUpgradeStatusUpdate();
				_GameManager->SendPlayerStatusUpdate();
				_GameManager->SendUnitStatusUpdate();
			}
			else if (strcmp(document["command"]["cmd"].GetString(), "purchase_upgrade") == 0)
			{
				int player = document["command"]["player"].GetInt();
				int upgrade = document["command"]["args"].GetInt();
				_GameManager->PurchaseUpgrade(player, upgrade);
				_GameManager->SendUpgradeStatusUpdate();
				_GameManager->SendPlayerStatusUpdate();
			}
		}
	}
}

static int request_handler(struct mg_connection *conn) {
	
	//connection not closed.
	if (!(conn->flags & 0x01))
	{
		if (conn->is_websocket) 
		{
			bool exit = conn->content_len == 4 && !memcmp(conn->content, "exit", 4);
			if (exit)
			{
				return MG_CLIENT_CLOSE;
			}
			conn->content[conn->content_len] = 0;
			msg_handler(conn->content, conn);
			return MG_CLIENT_CONTINUE;
		}
		else {
			return MG_REQUEST_NOT_PROCESSED;
		}
	}
	else
	{
		if (conn->is_websocket)
		{
			Player::LogOut(conn);
			_GameManager->SendPlayerStatusUpdate();
		}
		return MG_REQUEST_NOT_PROCESSED;
	}
}

static void *serve(void *server) {
	for (;;) mg_poll_server((struct mg_server *) server, 1000);
	return NULL;
}

int p = -1;
int t = 0;
bool cmd_handler()
{
	char c = getc(stdin);
	if ( c == 'e')
	{
		return true;
	}
	else if (c == 's')
	{
		_GameManager->SaveState(filename);
	}
	else if (c == 'l')
	{
		_GameManager->LoadState(filename);
		_GameManager->SendPlayerStatusUpdate();
		_GameManager->SendWonderStatusUpdate();
		_GameManager->SendTechStatusUpdate();
		_GameManager->SendCityStatusUpdate();
		_GameManager->SendUpgradeStatusUpdate();
	}
	else if (c <= '9' && c >= '0')
	{
		if (p == -1)
		{
			p = c - '0';
		}
		else
		{
			t = c - '0';
			_GameManager->PurchaseTechOverride(p, t);
			p = -1;
		}
	}
	return false;
}


int main(int argc, char* argv[]){

	printf("Players: ");
	for (int i = 1; i < argc; i++)
	{
		printf("%s ", argv[i]);
	}

	filename = argv[1];
	//Player::PlayersInit(&(argv[2]), argc - 2);

	struct mg_server *server = mg_create_server(NULL);
	
	_GameManager = new GameManager(server);
	//_GameManager->LoadState(filename);

	const char* err = mg_set_option(server, "listening_port", "80");
	if (err != NULL)
	{
		printf("ERROR: %s\n", err);
		getc(stdin);
		return 1;
	}
	mg_set_option(server, "document_root", "html");
	mg_set_request_handler(server, request_handler);

	
	printf("\nStarted on port %s\n", mg_get_option(server, "listening_port"));
	mg_start_thread(serve, server);
	while (true)
	{
		if (cmd_handler())
		{
			break;
		}
	}

	mg_destroy_server(&server);
	return 0;
}

