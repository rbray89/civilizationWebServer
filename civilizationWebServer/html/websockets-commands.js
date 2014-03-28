

  var Technologies;
  var SelectedTech;
  var Wonders;
  var TurnStatus;
  
  var EraIds = ["ancient","medieval","gunpowder_industrial", "modern"];
  var TechBenefits = ["wonder", "seminal", "productive", "happy", "city", "trade", 
					"infantry", "cavalry", "artillery", "fleet", "aircraft"];

  var GamePhases = ["Purchase Phase","Movement/Battle Phase","Trading Phase","Prouction Phase"];
  

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
	var techElementExpanded  = document.getElementById('tech_expanded_0');
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
			techElementExpanded.className = "technology-unexpanded";
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
			techOwner.innerHTML = Players[techObj.owner].name;
			techOwner.style.color = PlayerColors[Players[techObj.owner].color];
		}
		
		if( TurnStatus!=null && TurnStatus.current_phase == 0 && 
		TurnStatus.player_current == Player && techObj.owner == -1 && techObj.available)
		{
			techOwner.style.display = 'none';
			techPurchase.style.display = null;
		}
		else if(techObj.owner == -1)
		{
			techOwner.style.display = null;
			techPurchase.style.display = 'none';
		}
		else
		{
			if(tech.className != "technology-selected")
			{
				tech.className = "technology-purchased";
				techElementExpanded.className = "technology-unexpanded";
			}
			techOwner.style.display = null;
			techPurchase.style.display = 'none';
		}
		
		i++;
		tech = document.getElementById('tech_'+i);
		techOwner = document.getElementById('tech_owner_'+i);
		techPurchase = document.getElementById('tech_purchase_'+i);
		techElementExpanded  = document.getElementById('tech_expanded_'+i);
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
			var techElementExpanded = document.getElementById('tech_expanded_'+techObj.id);
			techElementExpanded.className = "technology-expanded";
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
		
		var techExpanded = document.createElement('div');
		techExpanded.setAttribute('id', 'tech_expanded_'+techObj.id);
		techExpanded.setAttribute('class', 'technology-unexpanded');
		techExpanded.onclick=function(event){selectTech(techObj);event.stopPropagation();};
		
		techText = document.createElement('div');
		var benefitText = techObj.benefit_text;
		techText.className = 'tech-benefit'
		if(benefitText == null)
		{
			if(techObj.wonders.length == 1)
			{
				benefitText = Wonders[techObj.wonders[0]].name+' Wonder';
			}
			else if(techObj.wonders.length == 2)
			{
				benefitText = Wonders[techObj.wonders[0]].name+
				' Wonder AND '+Wonders[techObj.wonders[1]].name+' Wonder';
			}
			
			if(techObj.benefits & 2)
			{
				if(techObj.wonders.length > 0)
				{
					benefitText += ' AND ';
				}
				benefitText += '4 Victory Points'; 
			}
			
			if(techObj.wonders.length == 1)
			{
				benefitText += '<br>' + Wonders[techObj.wonders[0]].description;
			}
			if(techObj.wonders.length == 2)
			{
				benefitText += '<br>' + Wonders[techObj.wonders[1]].description;
			}
		}
		techText.innerHTML = benefitText;
		techExpanded.appendChild(techText);	
		
		var enablesText = techObj.enables_text;
		if(enablesText != null)
		{
			techText = document.createElement('div');
			techText.innerHTML = 'Enables: '+enablesText;
			techExpanded.appendChild(techText);	
		}
		
		tech.appendChild(techExpanded);
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
  
  var updateWonders = function(obj) {
	Wonders = obj;
  };
  