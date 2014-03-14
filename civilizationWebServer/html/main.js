  var websocket;
  var Player;
  var PlayerColor = -1;
  var Players;
  var Technologies;
  var SelectedTech;
  var TurnStatus;
  
  var EraIds = ["ancient","medieval","gunpowder_industrial", "modern"];
  var TechBenefits = ["wonder", "seminal", "productive", "happy", "city", "trade", 
					"infantry", "cavalry", "artillery", "fleet", "aircraft"];
  var PlayerColors = ['#DC143C','#00008B','#228B22','#DAA520','#A9A9A9','#8B008B'];
  
  var GamePhases = ["Purchase Phase","Movement/Battle Phase","Trading Phase","Prouction Phase"];
  
  
  
  var cancelClick = function(){
	resetTech();
  };
  
  var updateTurnStatus = function(obj) {
	TurnStatus = obj
    var timer_status = document.getElementById('timer-status');
	var minutes = Math.floor(TurnStatus.time_remaining/60);
	var seconds = (TurnStatus.time_remaining%60);
	seconds = (seconds < 10 ? "0" : "")+seconds;
	timer_status.textContent  = 'Time Remaining:' + minutes + ':' + seconds;
	if( TurnStatus.time_remaining > 120){
		timer_status.style.color="#3CB371";
	} else if(TurnStatus.time_remaining > 60){
		timer_status.style.color="#DAA520";
	} else {
		timer_status.style.color="#ff0000";
	}
	var player_current = document.getElementById('turn-status-current');
	var player_next = document.getElementById('turn-status-next');
	if(TurnStatus.player_current != -1)
	{
		player_current.textContent  = "Current: " + Players[TurnStatus.player_current].name;
		if(Players[TurnStatus.player_current].color != -1)
		{
			player_current.style.color = PlayerColors[Players[TurnStatus.player_current].color];
		}
		else
		{
			player_current.style.color = null;
		}
	}
	else
	{
		player_current.textContent  = "-";
		player_current.style.color = null;
	}
	if(TurnStatus.player_next != -1)
	{
		player_next.textContent  = "Next: " + Players[TurnStatus.player_next].name;
		if(Players[TurnStatus.player_next].color != -1)
		{
			player_next.style.color = PlayerColors[Players[TurnStatus.player_next].color];
		}
		else
		{
			player_next.style.color = null;
		}
	}
	else
	{
		player_next.textContent  = "-";
		player_next.style.color = null;
	}
	var gamePhase = document.getElementById('game-current-phase');
	gamePhase.textContent = GamePhases[TurnStatus.current_phase];
	
	var playerBanner = document.getElementById('player-banner');
	var addTimeButton = document.getElementById('turn-button-left');
	var endTurnButton = document.getElementById('turn-button-right');
	if(Player == TurnStatus.player_current)
	{
		addTimeButton.style.visibility = 'visible';
	}
	else
	{
		addTimeButton.style.visibility = 'hidden';
	}
	if(Player == TurnStatus.player_current || TurnStatus.player_current == -1)
	{
		playerBanner.style.background = '#F5DEB3';
		endTurnButton.style.visibility = 'visible';
	}
	else
	{
		playerBanner.style.background = 'FloralWhite';
		endTurnButton.style.visibility = 'hidden';
	}
	resetTech(false);
  };
  
  var resetTech = function(selectReset)
  {
	var tech = document.getElementById('tech_0');
	var techOwner = document.getElementById('tech_owner_0');
	var techPurchase = document.getElementById('tech_purchase_0');
	var i = 0;
	do
	{ 
		var techObj = Technologies[i];
		var available = true;
		for( var n = 0; n < techObj.dependencies.length; n++)
		{
			var t = techObj.dependencies[n];
			if(Technologies[t].owner == -1)
			{
				available = false;
				break;
			}
		}
		techObj.available = available;
		if(selectReset)
		{
			tech.style.border = null;
		}
		if(selectReset || tech.className != "technology-selected")
		{
			if(available)
			{
				tech.className = "technology-available";
			}
			else
			{
				tech.className = "technology-unavailable";
			}
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
		
		if( TurnStatus!=null && TurnStatus.current_phase == 0 && 
		TurnStatus.player_current == Player && techObj.owner == -1 && techObj.available)
		{
			techOwner.style.display = 'none';
			techPurchase.style.display = null;
		}
		else if(techObj.owner == -1)
		{
			techOwner.innerHTML = "-";
			techOwner.style.display = null;
			techPurchase.style.display = 'none';
		}
		else
		{
			if(tech.className != "technology-selected")
			{
				tech.className = "technology-purchased";
			}
			techOwner.innerHTML = Players[techObj.owner].name;
			techOwner.style.display = null;
			techPurchase.style.display = 'none';
		}
		
		i++;
		tech = document.getElementById('tech_'+i);
		techOwner = document.getElementById('tech_owner_'+i);
		techPurchase = document.getElementById('tech_purchase_'+i);
	}while(tech != null);
  };
  
  var selectSecondDeps = function(techObj)
  {
	var techElement = document.getElementById('tech_'+techObj.id);
	techObj.dependencies.forEach(function(entry) {
		var techDependencyElement = document.getElementById('tech_'+entry);
		techDependencyElement.style.border= '3px dotted #6A5ACD';
		selectSecondDeps(Technologies[entry]);
	});
  };
  
  var purchaseTech = function(techObj)
  {
	if (confirm("You are purchasing "+techObj.name+"."))
	  {
		send_cmd('purchase_tech', techObj.id);
	  }
  }
  
  var selectTech = function(techObj)
  {
	if(techObj != null)
	{
		SelectedTech = techObj;
		var techElement = document.getElementById('tech_'+techObj.id);
		if(techElement.className != "technology-selected")
		{
			resetTech(true);
			techObj.dependencies.forEach(function(entry) {
				var techDependencyElement = document.getElementById('tech_'+entry);
				techDependencyElement.style.border= '3px dashed #FF0000';
				selectSecondDeps(Technologies[entry]);
			});
			techElement.className = "technology-selected";
		}
		else
		{
			resetTech(true);
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
		techText = document.createElement('div');
		techText.setAttribute('class', 'tech-purchase');
		techText.setAttribute('id', 'tech_purchase_'+techObj.id);
		techText.onclick=function(event){purchaseTech(techObj);event.stopPropagation();};
		techText.innerHTML = "<span class='span18'></span>Purchase";
		techText.style.display = 'none';
		tech.appendChild(techText);	
		document.getElementById(EraIds[techObj.era]).appendChild(tech);
	}
	resetTech(false);
  };  
  
  var updateTechnologies = function(obj) {
	Technologies = obj;
	obj.forEach(function(entry) {
		updateTech(entry);
	});
	selectTech(SelectedTech);
  };
  
  var loginPlayer = function(player, color, serverVerified) {
	if(serverVerified)
	{
		Player = player;
		var header = document.getElementById('player');
		header.innerText = 'Player: '+Players[Player].name;
		header.style.color = color;
		var loginPrompt = document.getElementById('login');
		loginPrompt.style.visibility = 'hidden';
		var civDisplay = document.getElementById('civilization-manager');
		civDisplay.style.visibility = 'visible'
	}
	else
	{
		var obj = {};
		obj.player = player;
		obj.color = color;
		send_cmd('login', obj)
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
	loginButton.onclick=function(){loginPlayer(parseInt(playerLogin.getAttribute('name')), PlayerColor); button_up(this);};
  };
  
  var selectPlayerColor = function(id)
  {
	var playerColor = document.getElementById('login-color-'+id);
	for(var n = 0; n < PlayerColors.length; n++)
	{
		var color = document.getElementById('login-color-'+n);
		color.style.borderColor='gray';
	}
	playerColor.style.borderColor='#6495ED';
	PlayerColor = id;
  }
  
  var assignLoginColor = function(i)
  {
	var playerColor = document.getElementById('login-color-'+i);
	playerColor.onclick=function(){selectPlayerColor(i)};
	playerColor.style.background = PlayerColors[i];
	Players.forEach(function(entry) {
		if(entry.logged_in && entry.color == i)
		{
			playerColor.style.visibility = 'hidden';
		}
	});
  }
  
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
		for(var i = 0; i < PlayerColors.length; i++)
		{
			assignLoginColor(i);
		}
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
			updateTurnStatus(json.game_timer);
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
