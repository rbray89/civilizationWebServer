	var showContainer = function(id)
	{
		var nodeList = document.querySelectorAll(".container");
		  for (var i = 0, length = nodeList.length; i < length; i++) {
			 nodeList[i].style.display = "none";
		  }
		  var element = document.getElementById(id);
		element.style.display = null;
	};
	
  var button_up = function(element)
  {
	element.className = element.className.replace("button-down","button-up");
  };
  var button_down = function(element)
  {
	element.className = element.className.replace("button-up","button-down");
  };
  
  var cancelClick = function(){
	resetTech();
  };
	
  var setupButton = function(element){
	var onclickOrig = element.onclick;
	 element.onclick = function(){
			button_up(this);
			eval(onclickOrig);
		};
	var onmousedownOrig = element.onmousedown;
	 element.onmousedown = function(){
			button_down(this);
			eval(onmousedownOrig);
		};
  };
  
  var setupPage = function(){
	var nodeList = document.querySelectorAll(".button-up");
		  for (var i = 0, length = nodeList.length; i < length; i++) {
			 setupButton(nodeList[i]);
		  }
  };