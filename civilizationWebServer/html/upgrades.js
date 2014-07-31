  var Upgrades;

  var purchaseUpgrade = function(upgradeObj)
  {
	if (confirm("You are purchasing "+upgradeObj.name+"."))
	{
	  send_cmd('purchase_upgrade', upgradeObj.id);
	}
  }
  
  var deprecateUpgrade = function(upgradeObj)
  {
	if (confirm("You are depricating "+upgradeObj.name+"."))
	{
	  send_cmd('deprecate_upgrade', -1, upgradeObj.id);
	}  
  }
  
  
  var updateUpgrade = function(upgradeObj) {
	var upgrade = document.getElementById('upgrade_'+upgradeObj.id);
	if(upgrade == null)
	{        
		upgrade = document.createElement('div');
		upgrade.setAttribute('id', 'upgrade_'+upgradeObj.id);
		upgradeElement = document.createElement('div');
		upgradeElement.setAttribute('class', 'upgrade-text-name');
		upgradeElement.innerHTML = upgradeObj.name;
		upgrade.appendChild(upgradeElement);
        
		//benefits
        for(var i = 0; i <= upgradeObj.type/2; i++)
        {
            upgradeElement = document.createElement('img');
            upgradeElement.setAttribute('class', 'upgrade-img-benefits');
            if (upgradeObj.type % 2 == 0)
            {
                upgradeElement.src="happy16.png";
            }
            else
            {
                upgradeElement.src="productive16.png";
            }
            upgrade.appendChild(upgradeElement);
        }
     
		upgradeElement = document.createElement('div');
		upgradeElement.setAttribute('class', 'upgrade-purchase');
		upgradeElement.setAttribute('id', 'upgrade_purchase_'+upgradeObj.id);
		upgradeElement.onclick=function(event){purchaseUpgrade(upgradeObj);event.stopPropagation();};
		upgradeElement.innerHTML = "<span class='span18'></span>Purchase";
		upgradeElement.style.display = 'none';
		upgrade.appendChild(upgradeElement);
        
        upgradeElement = document.createElement('div');
        upgradeElement.setAttribute('class', 'upgrade-deprecate');
        upgradeElement.setAttribute('id', 'upgrade_deprecate_'+upgradeObj.id);
        upgradeElement.onclick=function(event){deprecateUpgrade(upgradeObj);event.stopPropagation();};
        upgradeElement.innerHTML = "<span class='span18'></span>Deprecate";
        upgradeElement.style.display = 'none';
        upgrade.appendChild(upgradeElement);           
        
		upgradeElement = document.createElement('div');
		upgradeElement.setAttribute('class', 'tech-owner');
		upgradeElement.setAttribute('id', 'upgrade_unlockedby_'+upgradeObj.id);
		upgradeElement.innerHTML = "";
		upgrade.appendChild(upgradeElement);        
        
        document.getElementById('upgrades').appendChild(upgrade);        
	}
    
    var deprecateButton = document.getElementById('upgrade_deprecate_'+upgradeObj.id);
    var purchaseButton = document.getElementById('upgrade_purchase_'+upgradeObj.id);
    var unlockedBy = document.getElementById('upgrade_unlockedby_'+upgradeObj.id);
    purchaseButton.style.display = 'none';
    unlockedBy.innerHTML = upgradeObj.unlockedby;
    


    if (Player == null && !upgradeObj.deprecated)
    {
        deprecateButton.style.display = 'block'; 
    }
    else
    {
        deprecateButton.style.display = 'none'; 
    }
    
    if (upgradeObj.deprecated)
    {
        upgrade.setAttribute('class', 'upgrade-deprecated'); // TODO fix this spelling everywhere at some point...
    }
    else if (upgradeObj['purchased_'+Player])
    {
        upgrade.setAttribute('class', 'upgrade-purchased');
    }
    else if (upgradeObj.unlocked)
    {
        upgrade.setAttribute('class', 'upgrade-unlocked');
        if (Player != null && Player != -1)
        {
            purchaseButton.style.display = 'block';         
        }
    }
    else
    {
        upgrade.setAttribute('class', 'upgrade-locked');        
    }
    
  };  
  
  var updateUpgrades = function(obj) {
	Upgrades = obj;
	obj.forEach(function(entry) {
		updateUpgrade(entry);
	});
  };
 
  