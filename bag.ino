#include <WiFi.h>
#include <TinyGPS++.h>

// ==========================================
// ⚠️ UPDATE THESE 3 LINES BEFORE UPLOADING ⚠️
// ==========================================
const char* ssid = "YOUR_HOTSPOT_NAME";       // Exactly as it appears on your phone
const char* password = "YOUR_HOTSPOT_PASSWORD";   // Your hotspot password
const String ownerPhone = "+91XXXXXXXXXX";    // Your mobile number (must include +91)
// ==========================================

// --- Pin Definitions ---
const int BUTTON_PIN = 32; // Guard Mode toggle button

// --- Serial Ports for Modules ---
// ESP32 Hardware Serial: Serial(0) is USB. We use 1 & 2 for GSM/GPS.
HardwareSerial SerialGSM(1); 
HardwareSerial SerialGPS(2); 
const int GSM_RX = 26, GSM_TX = 27;
const int GPS_RX = 16, GPS_TX = 17;

// --- System Variables ---
TinyGPSPlus gps;
bool wasConnected = false;
unsigned long disconnectTime = 0;
const unsigned long delayTime = 30000; // 30 seconds delay before triggering alarm
bool alertTriggered = false;

// --- Guard Mode Variables ---
bool guardModeActive = false; // Starts OFF by default so it doesn't ring immediately
int lastButtonState = HIGH;

void setup() {
  Serial.begin(115200);
  
  // Initialize Serial communication for GPS & GSM
  SerialGSM.begin(9600, SERIAL_8N1, GSM_RX, GSM_TX);
  SerialGPS.begin(9600, SERIAL_8N1, GPS_RX, GPS_TX);

  // Initialize the Push Button
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  // Initialize GSM Module for text mode
  Serial.println("Initializing GSM Module...");
  delay(3000); // Give GSM module time to find cellular network
  SerialGSM.println("AT"); 
  delay(1000);
  SerialGSM.println("AT+CMGF=1"); // Set SMS to Text Mode
  delay(1000);

  Serial.println("=============================================");
  Serial.println("System Ready. Connecting to Phone Hotspot...");
  Serial.println("Guard Mode is currently OFF. Press button to Arm.");
  Serial.println("=============================================");
  
  // Automatically connect to Wi-Fi on startup
  WiFi.begin(ssid, password);
}

void loop() {
  // 1. Continuously parse incoming GPS data
  while (SerialGPS.available() > 0) {
    gps.encode(SerialGPS.read());
  }

  // 2. Read the Guard Mode Button
  int currentButtonState = digitalRead(BUTTON_PIN);
  if (currentButtonState != lastButtonState) {
    if (currentButtonState == LOW) { // Button was just pressed down
      guardModeActive = !guardModeActive; // Toggle the state
      
      if (guardModeActive) {
        Serial.println("🛡️ GUARD MODE ON: Bag is now secured!");
      } else {
        Serial.println("🛡️ GUARD MODE OFF: Bag is disarmed.");
        alertTriggered = false;        // Reset the alert flag
      }
    }
    lastButtonState = currentButtonState;
    delay(50); // Debounce
  }

  // 3. Monitor Wi-Fi Connection (Always tracking)
  if (WiFi.status() == WL_CONNECTED) {
    if (!wasConnected) {
      Serial.println("Hotspot Connected! Monitoring active.");
      wasConnected = true;
      alertTriggered = false;
    }
  } else {
    if (wasConnected) {
      Serial.println("Connection Lost! Starting 30-second countdown...");
      wasConnected = false;
      disconnectTime = millis();
    }

    // 4. Trigger Alert ONLY if Guard Mode is ON and time has passed
    if (guardModeActive && !wasConnected && !alertTriggered && (millis() - disconnectTime >= delayTime)) {
      Serial.println("Separation Confirmed! Triggering Alarm & SMS...");
      sendSMSAlert();                 // Send GPS coordinates via SMS
      alertTriggered = true;          // Flag to prevent sending infinite SMS messages
    }
  }
}

// Function to handle the GSM SMS transmission
void sendSMSAlert() {

  String locationMsg = "ALERT: Bag left behind! Location: ";
  if (gps.location.isValid()) {
    // Read latitude and longitude from the NEO-6M
    String lat = String(gps.location.lat(), 6);
    String lng = String(gps.location.lng(), 6);
    locationMsg += "https://maps.google.com/?q=" + lat + "," + lng;

  } 
  else {
      locationMsg += "GPS fixing... Check last known area.";

  }
  Serial.println("Sending SMS: " + locationMsg);
  SerialGSM.println("AT+CMGS=\"" + ownerPhone + "\"");
  delay(1000);
  SerialGSM.print(locationMsg);
  delay(1000);
  SerialGSM.write(26); // Sends the ASCII code for CTRL+Z to execute the send command
  delay(3000);
  Serial.println("SMS Sent Successfully.");

}