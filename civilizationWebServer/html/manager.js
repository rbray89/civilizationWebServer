  var websocket;

  var MANAGER = true;
  
  var send_cmd = function(cmd, player, args) {
	var msg = {};
	msg.command = {};
	msg.command.player = player;
	msg.command.cmd = cmd;
	msg.command.args = args;
	websocket.send(JSON.stringify(msg));
  };
  
  window.onload = function() {
	setupPage();
    var url = 'ws://' + location.host + '/ws-manager';

    websocket = new WebSocket(url);
    websocket.onopen = function(ev) {
		send_cmd("get_player_status");
		send_cmd("get_tech_status");
		send_cmd("get_city_status");
    };
    websocket.onclose = function(ev) {

    };
    websocket.onmessage = function(ev) {
      if (!ev.data) {
        //ping message
      } else {
		var json = JSON.parse(ev.data);
		if(json.players != null)
		{
			updatePlayers(json.players);
			if(Players.length > 0)
			{
				var playerSelector = document.getElementById("players-show");
				playerSelector.style.display= "none";
				showContainer("tech");
			}
		}
		if(Players.length > 0)
		{
			if(json.game_timer != null)
			{
				updateTurnStatus(json.game_timer);
			}
			if(json.wonders != null)
			{
				updateWonders(json.wonders);
			}
			if(json.technologies != null)
			{
				updateTechnologies(json.technologies);
			}
			if(json.login != null)
			{
				if(json.login.verified == true)
				{
					loginPlayer(json.login.player, json.login.color, true);
				}
			}
			if(json.cities != null)
			{
				updateCities(json.cities);
			}
		}
      }
    };
    websocket.onerror = function(ev) {
      
    };
  };
