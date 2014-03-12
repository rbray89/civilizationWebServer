// civilizationWebServer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <string.h>
#include <iostream>
#include <fstream>
#include "mongoose.h"
#include "GameTimer.h"
#include "TechManager.h"
#include "rapidjson\document.h"
#include "rapidjson\writer.h"
#include "rapidjson\stringbuffer.h"

using namespace rapidjson;
using namespace std;

#define UPDATE_BUFF_SIZE 500

GameTimer* _GameTimer;
TechManager* _TechManager;

static void msg_handler(char* msg, struct mg_connection *conn)
{
	Document document;
	if (document.Parse<0>(msg).HasParseError() || !document.IsObject())
	{
		return;
	}
	if (document.HasMember("command"))
	{
		if (strcmp(document["command"]["cmd"].GetString(),"buy_time") == 0)
		{
			_GameTimer->ExtendTurn();
		}
		if (strcmp(document["command"]["cmd"].GetString(), "end_turn") == 0)
		{
			_GameTimer->EndTurn();
		}
		if (strcmp(document["command"]["cmd"].GetString(), "send_tech_status") == 0)
		{
			char* string = _TechManager->GetTechStatusJSON();
			printf(string);
			mg_websocket_write(conn, 1, string, strlen(string));
		}
	}
}

static int request_handler(struct mg_connection *conn) {
	
	if (conn->is_websocket) {
		conn->content[conn->content_len] = 0;
		msg_handler(conn->content, conn);
		return conn->content_len == 4 && !memcmp(conn->content, "exit", 4) ?
		MG_CLIENT_CLOSE : MG_CLIENT_CONTINUE;
	}
	else {
		return MG_REQUEST_NOT_PROCESSED;
	}
}

static void *serve(void *server) {
	for (;;) mg_poll_server((struct mg_server *) server, 1000);
	return NULL;
}

int main(int argc, char* argv[]){

	printf("Players: ");
	for (int i = 1; i < argc; i++)
	{
		printf("%s ", argv[i]);
	}

	struct mg_server *server = mg_create_server(NULL);
	_GameTimer = new GameTimer(server, &(argv[1]), argc-1);
	_TechManager = new TechManager();

	mg_set_option(server, "listening_port", "8080");
	mg_set_option(server, "document_root", "html");
	mg_set_request_handler(server, request_handler);

	
	printf("\nStarted on port %s\n", mg_get_option(server, "listening_port"));
	mg_start_thread(serve, server);
	
	getc(stdin);
	mg_destroy_server(&server);
	return 0;
}

