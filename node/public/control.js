function httpGet(theUrl, callback = () => {}) {
  var xmlHttp = new XMLHttpRequest();
  xmlHttp.open("GET", theUrl, true); // false for synchronous request
  xmlHttp.onreadystatechange = function () {
    if (xmlHttp.readyState == 4 && xmlHttp.status == 200)
      callback(xmlHttp.responseText);
  };
  xmlHttp.send(null);
  return xmlHttp.responseText;
}

var info = {
  dist: 0,
  lng: 0,
  lat: 0,
  sost: 0,
  term: 0,
  fill: 0,
  charge: 0,
  angle: 0,
};

function generaleView(){
  document.getElementById('logElem').innerHTML="Температура="+ info.term+"  Заполненость бака="+ info.fill+" Зарядка="+ info.charge + "Угол по компасу="+info.angle
  //12v - 788, 533 

}

function parsData(text) {//parsData("qwertyuiopS10X25.563265X85.322561X1X25X90X91X180;zxcvbnm")
  var i = 0;
  while (text[i] != "S" && i < text.length) {
    i++;
  }
  if (i == text.length) return 0;
  text = text.substring(i + 1, text.length);
  var i = 0;
  while (text[i] != ";" && i < text.length) {
    i++;
  }
  if (i == text.length) return 0;
  text = text.substring(0, i);
  text = text.split("X");

  key = Object.keys(info);

  for (var i = 0; i < text.length; i++) {
    info[key[i]] = text[i];
  }

  console.log(info);
  return 1;
}

function getDataPrint() {
  httpGet("/front?cmd=S");
  setTimeout(() => {
    var buff = httpGet("/read", (text) => {
      if( parsData(text)==1);
        generaleView();
      console.log(text);
    });
  }, 1000);
}

function read() {
  document.getElementById("text").innerText = httpGet("/read");
}
