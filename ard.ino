#include "HCPCA9685.h"
#define I2CAdd 0x40

HCPCA9685 HCPCA9685(I2CAdd);


struct ServoAngles {
  uint8_t angles[8]; 
};
ServoAngles letterAngles[26];


const int BUFFER_SIZE = 200;
char inputBuffer[BUFFER_SIZE];
int bufferIndex = 0;
bool stringComplete = false;

unsigned long previousMillis = 0;
const long interval = 500;  
int currentElement = 0;
int elementCount = 0;
char* elements[100]; 

void setup() {
  Serial.begin(115200);
  HCPCA9685.Init(SERVO_MODE);
  HCPCA9685.Sleep(false);
  memset(inputBuffer, 0, BUFFER_SIZE);
  randomSeed(analogRead(0));
  for(int i = 0; i < 26; i++) {
    for(int j = 0; j < 8; j++) {
      letterAngles[i].angles[j] = random(0, 181);
    }
  }
}

void loop() {
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
    if (strcmp(inputBuffer, "121") != 0) {
      Serial.print("Received: ");
      Serial.println(inputBuffer);

      elementCount = 0;
      char* token = strtok(inputBuffer, ",");
      while (token != NULL && elementCount < 100) {
        elements[elementCount++] = token;
        token = strtok(NULL, ",");
      }

      currentElement = 0;  
    }

    // Reset for next input
    bufferIndex = 0;
    stringComplete = false;
    memset(inputBuffer, 0, BUFFER_SIZE);
  }

  if (currentElement < elementCount) {
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= interval) {
      previousMillis = currentMillis;

      char* element = elements[currentElement];
      int angle = random(0, 180);  // Example conversion
      HCPCA9685.Servo(0, letterAngles[random(0,26)].angles[0]);
      HCPCA9685.Servo(1, letterAngles[random(0,26)].angles[1]);
      HCPCA9685.Servo(2, letterAngles[random(0,26)].angles[2]);
      HCPCA9685.Servo(3, letterAngles[random(0,26)].angles[3]);
      HCPCA9685.Servo(4, letterAngles[random(0,26)].angles[4]);
      HCPCA9685.Servo(5, letterAngles[random(0,26)].angles[5]);
      HCPCA9685.Servo(6, letterAngles[random(0,26)].angles[6]);
      HCPCA9685.Servo(7, letterAngles[random(0,26)].angles[7]);
      currentElement++;
    }
  }
}