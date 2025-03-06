#include "HCPCA9685.h"
#define I2CAdd 0x40

HCPCA9685 HCPCA9685(I2CAdd);


struct ServoAngles {
  uint8_t angles[8]; // 0-180 for each servo
};
ServoAngles letterAngles[27] = {
  {{180, 90, 45, 30, 15, 60, 75, 10}},  // a
  {{170, 85, 40, 25, 20, 55, 80, 15}},  // b
  {{110, 95, 35, 20, 25, 50, 85, 20}},  // c
  {{10, 100, 30, 15, 30, 45, 90, 25}},  // d
  {{20, 105, 25, 10, 35, 40, 95, 30}},  // e
  {{30, 110, 20, 5, 40, 35, 100, 35}},  // f
  {{45, 115, 15, 10, 45, 30, 105, 40}}, // g
  {{60, 120, 10, 15, 50, 25, 110, 45}}, // h
  {{70, 125, 5, 20, 55, 20, 115, 50}},  // i
  {{80, 130, 10, 25, 60, 15, 120, 55}}, // j
  {{70, 135, 15, 30, 65, 10, 125, 60}}, // k
  {{80, 140, 20, 35, 70, 5, 130, 65}},  // l
  {{90, 145, 25, 40, 75, 10, 135, 70}}, // m
  {{0, 150, 30, 45, 80, 15, 140, 75}},  // n
  {{5, 155, 35, 50, 85, 20, 145, 80}},  // o
  {{10, 160, 40, 55, 90, 25, 150, 85}}, // p
  {{15, 165, 45, 60, 95, 30, 155, 90}}, // q
  {{20, 170, 50, 65, 100, 35, 160, 95}},// r
  {{25, 175, 55, 70, 105, 40, 165, 100}},// s
  {{30, 180, 60, 75, 110, 45, 170, 105}},// t
  {{35, 185, 65, 80, 115, 50, 175, 110}},// u
  {{40, 190, 70, 85, 120, 55, 180, 115}},// v
  {{45, 195, 75, 90, 125, 60, 185, 120}},// w
  {{50, 200, 80, 95, 130, 65, 190, 125}},// x
  {{55, 205, 85, 100, 135, 70, 195, 130}},// y 
  {{60, 210, 90, 105, 140, 75, 200, 135}},// z
  {{65, 215, 95, 110, 145, 80, 205, 140}} // space
};



const int BUFFER_SIZE = 200;
char inputBuffer[BUFFER_SIZE];
int bufferIndex = 0;
bool stringComplete = false;

unsigned long previousMillis = 0;
const long interval = 500;  // Non-blocking delay interval
int currentElement = 0;
int elementCount = 0;
char* elements[100];  // Max 100 elements

void setup() {
  Serial.begin(115200);
  HCPCA9685.Init(SERVO_MODE);
  HCPCA9685.Sleep(false);
  memset(inputBuffer, 0, BUFFER_SIZE);
  randomSeed(analogRead(0));
  
  

  //SETUP ANGLES. TESTING NEEDED.
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
    // Skip processing if input is "121"
    if (strcmp(inputBuffer, "121") != 0) {
      Serial.print("Received: ");
      Serial.println(inputBuffer);

      // Split input into elements
      elementCount = 0;
      char* token = strtok(inputBuffer, ",");
      while (token != NULL && elementCount < 100) {
        elements[elementCount++] = token;
        token = strtok(NULL, ",");
      }

      currentElement = 0;  
    }

    bufferIndex = 0;
    stringComplete = false;
    memset(inputBuffer, 0, BUFFER_SIZE);
  }

  if (currentElement < elementCount) {
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= interval) {
      previousMillis = currentMillis;

      char* element = elements[currentElement];
      int angle = random(0, 180); 
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