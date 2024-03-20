#include "Buzzer.h"

#define BUZZER_PIN 12     // Replace with your buzzer pin number
#define BUZZER_CHANNEL 15 // Buzzer channel

void setupBuzzer(){
    ledcSetup(BUZZER_CHANNEL, 1000, 8);         // Setup channel 0 with 1000 Hz and 8-bit depth
  ledcAttachPin(BUZZER_PIN, BUZZER_CHANNEL);  // Attach channel to the pin
}

void Buzzer() {
  // Turn the buzzer on
  ledcWriteTone(BUZZER_CHANNEL, 1000); // Play 1000Hz tone on the channel for 5 seconds
  delay(5000); // Keep the buzzer on for 5 seconds
  
  // Turn the buzzer off
  ledcWrite(BUZZER_CHANNEL, 0); // Stop the tone on the channel
  delay(1000); // Keep the buzzer off for 1 second before repeating
}