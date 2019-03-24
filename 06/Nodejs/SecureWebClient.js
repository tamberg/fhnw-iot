const https = require("https"),
  qs = require("querystring");

const reqData = qs.stringify({
  "value": 42,
  "date_time": "2019-03-05T00:00:00Z"
});

const options = {
  hostname: "postb.in",
  path: "/MY_POSTBIN_ID", // TODO, see https://postb.in/
  method: "POST",
  port: 443,
  headers: {
    "Content-Type": "application/x-www-form-urlencoded",
    "Content-Length": Buffer.byteLength(reqData)
  }
};

const req = https.request(options, (res) => {
  let resData = "";

  res.on("data", (chunk) => {
    resData += chunk;
  });

  res.on("end", () => {
    console.log(resData);
  });
});

req.on("error", (err) => {
  console.log("Error: " + err.message);
});

req.write(reqData);
req.end();
