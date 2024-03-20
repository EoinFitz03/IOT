#include <Wire.h>
#include <Keypad.h>

const int ROW_NUM = 4;
const int COLUMN_NUM = 3;

char keys[4][3] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};

byte pin_rows[4] = {18, 5, 17, 16};
byte pin_column[3] = {4, 0, 2};

Keypad keypad(makeKeymap(keys), pin_rows, pin_column, ROW_NUM, COLUMN_NUM);

extern const String password = "7890";
String input_password;

void setupKeypad() {
  input_password.reserve(32);
}

void checkKeypadInput() {
  char key = keypad.getKey();
  if (key) {
    Serial.println(key);

    if (key == '*') {
      input_password = ""; // clear input password
    } else if (key == '#') {
      if (password == input_password) {
        Serial.println("The password is correct, ACCESS GRANTED!");
        // DO YOUR WORK HERE

      } else {
        Serial.println("The password is incorrect, ACCESS DENIED!");
      }

      input_password = ""; // clear input password
    } else {
      input_password += key; // append new character to input password string
    }
  }
}