

// Libraries for the LED display
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_LEDBackpack.h>
#include <WiFi.h>
#include "time.h"
#include "esp_sntp.h"

//Library for servo motors
#include <ESP32Servo.h>

//Library for speaker 
#include <Arduino.h>
#include <MD_YX5300.h>

//LED 
#define NUM_DISPLAY 1
Adafruit_AlphaNum4 alpha4 = Adafruit_AlphaNum4();

const char* ssid = "Boshra's Galaxy A12";
const char* password = "boshra16";
const char* ntpServer1 = "pool.ntp.org";
const long gmtOffset_sec = 43200;  // GMT+12 hours for Auckland, NZ
const int daylightOffset_sec = 0;

//SERVO MOTORS
Servo servo1;
Servo servo2;

const int servoPin2 = 27;
const int servoPin1 = 14;

//SPEAKER
HardwareSerial mp3Serial(2); // UART1
MD_YX5300 mp3(mp3Serial);

const int rxPin = 16; // connect to TX of MP3 Player module 
const int txPin = 17; // connect to RX of MP3 Player module 
const int triggerPin = 36; // Sensor_VP, ADC pin for voltage input
const float thresholdVoltage = 1.5; 
const float hysteresis = 0.2; // Hysteresis: 2.4V - 2.6V is when changes
const float vRef = 3.3;
const float maxADCValue = 4095; //ESP32 is a 12 bit ADC => 2^12 = 4095
const int screamDelay = 1000; //delay in ms 
bool isPlaying = false; // track mp3Serial state


//OTHER FUNCTIONS BELOW
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

//MANDATORY FUNCTIONS
void setup() {
  //LED SETUP
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
  
 
  
  //SERVO SETUP
  servo1.attach(servoPin1, 500, 2500);
  servo2.attach(servoPin2, 500, 2500);

  
  //SPEAKER SETUP
   analogReadResolution(12); // setting ADC to 12 bits
  // Initialise player
  mp3Serial.begin(9600, SERIAL_8N1, rxPin, txPin);
  mp3.begin();
  mp3.setSynchronous(true); 
}

void loop() {
  
  
  //SPEAKER CODE
  // Using ADC to find value of the input voltage
  int adcValue = analogRead(triggerPin);
  float voltage = adcValue * vRef / maxADCValue; 
  
  if (voltage >= (thresholdVoltage + hysteresis/2)) {
	  
    if (!isPlaying) {
	  //LED display
	  displayText("AHHH");	  
	  //SPEAKER 
      Serial.println("play");
      // play first song
      isPlaying = true;
      mp3.playStart();
	  
	  //SERVO CODE
	  servo1.write(35);
	  servo2.write(35);

	  delay(100);       // Wait for 1 second
   
      servo1.write(0);
	  servo2.write(0);

      delay(100);  

      servo1.write(35);
	  servo2.write(35);

	  delay(100);       // Wait for 1 second
   
      servo1.write(0);


	  servo2.write(0);
    }
  }
  else if (voltage <= (thresholdVoltage - hysteresis/2)) {
    
    if (isPlaying) {
	   // INSERT THE CODE FOR STOPPING THE MOTOR - put this in a loop?
      Serial.println("stop");

      // stop song
      isPlaying = false;
      mp3.playStop();
    }
  }
  // delay ensures sound is not immediately retriggered
  delay(screamDelay);


  //LED CODE
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
  
}