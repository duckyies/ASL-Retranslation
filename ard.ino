  #include "HCPCA9685.h"
  #define I2CAdd 0x40

  HCPCA9685 HCPCA9685(I2CAdd);


  struct ServoAngles {
    uint8_t angles[8]; // 0-180 for each servo
  };
  ServoAngles letterAngles[26][8] = {{0,360,0,360,360,360,360,80},
  {360,0,360,0,0,0,0,80},
  {180,180,360,180,180,180,180,80},
  {0,360,360,270,0,360,360,80},
  {90,270,360,270,270,270,270,80},
  {360,0,360,0,270,0,0,80},
  {0,0,0,360,0,0,360,120},
  {0,0,360,0,0,0,360,120},
  {360,360,360,360,360,360,360,80},
  {360,360,360,360,360,360,360,120},
  {0,0,0,0,0,0,360,80},
  {0,0,0,360,0,0,360,80},
  {0,0,360,270,270,0,270,80},
  {0,0,360,270,270,0,360,80},
  {90,0,360,270,270,0,270,120},
  {0,0,360,270,0,0,360,120},
  {0,0,360,0,0,0,360,120},{
    0,0,360,0,180,0,360,80},
    {0,360,360,360,360,360,360,80},{0,0,360,180,250,0,360,80},{0,0,360,0,0,0,360,80},{0,0,360,0,0,0,0,80},{0,0,360,360,180,0,360,80},{360,0,0,360,360,0,360,80},{0,360,360,360,0,360,360,120}};



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