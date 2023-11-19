#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>

const byte SENSOR1_PIN = A0;
const byte SENSOR2_PIN = A1;
const byte DHTTYPE = DHT11;
const byte DHTPIN1 = 2;
const byte DHTPIN2 = 3;
const byte HUMEDAD_MIN = 50;
const byte MOTOR_PIN = 4;
const byte LED_ROJO = 5;
const byte LED_VERDE = 6;

DHT dht1(DHTPIN1, DHTTYPE);
DHT dht2(DHTPIN2, DHTTYPE);
LiquidCrystal_I2C lcd(0x27,16,2);

void setup() {
  
  Serial.begin(9600);
 
  pinMode(SENSOR1_PIN, INPUT);
  pinMode(SENSOR2_PIN, INPUT);

  pinMode(MOTOR_PIN, OUTPUT);
  pinMode(LED_ROJO, OUTPUT);
  pinMode(LED_VERDE, OUTPUT);

  dht1.begin();
  dht2.begin();

  Wire.begin();  
  lcd.begin(16, 2);
  lcd.clear();
  lcd.backlight();   
}

void loop() {

  int humedad = medirHumedad();

  float temperatura = medirTemperatura();

  mostrarDatos(humedad, temperatura);

  controlarMotor(humedad);
  controlarLEDs(humedad);

  delay(1000);
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