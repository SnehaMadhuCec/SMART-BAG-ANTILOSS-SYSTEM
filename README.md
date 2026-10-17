# SMART-BAG-ANTILOSS-SYSTEM
An IoT-based security system designed to prevent luggage or bags from being lost or stolen. Powered by an ESP32, this system creates a digital tether to the user's smartphone Wi-Fi hotspot. If the bag gets separated from the owner, it triggers a local alarm and sends an SMS alert with its GPS location.

## Features
*   **Guard Mode:** A physical push-button to easily arm and disarm the system.
*   **Digital Tethering:** Continuously monitors the connection to the owner's mobile hotspot.
*   **Separation Alarm:** Triggers a 30-second countdown upon disconnection. If not reconnected, a local buzzer sounds.
*   **GPS & SMS Tracking:** Uses a GSM module to send a Google Maps link of the bag's coordinates via SMS.

## Hardware Components
*   **Microcontroller:** ESP32-DevKitC V4
*   **GSM Module:** SIM900A (Requires dedicated 5V/2A power supply)
*   **GPS Module:** NEO-6M
*   **Peripherals:** Push Button, Active Buzzer
*   **Power:** USB Power Bank (Must support 5V/3A output to handle GSM transmission spikes)

##  Wiring Guide
**ESP32 to SIM900A (GSM)**
*   ESP32 Pin 26 (RX) -> SIM900A TX
*   ESP32 Pin 27 (TX) -> SIM900A RX
*   ESP32 GND -> SIM900A GND *(Mandatory shared ground)*

**ESP32 to NEO-6M (GPS)**
*   ESP32 Pin 16 (RX) -> NEO-6M TX
*   ESP32 Pin 17 (TX) -> NEO-6M RX
*   ESP32 3.3V (or 5V if needed) -> NEO-6M VCC

**Peripherals**
*   **Button:** ESP32 Pin 32 to GND (Uses internal pull-up)
*   **Buzzer:** ESP32 Pin 12 to GND

## Software Setup
1. Install the [Arduino IDE](https://www.arduino.cc/en/software).
2. CP210x Universal Windows Driver: Required for the Windows PC to recognize the ESP32 and assign it a COM port for uploading code.
3. ESP32 Board Package: Installed via the Arduino Boards Manager to allow the IDE to compile code specifically for the ESP32 architecture.
4. TinyGPS++ Library: An external library (by Mikal Hart) used to parse the raw satellite data from the NEO-6M module into usable coordinates.

## How to Use
1. Power on the ESP32 and the SIM900A GSM module.
2. Wait for the SIM900A network LED to blink slowly (every 3 seconds), indicating cellular connection.
3. Press the **Guard Mode** button. The system will connect to your phone's hotspot.
4. If the bag moves out of Wi-Fi range, the 30-second countdown begins.
5. Once confirmed, the buzzer sounds and an SMS with the location is dispatched.

##  Engineering Challenges & Hardware Notes
During the development of this project, hardware constraints were identified and resolved to ensure reliable operation:

* **GSM SIM Compatibility & Network Registration:** The SIM900A module relies on 2G network bands. A primary challenge was ensuring the inserted SIM card supported 2G fallback and had an active outgoing SMS tariff plan. Additionally, the module requires a specific initialization period to search for and register to a local cell tower.
  **Solution:** Verified network compatibility and programmed a dedicated delay sequence during the system bootup, allowing the GSM module adequate time to lock onto the network before attempting to send alerts.
* **GPS Indoor Limitations (Demo Mode):** Standard NEO-6M GPS modules require a clear line-of-sight to the sky to lock onto satellite signals. Because presentations typically occur indoors, the module cannot receive live coordinates.
   **Solution:** A specific "Demo Mode" is coded into the presentation version of the software to bypass the satellite check and transmit a hardcoded Google Maps link, ensuring the Wi-Fi separation logic and SMS transmission can be reliably demonstrated in a classroom setting.

  ## Future Scope & Upgrades

While the current prototype successfully demonstrates the core anti-loss logic, future iterations of this project can be enhanced with the following upgrades:

* **Migration to 4G LTE:** The current system uses the SIM900A, which relies on legacy 2G networks. Upgrading to a 4G LTE module (such as the SIM7600 or A7670) will ensure long-term network compatibility, broader SIM card support (like Jio), and faster data transmission.
* **Enhanced GNSS/GPS Efficiency:** Replacing the NEO-6M with a multi-constellation module (like the NEO-M8N) would allow the system to track GPS, GLONASS, and Galileo satellites simultaneously. This would drastically improve the Time-To-First-Fix (TTFF), provide higher accuracy, and offer better performance in urban environments with tall buildings.
* **Power Optimization & Battery Management:** Implementing ESP32 "Deep Sleep" modes and integrating a dedicated Battery Management System (BMS) would allow the bag to run on a compact Li-Ion battery for weeks without needing a recharge.
* **Live IoT Dashboard:** Transitioning from simple SMS alerts to real-time tracking via an IoT platform (like Blynk or Firebase) integrated with a custom mobile application.
