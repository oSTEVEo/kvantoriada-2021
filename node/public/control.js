var basIsSet = 1;

function httpGet(theUrl, callback = () => { }) {
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
var bac = 0;
var zar = 0;

var trig = 0;

function generaleView() {
  charges = Math.round(((info.charge - 533) * 100) / (255));
  if (charges > 100) {
    charges = 100;
  } else if (charges < 0) {
    charges = 0;
  }

  if (charges < 50&&trig==0) {
    trig=1;
    var t = document.createElement('a');
    t.setAttribute("href","#zatemnenie");
    t.click()
    //window.location.href = "http://"+window.location.host + '/menu.html#zatemnenie'
  }
/*
  if (zar > 0) {
    charges += "% ϟ"
  } else {
    charges += '% &nbsp;';
  }*/
  charges += "% ϟ"

  document.getElementById("logElem").innerHTML =
    "Температура: " +
    info.term +
    "C°; Заполненость бака: " +
    bac +
    "%; Зарядка: " +
    charges +
    " ; Угол по компасу: " +
    info.angle;
  //12v - 788, 8v 533
  if (map.getLayers().getArray().length > basIsSet)
    map.getLayers().removeAt(map.getLayers().getArray().length - 1)//удалить слои

  createRound(info.lat, info.lng, 4, "red")
}

function parsData(text) {
  //parsData("qwertyuiopS10X25.563265X85.322561X1X25X90X91X180;zxcvbnm")
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

function getDataPrintCikle() {
  setInterval(() => {
    httpGet('/getback', (text) => {
      bac = text;
    })

    httpGet('/getzar', (text) => {
      zar = text;
    })

    getDataPrint()
  }, 3000);
}

function getDataPrint() {


  httpGet("/front?cmd=S");
  setTimeout(() => {
    var buff = httpGet("/read", (text) => {
      if (parsData(text) == 1);
      generaleView();
      console.log(text);
    });
  }, 1000);
}

function read() {
  document.getElementById("text").innerText = httpGet("/read");
}

function addBasePoint() {
  basIsSet = 2;
  //map.removeLayer(vectorLayer);
  map.getLayers().removeAt(map.getLayers().getArray().length - 1)
  addMapPoint(info.lat, info.lng, "base", 0)
  //addMapPoint(56.332871,44.038892,"base",0)
}

function ComeBack() {
  return;
}
