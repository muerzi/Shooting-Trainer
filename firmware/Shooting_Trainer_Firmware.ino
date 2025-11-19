/*
 * Shooting Trainer Firmware v1.0
 * Hardware: Seeed XIAO BLE Sense (nRF52840)
 * Kompatibel mit WebApp v5.0
 */

#include <bluefruit.h>
#include "LSM6DS3.h"
#include "Wire.h"

// --- EINSTELLUNGEN ---
const float SHOT_THRESHOLD = 2.0; 
const int SHOT_COOLDOWN = 200; 
const int UPDATE_INTERVAL = 20; 

// --- OBJEKTE ---
LSM6DS3 myIMU(I2C_MODE, 0x6A);

// BLE Service und Characteristic
BLEService trainerService = BLEService(0x1214);
BLECharacteristic sensorDataChar = BLECharacteristic(0x1215);

// --- VARIABLEN ---
float ax, ay, az;
float gx, gy, gz;
long lastUpdate = 0;
long lastShotTime = 0;
bool shotTriggered = false;

void setup() {
  Serial.begin(115200);
  
  // 1. IMU Starten
  if (myIMU.begin() != 0) {
    Serial.println("IMU Error!");
    while (1);
  }

  // 2. BLE Starten
  Bluefruit.begin();
  Bluefruit.setName("Shooting Trainer");
  Bluefruit.Periph.setConnectCallback(connect_callback);
  Bluefruit.Periph.setDisconnectCallback(disconnect_callback);

  // 3. Service und Characteristic konfigurieren
  setupBLE();

  // 4. Advertising starten
  startAdvertising();

  Serial.println("Bluetooth ready. Waiting for connection...");
}

void setupBLE() {
  // Service hinzufügen
  trainerService.begin();

  // Characteristic konfigurieren
  sensorDataChar.setProperties(CHR_PROPS_READ | CHR_PROPS_NOTIFY);
  sensorDataChar.setPermission(SECMODE_OPEN, SECMODE_NO_ACCESS);
  sensorDataChar.setMaxLen(128);
  sensorDataChar.begin();
}

void startAdvertising() {
  Bluefruit.Advertising.addFlags(BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE);
  Bluefruit.Advertising.addTxPower();
  Bluefruit.Advertising.addService(trainerService);
  Bluefruit.Advertising.addName();
  
  Bluefruit.Advertising.restartOnDisconnect(true);
  Bluefruit.Advertising.setInterval(32, 244);    // in units of 0.625 ms
  Bluefruit.Advertising.setFastTimeout(30);      // seconds
  Bluefruit.Advertising.start(0);                // 0 = Don't stop advertising
}

void connect_callback(uint16_t conn_handle) {
  Serial.print("Verbunden mit: ");
  Serial.println(conn_handle);
}

void disconnect_callback(uint16_t conn_handle, uint8_t reason) {
  Serial.println("Connection closed.");
}

void loop() {
  if (Bluefruit.connected()) {
    long currentTime = millis();

    if (currentTime - lastUpdate >= UPDATE_INTERVAL) {
      readSensorData();
      detectShot(currentTime);
      sendToWebapp();
      lastUpdate = currentTime;
    }
  }
}

void readSensorData() {
  gx = myIMU.readFloatGyroX();
  gy = myIMU.readFloatGyroY();
  gz = myIMU.readFloatGyroZ();

  ax = myIMU.readFloatAccelX();
  ay = myIMU.readFloatAccelY();
  az = myIMU.readFloatAccelZ();
}

void detectShot(long currentTime) {
  float magnitude = sqrt(ax*ax + ay*ay + az*az);

  if (magnitude > SHOT_THRESHOLD && (currentTime - lastShotTime > SHOT_COOLDOWN)) {
    shotTriggered = true;
    lastShotTime = currentTime;
  } else {
    shotTriggered = false;
  }
}

void sendToWebapp() {
  String json = "{";
  json += "\"gx\":" + String(gx, 2) + ",";
  json += "\"gy\":" + String(gy, 2) + ",";
  json += "\"gz\":" + String(gz, 2) + ",";
  json += "\"s\":" + String(shotTriggered ? 1 : 0);
  json += "}";

  sensorDataChar.notify(json.c_str(), json.length());
}
