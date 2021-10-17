// Scanner BLE central. Copyright (c) Thomas Amberg, FHNW

// Based on https://github.com/adafruit/Adafruit_nRF52_Arduino
// /tree/master/libraries/Bluefruit52Lib/examples/Central
// Copyright (c) Adafruit.com, all rights reserved.

// Licensed under the MIT license, see LICENSE or
// https://choosealicense.com/licenses/mit/

#include "Adafruit_TinyUSB.h"; // Fix https://github.com/adafruit/Adafruit_nRF52_Arduino/issues/653
#include <bluefruit.h>

void scanCallback(ble_gap_evt_adv_report_t* report) {
  Serial.printBufferReverse(report->peer_addr.addr, 6, ':'); // MAC address is little endian
  Serial.print(", RSSI = ");
  Serial.print(report->rssi);
  Serial.print(", data = ");
  Serial.printBuffer(report->data.p_data, report->data.len, '-');
  // See https://www.bluetooth.com/specifications/gatt/services
  BLEUuid hrmServiceUuid = BLEUuid(UUID16_SVC_HEART_RATE); // 0x180D
  if (Bluefruit.Scanner.checkReportForUuid(report, hrmServiceUuid)) {
    Serial.print(", Heart Rate Monitor");
  }
  Serial.println();
  Bluefruit.Scanner.resume(); // required for Softdevice v6
}

void setup() {
  Serial.begin(115200);
  while (!Serial) { delay(10); } // only if usb connected

  Bluefruit.begin(0, 1); // 0 peripheral, max 1 central connection, save SoftDevice SRAM
  Bluefruit.setTxPower(4); // -40, -30, -20, -16, -12, -8, -4, 0, 4
  Bluefruit.setName("nRF52840");
  Bluefruit.setConnLedInterval(250);
  Bluefruit.Scanner.setRxCallback(scanCallback);
  Bluefruit.Scanner.start(0);
  Serial.println("Scanning ...");
}

void loop() {}
