  var websocket;

  var MANAGER = false;
  
    var send_cmd = function(cmd, args) {
	var msg = {};
	msg.command = {};
	msg.command.player = Player;
	msg.command.cmd = cmd;
	msg.command.args = args;
	websocket.send(JSON.stringify(msg));
  };
  
  var button_up = function(element)
  {
	element.style.border="4px groove gray";
  };
  var button_down = function(element)
  {
	element.style.border="4px ridge gray";
  };
  
  var cancelClick = function(){
	resetTech();
  };
  
  
  window.onload = function() {
    var url = 'ws://' + location.host + '/ws';

    websocket = new WebSocket(url);
    websocket.onopen = function(ev) {
		send_cmd("get_player_status");
		send_cmd("get_tech_status");
    };
    websocket.onclose = function(ev) {

    };
    websocket.onmessage = function(ev) {
      if (!ev.data) {
        //ping message
      } else {
		var json = JSON.parse(ev.data);
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
		if(json.players != null)
		{
			updatePlayers(json.players);
		}
		if(json.login != null)
		{
			if(json.login.verified == true)
			{
				loginPlayer(json.login.player, json.login.color, true);
			}
		}
      }
    };
    websocket.onerror = function(ev) {
      
    };
  };
