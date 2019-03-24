const noble = require("noble");

noble.on("discover", function(peripheral) {
  //console.log(peripheral); // lots of information
  console.log(
    peripheral.address + ", " +
    peripheral.advertisement.localName);
});

noble.on("stateChange", function(state) {
  console.log("state changed to: ", state);
  if ("poweredOn" == state) {
    console.log("scanning...");
    //noble.startScanning([], true); // all services
    noble.startScanning(["180d"], false); // HRM service only
  } else {
    noble.stopScanning();
  }
});
