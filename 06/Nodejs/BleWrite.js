const noble = require("@abandonware/noble");

const hrmServiceUuid = "180d";
const hrmControlPointCharacteristicUuid = "2a39";
const serviceUuids = [hrmServiceUuid];
const characteristicUuids = [hrmControlPointCharacteristicUuid];

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
            const data = Buffer.alloc(1);
            data.writeUInt8(0x01, 0);
            console.log("writing data: ", data);
            characteristic.write(data, false, function (err) {
              peripheral.disconnect((err) => {
                console.log("disconnected");
                process.exit();
              });
            });
          });
        });
      });
    });
  });
});

console.log("scanning...");
noble.startScanning(serviceUuids, false);
