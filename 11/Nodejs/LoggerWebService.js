// http://tamberg.mit-license.org/

var http = require('http');
var requestListener = function (req, res) {
  var body = "";
  req.on('data', function (chunk) {
    body += chunk;
  });
  req.on('end', function () {
    console.log(req.method + ' ' + req.url);
    console.log(req.headers);
    console.log(body);
    res.writeHead(200);
    res.end('200 OK');
  });
}

var server = http.createServer(requestListener);
server.listen(8080);
