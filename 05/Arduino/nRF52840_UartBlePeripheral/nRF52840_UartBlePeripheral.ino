// Nordic UART BLE peripheral. Copyright (c) Thomas Amberg, FHNW

// Based on https://github.com/adafruit/Adafruit_nRF52_Arduino
// /tree/master/libraries/Bluefruit52Lib/examples/Peripheral
// Copyright (c) Adafruit.com, all rights reserved.

// Licensed under the MIT license, see LICENSE or
// https://choosealicense.com/licenses/mit/

#include <bluefruit.h>

BLEDis deviceInfoService;
BLEBas batteryService;

// See https://infocenter.nordicsemi.com/index.jsp?topic=%2Fcom.nordic.infocenter.sdk5.v15.3.0%2Fble_sdk_app_nus_eval.html
// Nordic UART service UUID: 6E400001-B5A3-F393-E0A9-E50E24DCCA9E
// RX characteristic   UUID: 6E400002-B5A3-F393-E0A9-E50E24DCCA9E
// TX characteristic   UUID: 6E400003-B5A3-F393-E0A9-E50E24DCCA9E
// The arrays below are ordered "least significant byte first":
uint8_t const uartServiceUuid[] = { 0x9E, 0xCA, 0xDC, 0x24, 0x0E, 0xE5, 0xA9, 0xE0, 0x93, 0xF3, 0xA3, 0xB5, 0x01, 0x00, 0x40, 0x6E };
uint8_t const rxCharacteristicUuid[] = { 0x9E, 0xCA, 0xDC, 0x24, 0x0E, 0xE5, 0xA9, 0xE0, 0x93, 0xF3, 0xA3, 0xB5, 0x02, 0x00, 0x40, 0x6E };
uint8_t const txCharacteristicUuid[] = { 0x9E, 0xCA, 0xDC, 0x24, 0x0E, 0xE5, 0xA9, 0xE0, 0x93, 0xF3, 0xA3, 0xB5, 0x03, 0x00, 0x40, 0x6E };

uint16_t mtu; // Maximum Transmission Unit
BLEService uartService = BLEService(uartServiceUuid);
BLECharacteristic rxCharacteristic = BLECharacteristic(rxCharacteristicUuid);
BLECharacteristic txCharacteristic = BLECharacteristic(txCharacteristicUuid);

void connectedCallback(uint16_t connHandle) {
  char centralName[32] = { 0 };
  BLEConnection *connection = Bluefruit.Connection(connHandle);
  connection->getPeerName(centralName, sizeof(centralName));
  Serial.print(connHandle);
  Serial.print(", connected to ");
  Serial.print(centralName);
  Serial.println();
}

void disconnectedCallback(uint16_t connHandle, uint8_t reason) {
  Serial.print(connHandle);
  Serial.print(" disconnected, reason = ");
  Serial.println(reason); // see https://github.com/adafruit/Adafruit_nRF52_Arduino
  // /blob/master/cores/nRF5/nordic/softdevice/s140_nrf52_6.1.1_API/include/ble_hci.h
  Serial.println("Advertising ...");
}

void cccdCallback(uint16_t connHandle, BLECharacteristic* characteristic, uint16_t cccdValue) {
  if (characteristic->uuid == txCharacteristic.uuid) {
    Serial.print("UART 'Notify', ");
    if (characteristic->notifyEnabled()) {
      Serial.println("enabled");
    } else {
      Serial.println("disabled");
    }
  }
}

void writeCallback(uint16_t connHandle, BLECharacteristic* characteristic, uint8_t* rxData, uint16_t len) {
  if (characteristic->uuid == rxCharacteristic.uuid) {
    Serial.print("rx: ");
    int i = 0;
    while (i < len) {
      Serial.print((char) rxData[i]);
      i++;
    }
    Serial.print("\n");
  }
}

void setupUartService() {
  uartService.begin(); // Must be called before calling .begin() on its characteristics

  txCharacteristic.setProperties(CHR_PROPS_NOTIFY);
  txCharacteristic.setPermission(SECMODE_OPEN, SECMODE_NO_ACCESS);
  txCharacteristic.setMaxLen(mtu);
  txCharacteristic.setCccdWriteCallback(cccdCallback);  // Optionally capture CCCD updates
  txCharacteristic.begin();

  rxCharacteristic.setProperties(CHR_PROPS_WRITE | CHR_PROPS_WRITE_WO_RESP);
  rxCharacteristic.setPermission(SECMODE_NO_ACCESS, SECMODE_OPEN);
  rxCharacteristic.setMaxLen(mtu);
  rxCharacteristic.setWriteCallback(writeCallback, true);
  rxCharacteristic.begin();
}

void startAdvertising() {
  Bluefruit.Advertising.addFlags(BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE);
  Bluefruit.Advertising.addTxPower();
  Bluefruit.Advertising.addService(uartService);
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
  Bluefruit.Periph.setConnectCallback(connectedCallback);
  Bluefruit.Periph.setDisconnectCallback(disconnectedCallback);

  deviceInfoService.setManufacturer("Adafruit Industries");
  deviceInfoService.setModel("Feather nRF52840 Express");
  deviceInfoService.begin();

  batteryService.begin();
  batteryService.write(100); // %

  mtu = Bluefruit.getMaxMtu(BLE_GAP_ROLE_PERIPH);

  setupUartService();
  startAdvertising();
}

void loop() {
  if (Bluefruit.connected()) {
    if (Serial.available()) {
      uint8_t txData[mtu];
      size_t bytesRead = Serial.readBytes(txData, mtu);
      if (txCharacteristic.notify(txData, bytesRead)) {
        Serial.print("tx: ");
        int i = 0;
        while (i < bytesRead) {
          Serial.print((char) txData[i]); // echo
          i++;
        }
        Serial.print("\n");
      } else {
        Serial.println("tx: Notify error.");
      }
    }
  }
  delay(200); // ms
}
