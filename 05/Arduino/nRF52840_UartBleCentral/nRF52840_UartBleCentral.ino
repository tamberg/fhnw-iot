// Nordic UART BLE central. Copyright (c) Thomas Amberg, FHNW

// Based on https://github.com/adafruit/Adafruit_nRF52_Arduino
// /tree/master/libraries/Bluefruit52Lib/examples/Central
// Copyright (c) Adafruit.com, all rights reserved.

// Licensed under the MIT license, see LICENSE or
// https://choosealicense.com/licenses/mit/

#include <bluefruit.h>

uint16_t mtu; // Maximum Transmission Unit
BLEClientDis  deviceInfoServiceClient;
BLEClientBas  batteryServiceClient;
BLEClientUart uartServiceClient;

void scanCallback(ble_gap_evt_adv_report_t* report) {
  if (Bluefruit.Scanner.checkReportForService(report, uartServiceClient)) {
    Serial.print("UART service detected. Connecting ... ");
    Bluefruit.Central.connect(report);
  } else {
    Bluefruit.Scanner.resume(); // required for Softdevice v6
  }
}

void connectCallback(uint16_t conn_handle) {
  Serial.println("Connected");

  Serial.print("Dicovering Device Information ... ");
  if (deviceInfoServiceClient.discover(conn_handle)) {
    Serial.println("Found it");
    char buffer[32+1];

    // read and print out Manufacturer
    memset(buffer, 0, sizeof(buffer));
    if (deviceInfoServiceClient.getManufacturer(buffer, sizeof(buffer))) {
      Serial.print("Manufacturer: ");
      Serial.println(buffer);
    }

    // read and print out Model Number
    memset(buffer, 0, sizeof(buffer));
    if (deviceInfoServiceClient.getModel(buffer, sizeof(buffer))) {
      Serial.print("Model: ");
      Serial.println(buffer);
    }
    Serial.println();
  } else {
    Serial.println("Not found.");
  }

  Serial.print("Dicovering battery service... ");
  if (batteryServiceClient.discover(conn_handle)) {
    Serial.print("Battery level: ");
    Serial.print(batteryServiceClient.read());
    Serial.println("%");
  } else {
    Serial.println("Not found.");  
  }

  Serial.print("Discovering UART service... ");
  if (uartServiceClient.discover(conn_handle)) {
    uartServiceClient.enableTXD();
    Serial.println("Ready to receive from peripheral");
  } else {
    Serial.println("Not found. Disconnecting...");
    Bluefruit.disconnect(conn_handle);
  }  
}

void disconnectCallback(uint16_t conn_handle, uint8_t reason) {
  Serial.print("Disconnected, reason = ");
  Serial.println(reason);
}

void rxCallback(BLEClientUart &uartService) {
  Serial.print("rx: ");
  while (uartService.available()) {
    Serial.print((char) uartService.read());
  }
  Serial.print("\n");
}

void setup() {
  Serial.begin(115200);
  while (!Serial) { delay(10); } // only if usb connected
  Serial.println("Setup");

  Bluefruit.begin(0, 1); // 0 peripheral, max 1 central connection, save SoftDevice SRAM
  Bluefruit.setName("nRF52840");
  Bluefruit.setConnLedInterval(250); // ms

  batteryServiceClient.begin();  
  deviceInfoServiceClient.begin();
  uartServiceClient.begin();
  uartServiceClient.setRxCallback(rxCallback);

  Bluefruit.Central.setConnectCallback(connectCallback);
  Bluefruit.Central.setDisconnectCallback(disconnectCallback);

  mtu = Bluefruit.getMaxMtu(BLE_GAP_ROLE_CENTRAL);

  Bluefruit.Scanner.setRxCallback(scanCallback);
  Bluefruit.Scanner.restartOnDisconnect(true);
  Bluefruit.Scanner.setInterval(160, 80); // in unit of 0.625 ms
  Bluefruit.Scanner.useActiveScan(false);
  Bluefruit.Scanner.start(0);
}

void loop() {
  if (Bluefruit.Central.connected()) {
    if (uartServiceClient.discovered()) {
      Serial.print("tx: ");
      while (Serial.available()) {
        uint8_t txData[mtu];
        size_t bytesRead = Serial.readBytes(txData, mtu);
        int i = 0;
        while (i < bytesRead) {
          Serial.print((char) txData[i]); // echo
          uartServiceClient.print((char) txData[i]);
          i++;
        }
        Serial.print("\n");
      }
    }
  }
}
