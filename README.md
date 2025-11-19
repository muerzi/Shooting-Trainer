# 🎯 Shooting Trainer

An open-source shooting training analysis system based on the Seeed XIAO BLE Sense. It detects shots, analyzes movement, and visualizes hits in a real-time web app.

## 📦 Package Contents
*   `firmware/`: Arduino code for the microcontroller.
*   `webapp/`: The HTML5 Web App (runs locally).
*   `3d_model/`: STL file for the enclosure.

## 🛠 Hardware Requirements
1.  **Seeed XIAO BLE Sense** (nRF52840 with IMU)
2.  LiPo Battery (3.7V, optional)
3.  USB-C Cable

## 🚀 Installation (Firmware)
1.  Install the **Arduino IDE**.
2.  Add `Seeed nRF52 Boards` in the Board Manager.
3.  Install the following libraries:
    *   `ArduinoBLE`
    *   `Seeed Arduino LSM6DS3`
4.  Open `firmware/Shooting_Trainer_Firmware.ino` and upload it to the board.

## 📱 How to Use (Web App)
1.  Open `webapp/index.html` in a Web Bluetooth compatible browser.
    *   **PC/Mac:** Chrome, Edge.
    *   **Android:** Chrome.
    *   **iOS (iPhone/iPad):** Use the **Bluefy Browser** (Safari does not support Web Bluetooth).
2.  Click **Verbinden (Connect)** and select "Shooting Trainer".
3.  Mount the sensor on your firearm.
4.  Aim and click **ZENTRIEREN (CENTER)** (bottom right button).
5.  **Start Training!** (Dry fire or live fire).

## ⚙️ Settings
Click the gear icon ⚙️ in the app:
*   **Sensor:** Invert axes if the movement direction is wrong.
*   **Target:** Change target size (distance simulation) and style (ISSF/Neon).
*   **Demo:** Simulate trigger errors (flinching, jerking) for educational purposes.

## 📝 License
MIT License. Feel free to modify and share!
