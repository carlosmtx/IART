function drawAll(canvasName,divID){
	var red;
	var green;
	var blue;
	var scaleFactor;
	function getSortedData(divID){
		var json = document.getElementById(divID).innerHTML;
		var data = JSON.parse(json);
		data.sort(function(a,b){return a.hAterragem - b.hAterragem;});
		console.log(data);
		return data;
	}

	function draw(i,data,canvasName,divID){
		
		var c=document.getElementById(canvasName);
		var ctx=c.getContext('2d');
		ctx.fillStyle= "rgba("+red.toString()+","+green.toString()+","+blue.toString()+",0.5)";
		
		red += 100;
		green = green + red % 256;
		blue  = blue  + green % 256;

		red = red%256;
		green=green%256;
		blue = blue%256; 		


		
		ctx.fillRect(data[i].hAterragem*scaleFactor,60,data[i].tempoOcup*scaleFactor,30);
		ctx.fillStyle= "rgba(0,0,0,1)";
		

		while( ctx.measureText(data[i].nome).width > (data[i].tempoOcup*scaleFactor) ){
			data[i].nome = data[i].nome.substr(0,data[i].nome.length-1);
		}


		ctx.font="10px Georgia";
		ctx.fillText(data[i].nome,data[i].hAterragem*scaleFactor,50);
		ctx.fillText(data[i].intervInf.toString()+" - "+data[i].intervSup.toString(),data[i].hAterragem*scaleFactor,80);


		if ( i+1 < data.length){
			{setTimeout(function(){draw(i+1,data,canvasName,divID);},1000);}
		}
	}
	
	var data = getSortedData(divID);
	red = 0;
	green = 0;
	blue = 0;

	
	var MAXSIZE = 1500;
	var MAXTIME = data[data.length-1].hAterragem+data[data.length-1].tempoOcup

	scaleFactor = MAXSIZE / MAXTIME;
	
	var c=document.getElementById(canvasName);
	console.log(c);
	var ctx=c.getContext('2d');
	ctx.fillStyle= "rgba(0,0,0,0.1)";
	ctx.fillRect(0,90,MAXTIME*scaleFactor,30);
	ctx.fillStyle= "rgba(0,0,0,1)";		
	ctx.font="10px Georgia";
	if(canvasName == "genCanvas") ctx.fillText("Algoritmo Genetico",0,30);
	if(canvasName == "saCanvas") ctx.fillText("Algoritmo Arrefecimento Simulado",0,30);
	if(canvasName == "bnbCanvas") ctx.fillText("Algoritmo Custo Uniforme",0,30);
	for ( var i = 0 ; i < MAXTIME;i+=2){
		ctx.fillStyle= "rgba(0,0,0,0.1)";

		ctx.fillRect(i*scaleFactor,90,scaleFactor,30);
		ctx.fillStyle= "rgba(0,0,0,1)";		
		ctx.font="10px Georgia";
		ctx.fillText(i.toString(),i*scaleFactor,105);
	}


	draw(0,data,canvasName,divID);
	
}


