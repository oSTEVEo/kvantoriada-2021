function httpGet(theUrl) {
  var xmlHttp = new XMLHttpRequest();
  xmlHttp.open("GET", theUrl, false); // false for synchronous request
  xmlHttp.send(null);
  return xmlHttp.responseText;
}

function front() {
  httpGet("/front?cmd=" + "A");
}

function read() {
  document.getElementById("text").innerText = httpGet("/read");
}
