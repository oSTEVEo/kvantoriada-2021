const express = require("express");
const app = express();
const httpPort = 3000;

const serialPortL = require("serialport");

let sPort;
let buf = "";

function getParameterByName(name, url) {
  name = name.replace(/[\[\]]/g, "\\$&");
  var regex = new RegExp("[?&]" + name + "(=([^&#]*)|&|#|$)"),
    results = regex.exec(url);
  if (!results) return null;
  if (!results[2]) return "";
  return decodeURIComponent(results[2].replace(/\+/g, " "));
}

serialPortL.list().then(function (ports) {
  ports.forEach(function (port) {
    if (port.manufacturer == "FTDI") {
      const por = new serialPortL(port.path);
      sPort = por;

      sPort.on("error", function (err) {
        console.log("Error: ", err.message);
      });

      sPort.on("data", function (data) {
        buf = buf + data;
        console.log(data);
      });
    }
  });
});

app.use(express.static("public"));

app.get("/front", (req, res) => {
  res.send("OK");

  let a = getParameterByName("cmd", req.url);

  sPort.write(a, function (err) {
    if (err) {
      return console.log("Error on write: ", err.message);
    }
    console.log("message written");
  });
  console.log("front");
});

app.get("/read", (req, res) => {
  console.log("Data:", buf);
  res.send(buf);
  buf = "";
});

app.listen(httpPort, () => {
  console.log(`App at http://localhost:${httpPort}`);
});
