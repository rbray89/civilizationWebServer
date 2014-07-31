
  var Technologies;
  var SelectedTech;

  var TechBenefits = ["wonder", "seminal", "productive", "happy", "city", "trade", 
					"infantry", "cavalry", "artillery", "fleet", "aircraft"];

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
		if(!MANAGER)
		{
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
		}
		else
		{
			techOwner.options[techObj.owner+1].selected = true;
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
  
  var assignTech = function(techObj, selector)
  {
	send_cmd("assign_tech",selector.options[selector.selectedIndex].value-1, techObj.id);
  }
  
  var selectTech = function(techObj)
  {
	if(!MANAGER && techObj != null)
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
			var techElementExpanded = document.getElementById('tech_expanded_'+techObj.id);
			techElementExpanded.className = "technology-expanded";
		
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
		if(!MANAGER)
		{
			techText = document.createElement('div');
			techText.setAttribute('class', 'tech-owner');
			techText.setAttribute('id', 'tech_owner_'+techObj.id);
			tech.appendChild(techText);
		}
		else
		{
			var selector = document.createElement('select');
			selector.setAttribute('class', 'tech-owner');
			selector.setAttribute('id', 'tech_owner_'+techObj.id);
			selector.options.add(new Option('-', 0));
			for(var i = 0; i < Players.length; i++)
			{
				selector.options.add(new Option(Players[i].name, 1+i));
			}
			selector.onchange = function(){assignTech(techObj, selector);};
			tech.appendChild(selector);
		}
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
			benefitText = "";
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
  };  
  
  var updateTechnologies = function(obj) {
	Technologies = obj;
	obj.forEach(function(entry) {
		updateTech(entry);
	});
    resetTech(false);
	selectTech(SelectedTech);
  };
  
//  var updateCities = function(obj) {
//	resetTech(false)
//  };  
  