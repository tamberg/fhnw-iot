const noble = require("@abandonware/noble");

const hrmServiceUuid = "180d";
const hrmHeartRateCharacteristicUuid = "2a37";
const serviceUuids = [hrmServiceUuid];
const characteristicUuids = [hrmHeartRateCharacteristicUuid];

noble.on("discover", (peripheral) => {
  noble.stopScanning();
  console.log(
    peripheral.address + ", " +
    peripheral.advertisement.localName);
  peripheral.connect((err) => {
    console.log("connected");
    peripheral.discoverServices(serviceUuids, (err, services) => {
      services.forEach((service) => {
        console.log("found service:", service.uuid);
        service.discoverCharacteristics(characteristicUuids, (err, characteristics) => {
          characteristics.forEach((characteristic) => {
            console.log("found characteristic:", characteristic.uuid);
            characteristic.on("data", (data, isNotification) => {
              if (isNotification) {
                const notifValue = data.readUInt8(1);
                console.log("notified:", notifValue);
              }
            });
            characteristic.subscribe((err) => {
              console.log("subscribed");
            });
          });
        });
      });
    });
  });
});

console.log("scanning...");
noble.startScanning(serviceUuids, false);
