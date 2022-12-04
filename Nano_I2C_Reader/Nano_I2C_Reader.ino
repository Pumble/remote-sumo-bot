/*
 * REFERENCES
 * Arduino Nano i2c: https://docs.arduino.cc/tutorials/nano-every/i2c
 */

// INCLUDES AGREGADOS
#include <Wire.h>

// VARIABLES PARA EL SLINGSHOT
#define SLINGSHOT_RELOAD 'Q'
#define SLINGSHOT_RELEASE 'W'
#define SLINGSHOT_DEVICE 8
char SLINGSHOT_STATE = 0;  // 0: reloading, 1: released

void setup() {
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);

  setupSlingshot();
}

void loop() {
  // delay(300);
  // digitalWrite(LED_BUILTIN, HIGH);
  // delay(300);
  // digitalWrite(LED_BUILTIN, LOW);
}

void processCommand(int howMany) {
  char input = Wire.read();  // receive a character
  Serial.println((char)input);

  switch (input) {
    case SLINGSHOT_RELOAD:
      digitalWrite(LED_BUILTIN, LOW);
      SLINGSHOT_STATE = 0;
      break;
    case SLINGSHOT_RELEASE:
      digitalWrite(LED_BUILTIN, HIGH);
      SLINGSHOT_STATE = 1;
      break;
  }
}

/* =========================== SETUP FUNCTIONS =========================== */
void setupSlingshot() {
  Wire.begin(SLINGSHOT_DEVICE);     // join i2c bus with address #8
  Wire.onReceive(processCommand);   // function that executes whenever data is received from writer
  Serial.print("Slingshot setup completed");
}
