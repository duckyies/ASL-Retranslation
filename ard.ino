#include "HCPCA9685.h"
#define I2CAdd 0x40
HCPCA9685 HCPCA9685(I2CAdd);

// Structure to store servo angles for each letter
struct ServoAngles {
  uint16_t angles[8]; // 0-180 for each servo
};

// Array of angle sets for each letter of the alphabet (A-Z)
ServoAngles letterAngles[26] = {
  {0,360,0,360,360,360,360,80},    // A
  {360,0,360,0,0,0,0,80},          // B
  {180,180,360,180,180,180,180,80}, // C
  {0,360,360,270,0,360,360,80},    // D
  {90,270,360,270,270,270,270,80}, // E
  {360,0,360,0,270,0,0,80},        // F
  {0,0,0,360,0,0,360,120},         // G
  {0,0,360,0,0,0,360,120},         // H
  {360,360,360,360,360,360,360,80}, // I
  {360,360,360,360,360,360,360,120}, // J
  {0,0,0,0,0,0,360,80},            // K
  {0,0,0,360,0,0,360,80},          // L
  {0,0,360,270,270,0,270,80},      // M
  {0,0,360,270,270,0,360,80},      // N
  {90,0,360,270,270,0,270,120},    // O
  {0,0,360,270,0,0,360,120},       // P
  {0,0,360,0,0,0,360,120},         // Q
  {0,0,360,0,180,0,360,80},        // R
  {0,360,360,360,360,360,360,80},  // S
  {0,0,360,180,250,0,360,80},      // T
  {0,0,360,0,0,0,360,80},          // U
  {0,0,360,0,0,0,360,80},          // V
  {0,0,360,0,0,0,0,80},            // W
  {0,0,360,360,180,0,360,80},      // X
  {360,0,0,360,360,0,360,80},      // Y
  {0,360,360,360,0,360,360,120},   // Z
  
};

// Current positions of each servo
int currentAngles[8] = {0, 0, 0, 0, 0, 0, 0, 80};

// Serial communication variables
const int BUFFER_SIZE = 200;
char inputBuffer[BUFFER_SIZE];
int bufferIndex = 0;
bool stringComplete = false;

void setup() {
  Serial.begin(115200);
  HCPCA9685.Init(SERVO_MODE);
  HCPCA9685.Sleep(false);
  
  // Initialize all servos to starting position
  for(int i = 0; i < 8; i++) {
    HCPCA9685.Servo(i, currentAngles[i]);
  }
  
  memset(inputBuffer, 0, BUFFER_SIZE);
  Serial.println("Ready to receive letters...");
}

// Function to move servos slowly to target positions
void moveSlowly(uint16_t targetAngles[8]) {
  Serial.println("Starting movement");
  bool moving = true;
  
  while(moving) {
    int inPlace = 0;
    
    for(int i = 0; i < 8; i++) {
      if(targetAngles[i] < currentAngles[i]) {
        currentAngles[i]--;
        HCPCA9685.Servo(i, currentAngles[i]);
      }
      else if(targetAngles[i] > currentAngles[i]) {
        currentAngles[i]++;
        HCPCA9685.Servo(i, currentAngles[i]);
      }
      else {
        inPlace++;
      }
    }
    
    if(inPlace == 8) {
      Serial.println("Movement complete");
      moving = false;
      return;
    }
    
    // Small delay for smooth movement
    delay(1.5); // Adjust this value to control speed (higher = slower)
  }
}

void loop() {
  // Read serial data without blocking
  while (Serial.available() && !stringComplete) {
    char inChar = Serial.read();
    if (inChar == '\n') {
      inputBuffer[bufferIndex] = '\0';  // Null-terminate
      stringComplete = true;
    } else if (bufferIndex < BUFFER_SIZE - 1) {
      inputBuffer[bufferIndex++] = inChar;
    }
  }
  
  if (stringComplete) {
    // Process the received letter
    Serial.print("Received: ");
    Serial.println(inputBuffer);
    
    // Check if we have a single letter
    if (strlen(inputBuffer) == 1 && isAlpha(inputBuffer[0])) {
      char letter = toupper(inputBuffer[0]);
      int letterIndex = letter - 'A';
      
      // Check if it's a valid letter (A-Z)
      if (letterIndex >= 0 && letterIndex < 26) {
        Serial.print("Moving to position for letter: ");
        Serial.println(letter);
        
        // Move servos to the position for this letter
        moveSlowly(letterAngles[letterIndex].angles);
      } else {
        Serial.println("Invalid letter received. Please send A-Z.");
      }
    } else {
      // If we received a longer message, check if it contains multiple letters
      for (int i = 0; i < strlen(inputBuffer); i++) {
        if (isAlpha(inputBuffer[i])) {
          char letter = toupper(inputBuffer[i]);
          int letterIndex = letter - 'A';
          
          if (letterIndex >= 0 && letterIndex < 26) {
            Serial.print("Moving to position for letter: ");
            Serial.println(letter);
            
            // Move servos to the position for this letter
            moveSlowly(letterAngles[letterIndex].angles);
            
            // Small delay between letters
            delay(1000);
          }
        }
      }
    }
    
    // Reset for next command
    bufferIndex = 0;
    stringComplete = false;
    memset(inputBuffer, 0, BUFFER_SIZE);
  }
}