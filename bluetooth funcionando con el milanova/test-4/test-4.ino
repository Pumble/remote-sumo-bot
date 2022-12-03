#include <SoftwareSerial.h>

/* Programa el modulo bluetooth HC-06 con un nuevo: 
  NOMBRE  (Nombre de 20 caracteres)
  PIN     (Clave de cuatro numeros)
  BPS     (Velocidad de conexion en baudios) //Mejor no cambiar

  CONEXIONES:
  ARDUINO   BLUETOOTH
  5V        VCC
  GND       GND
  PIN 1     TX
  PIN 0     RX
  
 */

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

// BUILT-IN LED
#define LED 13
#define BLINK_INTERVAL 300

int input;

void setup() {
  pinMode(LED, OUTPUT);

  Serial.begin(9600);
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

  // bt.print("AT+BAUD");  // Configura la nueva velocidad
  // bt.print(BT_BPS);
  // delay(1000);

  bt.print("AT+PIN");  // Configura el nuevo PIN
  bt.print(BT_PASSWORD);
  delay(1000);

  bt.flush();

  Serial.print("Bluetooth started at ");
  Serial.print(BT_BPS);
}

void loop() {
  while (bt.available() == 0) {}
  input = bt.read();
  Serial.println((char)input);

  switch (input) {
    case MOVEMENT_FORWARD:
      digitalWrite(LED, HIGH);
      break;
    case MOVEMENT_BACKWARD:
      digitalWrite(LED, LOW);
      break;
    case MOVEMENT_LEFT:
      digitalWrite(LED, LOW);
      break;
    case MOVEMENT_RIGHT:
      digitalWrite(LED, HIGH);
      break;
  }
}

void toogleLight(int times) {
  for (int i = 0; i < times; i++) {
    digitalWrite(LED, HIGH);
    delay(BLINK_INTERVAL);
    digitalWrite(LED, LOW);
    delay(BLINK_INTERVAL);
  }
}