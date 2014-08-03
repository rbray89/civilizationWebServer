  var Units; 
  
  var UnitCategories = ["infantry", "cavalry", "artillery", "fleet", "aircraft"];
  
  var updateUnit = function(unitObj) {
	var unit = document.getElementById('unit_'+unitObj.id);
	if(unit == null)
	{        
		unit = document.createElement('div');
		unit.setAttribute('id', 'unit_'+unitObj.id);
		unitElement = document.createElement('div');
		unitElement.setAttribute('class', 'unit-text-name');
		unitElement.innerHTML = unitObj.name;
		unit.appendChild(unitElement);
        
        unitElement = document.createElement('img');
        unitElement.setAttribute('class', 'unit-img-category'); 
        unitElement.src=UnitCategories[unitObj.category]+"16.png";  
        unit.appendChild(unitElement);                 
        
		unitElement = document.createElement('div');
		unitElement.setAttribute('class', 'tech-owner');
		unitElement.setAttribute('id', 'unit_unlockedby_'+unitObj.id);
		unitElement.innerHTML = "";
		unit.appendChild(unitElement);       

		unitElement = document.createElement('div');
		unitElement.setAttribute('class', 'unit_die_roll');
		unitElement.innerHTML = unitObj.dieRolled+"d";
        if (unitObj.dieBonus != 0)
        {
           unitElement.innerHTML += " + "+unitObj.dieBonus;
        }
		unit.appendChild(unitElement);          
        
        document.getElementById(EraIds[unitObj.era]+"-unit").appendChild(unit);
	}
    
    var unlockedBy = document.getElementById('unit_unlockedby_'+unitObj.id);
    unlockedBy.innerHTML = unitObj.unlockedby;
       
    if (unitObj.unlocked)
    {
        unit.setAttribute('class', 'unit-unlocked');
    }
    else
    {
        unit.setAttribute('class', 'unit-locked');        
    }
    
  };  
  
  var updateUnits = function(obj) {
	Units = obj;
	obj.forEach(function(entry) {
		updateUnit(entry);
	});
  };
 
  