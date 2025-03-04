#include "HCPCA9685.h"
#define I2CAdd 0x40

HCPCA9685 HCPCA9685(I2CAdd);
String inputString = "";  // String to hold incoming data
bool stringComplete = false;  // Whether the string is complete

void setup() {
  Serial.begin(115200);
  Serial.setTimeout(1);
  
  // Initialize servo controller
  HCPCA9685.Init(SERVO_MODE);
  HCPCA9685.Sleep(false);
  
  // Reserve 200 bytes for the inputString
  inputString.reserve(200);
}

void loop() {
  // Wait for serial data
  while (Serial.available()) {
    // Get the new byte
    char inChar = (char)Serial.read();
    
    // Add it to the inputString
    inputString += inChar;
    
    // If the incoming character is a newline, set a flag
    if (inChar == '\n') {
      stringComplete = true;
    }
  }
  
  // Process the string when it's complete
  if (stringComplete) {
    // Remove whitespace and newline
    inputString.trim();
    
    // Echo back the received string
    Serial.println(inputString);
    
    // Example: Do something based on the received string
    if (inputString == "move") {
      HCPCA9685.Servo(3, random(0, 180));
    }
    
    // Clear the string for next input
    inputString = "";
    stringComplete = false;
  }
}