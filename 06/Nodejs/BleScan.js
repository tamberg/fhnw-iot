const noble = require("noble");

noble.on("discover", function(peripheral) {
  //console.log("found peripheral:", peripheral);
  console.log("found peripheral:", peripheral.advertisement.localName);
});

noble.startScanning(["180d"], true);
