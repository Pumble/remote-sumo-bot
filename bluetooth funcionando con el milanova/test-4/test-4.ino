/*
 * REFERENCES
 * DC motor: https://www.tutorialspoint.com/arduino/arduino_dc_motor.htm
 * Arduino Nano i2c: https://docs.arduino.cc/tutorials/nano-every/i2c
 * Duemilanove Docs: https://docs.arduino.cc/retired/boards/arduino-duemilanove
 */

// INCLUDES AGREGADOS
#include <SoftwareSerial.h>
#include <Wire.h>

// VARIABLES PARA EL SETUP DEL Bluetooth
char BT_NAME[21] = "hentaiGratis";  // Nombre de 20 caracteres maximo+
char BT_PASSWORD[5] = "1234";       // PIN O CLAVE de 4 caracteres numericos
#define BT_BPS 19200                // 1=1200 , 2=2400, 3=4800, 4=9600, 5=19200, 6=38400, 7=57600, 8=115200
#define BT_TX_PIN 2
#define BT_RX_PIN 3
SoftwareSerial bt(BT_TX_PIN, BT_RX_PIN);  //Crea conexion al bluetooth - PIN 1 a TX y PIN 0 a RX

#define MOVEMENT_FORWARD 'F'
#define MOVEMENT_BACKWARD 'B'
#define MOVEMENT_LEFT 'L'
#define MOVEMENT_RIGHT 'R'
#define MOVEMENT_STOP 'S'
#define GEAR_UP '+'
#define GEAR_DOWN '-'

// BUILT-IN LED
#define LED 13
#define BLINK_INTERVAL 300

// VARIABLES PARA EL SETUP DE LOS MOTORES
#define MOTOR_LEFT_SPEED_PIN 5
#define MOTOR_LEFT_DIRECTION_PIN 10
#define MOTOR_RIGHT_SPEED_PIN 6
#define MOTOR_RIGHT_DIRECTION_PIN 11
#define SPEED_MULTIPLIER 51
int gear = 3;
int speed = 0;

// VARIABLES PARA LA RESORTERA
#define SLINGSHOT_RELOAD 'Q'   // 'R': reloading
#define SLINGSHOT_RELEASE 'W'  // 'W': released,
#define SLINGSHOT_SECURE 'E'   // 'E': secured
#define SLINGSHOT_DEVICE 8

void setup() {
  Serial.begin(9600);
  pinMode(LED, OUTPUT);

  bluetoothSetup();
  motorsSetup();
  slingshotSetup();
}

void loop() {
  while (bt.available() == 0) {}
  int input = bt.read();
  Serial.println((char)input);

  processCommand(input);
}

/* =========================== DEBUG FUNCTIONS =========================== */
void toogleLight(int times) {
  for (int i = 0; i < times; i++) {
    digitalWrite(LED, HIGH);
    delay(BLINK_INTERVAL);
    digitalWrite(LED, LOW);
    delay(BLINK_INTERVAL);
  }
}

/* =========================== SETUP FUNCTIONS =========================== */
void bluetoothSetup() {
  bt.begin(BT_BPS);  // inicialmente la comunicacion serial a 9600 Baudios (velocidad de fabrica) MEJOR NO CAMBIAR

  bt.print("AT");  // Inicializa comando AT
  delay(1000);

  bt.print("AT+RESET");  // Configura el nuevo nombre
  delay(1000);           // espera 1 segundo

  Serial.print("Version: ");
  Serial.println(bt.print("AT+VERSION"));  // Configura el nuevo nombre
  delay(1000);                             // espera 1 segundo

  bt.print("AT+NAME");  // Configura el nuevo nombre
  bt.print(BT_NAME);
  delay(1000);  // espera 1 segundo

  bt.print("AT+PIN");  // Configura el nuevo PIN
  bt.print(BT_PASSWORD);
  delay(1000);

  bt.flush();

  Serial.print("Bluetooth setup completed: ");
  Serial.println(BT_BPS);
}

