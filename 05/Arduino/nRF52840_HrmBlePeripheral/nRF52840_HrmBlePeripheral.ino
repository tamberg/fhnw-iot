// Heart Rate Monitor BLE peripheral. Copyright (c) Thomas Amberg, FHNW

// Based on https://github.com/adafruit/Adafruit_nRF52_Arduino
// /tree/master/libraries/Bluefruit52Lib/examples/Peripheral
// Copyright (c) Adafruit.com, all rights reserved.

// Licensed under the MIT license, see LICENSE or
// https://choosealicense.com/licenses/mit/

#include <bluefruit.h>

BLEDis deviceInfoService;
BLEBas batteryService;

BLEService heartRateMonitorService = BLEService(UUID16_SVC_HEART_RATE); // 0x180D
BLECharacteristic heartRateMeasurementCharacteristic = BLECharacteristic(UUID16_CHR_HEART_RATE_MEASUREMENT); // 0x2A37
BLECharacteristic bodySensorLocationCharacteristic = BLECharacteristic(UUID16_CHR_BODY_SENSOR_LOCATION); // 0x2A38

void connectedCallback(uint16_t connectionHandle) {
  char centralName[32] = { 0 };
  Bluefruit.Gap.getPeerName(connectionHandle, centralName, sizeof(centralName));
  Serial.print(connectionHandle);
  Serial.print(", connected to ");
  Serial.print(centralName);
  Serial.println();
}

void disconnectedCallback(uint16_t connectionHandle, uint8_t reason) {
  Serial.print(connectionHandle);
  Serial.print(" disconnected, reason = ");
  Serial.println(reason); // see https://github.com/adafruit/Adafruit_nRF52_Arduino
  // /blob/master/cores/nRF5/nordic/softdevice/s140_nrf52_6.1.1_API/include/ble_hci.h
  Serial.println("Advertising ...");
}

void cccdCallback(BLECharacteristic& characteristic, uint16_t cccdValue) {
  if (characteristic.uuid == heartRateMeasurementCharacteristic.uuid) {
    Serial.print("Heart Rate Measurement 'Notify', ");
    if (characteristic.notifyEnabled()) {
      Serial.println("enabled");
    } else {
      Serial.println("disabled");
    }
  }
}

void setupHeartRateMonitorService() {
  // See https://www.bluetooth.com/specifications/gatt/viewer?attributeXmlFile=org.bluetooth.service.heart_rate.xml
  heartRateMonitorService.begin(); // Must be called before calling .begin() on its characteristics

  // See https://www.bluetooth.com/specifications/gatt/viewer?attributeXmlFile=org.bluetooth.characteristic.heart_rate_measurement.xml
  heartRateMeasurementCharacteristic.setProperties(CHR_PROPS_NOTIFY); // 0x2A37
  heartRateMeasurementCharacteristic.setPermission(SECMODE_OPEN, SECMODE_NO_ACCESS);
  heartRateMeasurementCharacteristic.setFixedLen(2);
  heartRateMeasurementCharacteristic.setCccdWriteCallback(cccdCallback);  // Optionally capture CCCD updates
  heartRateMeasurementCharacteristic.begin();

  uint8_t hrmData[2] = { 0b00000110, 0x40 }; // Use 8-bit values, sensor connected and detected
  heartRateMeasurementCharacteristic.notify(hrmData, 2); // Use .notify instead of .write

  // See https://www.bluetooth.com/specifications/gatt/viewer?attributeXmlFile=org.bluetooth.characteristic.body_sensor_location.xml
  bodySensorLocationCharacteristic.setProperties(CHR_PROPS_READ); // 0x2A38
  bodySensorLocationCharacteristic.setPermission(SECMODE_OPEN, SECMODE_NO_ACCESS);
  bodySensorLocationCharacteristic.setFixedLen(1);
  bodySensorLocationCharacteristic.begin();
  bodySensorLocationCharacteristic.write8(0); // Sensor location 'Other'
}

void startAdvertising() {
  Bluefruit.Advertising.addFlags(BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE);
  Bluefruit.Advertising.addTxPower();
  Bluefruit.Advertising.addService(heartRateMonitorService);
  Bluefruit.Advertising.addName();

  // See https://developer.apple.com/library/content/qa/qa1931/_index.html   
  const int fastModeInterval = 32; // * 0.625 ms = 20 ms
  const int slowModeInterval = 244; // * 0.625 ms = 152.5 ms
  const int fastModeTimeout = 30; // s
  Bluefruit.Advertising.restartOnDisconnect(true);
  Bluefruit.Advertising.setInterval(fastModeInterval, slowModeInterval);
  Bluefruit.Advertising.setFastTimeout(fastModeTimeout);
  // 0 = continue advertising after fast mode, until connected
  Bluefruit.Advertising.start(0);
  Serial.println("Advertising ...");
}

void setup() {
  Serial.begin(115200);
  while (!Serial) { delay(10); } // only if usb connected
  Serial.println("Setup");

  Bluefruit.begin();
  Bluefruit.setName("nRF52840");
  Bluefruit.setConnectCallback(connectedCallback);
  Bluefruit.setDisconnectCallback(disconnectedCallback);

  deviceInfoService.setManufacturer("Adafruit Industries");
  deviceInfoService.setModel("Feather nRF52840 Express");
  deviceInfoService.begin();

  batteryService.begin();
  batteryService.write(100); // %

  setupHeartRateMonitorService();
  startAdvertising();
}

void loop() {
  if (Bluefruit.connected()) {
    int value = analogRead(A0);
    uint8_t hrmData[2] = { 0b00000110, value };
    if (heartRateMeasurementCharacteristic.notify(hrmData, sizeof(hrmData))) {
      Serial.print("Heart rate = ");
      Serial.println(value);
    } else {
      Serial.println("Notify not set, or not connected");
    }
  }
  delay(1000); // ms
}
