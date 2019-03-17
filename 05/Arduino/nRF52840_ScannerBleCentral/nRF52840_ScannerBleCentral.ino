// Heart Rate Monitor BLE peripheral. Copyright (c) Thomas Amberg, FHNW

// Based on https://github.com/adafruit/Adafruit_nRF52_Arduino
// /tree/master/libraries/Bluefruit52Lib/examples/Central
// Copyright (c) Adafruit.com, all rights reserved.

// Licensed under the MIT license, see LICENSE or
// https://choosealicense.com/licenses/mit/

#include <bluefruit.h>

void scanCallback(ble_gap_evt_adv_report_t* report) {
  Serial.printBufferReverse(report->peer_addr.addr, 6, ':'); // MAC address is little endian
  Serial.print(", RSSI = ");
  Serial.print(report->rssi);
  Serial.print(", data = ");
  Serial.printBuffer(report->data.p_data, report->data.len, '-');
  if (Bluefruit.Scanner.checkReportForUuid(report, BLEUART_UUID_SERVICE)) {
    Serial.print(", ");
    Serial.println("BLEUART_UUID_SERVICE");
  }
  Serial.println();
  Bluefruit.Scanner.resume(); // required for Softdevice v6
}

void setup() {
  Serial.begin(115200);
  while ( !Serial ) delay(10); // only if usb connected

  Bluefruit.begin(0, 1); // 0 peripheral, max 1 central connection, save SoftDevice SRAM
  Bluefruit.setTxPower(4); // -40, -30, -20, -16, -12, -8, -4, 0, 4
  Bluefruit.setName("nRF52840");
  Bluefruit.setConnLedInterval(250);
  Bluefruit.Scanner.setRxCallback(scanCallback);
  Bluefruit.Scanner.start(0);
  Serial.println("Scanning ...");
}

void loop() {}
