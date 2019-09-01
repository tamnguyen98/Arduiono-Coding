#include <Servo.h>
#define servoPin 2
/*
 * d0 = 16
 * d1 = 5
 * d2 = 4
 * d3 = 0
 * d4 = 2
 */

int servoAngle = 0;   // servo position in degrees
Servo servo;


void setup() {
  // put your setup code here, to run once:
  //pinMODE(servoPin, OUTPUT);
  servo.attach(servoPin);
  Serial.begin(115200);         // Start the Serial communication to send messages to the computer
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("Turning 0");
  servo.write(0);      // Turn SG90 servo Left to 45 degrees
  delay(5000);          // Wait 1 second
  Serial.println("Turning 45");
  servo.write(45);      // Turn SG90 servo back to 90 degrees (center position)
  delay(5000);          // Wait 1 second
  Serial.println("Turning 90");
  servo.write(90);     // Turn SG90 servo Right to 135 degrees
  delay(5000);          // Wait 1 second
  Serial.println("Turning 0");
  servo.write(0);      // Turn SG90 servo back to 90 degrees (center position)
  delay(10000);
}
