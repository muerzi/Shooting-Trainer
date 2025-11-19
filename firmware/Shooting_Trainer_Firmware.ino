/*
 * Shooting Trainer Firmware v1.0
 * Hardware: Seeed XIAO BLE Sense (nRF52840)
 * Kompatibel mit WebApp v5.0
 */

#include <ArduinoBLE.h>
#include "LSM6DS3.h"
#include "Wire.h"

// --- EINSTELLUNGEN ---
// Schwellwert für Schusserkennung (in G-Kräften)
// Dry Fire (Klicken): ca. 1.5 - 2.5
// Live Fire (Scharf): ca. 3.0 - 5.0
const float SHOT_THRESHOLD = 2.0; 

// Mindestzeit zwischen zwei Schüssen (in ms), um "Prellen" zu verhindern
const int SHOT_COOLDOWN = 200; 

// Datenrate (in ms). 20ms = 50Hz. 
// Schneller als 15ms kann bei Web Bluetooth zu Paketverlust führen.
const int UPDATE_INTERVAL = 20; 

// --- OBJEKTE ---
LSM6DS3 myIMU(I2C_MODE, 0x6A); // I2C Adresse des internen Sensors

// UUIDs (Müssen exakt mit der WebApp übereinstimmen!)
BLEService trainerService("19B10000-E8F2-537E-4F6C-D104768A1214");
BLEStringCharacteristic sensorDataChar("19B10001-E8F2-537E-4F6C-D104768A1214", BLERead | BLENotify, 128);

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
  if (!BLE.begin()) {
    Serial.println("BLE Error!");
    while (1);
  }

  // BLE Konfiguration
  BLE.setLocalName("Shooting Trainer");
  BLE.setAdvertisedService(trainerService);
  trainerService.addCharacteristic(sensorDataChar);
  BLE.addService(trainerService);
  BLE.advertise();

  Serial.println("Bluetooth ready. Waiting for connection...");
}

void loop() {
  // Auf Verbindung warten
  BLEDevice central = BLE.central();

  if (central) {
    Serial.print("Verbunden mit: ");
    Serial.println(central.address());

    while (central.connected()) {
      long currentTime = millis();

      // Loop-Timing einhalten (50Hz)
      if (currentTime - lastUpdate >= UPDATE_INTERVAL) {
        readSensorData();
        detectShot(currentTime);
        sendToWebapp();
        lastUpdate = currentTime;
      }
    }
    
    Serial.println("Connection closed.");
  }
}

void readSensorData() {
  // Gyroskop Daten lesen (Winkelgeschwindigkeit in dps)
  gx = myIMU.readFloatGyroX();
  gy = myIMU.readFloatGyroY();
  gz = myIMU.readFloatGyroZ();

  // Beschleunigung lesen (für Schusserkennung)
  ax = myIMU.readFloatAccelX();
  ay = myIMU.readFloatAccelY();
  az = myIMU.readFloatAccelZ();
}

void detectShot(long currentTime) {
  // Vektor-Summe der Beschleunigung berechnen (Betrag)
  float magnitude = sqrt(ax*ax + ay*ay + az*az);

  if (magnitude > SHOT_THRESHOLD && (currentTime - lastShotTime > SHOT_COOLDOWN)) {
    shotTriggered = true;
    lastShotTime = currentTime;
  } else {
    shotTriggered = false;
  }
}

void sendToWebapp() {
  // JSON String bauen
  String json = "{";
  json += "\"gx\":" + String(gx, 2) + ",";
  json += "\"gy\":" + String(gy, 2) + ",";
  json += "\"gz\":" + String(gz, 2) + ",";
  json += "\"s\":" + String(shotTriggered ? 1 : 0);
  json += "}";

  // Daten senden
  sensorDataChar.writeValue(json);
}
