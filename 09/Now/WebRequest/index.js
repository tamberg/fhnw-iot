var http = require("http");

module.exports = async (req, res) => {
  http.get("http://tmb.gr/hello.html", (res2) => {
    let data = "";

    res2.on("data", (chunk) => {
      data += chunk;
    });

    res2.on("end", () => {
      res.end(data);
    });

  }).on("error", (err) => {
      res.statusCode = 500;
      res.end(err.message);
  });
};
