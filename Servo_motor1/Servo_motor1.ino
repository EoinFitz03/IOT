#include<Servo.h>

static const int SERVO_PIN=27;

Servo servo1;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  servo1.attach(SERVO_PIN);

}

void loop() {
  // put your main code here, to run repeatedly:
for (int posDegrees = 0; posDegrees <= 90; posDegrees++){
  servo1.write(posDegrees);
  Serial.println(posDegrees);
  delay(20);
}
for (int posDegrees = 90; posDegrees >= 0; posDegrees--){
  servo1.write(posDegrees);
  Serial.println(posDegrees);
  delay(20);
}
}
