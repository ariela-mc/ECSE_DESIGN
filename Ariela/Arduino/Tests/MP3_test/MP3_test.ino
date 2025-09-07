
#include <Arduino.h>
#include <MD_YX5300.h>

HardwareSerial mp3Serial(2); // UART2
MD_YX5300 mp3(mp3Serial);

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
  mp3.playStart();
  delay(100)
}