
const int TOUCH_PIN = 4;
const int LED_PIN = 23;

const int THRESHOLD = 20;

int touchValue;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  delay(1000);
pinMode (LED_PIN,OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
touchValue= touchRead(TOUCH_PIN);
Serial.print(touchValue);

if(touchValue < THRESHOLD){
  digitalWrite(LED_PIN , HIGH);
  Serial.print("- LED on ");
}
else {
  digitalWrite(LED_PIN , LOW);
  Serial.print("- LED off ");
}
}
