
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
  
  var updateCity= function(city)
  {
	var cityElement = document.getElementById('city_'+city.id);
	if(cityElement == null)
	{
		cityELement = document.createElement('div');
		cityELement.setAttribute('id', 'city_'+city.id);
		cityELement.setAttribute('class', 'city');
		document.getElementById('cities').appendChild(cityELement);
		if(!MANAGER)
		{
			//<img class="city-production-ico" src="coin24.png"></img>
			var cityProductionIcon = document.createElement('img');
			cityProductionIcon.src='coins24.png';
			cityProductionIcon.className='city-production-ico';
			cityELement.appendChild(cityProductionIcon);
			//	<div class="city-production-text">20</div>
			var cityProductionText = document.createElement('div');
			cityProductionText.className='city-production-text';
			cityProductionText.innerHTML = city.production;
			cityELement.appendChild(cityProductionText);
			//	<img class="city-happy" src="happy24.png"></img>
			if(city.isHappy)
			{
				var cityHappy = document.createElement('img');
				cityHappy.className='city-happy';
				cityHappy.src='happy24.png';
				cityELement.appendChild(cityHappy);
			}
			//	<img class="city-productive" src="productive24.png"></img>
			if(city.isProductive)
			{
				var cityProductive = document.createElement('img');
				cityProductive.className='city-productive';
				cityProductive.src='productive24.png';
				cityELement.appendChild(cityProductive);
			}
			//<div class="city-resource-center">
			//		<img class='city-resource-ico' src="wine24.png"></img>
			//		<div class='city-resource-text span24'>Wine</div>
			//	</div>
			
			var cityResource = document.createElement('div');
			cityResource.className='city-resource-center';
			var cityResourceIcon = document.createElement('img');
			cityResourceIcon.className='city-resource-ico';
			cityResourceIcon.src=ResourcesID[city.resource]+'24.png';
			cityResource.appendChild(cityResourceIcon);
			var cityResourceText = document.createElement('div');
			cityResourceText.className='city-resource-text span24';
			if(city.resource > 0)
			{
				cityResourceText.innerHTML += Resources[city.resource];
			}
			cityResource.appendChild(cityResourceText);
			cityELement.appendChild(cityResource);
			
			//<img class="city-production-ico" src="coin24.png"></img>
			var citySizeIcon = document.createElement('img');
			citySizeIcon.src='size24.png';
			citySizeIcon.className='city-size-ico';
			cityELement.appendChild(citySizeIcon);
			//	<div class="city-production-text">20</div>
			var citySizeText = document.createElement('div');
			citySizeText.className='city-size-text';
			citySizeText.innerHTML = city.size + 1;
			cityELement.appendChild(citySizeText);
			
			//	<div class="city-option-select">
			//		Trade To:
			//	</div>
			var cityTradeText = document.createElement('div');
			cityTradeText.className='city-trade-text';
			cityTradeText.innerHTML = 'Trade:';
			cityELement.appendChild(cityTradeText);
			//	<select class="city-option-select">
			//		<option value="0">-</option>
			//		<option value="1">Ryan</option>
			//		<option value="2">Jonothan</option>
			//	</select>
			var cityTradeSelect = document.createElement('select');
			cityTradeSelect.className='city-option-select';
			cityTradeSelect.options.add(new Option('-', 0));
			for(var i = 0; i < Players.length; i++)
			{
				if(Player != i)
				{
					cityTradeSelect.options.add(new Option(Players[i].name, 1+i));
				}
			}
			cityTradeSelect.onchange = function(){assignTrade(cityTradeSelect);};
			cityELement.appendChild(cityTradeSelect);
			
		}
	}
  }
  
  var updateCities = function(obj) {
	Cities = obj;
	obj.forEach(function(entry) {
		updateCity(entry);
	});
  };
  
  