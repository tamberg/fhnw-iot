const http = require("http");

//const host = "127.0.0.1"; // loopback address
//const host = "192.168.2.23"; // local address
const host = "0.0.0.0"; // any incoming address

const port = 8080;

const server = http.createServer((req, res) => {
  res.statusCode = 200;
  res.setHeader("Content-Type", "text/plain");
  res.end("It works!\n");
});

server.listen(port, host, () => {
  console.log("Server running on port " + port);
});
