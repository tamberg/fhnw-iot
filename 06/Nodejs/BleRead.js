const noble = require("noble");

const hrmServiceUuid = "180d";
const hrmHeartRateCharacteristicUuid = "2a37";

noble.on("discover", (peripheral) => {
  //console.log(peripheral); // lots of information
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
              characteristic.on("data", (data, isNotification) =>  {
                if (isNotification) {
                  const notifValue = data.readUInt8(0);
                  console.log("notify:", notifValue);
                }
              });
              characteristic.read((error, data) =>  {
                const readValue = data.readUInt8(0);
                console.log("read:", readValue);
                characteristic.subscribe((err) =>  {
                  console.log("subscribed");
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
