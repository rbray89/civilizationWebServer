
  var Resources = ["None", "Horses", "Spices", "Rare Metals", "Iron", "Oil", "Coal", "Wine", "Gems"];
  var ResourcesID = ["none", "horses", "spices", "rareMetals", "iron", "oil", "coal", "wine", "gems"];
  var Cities;
  
  var addCity= function()
  {
	var cityFertile = document.getElementById('city-fertile-add');
	var cityResource = document.getElementById('city-resource-add');
	var cityPlayer = document.getElementById('city-player-add');
	var obj = {};
	obj.resource = parseInt(cityResource.options[cityResource.selectedIndex].value);
	obj.fertile = cityFertile.checked;
	send_cmd('create_city', parseInt(cityPlayer.options[cityPlayer.selectedIndex].value), obj);
  };
  
  var assignCity = function(city, selector)
  {
	send_cmd("assign_city",  parseInt(selector.options[selector.selectedIndex].value), city.id);
  }
  
  var assignCityTrade = function(city, selector)
  {
	city.traded = parseInt(selector.options[selector.selectedIndex].value); 
	send_cmd("assign_city_trade", city);
  }
  
  var toggleCityHappiness = function(city)
  {
    if (!MANAGER)
    {
      send_cmd("toggle_city_happiness", city)
    }
  }
  
  var toggleCityProduction = function(city)
  {
    if (!MANAGER)
    {  
      send_cmd("toggle_city_productivity", city)
    }
  }
  
  var increaseCitySize = function(city)
  {
    if (!MANAGER)
    {  
	  if (confirm("You are upgrading the city size"))
	  {
	   send_cmd("increase_city_size", city)
	  }    
    }
  }
 
  
  // Manager sees all fields except trade (instead manager sees owner)
  // Owner sees all fields
  // Trader sees only resource and owner text  
  var showCity= function(city)
  {  
  
    if(!MANAGER && Player != city.owner && Player != city.traded)
    {
    	hideCityBox(city)
        return;
    }
    
    // Show city
    var cityElement = showCityBox(city);
    
    // Always show resource
    showCityResourceIcon(city, cityElement);
    showCityResourceText(city, cityElement);    
    
    // Only show prod, happy, productivity to Manager or owner
    if (Player == city.owner || MANAGER)
    {
        showCityProdIcon(city, cityElement);    
        showCityProductionText(city, cityElement);
        showCityHappiness(city, cityElement);
        showCityProductivity(city, cityElement);
        showCitySizeIcon(city, cityElement);
        showCitySizeText(city, cityElement);   
        hideCityOwner(city, cityElement);
    }
    // Otherwise, if trader show owner
    else if (Player == city.traded)
    {
        showCityOwner(city, cityElement);
    }

    // Show trade text only to owner
    if (Player == city.owner)
    {
        //showCityTradeText(city, cityElement);
        showCityTradeSelect(city, cityElement);       
    }
    
    // Show owner selection box to manager
    if (MANAGER)
    {
        showCityOwnerSelect(city, cityElement);
    }
    
  }
  
  var updateCities = function(obj) {
	Cities = obj;
	obj.forEach(function(entry) {
		showCity(entry);
	});
  };
  
  ///////////////////////////////////////////////////////////////
    
  var showCityProductionText = function(city, cityElement)
  {
    var id = 'city_'+city.id+'_prod';
    var element = document.getElementById(id);
     
    if (element == null)
    {
      element = document.createElement('div');
      element.setAttribute('id', id); 
      element.setAttribute('class', 'city-production-text');
      cityElement.appendChild(element);
    }
     
    element.innerHTML = city.production;
  }
        
  var showCityHappiness = function(city, cityElement)
  {
    var id = 'city_'+city.id+'_happyico';
    var element = document.getElementById(id);
     
    if (element == null)
    {
      element = document.createElement('img');
      element.setAttribute('id', id); 
      element.setAttribute('class', 'city-happy');
      element.onclick = function(){ toggleCityHappiness(city); }
      cityElement.appendChild(element);
    }
     
    if(city.isHappy)
    {
        element.src='happy24.png';
    }
    else
    {
        element.src='unhappy24.png';
    }     
  }  
  
  var showCityProductivity = function(city, cityElement)
  {
    var id = 'city_'+city.id+'_prodico';
    var element = document.getElementById(id);
     
    if (element == null)
    {
      element = document.createElement('img');
      element.setAttribute('id', id); 
      element.setAttribute('class', 'city-productive');
      element.onclick = function(){ toggleCityProduction(city); }
      cityElement.appendChild(element);
    }
    
    if(city.isFertile)
    {
        element.src='fertile24.png';    
    }
    else if(city.isProductive)
    {            
        element.src='productive24.png';
    }
    else
    {
        element.src='unproductive24.png';
    }
  }
  
  var showCityResourceText = function(city, cityElement)
  {
    var id = 'city_'+city.id+'_res';
    var element = document.getElementById(id);
     
    if (element == null)
    {
      element = document.createElement('div');
      element.setAttribute('id', id); 
      element.setAttribute('class', 'city-resource-text span24');
      cityElement.appendChild(element);
    }

    if(city.resource > 0)
    {
        element.innerHTML = Resources[city.resource];
    }
    else
    {
        element.innerHTML = "None";
    }    
  }
  
  var showCityResourceIcon = function(city, cityElement)
  {
    var id = 'city_'+city.id+'_resico';
    var element = document.getElementById(id);
     
    if (element == null)
    {
      element = document.createElement('img');
      element.setAttribute('id', id); 
      element.setAttribute('class', 'city-resource-ico');
      cityElement.appendChild(element);
    }

    element.src=ResourcesID[city.resource]+'24.png';
  }  
  
  var showCityProdIcon = function(city, cityElement)
  {
    var id = 'city_'+city.id+'_prodico2';
    var element = document.getElementById(id);
     
    if (element == null)
    {
      element = document.createElement('img');
      element.setAttribute('id', id); 
      element.setAttribute('class', 'city-production-ico');
      element.src='coins24.png';
      cityElement.appendChild(element);
    }
  }
  
  var showCitySizeIcon = function(city, cityElement)
  {
    var id = 'city_'+city.id+'_sizeico';
    var element = document.getElementById(id);
     
    if (element == null)
    {
      element = document.createElement('img');
      element.setAttribute('id', id); 
      element.setAttribute('class', 'city-size-ico');
      element.src='size24.png';
      element.onclick = function(){ increaseCitySize(city); }
      cityElement.appendChild(element);
    }
  }  
  
  var showCitySizeText = function(city, cityElement)
  {
    var id = 'city_'+city.id+'_size';
    var element = document.getElementById(id);
     
    if (element == null)
    {
      element = document.createElement('div');
      element.setAttribute('id', id); 
      element.setAttribute('class', 'city-size-text');
      cityElement.appendChild(element);
    }
     
    element.innerHTML = city.size + 1;
  }
  
  var showCityTradeText = function(city, cityElement)
  {
    var id = 'city_'+city.id+'_tradetext';
    var element = document.getElementById(id);
     
    if (element == null)
    {
      element = document.createElement('div');
      element.setAttribute('id', id); 
      element.setAttribute('class', 'city-trade-text');
      element.innerHTML = 'Trade:'
      cityElement.appendChild(element);
    }
  }
  
  var showCityTradeSelect = function(city, cityElement)
  {
    var id = 'city_'+city.id+'_tradeselect';
    var element = document.getElementById(id);
     
    if (element == null)
    {
      var outerElement = document.createElement('div');
      
      element = document.createElement('div');
      element.setAttribute('id', id); 
      element.setAttribute('class', 'city-trade-text');
      element.innerHTML = 'Trade:'
      outerElement.appendChild(element);      
      
      element = document.createElement('select');
      element.setAttribute('id', id); 
      element.setAttribute('class', 'city-option-select');
      element.options.add(new Option('-', -1));
      for(var i = 0; i < Players.length; i++)
      {
         //if(Player != i) // Causes crash on reload on ....selected = true;
         element.options.add(new Option(Players[i].name, i));
      }
      element.options[city.traded+1].selected = true;
      element.onchange = function(){assignCityTrade(city, element);};
      outerElement.appendChild(element);
      
      cityElement.appendChild(outerElement);      
    }
  }  
  
  var showCityOwnerSelect = function(city, cityElement)
  {
    var id = 'city_'+city.id+'_ownselect';
    var element = document.getElementById(id);
     
    if (element == null)
    {
      element = document.createElement('select');
      element.setAttribute('id', id); 
      element.setAttribute('class', 'city-option-select');
      element.options.add(new Option('-', -1));
      for(var i = 0; i < Players.length; i++)
      {
         element.options.add(new Option(Players[i].name, i));
      }
      element.options[city.owner+1].selected = true;
      element.onchange = function(){assignCity(city, element);};
      cityElement.appendChild(element);
    }
  }   
  
  var showCityOwner = function(city, cityElement)
  {
    var id = 'city_'+city.id+'_own';
    var element = document.getElementById(id);
     
    if (element == null)
    {
      element = document.createElement('div');
      element.setAttribute('id', id); 
      element.setAttribute('class', 'city-owner-text');
      cityElement.appendChild(element);
    }
    element.innerHTML = 'Owner: '+Players[city.owner].name;
  }   
  
  var hideCityOwner = function(city, cityElement)
  {
    var id = 'city_'+city.id+'_own';
    var element = document.getElementById(id);
     
    if (element != null)
    {
        element.parentNode.removeChild(element);
    }    
  }
  
  var showCityBox = function(city)
  {
    var id = 'city_'+city.id;
    var element = document.getElementById(id);
     
    if (element == null)
    {
      element = document.createElement('div');
      element.setAttribute('id', id); 
      element.setAttribute('class', 'city');
      document.getElementById('cities').appendChild(element);
    }
    
    return element;
  }  
  
  var hideCityBox = function(city)
  {
    var id = 'city_'+city.id;
    var element = document.getElementById(id);
     
    if (element != null)
    {
      element.parentNode.removeChild(element);
    }
    
    return element;
  }    
  
  