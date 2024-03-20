#include <Keypad.h>
#include <Servo.h>
#include <Wire.h>
#include "Buzzer.h"
#include "rgb_lcd.h"
#include <Adafruit_Fingerprint.h> 
#include <DFRobot_DHT11.h>

DFRobot_DHT11 DHT;
#define DHT11_PIN 27

rgb_lcd lcd;
const int colorR = 255;
const int colorG = 0;
const int colorB = 0;

static const int servoPin = 14;
Servo servo1;

int incorrectPasswordAttempts = 0; // Counter for incorrect attempts


#define ROW_NUM 4     // four rows
#define COLUMN_NUM 3  // three columns
char keys[ROW_NUM][COLUMN_NUM] = {
  { '1', '2', '3' },
  { '4', '5', '6' },
  { '7', '8', '9' },
  { '*', '0', '#' }
};

byte pin_rows[ROW_NUM] = { 18, 5, 33, 32 };
byte pin_column[COLUMN_NUM] = { 4, 0, 2 };

Keypad keypad = Keypad(makeKeymap(keys), pin_rows, pin_column, ROW_NUM, COLUMN_NUM);

const String password = "7890";
String input_password;

Adafruit_Fingerprint finger = Adafruit_Fingerprint(&Serial2);

void setup() {
  Serial.begin(115200);
  input_password.reserve(32);
  lcd.begin(16, 2);

  DHT.read(DHT11_PIN);
   lcd.setCursor(0, 0);
  lcd.print("temperature is :");
    lcd.setCursor(0, 1);
  lcd.print(DHT.temperature);
  lcd.print(" degrees");
  delay(10000);




  lcd.setCursor(0, 1);
  lcd.setRGB(colorR, colorG, colorB);
  Serial.begin(115200);
  servo1.attach(servoPin);
  setupBuzzer();
  Serial2.begin(9600);
  finger.begin(57600);





  if (finger.verifyPassword()) {
    Serial.println("Found fingerprint sensor!");
    lcd.clear();
    Serial.print("Fingerprint");
    Serial.print("sensor connected");
    delay(2000);
    lcd.clear();
    lcd.print("Please enter PIN");
    lcd.setCursor(0, 1);

  } else {
    Serial.println("Did not find fingerprint sensor :(");
    while (1);
  }
}

void loop() {
 





  char key = keypad.getKey();
  unsigned long currentMillis = millis();

  if (key) {
    lcd.print(key);

    if (key == '*') {
      input_password = "";
      lcd.print("input cleared");
      delay(3000);
      lcd.clear();
    } else if (key == '#') {
      if (password == input_password) {
        lcd.clear();
        lcd.setCursor(1, 0);
        lcd.print("Opening :)");
        for (int posDegrees = 0; posDegrees <= 90; posDegrees++) {
          servo1.write(posDegrees);
          Serial.println(posDegrees);
          delay(20);
          incorrectPasswordAttempts = 0;
        }
        delay(3000);
        lcd.clear();
        lcd.print("Enter 1 to close ");
      } else {
        incorrectPasswordAttempts++;
        lcd.clear();
        lcd.setCursor(1, 0);
        lcd.print("WRONG PIN :(");
        Buzzer();    
        lcd.clear();
        lcd.print("Enter PIN or");
        lcd.setCursor(0, 1);
        lcd.print("use fingerprint");
      }
      input_password = "";
    } else if (key == '1') {
      lcd.clear();
      lcd.setCursor(1, 0);
      lcd.print("Closing servo");

      for (int posDegrees = 90; posDegrees >= 0; posDegrees--) {
        servo1.write(posDegrees);
        Serial.println(posDegrees);
        delay(20);
      }

      delay(3000);
      lcd.clear();
      lcd.print("Enter PIN or");
      lcd.setCursor(0, 1);
      lcd.print("use fingerprint");
    } 

     if (incorrectPasswordAttempts >= 3) {
    // Lockout mechanism
    lcd.clear();
    lcd.print("LOCKEDOUT");
    lcd.setCursor(0, 1);
    lcd.print("Wait 30 seconds");
    delay(30000); // Lockout for 30 seconds
    incorrectPasswordAttempts = 0; // Reset counter after lockout
    lcd.clear();
    lcd.print("Enter PIN or");
    lcd.setCursor(0, 1);
    lcd.print("use fingerprint");
  }

    else {
      input_password += key;
    }
  }

  uint8_t p = finger.getImage();
  if (p == FINGERPRINT_OK) {
    Serial.println("Image taken");
    p = finger.image2Tz();
    if (p == FINGERPRINT_OK) {
      Serial.println("Image converted");
      p = finger.fingerFastSearch();
      if (p == FINGERPRINT_OK) {
        Serial.println("Fingerprint found!");
        lcd.clear();
        lcd.print("Fingerprint");
        lcd.setCursor(0, 1);
        lcd.print("detected OPENING");
        // Perform servo action for fingerprint found
        // Your existing servo opening code here
          for (int posDegrees = 0; posDegrees <= 90; posDegrees++) {
          servo1.write(posDegrees);
          Serial.println(posDegrees);
          delay(20);
        }
        delay(3000);
        lcd.clear();
        lcd.print("Enter 1 to close ");

      } else {
        Serial.println("No match found");
      }
    } else {
      Serial.println("Image conversion failed");
    }
  }
}
