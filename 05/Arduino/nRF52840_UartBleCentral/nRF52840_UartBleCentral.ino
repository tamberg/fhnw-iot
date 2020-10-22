// Nordic UART BLE central. Copyright (c) Thomas Amberg, FHNW

// Based on https://github.com/adafruit/Adafruit_nRF52_Arduino
// /tree/master/libraries/Bluefruit52Lib/examples/Central
// Copyright (c) Adafruit.com, all rights reserved.

// Licensed under the MIT license, see LICENSE or
// https://choosealicense.com/licenses/mit/

#include <bluefruit.h>

uint16_t mtu; // Maximum Transmission Unit
BLEClientDis deviceInfoServiceClient;
BLEClientBas batteryServiceClient;
BLEClientUart uartServiceClient;

void scanCallback(ble_gap_evt_adv_report_t* report) {
  if (Bluefruit.Scanner.checkReportForService(report, uartServiceClient)) {
    Serial.printBufferReverse( // little endian
      report->peer_addr.addr, 6, ':');
    Serial.println();
    Serial.print("UART service detected. Connecting ... ");
    Bluefruit.Central.connect(report);
  } else {
    Bluefruit.Scanner.resume(); // required for Softdevice v6
  }
}

void connectCallback(uint16_t connHandle) {
  Serial.println("Connected");

  Serial.println("Discovering device information service... ");
  if (deviceInfoServiceClient.discover(connHandle)) {
    char manuName[mtu];
    memset(manuName, 0, sizeof(manuName));
    if (deviceInfoServiceClient.getManufacturer(manuName, sizeof(manuName))) {
      Serial.print("Manufacturer: ");
      Serial.println(manuName);
    }
    char modelName[mtu];
    memset(modelName, 0, sizeof(modelName));
    if (deviceInfoServiceClient.getModel(modelName, sizeof(modelName))) {
      Serial.print("Model: ");
      Serial.println(modelName);
    }
  } else {
    Serial.println("Not found.");
  }

  Serial.println("Discovering battery service... ");
  if (batteryServiceClient.discover(connHandle)) {
    Serial.print("Battery level: ");
    Serial.print(batteryServiceClient.read());
    Serial.println("%");
  } else {
    Serial.println("Not found.");  
  }

  Serial.println("Discovering UART service... ");
  if (uartServiceClient.discover(connHandle)) {
    uartServiceClient.enableTXD();
    Serial.println("Ready to receive from peripheral");
  } else {
    Serial.println("Not found. Disconnecting...");
    Bluefruit.disconnect(connHandle);
  }  
}

void disconnectCallback(uint16_t connHandle, uint8_t reason) {
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
      while (Serial.available()) {
        uint8_t txData[mtu + 1];
        size_t bytesRead = Serial.readBytes(txData, mtu);
        txData[bytesRead] = '\0';
        uartServiceClient.print((char *) txData);
        Serial.print("tx: ");
        Serial.print((char *) txData);
        Serial.print("\n");
      }
    }
  }
}
