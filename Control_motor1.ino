#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>
#define SENSOR1_PIN A0 
#define SENSOR2_PIN A1
#define DHTTYPE DHT11 // Tipo de sensor
#define DHTPIN1 2 // Pin 2 del primer sensor
#define DHTPIN2 3 // Pin 3 del segundo sensor
#define HUMEDAD_MIN 50
#define RELAY_PIN 12 // Pin del relé
DHT dht1(DHTPIN1, DHTTYPE); // Objeto para el primer sensor
DHT dht2(DHTPIN2, DHTTYPE); // Objeto para el segundo sensor

LiquidCrystal_I2C lcd(0x27,16,2);

void setup() {
  
  Serial.begin(9600);

  pinMode(SENSOR1_PIN, INPUT);
  pinMode(SENSOR2_PIN, INPUT);
  pinMode(RELAY_PIN, OUTPUT); // Configura el pin del relé como salida

  dht1.begin(); // Inicializa el primer sensor
  dht2.begin(); // Inicializa el segundo sensor

  Wire.begin();  
  lcd.begin(16, 2);
  lcd.clear();
  lcd.backlight();   

}
void encender_motor(){
  digitalWrite(RELAY_PIN, HIGH);  // Activa el relé y enciende el motor
  delay(2000);                      // Espera dos segundos
  digitalWrite(RELAY_PIN, LOW);  // Desactiva el relé y apaga el motor
}

void loop() {
  int valor1 = analogRead(SENSOR1_PIN);   // Lee los valores analógicos de los pines del sensor
  int valor2 = analogRead(SENSOR2_PIN);
  int HumedadP = (valor1 + valor2) / 2; // Calcula el promedio de los valores
  int H = map(HumedadP, 400, 1013, 100, 0); // Usa la función map para escalar el valor de 0 a 1023 a 0 a 100 para q el valor sea en %

  float temp1 = dht1.readTemperature(); // Lee la temperatura del primer sensor
  float temp2 = dht2.readTemperature(); // Lee la temperatura del segundo sensor
  float T = (temp1 + temp2) / 2; // Calcula el promedio de la temperatura

  lcd.setCursor(0, 0);  // Ubica el cursor en la primera posición (columna:0) de la segunda línea (fila:1)
  lcd.print("Humedad: ");  // Escribe el texto "Humedad: "
  lcd.print(H);  // Escribe el valor de la variable H
  lcd.print("%");  // Escribe el símbolo de porcentaje
  
  lcd.setCursor(0, 1);
  lcd.print("Temp: ");
  lcd.print(T);
  lcd.print(" 'C");

  delay(2000);  // Espera un segundo

  if (H<HUMEDAD_MIN){
    encender_motor(); // Llama a la función que enciende el motor
    } 
  else if (H>HUMEDAD_MIN){
    digitalWrite(RELAY_PIN, LOW); // Desactiva el relé
  }
}
