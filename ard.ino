  #include "HCPCA9685.h"
  #define I2CAdd 0x40

  HCPCA9685 HCPCA9685(I2CAdd);


  struct ServoAngles {
    uint8_t angles[8]; // 0-180 for each servo
  };
  ServoAngles letterAngles[27] = {
      {{0, 20, 40, 60, 80, 100, 120, 140}},   // a  
      {{10, 30, 50, 70, 90, 110, 130, 150}},  // b  
      {{20, 40, 60, 80, 100, 120, 140, 160}}, // c  
      {{30, 50, 70, 90, 110, 130, 150, 170}}, // d  
      {{40, 60, 80, 100, 120, 140, 160, 180}},// e  
      {{50, 70, 90, 110, 130, 150, 170, 190}},// f  
      {{60, 80, 100, 120, 140, 160, 180, 200}},// g  
      {{70, 90, 110, 130, 150, 170, 190, 210}},// h  
      {{80, 100, 120, 140, 160, 180, 200, 220}},// i  
      {{90, 110, 130, 150, 170, 190, 210, 230}},// j  
      {{100, 120, 140, 160, 180, 200, 220, 240}},// k  
      {{110, 130, 150, 170, 190, 210, 230, 250}},// l  
      {{120, 140, 160, 180, 200, 220, 240, 260}},// m  
      {{130, 150, 170, 190, 210, 230, 250, 270}},// n  
      {{140, 160, 180, 200, 220, 240, 260, 280}},// o  
      {{150, 170, 190, 210, 230, 250, 270, 290}},// p  
      {{160, 180, 200, 220, 240, 260, 280, 300}},// q  
      {{170, 190, 210, 230, 250, 270, 290, 310}},// r  
      {{180, 200, 220, 240, 260, 280, 300, 320}},// s  
      {{190, 210, 230, 250, 270, 290, 310, 330}},// t  
      {{200, 220, 240, 260, 280, 300, 320, 340}},// u  
      {{210, 230, 250, 270, 290, 310, 330, 350}},// v  
      {{220, 240, 260, 280, 300, 320, 340, 360}},// w  
      {{230, 250, 270, 290, 310, 330, 350, 360}},// x  
      {{240, 260, 280, 300, 320, 340, 360, 360}},// y  
      {{250, 270, 290, 310, 330, 350, 360, 360}},// z  
      {{260, 280, 300, 320, 340, 360, 360, 360}} // space  

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
        delay(1000);
      }
    }
  }