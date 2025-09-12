
#include <Arduino.h>
#include <MD_YX5300.h>


const int rxPin = 16; // connect to TX of MP3 Player module 
const int txPin = 17; // connect to RX of MP3 Player module 
HardwareSerial mp3Serial(2); // UART2
MD_YX5300 mp3(mp3Serial);


const int screamDelay = 1000; //delay in ms 


void setup() {
  Serial.begin(115200); //setting the baud rate
  mp3Serial.begin(9600, SERIAL_8N1, rxPin, txPin);
  mp3.begin();
  mp3.setSynchronous(true); 
  Serial.println("hi");
}

void loop() {
  // check the status of mp3; visit the library readme for more.
  mp3.playStart();
  // delay ensures sound is not immediately retriggered
  delay(screamDelay);
  Serial.println("loop");

}