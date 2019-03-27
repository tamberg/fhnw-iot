const noble = require("@abandonware/noble");

noble.on("discover", (peripheral) => {
  //console.log(peripheral); // lots of information
  console.log(
    peripheral.address + ", " +
    peripheral.advertisement.localName);
});

const serviceUuids = []; // or ["180d"]; // HRM only

console.log("scanning...");
noble.startScanning(serviceUuids, false);
