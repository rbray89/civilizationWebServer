
  var Player;
  var PlayerColor = -1;
  var Players = null;
  
  var PlayerColors = ['#DC143C','#00008B','#228B22','#DAA520','#A9A9A9','#8B008B'];
  
  var loginPlayer = function(player, color, serverVerified) {
	if(serverVerified)
	{
		Player = player;
		var header = document.getElementById('player');
		header.innerText = 'Player: '+Players[Player].name;
		header.style.color = PlayerColors[color];
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
	loginButton.onclick=function(){button_up(this); loginPlayer(parseInt(playerLogin.getAttribute('name')), PlayerColor);};
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
  
  var updatePlayerAddCount = function(element) {
	var count = element.options[element.selectedIndex].value;
	var playerContainer = document.getElementById("player-add-container");
	while(playerContainer.firstChild) {
		playerContainer.removeChild(playerContainer.firstChild);
	}
	for(var i = 1; i <= count; i++)
	{
		var player = document.createElement('input');
		player.className = "players-set-class";
		player.id = "player-add-" + i;
		playerContainer.appendChild(player);
	}
	
  };
  
  var setPlayers = function() {
	var playerInput = document.getElementById("player-add-1");
	var players = new Array();
	for(var i = 2; playerInput != null; i++)
	{
		players[players.length] = playerInput.value;
		playerInput = document.getElementById("player-add-" + i);
	}
	send_cmd('init_players', -1, players);
  };
  
  var updatePlayers = function(players) {
	if(MANAGER && Players == null)
	{
		var cityPlayerAdd = document.getElementById("city-player-add");
		players.forEach(function(entry) {
			cityPlayerAdd.options.add(new Option(entry.name, entry.id));
		});
	}
	Players = players;
	if(!MANAGER && Player == null)
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
  

