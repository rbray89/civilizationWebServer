// civilizationWebServer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <string.h>
#include <iostream>
#include <fstream>
#include "mongoose.h"
#include "GameManager.h"
#include "TechManager.h"
#include "rapidjson\document.h"
#include "rapidjson\writer.h"
#include "rapidjson\stringbuffer.h"

using namespace rapidjson;
using namespace std;

#define UPDATE_BUFF_SIZE 500

GameManager* _GameManager;

static void msg_handler(char* msg, struct mg_connection *conn)
{
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
		if (strcmp(document["command"]["cmd"].GetString(), "login") == 0)
		{
			int player = document["command"]["args"].GetInt();
			bool success = Player::LogIn(player, conn);
			_GameManager->SendPlayerStatusUpdate();
			char* string = Player::GetLoginStatusJSON(player, success);
			mg_websocket_write(conn, 1, string, strlen(string));
		}
		if (strcmp(document["command"]["cmd"].GetString(), "get_tech_status") == 0)
		{
			char* string = _GameManager->GetTechStatusJSON();
			mg_websocket_write(conn, 1, string, strlen(string));
		}
		if (strcmp(document["command"]["cmd"].GetString(),"buy_time") == 0)
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
			_GameManager->SendTechStatusUpdate();
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

bool cmd_handler()
{
	char c = getc(stdin);
	if ( c == 'e')
	{
		return true;
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

	Player::PlayersInit(&(argv[1]), argc - 1);

	struct mg_server *server = mg_create_server(NULL);
	
	_GameManager = new GameManager(server);
	

	mg_set_option(server, "listening_port", "8080");
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

