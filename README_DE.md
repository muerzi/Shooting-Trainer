# 🎯 Shooting Trainer

Ein Open-Source Schießtrainings-Analysesystem, basierend auf dem Seeed XIAO BLE Sense. Es analysiert Bewegungen, erkennt Schüsse und visualisiert Treffer in einer Web-App.

## 📦 Inhalt des Pakets
*   `firmware/`: Arduino-Code für den Mikrocontroller.
*   `webapp/`: Die HTML5 Web-App (lokal ausführbar).
*   `3d_model/`: STL-Datei für das Gehäuse.

## 🛠 Hardware
1.  **Seeed XIAO BLE Sense** (nRF52840 mit IMU)
2.  LiPo Akku (3.7V, optional)
3.  USB-C Kabel

## 🚀 Installation (Firmware)
1.  Installiere die **Arduino IDE**.
2.  Füge `Seeed nRF52 Boards` im Boardverwalter hinzu.
3.  Installiere folgende Bibliotheken:
    *   `ArduinoBLE`
    *   `Seeed Arduino LSM6DS3`
4.  Öffne `firmware/Shooting_Trainer_Firmware.ino` und lade es auf den Chip hoch.

## 📱 Nutzung (Web App)
1.  Öffne `webapp/index.html` in einem Bluetooth-fähigen Browser.
    *   **PC/Mac:** Chrome, Edge.
    *   **Android:** Chrome.
    *   **iOS (iPhone/iPad):** Nutze den **Bluefy Browser** (Safari unterstützt kein Web Bluetooth).
2.  Klicke auf **Verbinden** und wähle "Shooting Trainer".
3.  Befestige den Sensor an der Waffe.
4.  Drücke **ZENTRIEREN** (unten rechts), während du zielst.
5.  **Feuer frei!** (Trocken oder Scharf).

## ⚙️ Einstellungen
Klicke auf das Zahnrad-Symbol ⚙️ in der App:
*   **Sensor:** Invertiere Achsen, falls der Punkt falsch wandert.
*   **Zielscheibe:** Ändere Größe (Entfernung) und Stil (ISSF/Neon).
*   **Demo:** Simuliere Abzugsfehler (Mucken, Reißen) zu Trainingszwecken.

## 📝 Lizenz
MIT License. Feel free to modify and share!
