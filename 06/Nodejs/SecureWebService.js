// $ curl -v https://127.0.0.1:4443/ --insecure

const fs = require("fs"),
  https = require("https");

//const host = "127.0.0.1"; // loopback address
//const host = "192.168.2.23"; // local address
const host = "0.0.0.0"; // any incoming address

//const port = 443; // requires $ sudo ...
const port = 4443;

const options = {
  // TODO: create a self-signed certificate in the same directory as this .js source code
  // $ openssl req -x509 -nodes -newkey rsa:2048 -keyout key.pem -out cert.pem -days 4096
  key: fs.readFileSync("./key.pem"),
  cert: fs.readFileSync("./cert.pem"),
}

const server = https.createServer(options, (req, res) => {
  res.statusCode = 200;
  res.setHeader("Content-Type", "text/plain");
  res.end("It works!\n");
});

server.listen(port, host, () => {
  console.log("Server running on port " + port);
});
