/*
 * REFERENCES
 * Arduino Nano i2c: https://docs.arduino.cc/tutorials/nano-every/i2c
 * Servos: https://programarfacil.com/blog/arduino-blog/servomotor-con-arduino/
 * Stepper Motor: https://programarfacil.com/blog/arduino-blog/motor-paso-a-paso-uln2003-l298n/
 */

// INCLUDES AGREGADOS
#include <Wire.h>
#include <Servo.h>

// VARIABLES PARA EL SLINGSHOT
#define SLINGSHOT_RELOAD 'Q'   // 'Q': reloading
#define SLINGSHOT_RELEASE 'W'  // 'W': released,
#define SLINGSHOT_SECURE 'E'   // 'E': secured
#define SLINGSHOT_DEVICE 8
char SLINGSHOT_STATE = 'E';
char SLINGSHOT_PREVIOUS_STATE = 'E';

// VARIABLES PARA EL SERVO
Servo SERVO_MOTOR;
#define SERVO_PIN 2
#define SERVO_LOCK_ANGLE 0
#define SERVO_RELEASED_ANGLE 120

// VARIABLES PARA EL MOTOR STEPPER
// SM_PIN: Stepper Motor Pin
#define SM_PIN_1 8
#define SM_PIN_2 9
#define SM_PIN_3 10
#define SM_PIN_4 11
int STEP[4][4] = {
  { 1, 1, 0, 0 },
  { 0, 1, 1, 0 },
  { 0, 0, 1, 1 },
  { 1, 0, 0, 1 }
};
int STEP_REVERSE[4][4] = {
  { 1, 0, 0, 1 },
  { 0, 0, 1, 1 },
  { 0, 1, 1, 0 },
  { 1, 1, 0, 0 }  
};

void setup() {
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);

  setupSlingshot();
  setupServo();
  setupStepperMotor();
}

void loop() {
  if (SLINGSHOT_STATE != SLINGSHOT_PREVIOUS_STATE) {
    setSecureState();
  }

  for (int i = 0; i < 4; i++) {
    digitalWrite(SM_PIN_1, STEP_REVERSE[i][0]);
    digitalWrite(SM_PIN_2, STEP_REVERSE[i][1]);
    digitalWrite(SM_PIN_3, STEP_REVERSE[i][2]);
    digitalWrite(SM_PIN_4, STEP_REVERSE[i][3]);
    delay(10);
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

void setupStepperMotor() {
  pinMode(SM_PIN_1, OUTPUT);
  pinMode(SM_PIN_2, OUTPUT);
  pinMode(SM_PIN_3, OUTPUT);
  pinMode(SM_PIN_4, OUTPUT);

  Serial.println("Stepper motor setup completed");
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
  SLINGSHOT_PREVIOUS_STATE = SLINGSHOT_STATE;
  delay(1000);
}