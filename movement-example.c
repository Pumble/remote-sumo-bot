/*CONFIGURACION DEL S4A EDU
 * Pines Motor Derecho:
 * D6 -> Velocidad
 * D11 -> Dirección de Giro
 *
 * Pines Motor Izquierdo:
 * D5 -> Velocidad
 * D10 -> Dirección de Giro
 */

//Variables
int MOTOR_LEFT_SPEED_PIN = 5, MOTOR_RIGHT_SPEED_PIN = 6;           //Pines de Velocidad
int MOTOR_LEFT_DIRECTION_PIN = 10, MOTOR_RIGHT_DIRECTION_PIN = 11; //Pines de Dirección de Giro
int vel = 255;                                                     //de 50 a 255 funciona bien

void setup()
{
   //Seteamos los motores
   pinMode(MOTOR_LEFT_SPEED_PIN, OUTPUT);
   pinMode(MOTOR_RIGHT_SPEED_PIN, OUTPUT);
   pinMode(MOTOR_LEFT_DIRECTION_PIN, OUTPUT);
   pinMode(MOTOR_RIGHT_DIRECTION_PIN, OUTPUT);
}


void loop()
{
   Probamos();
}


void Probamos()
{
   adelante();
   delay(1000);

   detener();
   delay(1000);

   atras();
   delay(1000);

   detener();
   delay(1000);

   izquierda();
   delay(1000);

   detener();
   delay(1000);

   derecha();
   delay(1000);

   detener();
   delay(1000);
}

//DIRECCIONES
void adelante()
{
   analogWrite(MOTOR_LEFT_SPEED_PIN, vel);
   digitalWrite(MOTOR_LEFT_DIRECTION_PIN, HIGH);
   analogWrite(MOTOR_RIGHT_SPEED_PIN, vel);
   digitalWrite(MOTOR_RIGHT_DIRECTION_PIN, HIGH);
}

void atras()
{
   analogWrite(MOTOR_LEFT_SPEED_PIN, vel);
   digitalWrite(MOTOR_LEFT_DIRECTION_PIN, LOW);
   analogWrite(MOTOR_RIGHT_SPEED_PIN, vel);
   digitalWrite(MOTOR_RIGHT_DIRECTION_PIN, LOW);
}

void detener()
{
   analogWrite(MOTOR_LEFT_SPEED_PIN, 0);
   analogWrite(MOTOR_RIGHT_SPEED_PIN, 0);
}

void izquierda()
{
   detener();
   analogWrite(MOTOR_LEFT_SPEED_PIN, vel);
   digitalWrite(MOTOR_LEFT_DIRECTION_PIN, HIGH);
}

void derecha()
{
   detener();
   analogWrite(MOTOR_RIGHT_SPEED_PIN, vel);
   digitalWrite(MOTOR_RIGHT_DIRECTION_PIN, HIGH);
}