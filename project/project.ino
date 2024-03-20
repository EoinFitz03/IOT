#include <Keypad.h>
#include <Servo.h>
#include <Wire.h>
#include "Buzzer.h"
#include "rgb_lcd.h"
#include <Adafruit_Fingerprint.h> 

// Initialize LCD and define RGB color values
rgb_lcd lcd;
const int colorR = 255;
const int colorG = 0;
const int colorB = 0;

// Define pins for servo and keypad
static const int servoPin = 14;
Servo servo1;
#define ROW_NUM 4     // Number of rows on the keypad
#define COLUMN_NUM 3  // Number of columns on the keypad

// Define the keypad layout and pins
char keys[ROW_NUM][COLUMN_NUM] = {
  { '1', '2', '3' },
  { '4', '5', '6' },
  { '7', '8', '9' },
  { '*', '0', '#' }
};
byte pin_rows[ROW_NUM] = { 18, 5, 33, 32 };
byte pin_column[COLUMN_NUM] = { 4, 0, 2 };

// Create a Keypad object
Keypad keypad = Keypad(makeKeymap(keys), pin_rows, pin_column, ROW_NUM, COLUMN_NUM);

// Define the password and input password
const String password = "7890";
String input_password;

// Initialize fingerprint sensor
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&Serial2);

void setup() {
  // Begin serial communication and LCD
  Serial.begin(115200);
  input_password.reserve(32);
  lcd.begin(16, 2);
  lcd.print("Please enter PIN");
  lcd.setCursor(0, 1);
  lcd.setRGB(colorR, colorG, colorB);
  servo1.attach(servoPin); 

  // Setup Buzzer and initialize fingerprint sensor
  setupBuzzer();
  Serial2.begin(9600);
  finger.begin(57600);

  // Verify fingerprint sensor connection
  if (finger.verifyPassword()) {
    Serial.println("Found fingerprint sensor!");
    serial.print("Fingerprint");
    serial.print("sensor connected");
    lcd.clear();
    lcd.print("Please enter PIN");
    lcd.setCursor(0, 1);
  } else {
    Serial.println("Did not find fingerprint sensor :(");
    while (1); // wait until sensor not found
  }
}

void loop() {
  char key = keypad.getKey();
  if (key) {
    lcd.print(key);  // prints key on the lcd 

    if (key == '*') {
      input_password = "";
      lcd.print("input cleared");
      delay(3000);
      lcd.clear();
    } else if (key == '#') {
      // Check entered password
      if (password == input_password) {
        // Open the servo if password matches
        lcd.clear();
        lcd.setCursor(1, 0);
        lcd.print("Opening :)");
        // Servo movement to open
        for (int posDegrees = 0; posDegrees <= 90; posDegrees++) {
          servo1.write(posDegrees);
          Serial.println(posDegrees);
          delay(20);
        }
        delay(3000);
        lcd.clear();
        lcd.print("Enter 1 to close ");
      } else {
        // Wrong password entered
        lcd.clear();
        lcd.setCursor(1, 0);
        lcd.print("WRONG PIN :(");
        Buzzer(); // Activate buzzer for wrong entry    
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
    } else {
      input_password += key;
    }
  }

  // Fingerprint sensor operations
  uint8_t p = finger.getImage();  //gets the fingerprint image 
  if (p == FINGERPRINT_OK) {
    Serial.println("Image taken");
    p = finger.image2Tz();    //converts the image into a template 
    if (p == FINGERPRINT_OK) {
      Serial.println("Image converted");
      p = finger.fingerFastSearch();
      if (p == FINGERPRINT_OK) {    // if image is found the servo wil open 
        Serial.println("Fingerprint found!");
        lcd.clear();
        lcd.print("Fingerprint");
        lcd.setCursor(0, 1);
        lcd.print("detected OPENING");
        // servo opening 
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
