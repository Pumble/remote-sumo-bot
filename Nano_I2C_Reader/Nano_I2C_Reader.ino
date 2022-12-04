/*
 * REFERENCES
 * Arduino Nano i2c: https://docs.arduino.cc/tutorials/nano-every/i2c
 * Servos: https://programarfacil.com/blog/arduino-blog/servomotor-con-arduino/
 */

// INCLUDES AGREGADOS
#include <Wire.h>
#include <Servo.h>

// VARIABLES PARA EL SLINGSHOT
#define SLINGSHOT_RELOAD 'Q'   // 'R': reloading
#define SLINGSHOT_RELEASE 'W'  // 'W': released,
#define SLINGSHOT_SECURE 'E'   // 'E': secured
#define SLINGSHOT_DEVICE 8
char SLINGSHOT_STATE = 'E';
char SLINGSHOT_PREVIOUS_STATE = NULL;

// VARIABLES PARA EL SERVO
Servo SERVO_MOTOR;
#define SERVO_PIN 2
#define SERVO_LOCK_ANGLE 0
#define SERVO_RELEASED_ANGLE 120

void setup() {
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);

  setupSlingshot();
  setupServo();
}

void loop() {
  if (SLINGSHOT_STATE != SLINGSHOT_PREVIOUS_STATE) {
    setSecureState();
    SLINGSHOT_PREVIOUS_STATE = SLINGSHOT_STATE;
  }
}

/* =========================== SETUP FUNCTIONS =========================== */
void setupSlingshot() {
  Wire.begin(SLINGSHOT_DEVICE);    // join i2c bus with address #8
  Wire.onReceive(processCommand);  // function that executes whenever data is received from writer
  Serial.println("Slingshot setup completed");
}

void setupServo() {
  SERVO_MOTOR.attach(SERVO_PIN);  // asignamos el pin 3 al servo.
  setSecureState();
  Serial.println("Servo setup completed");
}

/* =========================== COMMUNICATION FUNCTIONS =========================== */
void processCommand(int howMany) {
  char input = Wire.read();  // receive a character
  Serial.println((char)input);

  SLINGSHOT_STATE = input;
}

/* =========================== SERVO FUNCTIONS =========================== */
void setSecureState() {
  Serial.print("SLINGSHOT_STATE: ");
  Serial.println(SLINGSHOT_STATE);

  switch (SLINGSHOT_STATE) {
    case SLINGSHOT_RELOAD:
      digitalWrite(LED_BUILTIN, HIGH);
      SERVO_MOTOR.write(90);
      break;
    case SLINGSHOT_RELEASE:
      digitalWrite(LED_BUILTIN, HIGH);
      SERVO_MOTOR.write(120);
      break;
    case SLINGSHOT_SECURE:
      digitalWrite(LED_BUILTIN, LOW);
      SERVO_MOTOR.write(0);
      break;
  }
  delay(1000);
}