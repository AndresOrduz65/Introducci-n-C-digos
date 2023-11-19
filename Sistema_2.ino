// Definir las constantes de los pines y otros valores
const byte TRIG_PIN = 11; // Pin del sensor para enviar el pulso ultrasónico
const byte ECHO_PIN = 12; // Pin del sensor para recibir el eco
const byte MOTOR_PIN = 10; // Pin del motor para encenderlo
const byte LED_VERDE = 9; // Pin del LED verde para indicar que hay espacio
const byte LED_ROJO = 8; // Pin del LED rojo para indicar que no hay espacio
const byte LED_NARANJA = 7; // Pin del LED naranja para indicar que hay poco espacio
const float SOUND_SPEED = 0.034; // Velocidad del sonido en cm/us
const byte DISTANCIA_MIN = 12; // Distancia mínima para encender el motor
const byte DISTANCIA_MAX = 5; // Distancia máxima para apagar el LED verde

// Definir una variable para almacenar la distancia
float distancia;

void setup() {
  Serial.begin(9600); // Inicializar el monitor serie a 9600 baudios
  pinMode(TRIG_PIN, OUTPUT); // Configurar el pin del sensor como salida
  pinMode(ECHO_PIN, INPUT); // Configurar el pin del sensor como entrada
  pinMode(MOTOR_PIN, OUTPUT); // Configurar el pin del motor como salida
  pinMode(LED_VERDE, OUTPUT); // Configurar el pin del LED verde como salida
  pinMode(LED_ROJO, OUTPUT); // Configurar el pin del LED rojo como salida
  pinMode(LED_NARANJA, OUTPUT); // Configurar el pin del LED naranja como salida
}

void loop() {
  distancia = medirDistancia(); // Medir la distancia con el sensor
  mostrarDistancia(); // Mostrar la distancia en el monitor serie
  controlarMotor(); // Controlar el motor según la distancia
  controlarLEDs(); // Controlar los LEDs según la distancia
  delay(1000); // Esperar un segundo antes de repetir el ciclo
}

// Función para medir la distancia con el sensor
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
  if (distancia <= DISTANCIA_MAX) {
    digitalWrite(LED_VERDE, HIGH);
    digitalWrite(LED_ROJO, LOW);
    digitalWrite(LED_NARANJA, LOW);
  }
  // Si la distancia está entre la mínima y la máxima, apagar el LED rojo y encender el LED naranja
  else if (distancia >= DISTANCIA_MIN) {
    digitalWrite(LED_ROJO, HIGH);
    digitalWrite(LED_NARANJA, LOW);
    digitalWrite(LED_VERDE, LOW);
  }  
  // Si la distancia es mayor que la mínima, apagar el LED naranja y encender el LED verde
  else if(DISTANCIA_MAX < distancia && distancia < DISTANCIA_MIN){
    digitalWrite(LED_NARANJA, HIGH);
    digitalWrite(LED_VERDE, LOW);
    digitalWrite(LED_ROJO, LOW);
  }
}
