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
	printf(conn->uri);
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
		if (strcmp(document["command"]["cmd"].GetString(), "get_tech_status") == 0)
		{
			char* string = _GameManager->GetWonderStatusJSON();
			mg_websocket_write(conn, 1, string, strlen(string));
			string = _GameManager->GetTechStatusJSON();
			mg_websocket_write(conn, 1, string, strlen(string));
		}
		if (strcmp(document["command"]["cmd"].GetString(), "get_city_status") == 0)
		{
			char* string = _GameManager->GetCityStatusJSON();
			mg_websocket_write(conn, 1, string, strlen(string));
		}
		if (strcmp(document["command"]["cmd"].GetString(), "assign_city_trade") == 0)
		{
			int player = document["command"]["args"]["traded"].GetInt();
			int city = document["command"]["args"]["id"].GetInt();
			_GameManager->AssignCityTrade(player, city);
			_GameManager->SendCityStatusUpdate();
		}
		if (strcmp(conn->uri, "/ws-manager") == 0)
		{
			if (strcmp(document["command"]["cmd"].GetString(), "assign_tech") == 0)
			{
				int player = document["command"]["player"].GetInt();
				int tech = document["command"]["args"].GetInt();
				_GameManager->PurchaseTechOverride(player, tech);
				_GameManager->SendWonderStatusUpdate();
				_GameManager->SendTechStatusUpdate();
			}
			if (strcmp(document["command"]["cmd"].GetString(), "assign_city") == 0)
			{
				int player = document["command"]["player"].GetInt();
				int city = document["command"]["args"].GetInt();
				_GameManager->AssignCity(player, city);
				_GameManager->SendCityStatusUpdate();
			}
			if (strcmp(document["command"]["cmd"].GetString(), "create_city") == 0)
			{
				int player = document["command"]["player"].GetInt();
				RESOURCE resource = static_cast<RESOURCE>(document["command"]["args"]["resource"].GetInt());
				bool fertile = static_cast<bool>(document["command"]["args"]["fertile"].GetBool());
				_GameManager->CreateCity(player, resource, fertile);
				_GameManager->SendCityStatusUpdate();
			}
			if (strcmp(document["command"]["cmd"].GetString(), "end_turn") == 0)
			{
				_GameManager->EndTurn();
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
				char* string = Player::GetLoginStatusJSON(player, success);
				mg_websocket_write(conn, 1, string, strlen(string));
			}
			if (strcmp(document["command"]["cmd"].GetString(), "buy_time") == 0)
			{
				_GameManager->ExtendTurn();
			}
			if (strcmp(document["command"]["cmd"].GetString(), "end_turn") == 0)
			{
				_GameManager->EndTurn();
			}
			if (strcmp(document["command"]["cmd"].GetString(), "purchase_tech") == 0)
			{
				int player = document["command"]["player"].GetInt();
				int tech = document["command"]["args"].GetInt();
				_GameManager->PurchaseTech(player, tech);
				_GameManager->SendWonderStatusUpdate();
				_GameManager->SendTechStatusUpdate();
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
	_GameManager->StartStop();
	return false;
}


int main(int argc, char* argv[]){

	printf("Players: ");
	for (int i = 1; i < argc; i++)
	{
		printf("%s ", argv[i]);
	}

	filename = argv[1];
	Player::PlayersInit(&(argv[2]), argc - 2);

	struct mg_server *server = mg_create_server(NULL);
	
	_GameManager = new GameManager(server);
	_GameManager->LoadState(filename);

	mg_set_option(server, "listening_port", "80");
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

