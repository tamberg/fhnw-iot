const noble = require("noble");

const hrmServiceUuid = "180d";
const hrmHeartRateCharacteristicUuid = "2a37";

noble.on("discover", (peripheral) => {
  noble.stopScanning();
  console.log(
    peripheral.address + ", " +
    peripheral.advertisement.localName);
  peripheral.connect((err) => {
    console.log("connected");
    peripheral.discoverServices([hrmServiceUuid], (err, services) =>  {
      services.forEach((service) =>  {
        console.log("found service:", service.uuid);
        service.discoverCharacteristics([], (err, characteristics) =>  {
          characteristics.forEach((characteristic) =>  {
            console.log("found characteristic:", characteristic.uuid);
            if (hrmHeartRateCharacteristicUuid == characteristic.uuid) {
              characteristic.read((error, data) =>  {
                const value = data.readUInt8(0);
                console.log("read characteristic value:", value);
                peripheral.disconnect((err) => {
                  console.log("disconnected");
                });
              });
            }
          });
        });
      });
    });
  });
});

const serviceUuids = [hrmServiceUuid];

console.log("scanning...");
noble.startScanning(serviceUuids, false);
