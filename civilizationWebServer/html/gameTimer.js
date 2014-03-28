
  var TurnStatus;
  
  var EraIds = ["ancient","medieval","gunpowder_industrial", "modern"];
  var GamePhases = ["Purchase Phase","Movement/Battle Phase","Trading Phase","Production Phase"];

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
	if(Player == TurnStatus.player_current || MANAGER)
	{
		addTimeButton.style.visibility = 'visible';
	}
	else
	{
		addTimeButton.style.visibility = 'hidden';
	}
	if(Player == TurnStatus.player_current || TurnStatus.player_current == -1 || MANAGER)
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
 