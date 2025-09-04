
#include <Arduino.h>
#include <MD_YX5300.h>

HardwareSerial mp3Serial(1); // UART1
MD_YX5300 mp3(mp3Serial);

const int rxPin = 10; // connect to TX of MP3 Player module
const int txPin = 11; // connect to RX of MP3 Player module
const int triggerPin = 4; // Sensor_VP, ADC pin for voltage input
const float thresholdVoltage = 2.5; // Hysteresis: 2.4V - 2.6V is when changes
const float vRef = 3.3;
const float maxADCValue = 4095;

bool isPlaying = false; // track mp3Serial state

void setup() {
  Serial.begin(9600);
  // Initialise player
  mp3Serial.begin(9600, SERIAL_8N1, rxPin, txPin);
  
  mp3.begin();
  mp3.setSynchronous(true); 
  // Pause immediately on startup using raw command
  mp3.playStop();
}

void loop() {
  mp3.check();
  // check the status of mp3; visit the library readme for more.
  int adcValue = analogRead(triggerPin);
  float voltage = adcValue * vRef / maxADCValue;
  if (voltage >= (thresholdVoltage + 0.1) && !isPlaying) {
    // play first song
    isPlaying = true;
    mp3.playStart();
  }
  else if (voltage < (thresholdVoltage - 0.1) && isPlaying) {
    isPlaying = false;
    mp3.playStop();
  }
  // delay so that not immediately retriggered
  delay(100);
}