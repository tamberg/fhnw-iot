// Scanner BLE central. Copyright (c) Thomas Amberg, FHNW

// Based on https://github.com/adafruit/Adafruit_nRF52_Arduino
// /tree/master/libraries/Bluefruit52Lib/examples/Central
// Copyright (c) Adafruit.com, all rights reserved.

// Licensed under the MIT license, see LICENSE or
// https://choosealicense.com/licenses/mit/

#include "Adafruit_TinyUSB.h"; // Fix https://github.com/adafruit/Adafruit_nRF52_Arduino/issues/653
#include <bluefruit.h>

// Central takes client role, use BLEClient... types:
BLEClientService heartRateMonitorService(UUID16_SVC_HEART_RATE);
BLEClientCharacteristic heartRateMeasurementCharacteristic(UUID16_CHR_HEART_RATE_MEASUREMENT);
BLEClientCharacteristic bodySensorLocationCharacteristic(UUID16_CHR_BODY_SENSOR_LOCATION);

void scanCallback(ble_gap_evt_adv_report_t* report) {
  Bluefruit.Central.connect(report);
}

void connectCallback(uint16_t connHandle) {
  Serial.println("Connected");
  Serial.print("HRM service ... ");
  if (!heartRateMonitorService.discover(connHandle)) {
    Serial.println("not found.");
    Bluefruit.disconnect(connHandle);
    return;
  }
  Serial.println("found.");
  
  Serial.print("Heart Rate Measurement characteristic ... ");
  if (!heartRateMeasurementCharacteristic.discover()) {
    Serial.println("not found.");
    Bluefruit.disconnect(connHandle);
    return;
  }
  Serial.println("found.");

  Serial.print("Body Sensor Location characteristic ... ");
  if (bodySensorLocationCharacteristic.discover()) { // optional
    Serial.print("found: ");
    const char* locationNames[] = { "Other", "Chest", "Wrist", "Finger", "Hand", "Ear Lobe", "Foot" };
    uint8_t location = bodySensorLocationCharacteristic.read8();   
    Serial.println(locationNames[location]);
  } else {
    Serial.println("not found.");
  }

  Serial.print("Heart Rate Measurement characteristic notifications ... ");
  if (heartRateMeasurementCharacteristic.enableNotify()) {
    Serial.println("enabled.");
  } else {
    Serial.println("failed.");
  }
}

void disconnectCallback(uint16_t connHandle, uint8_t reason) {
  Serial.print("Disconnected, reason = ");
  Serial.println(reason);
}

void hrmNotifyCallback(BLEClientCharacteristic* chr, uint8_t* data, uint16_t len) {
  Serial.print("Heart Rate Measurement: ");
  if (data[0] & 0x01) { // 16-bit measurement
    uint16_t value;
    memcpy(&value, &data[1], 2);
    Serial.println(value);
  } else { // 8-bit measurement
    Serial.println(data[1]);
  }
}

void setup() {
  Serial.begin(115200);
  while (!Serial) { delay(10); } // only if usb connected
  Serial.println("Setup");

  Bluefruit.begin(0, 1); // 0 peripheral, max 1 central connection, save SoftDevice SRAM
  Bluefruit.setName("nRF52840");
  Bluefruit.setConnLedInterval(250);

  heartRateMonitorService.begin(); // sequence matters
  bodySensorLocationCharacteristic.begin(); // added to heartRateMonitorService
  heartRateMeasurementCharacteristic.setNotifyCallback(hrmNotifyCallback);
  heartRateMeasurementCharacteristic.begin(); // added to heartRateMonitorService

  Bluefruit.Central.setConnectCallback(connectCallback);
  Bluefruit.Central.setDisconnectCallback(disconnectCallback);

  Bluefruit.Scanner.setRxCallback(scanCallback);
  Bluefruit.Scanner.restartOnDisconnect(true);
  Bluefruit.Scanner.setInterval(160, 80); // in unit of 0.625 ms
  Bluefruit.Scanner.filterUuid(heartRateMonitorService.uuid);
  Bluefruit.Scanner.useActiveScan(false);
  Bluefruit.Scanner.start(0); // non-stop
}

void loop() {
  // skip
}
