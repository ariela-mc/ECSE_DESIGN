

#include <ESP32Servo.h>


Servo servo1;
Servo servo2;

const int servoPin2 = 27;
const int servoPin1 = 14;


void setup() {
servo1.attach(servoPin1, 500, 2500);


}


void loop() {
 servo1.write(90);
  delay(1000);       // Wait for 1 second
   
      servo1.write(0);
      delay(1000);  

      servo2.write(180);
      delay(1000);

      servo2.write(0);
      delay(1000);
}
