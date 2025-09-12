#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_LEDBackpack.h>
#include <WiFi.h>
#include "time.h"
#include "esp_sntp.h"


#define NUM_DISPLAY 1
Adafruit_AlphaNum4 alpha4 = Adafruit_AlphaNum4();

const char* ssid = "Boshra's Galaxy A12";
const char* password = "boshra16";
const char* ntpServer1 = "pool.ntp.org";
const long gmtOffset_sec = 43200;  // GMT+12 hours for Auckland, NZ
const int daylightOffset_sec = 0;

// Callback function (gets called when time adjusts via NTP)
void timeavailable(struct timeval *t) {
  Serial.println("Got time adjustment from NTP!");
}

void displayTime(int hours, int minutes) {
  alpha4.clear();
  alpha4.writeDigitAscii(0, '0' + hours / 10);
  alpha4.writeDigitAscii(1, '0' + hours % 10);
  alpha4.writeDigitAscii(2, '0' + minutes / 10);
  alpha4.writeDigitAscii(3, '0' + minutes % 10);
  alpha4.writeDisplay();
}

void displayText(const char* text) {
  alpha4.clear();
  for (int i = 0; i < 4 && text[i] != '\0'; i++) {
    alpha4.writeDigitAscii(i, text[i]);
  }
  alpha4.writeDisplay();
}

void setup() {
  Serial.begin(115200);
  
  // Initialize display
  Serial.println("Initializing 14-segment display...");
  if (!alpha4.begin(0x70)) { // default I2C address (check yours)
    Serial.println("Failed to initialize display!");
    while (1);
  }
  alpha4.setBrightness(5);

  
  // Show initialization
  displayText(" ON ");
  delay(1000);

  // Connect to WiFi
  Serial.printf("Connecting to %s ", ssid);
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) { 
    Serial.print("."); 
  }
  Serial.println(" CONNECTED");

  // Set up NTP time sync
  sntp_set_time_sync_notification_cb(timeavailable);
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer1);
  
  // displayText("SYNC");
  Serial.println("Waiting for time sync...");
}

void loop() {
  struct tm timeinfo;
  if (getLocalTime(&timeinfo)) {
    int h = timeinfo.tm_hour;
    int m = timeinfo.tm_min;
    
    // Display time on 14-segment display
    displayTime(h, m);
    
    // Print to Serial for debugging
    Serial.printf("Time: %02d:%02d:%02d\n", h, m, timeinfo.tm_sec);
  } else {
    Serial.println("Waiting for time...");
    displayText("OFF");
  }
  delay(1000);
}
