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