void motorsSetup() {
  speed = gear * SPEED_MULTIPLIER;

  pinMode(MOTOR_LEFT_SPEED_PIN, OUTPUT);
  pinMode(MOTOR_LEFT_DIRECTION_PIN, OUTPUT);

  pinMode(MOTOR_RIGHT_SPEED_PIN, OUTPUT);
  pinMode(MOTOR_RIGHT_DIRECTION_PIN, OUTPUT);

  Serial.println("Motor setup completed");
}

void slingshotSetup() {
  while (!Serial) {}
  Wire.begin();

  Serial.println("Slingshot setup completed");
}

/* =========================== BLUETOOTH FUNCTIONS =========================== */
void processCommand(int input) {
  switch (input) {
    case MOVEMENT_FORWARD:
      digitalWrite(LED, HIGH);
      moveForward();
      break;
    case MOVEMENT_BACKWARD:
      digitalWrite(LED, LOW);
      moveBackward();
      break;
    case MOVEMENT_LEFT:
      digitalWrite(LED, LOW);
      stopMotors();
      rotateLeft();
      break;
    case MOVEMENT_RIGHT:
      digitalWrite(LED, HIGH);
      stopMotors();
      rotateRight();
      break;
    case MOVEMENT_STOP:
      stopMotors();
      break;
    case GEAR_UP:
      gearUp();
      break;
    case GEAR_DOWN:
      gearDown();
      break;
    case SLINGSHOT_RELOAD:
    case SLINGSHOT_RELEASE:
    case SLINGSHOT_SECURE:
      slingshotState(input);
      break;
  }
}

/* =========================== MOTOR FUNCTIONS =========================== */
void stopMotors() {
  digitalWrite(MOTOR_LEFT_SPEED_PIN, LOW);
  digitalWrite(MOTOR_LEFT_DIRECTION_PIN, LOW);

  digitalWrite(MOTOR_RIGHT_SPEED_PIN, LOW);
  digitalWrite(MOTOR_RIGHT_DIRECTION_PIN, LOW);
}

void moveForward() {
  Serial.print("Speed: ");
  Serial.println(speed);

  analogWrite(MOTOR_LEFT_SPEED_PIN, speed);
  digitalWrite(MOTOR_LEFT_DIRECTION_PIN, HIGH);

  analogWrite(MOTOR_RIGHT_SPEED_PIN, speed);
  digitalWrite(MOTOR_RIGHT_DIRECTION_PIN, HIGH);
}

void moveBackward() {
  Serial.print("Speed: ");
  Serial.println(speed);

  analogWrite(MOTOR_LEFT_SPEED_PIN, speed);
  digitalWrite(MOTOR_LEFT_DIRECTION_PIN, LOW);

  analogWrite(MOTOR_RIGHT_SPEED_PIN, speed);
  digitalWrite(MOTOR_RIGHT_DIRECTION_PIN, LOW);
}

void rotateLeft() {
  analogWrite(MOTOR_LEFT_SPEED_PIN, speed);
  digitalWrite(MOTOR_LEFT_DIRECTION_PIN, LOW);

  analogWrite(MOTOR_RIGHT_SPEED_PIN, speed);
  digitalWrite(MOTOR_RIGHT_DIRECTION_PIN, HIGH);
}

void rotateRight() {
  analogWrite(MOTOR_RIGHT_SPEED_PIN, speed);
  digitalWrite(MOTOR_RIGHT_DIRECTION_PIN, LOW);

  analogWrite(MOTOR_LEFT_SPEED_PIN, speed);
  digitalWrite(MOTOR_LEFT_DIRECTION_PIN, HIGH);
}

void gearUp() {
  if (gear >= 5) {
    gear = 5;
  } else {
    gear++;
  }
  speed = gear * SPEED_MULTIPLIER;
  Serial.print("Currrent gear: ");
  Serial.println(gear);
}

void gearDown() {
  if (gear <= 1) {
    gear = 1;
  } else {
    gear--;
  }
  speed = gear * SPEED_MULTIPLIER;
  Serial.print("Currrent gear: ");
  Serial.println(gear);
}

/* =========================== SLINGSHOT FUNCTIONS =========================== */
void slingshotState(char input) {
  Wire.beginTransmission(SLINGSHOT_DEVICE);  // transmit to device #8
  Wire.write(input);                         // sends the given value
  Wire.endTransmission();
}