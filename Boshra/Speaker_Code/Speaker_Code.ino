
#include <Arduino.h>
#include <MD_YX5300.h>

HardwareSerial mp3Serial(1); // UART1
MD_YX5300 mp3(mp3Serial);

const int rxPin = 10; // connect to TX of MP3 Player module - DO WE NEED THIS? WE ARE ONLY SENDING COMMANDS?
const int txPin = 11; // connect to RX of MP3 Player module - SHOULD DATA TYPE BE SHORT?
const int triggerPin = 4; // Sensor_VP, ADC pin for voltage input
const float thresholdVoltage = 2.5; 
const float hysteresis = 0.2; // Hysteresis: 2.4V - 2.6V is when changes
const float vRef = 3.3;
const float maxADCValue = 4095; //ESP32 is a 12 bit ADC => 2^12 = 4095
const int screamDelay = 1000; //delay in ms 

bool isPlaying = false; // track mp3Serial state

void setup() {
  Serial.begin(9600); //setting the baud rate
  analogReadResolution(12); // setting ADC to 12 bits
  // Initialise player
  mp3Serial.begin(9600, SERIAL_8N1, rxPin, txPin);
  mp3.begin();
  mp3.setSynchronous(true); 
  // Pause immediately on startup using raw command
  mp3.playStop();
}

void loop() {
  // check the status of mp3; visit the library readme for more.
  mp3.check();
  // Using ADC to find value of the input voltage
  int adcValue = analogRead(triggerPin);
  float voltage = adcValue * vRef / maxADCValue; 
  
  if (voltage >= (thresholdVoltage + hysteresis/2)) {
	  
    if (!isPlaying) {
	  // INSERT CODE FOR RUNNING THE MOTOR - put this in a loop? 

      // play first song
      isPlaying = true;
      mp3.playStart();
    }
  }
  else if (voltage <= (thresholdVoltage - hysteresis/2)) {
    
    if (isPlaying) {
	   // INSERT THE CODE FOR STOPPING THE MOTOR - put this in a loop?

      // stop song
      isPlaying = false;
      mp3.playStop();
    }
  }
  // delay ensures sound is not immediately retriggered
  delay(screamDelay);
}