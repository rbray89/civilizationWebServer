  var websocket;
  var Player;
  var Players;
  var Technologies;
  var SelectedTech;
  
  var EraIds = ["ancient","medieval","gunpowder_industrial", "modern"];
  var TechBenefits = ["wonder", "seminal", "productive", "happy", "city", "trade", 
					"infantry", "cavalry", "artillery", "fleet", "aircraft"];
  
  var GamePhases = ["Purchase Phase","Movement/Battle Phase","Trading Phase","Prouction Phase"];
  
  var cancelClick = function(){
	resetTech();
  };
  
  var updateTimer = function(obj) {
    var timer_status = document.getElementById('timer-status');
	var minutes = Math.floor(obj.time_remaining/60);
	var seconds = (obj.time_remaining%60);
	seconds = (seconds < 10 ? "0" : "")+seconds;
	timer_status.textContent  = 'Time Remaining:' + minutes + ':' + seconds;
	if( obj.time_remaining > 120){
		timer_status.style.color="#3CB371";
	} else if(obj.time_remaining > 60){
		timer_status.style.color="#DAA520";
	} else {
		timer_status.style.color="#ff0000";
	}
	var player_current = document.getElementById('turn-status-current');
	var player_next = document.getElementById('turn-status-next');
	if(obj.player_current != -1)
	{
		player_current.textContent  = "Current: " + Players[obj.player_current].name;
	}
	else
	{
		player_current.textContent  = "-";
	}
	if(obj.player_next != -1)
	{
		player_next.textContent  = "Next: " + Players[obj.player_next].name;
	}
	else
	{
		player_next.textContent  = "-";
	}
	var gamePhase = document.getElementById('game-current-phase');
	gamePhase.textContent = GamePhases[obj.current_phase];
	
	var addTimeButton = document.getElementById('turn-button-left');
	var endTurnButton = document.getElementById('turn-button-right');
	if(Player == obj.player_current)
	{
		addTimeButton.style.visibility = 'visible';
	}
	else
	{
		addTimeButton.style.visibility = 'hidden';
	}
	if(Player == obj.player_current || obj.player_current == -1)
	{
		endTurnButton.style.visibility = 'visible';
	}
	else
	{
		endTurnButton.style.visibility = 'hidden';
	}
	
  };

  var resetTech = function()
  {
	var tech = document.getElementById('tech_0');
	var i = 0;
	do
	{ 
		var techObj = Technologies[i];
		var available = true;
		for( var n = 0; n < techObj.dependencies.length; n++)
		{
			var t = techObj.dependencies[n];
			if(Technologies[t].owner == "")
			{
				available = false;
				break;
			}
		}
		if(available)
		{
			tech.className = "technology-available";
		}
		else
		{
			tech.className = "technology-unavailable";
		}
		var techCost = document.getElementById('tech_cost_'+i);
		techCost.innerHTML = techObj.cost;
		var techOwner = document.getElementById('tech_owner_'+i);
		if(techObj.owner == -1)
		{
			techOwner.innerHTML = "-";
		}
		else
		{
			techOwner.innerHTML = Players[techObj.owner];
		}
		i++;
		tech = document.getElementById('tech_'+i);
	}while(tech != null);
  };
  
  var selectSecondDeps = function(techObj)
  {
	var techElement = document.getElementById('tech_'+techObj.id);
	techObj.dependencies.forEach(function(entry) {
		var techDependencyElement = document.getElementById('tech_'+entry);
		techDependencyElement.className += " technology-dependency-2";
		selectSecondDeps(Technologies[entry]);
	});
  };
  
  var selectTech = function(techObj)
  {
	if(techObj != null)
	{
		SelectedTech = techObj;
		var techElement = document.getElementById('tech_'+techObj.id);
		if(techElement.className != "technology-selected")
		{
			resetTech();
			techObj.dependencies.forEach(function(entry) {
				var techDependencyElement = document.getElementById('tech_'+entry);
				techDependencyElement.className += " technology-dependency-1";
				selectSecondDeps(Technologies[entry]);
			});
			techElement.className = "technology-selected";
		}
		else
		{
			resetTech();
		}
	}
  };
  
  var updateTech = function(techObj) {
	var tech = document.getElementById('tech_'+techObj.id);
	if(tech == null)
	{
		tech = document.createElement('div');
		tech.setAttribute('id', 'tech_'+techObj.id);
		tech.setAttribute('class', 'technology-unavailable');
		tech.onclick=function(){selectTech(techObj)};
		techText = document.createElement('div');
		techText.setAttribute('class', 'tech-text-name');
		techText.innerHTML = techObj.name;
		tech.appendChild(techText);
		//cost
		techText = document.createElement('img');
		techText.setAttribute('class', 'tech-text-cost-ico');
		techText.src="coins16.png";
		tech.appendChild(techText);
		techText = document.createElement('div');
		techText.setAttribute('class', 'tech-text-cost');
		techText.setAttribute('id', 'tech_cost_'+techObj.id);
		tech.appendChild(techText);
		//benefits
		for(var i = 0; i < TechBenefits.length; i++)
		{
			if(((1<<i)&techObj.benefits) != 0)
			{
				techText = document.createElement('img');
				techText.setAttribute('class', 'tech-text-benefits');
				techText.src=TechBenefits[i]+"16.png";
				tech.appendChild(techText);
			}
		}
		//owner
		techText = document.createElement('div');
		techText.setAttribute('class', 'tech-owner');
		techText.setAttribute('id', 'tech_owner_'+techObj.id);
		tech.appendChild(techText);
			
		document.getElementById(EraIds[techObj.era]).appendChild(tech);
	}
	resetTech();
  };  
  
  var updateTechnologies = function(obj) {
	Technologies = obj;
	obj.forEach(function(entry) {
		updateTech(entry);
	});
	selectTech(SelectedTech);
  };
  
  var loginPlayer = function(player, serverVerified) {
	if(serverVerified)
	{
		Player = player;
		var header = document.getElementById('player');
		header.innerText = 'Player: '+Players[Player].name;
		var loginPrompt = document.getElementById('login');
		loginPrompt.style.visibility = 'hidden';
		var civDisplay = document.getElementById('civilization-manager');
		civDisplay.style.visibility = 'visible'
	}
	else
	{
		send_cmd('login', player)
	}
  };
  
  var selectPlayerLogin = function(playerLogin) {
	for(var i = 0; i < Players.length; i++)
	{
		var otherLogins = document.getElementById('player_login_'+i);
		if(Players[i].logged_in)
		{
			otherLogins.className = "login-player-taken";
		}
		else
		{
			otherLogins.className = "login-player";
		}
	}
	playerLogin.className = "login-player-selected";
	var loginButton = document.getElementById('login-button');
	loginButton.onclick=function(){loginPlayer(parseInt(playerLogin.getAttribute('name'))); button_up(this);};
  };
  
  var updatePlayers = function(players) {
	Players = players;
	if(Player == null)
	{
		var loginPrompt = document.getElementById('login');
		Players.forEach(function(entry) {
			var playerLogin = document.getElementById("player_login_"+entry.id);
			if(playerLogin == null)
			{
				playerLogin = document.createElement('div');
				loginPrompt.appendChild(playerLogin);
			}
			playerLogin.innerHTML = "<span class='span50'></span>"+entry.name;
			playerLogin.id = "player_login_"+entry.id;
			playerLogin.setAttribute("name",entry.id);
			if(entry.logged_in)
			{
				playerLogin.className = "login-player-taken";
				playerLogin.onclick=null;
			}
			else
			{
				playerLogin.className = "login-player";
				playerLogin.onclick=function(){selectPlayerLogin(playerLogin)};
			}
		});
	}
  };
  
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
			updateTimer(json.game_timer);
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
				loginPlayer(json.login.player, true);
			}
		}
      }
    };
    websocket.onerror = function(ev) {
      
    };
  };