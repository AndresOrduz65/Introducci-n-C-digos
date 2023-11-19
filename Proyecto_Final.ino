#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>

const byte DHTTYPE = DHT11;
const byte SENSOR1_PIN = A0;
const byte SENSOR2_PIN = A1;
const byte DHTPIN1 = 2;
const byte DHTPIN2 = 3;
const byte MOTOR1_PIN = 4;
const byte LED_ROJO1 = 5;
const byte LED_VERDE1 = 6;
const byte TRIG_PIN = 11; // Pin del sensor para enviar el pulso ultrasónico
const byte ECHO_PIN = 12; // Pin del sensor para recibir el eco
const byte MOTOR_PIN = 10; // Pin del motor para encenderlo
const byte LED_VERDE = 9; // Pin del LED verde para indicar que hay espacio
const byte LED_ROJO = 8; // Pin del LED rojo para indicar que no hay espacio
const byte LED_NARANJA = 7; // Pin del LED naranja para indicar que hay poco espacio
#define BOTON_PIN 13

const float SOUND_SPEED = 0.034; // Velocidad del sonido en cm/us
const byte DISTANCIA_MIN = 12; // Distancia mínima para encender el motor
const byte DISTANCIA_MAX = 5; // Distancia máxima para apagar el LED verde
const byte HUMEDAD_MIN = 50;

float distancia; // Definir una variable para almacenar la distancia

boolean estado = true; // Crear una variable booleana para guardar el estado

DHT dht1(DHTPIN1, DHTTYPE);
DHT dht2(DHTPIN2, DHTTYPE);
LiquidCrystal_I2C lcd(0x27,16,2);

void setup() {
  
  Serial.begin(9600);
 
  pinMode(SENSOR1_PIN, INPUT);
  pinMode(SENSOR2_PIN, INPUT);

  pinMode(MOTOR1_PIN, OUTPUT);
  pinMode(LED_ROJO1, OUTPUT);
  pinMode(LED_VERDE1, OUTPUT);
  pinMode(TRIG_PIN, OUTPUT); // Configurar el pin del sensor como salida
  pinMode(ECHO_PIN, INPUT); // Configurar el pin del sensor como entrada
  pinMode(MOTOR_PIN, OUTPUT); // Configurar el pin del motor como salida
  pinMode(LED_VERDE, OUTPUT); // Configurar el pin del LED verde como salida
  pinMode(LED_ROJO, OUTPUT); // Configurar el pin del LED rojo como salida
  pinMode(LED_NARANJA, OUTPUT); // Configurar el pin del LED naranja como salida
  pinMode(BOTON_PIN, INPUT_PULLUP);   // Configurar el pin del pulsador como entrada con resistencia pull-up

  dht1.begin();
  dht2.begin();

  Wire.begin();  
  lcd.begin(16, 2);
  lcd.clear();
  lcd.backlight();

}

int medirHumedad() {

  int valor1 = analogRead(SENSOR1_PIN);
  int valor2 = analogRead(SENSOR2_PIN);

  int humedadP = (valor1 + valor2) / 2;

  int humedad = map(humedadP, 400, 1013, 100, 0);

  return humedad;
}


float medirTemperatura() {

  float temp1 = dht1.readTemperature();
  float temp2 = dht2.readTemperature();

  float temperatura = (temp1 + temp2) / 2;

  return temperatura;
}

void mostrarDatos(int humedad, float temperatura) {

  lcd.setCursor(0, 0);

  lcd.print("Humedad: ");
  lcd.print(humedad);
  lcd.print("%");

  lcd.setCursor(0, 1);

  lcd.print("Temp: ");
  lcd.print(temperatura);
  lcd.print(" 'C");
}

void controlarMotor(int humedad) {

  if (humedad < HUMEDAD_MIN) {

    digitalWrite(LED_ROJO, HIGH);

    digitalWrite(MOTOR_PIN, HIGH);

    delay(10000);

    digitalWrite(MOTOR_PIN, LOW);

    digitalWrite(LED_ROJO, LOW);
  }
}

void controlarLEDs(int humedad) {

  if (humedad > HUMEDAD_MIN) {

    digitalWrite(LED_VERDE, HIGH);
  }

  else {

    digitalWrite(LED_VERDE, LOW);
  }
}

float medirDistancia() {
  // Enviar un pulso de 10 us al pin TRIG
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // Medir el tiempo que tarda el eco en llegar al pin ECHO
  long duracion = pulseIn(ECHO_PIN, HIGH);

  // Calcular la distancia en cm usando la fórmula d = v * t / 2
  float distancia = duracion * SOUND_SPEED / 2;

  // Devolver la distancia
  return distancia;
}

// Función para mostrar la distancia en el monitor serie
void mostrarDistancia() {
  // Imprimir la distancia en el monitor serie
  Serial.print("Distancia: ");
  Serial.print(distancia);
  Serial.println(" cm");
}

// Función para controlar el motor según la distancia
void controlarMotor() {
  // Si la distancia es menor que la mínima, encender el motor
  if (distancia < DISTANCIA_MIN) {
    digitalWrite(MOTOR_PIN, HIGH);
  }
  // Si no, apagar el motor
  else {
    digitalWrite(MOTOR_PIN, LOW);
  }
}

// Función para controlar los LEDs según la distancia
void controlarLEDs() {
  // Si la distancia es menor que la máxima, apagar el LED verde y encender el LED rojo
  if (distancia < DISTANCIA_MAX) {
    digitalWrite(LED_VERDE, LOW);
    digitalWrite(LED_ROJO, HIGH);
    digitalWrite(LED_NARANJA, LOW);
  }
  // Si la distancia está entre la mínima y la máxima, apagar el LED rojo y encender el LED naranja
  else if (distancia < DISTANCIA_MIN) {
    digitalWrite(LED_ROJO, LOW);
    digitalWrite(LED_NARANJA, HIGH);
    digitalWrite(LED_VERDE, LOW);
  }
  // Si la distancia es mayor que la mínima, apagar el LED naranja y encender el LED verde
  else {
    digitalWrite(LED_NARANJA, LOW);
    digitalWrite(LED_VERDE, HIGH);
    digitalWrite(LED_ROJO, LOW);
  }
}


void system1(){
    int humedad = medirHumedad();

  float temperatura = medirTemperatura();

  mostrarDatos(humedad, temperatura);

  controlarMotor(humedad);
  controlarLEDs(humedad);

  delay(1000);
}

void System2(){
  distancia = medirDistancia(); // Medir la distancia con el sensor
  mostrarDistancia(); // Mostrar la distancia en el monitor serie
  controlarMotor(); // Controlar el motor según la distancia
  controlarLEDs(); // Controlar los LEDs según la distancia
  delay(1000); // Esperar un segundo antes de repetir el ciclo
}

void loop() {
  // Leer el valor del pin del pulsador
  boolean boton = digitalRead(BOTON_PIN);
  // Si el pulsador está presionado
  if (boton == LOW) {
    // Cambiar el valor de estado
    estado = !estado;
    // Esperar un tiempo para evitar rebotes
    delay(100);
  }

  if (estado==1){
    void system1();
  }
  else if(estado==0){
    void system2();
  }
}


