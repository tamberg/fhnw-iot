// Apple iBeacon BLE observable. Copyright (c) Thomas Amberg, FHNW

// Based on https://github.com/adafruit/Adafruit_nRF52_Arduino
// /tree/master/libraries/Bluefruit52Lib/examples/Peripheral
// Copyright (c) Adafruit.com, all rights reserved.

// Licensed under the MIT license, see LICENSE or
// https://choosealicense.com/licenses/mit/

#include "Adafruit_TinyUSB.h" // Fix https://github.com/adafruit/Adafruit_nRF52_Arduino/issues/653
#include <bluefruit.h>

// See https://www.bluetooth.com/specifications/assigned-numbers/company-identifiers
#define MANUFACTURER_ID 0x004C // Apple iBeacon

// AirLocate UUID: E2C56DB5-DFFB-48D2-B060-D0F5A71096E0
const uint8_t beaconUuid[16] = { // lsb first
  0xE0, 0x96, 0x10, 0xA7, 0xF5, 0xD0, 0x60, 0xB0,
  0xD2, 0x48, 0xFB, 0xDF, 0xB5, 0x6D, 0xC5, 0xE2 
};

const uint16_t beaconMajorVersion = 0x0001; // big endian
const uint16_t beaconMinorVersion = 0x0000; // big endian
const int8_t rssiAtOneMeter = -54;

BLEBeacon beacon(
  beaconUuid, 
  beaconMajorVersion, 
  beaconMinorVersion, 
  rssiAtOneMeter);

void startAdvertising() {
  Bluefruit.Advertising.setBeacon(beacon);
  Bluefruit.ScanResponse.addName(); // optional
  Bluefruit.Advertising.restartOnDisconnect(true);
  Bluefruit.Advertising.setInterval(160, 160); // * 0.625 ms
  Bluefruit.Advertising.setFastTimeout(30); // seconds in fast mode
  Bluefruit.Advertising.start(0); // advertise until connected
  Serial.println("Advertising...");
}

void setup() {
  Serial.begin(115200);
//  while (!Serial) { delay(10); } // only if usb connected

  Bluefruit.begin();
  Bluefruit.autoConnLed(false); // save power
  Bluefruit.setTxPower(0); // -40, -30, -20, -16, -12, -8, -4, 0, 4
  Bluefruit.setName("nRF52840");

  beacon.setManufacturer(MANUFACTURER_ID); // required for Manufacturer Specific Data
  startAdvertising();
  suspendLoop(); // save power
}

void loop() {} // suspended
