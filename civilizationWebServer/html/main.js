  var websocket;
  var User = "Ryan";
  var Technologies;
  var EraIds = ["ancient","medieval","gunpowder_industrial", "modern"];
  
  var updateTimer = function(obj) {
    var timer_status = document.getElementById('timer-status');
	var minutes = Math.floor(obj.time_remaining/60);
	var seconds = (obj.time_remaining%60);
	seconds = (seconds < 10 ? "0" : "")+seconds;
	timer_status.innerHTML = 'Time Remaining:' + minutes + ':' + seconds;
	if( obj.time_remaining > 120){
		timer_status.style.color="#3CB371";
	} else if(obj.time_remaining > 60){
		timer_status.style.color="#DAA520";
	} else {
		timer_status.style.color="#ff0000";
	}
	var player_current = document.getElementById('turn-status-current');
	player_current.innerHTML = "Current: " + obj.player_current;
	var player_current = document.getElementById('turn-status-next');
	player_current.innerHTML = "Next: " + obj.player_next;
  };

  var resetTech = function()
  {
	var tech = document.getElementById('tech_0');
	var i = 0;
	do
	{ 
		tech.className = "technology-unavailable";
		tech = document.getElementById('tech_'+i);
		i++;
	}while(tech != null);
  };
  
  var selectSecondDeps = function(techObj)
  {
	var techElement = document.getElementById('tech_'+techObj.id);
	techObj.dependencies.forEach(function(entry) {
		var techDependencyElement = document.getElementById('tech_'+entry);
		techDependencyElement.className = "technology-dependency-2";
		selectSecondDeps(Technologies[entry]);
	});
  };
  
  var selectTech = function(techObj)
  {
	resetTech();
	var techElement = document.getElementById('tech_'+techObj.id);
	techObj.dependencies.forEach(function(entry) {
		var techDependencyElement = document.getElementById('tech_'+entry);
		techDependencyElement.className = "technology-dependency-1";
		selectSecondDeps(Technologies[entry]);
	});
	techElement.className = "technology-selected";
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
		techText.setAttribute('class', 'tech-text');
		techText.innerHTML = techObj.name;
		tech.appendChild(techText);
		document.getElementById(EraIds[techObj.era]).appendChild(tech);
	}
  };  
  
  var updateTechnologies = function(obj) {
	Technologies = obj;
	obj.forEach(function(entry) {
		updateTech(entry);
	});
  };
  
  var send_cmd = function(cmd) {
	var msg = {};
	msg.command = {};
	msg.command.user = User;
	msg.command.cmd = cmd;
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
		send_cmd("send_tech_status");
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
      }
    };
    websocket.onerror = function(ev) {
      
    };
  };