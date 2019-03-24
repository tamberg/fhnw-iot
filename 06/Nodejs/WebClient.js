const http = require("http");

http.get("http://tmb.gr/hello.html", (resp) => {
  let data = "";

  resp.on("data", (chunk) => {
    data += chunk;
  });

  resp.on("end", () => {
    console.log(data);
  });

}).on("error", (err) => {
  console.log("Error: " + err.message);
});
