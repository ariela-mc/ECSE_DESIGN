#include <WiFi.h>
#include "time.h"
#include "esp_sntp.h"
#include <Adafruit_LEDBackpack.h>
#include <Adafruit_GFX.h>

// these errors should be fine? it compiles on arduino ide.

// 14-segment display object
Adafruit_AlphaNum4 alpha4 = Adafruit_AlphaNum4();

const char *ssid = "UoA-WiFi";
const char *password = "ilu2@M00N!";

const char *ntpServer1 = "pool.ntp.org";
const long gmtOffset_sec = 43200;
const int daylightOffset_sec = 46800;

void printLocalTime() {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("No time available (yet)");
    displayText("----");  // Show dashes when no time available
    return;
  }
  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
  
  // Format time for 14-segment display (HH:MM format)
  char timeStr[6];
  strftime(timeStr, sizeof(timeStr), "%H%M", &timeinfo);
  displayTime(timeStr);
}

// Function to display time on 14-segment display
void displayTime(const char* timeStr) {
  // Clear the display
  alpha4.clear();
  
  // Display hours and minutes (HHMM format)
  alpha4.writeDigitAscii(0, timeStr[0]);  // First hour digit
  alpha4.writeDigitAscii(1, timeStr[1]);  // Second hour digit
  alpha4.writeDigitAscii(2, timeStr[2]);  // First minute digit
  alpha4.writeDigitAscii(3, timeStr[3]);  // Second minute digit
  
  // Optional: Add colon between hours and minutes
  // Note: This depends on your specific display hardware
  // alpha4.writeDigitRaw(1, alpha4.getDigit(1) | 0x4000);  // Add colon
  
  alpha4.writeDisplay();
}

// Function to display text on 14-segment display
void displayText(const char* text) {
  alpha4.clear();
  
  // Display up to 4 characters
  for (int i = 0; i < 4 && text[i] != '\0'; i++) {
    alpha4.writeDigitAscii(i, text[i]);
  }
  
  alpha4.writeDisplay();
}

// Callback function (gets called when time adjusts via NTP)
void timeavailable(struct timeval *t) {
  Serial.println("Got time adjustment from NTP!");
  printLocalTime();
}

void setup() {
  Serial.begin(115200);
  
  // Initialize the 14-segment display
  Serial.println("Initializing 14-segment display...");
  if (!alpha4.begin(0x70)) {  // Default I2C address is 0x70
    Serial.println("Failed to initialize display!");
    while (1);  // Stop execution if display fails
  }
  
  // Set display brightness (0-15)
  alpha4.setBrightness(8);
  
  // Show initialization message
  displayText("INIT");
  delay(2000);

  // First step is to configure WiFi STA and connect in order to get the current time and date.
  Serial.printf("Connecting to %s ", ssid);
  displayText("WIFI");
  WiFi.begin(ssid, password);

  /**
   * NTP server address could be acquired via DHCP,
   *
   * NOTE: This call should be made BEFORE esp32 acquires IP address via DHCP,
   * otherwise SNTP option 42 would be rejected by default.
   * NOTE: configTime() function call if made AFTER DHCP-client run
   * will OVERRIDE acquired NTP server address
   */
  esp_sntp_servermode_dhcp(1);  // (optional)

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" CONNECTED");
  displayText("CONN");  // Show connection success
  delay(1000);

  // set notification call-back function
  sntp_set_time_sync_notification_cb(timeavailable);

  /**
   * This will set configured ntp servers and constant TimeZone/daylightOffset
   * should be OK if your time zone does not need to adjust daylightOffset twice a year,
   * in such a case time adjustment won't be handled automagically.
   */
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer1);
  
  // Show that we're waiting for time sync
  displayText("SYNC");

  /**
   * A more convenient approach to handle TimeZones with daylightOffset
   * would be to specify a environment variable with TimeZone definition including daylight adjustmnet rules.
   * A list of rules for your zone could be obtained from https://github.com/esp8266/Arduino/blob/master/cores/esp8266/TZ.h
   */
  //configTzTime(time_zone, ntpServer1, ntpServer2);
}

void loop() {
  delay(5000);
  printLocalTime();  // it will take some time to sync time :)
}
