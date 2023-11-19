#define BOTON_PIN 13

// Crear una variable booleana para guardar el estado
boolean estado = true;

void setup() {
  // Configurar el pin del pulsador como entrada con resistencia pull-up
  pinMode(BOTON_PIN, INPUT_PULLUP);
  // Iniciar la comunicación serial
  Serial.begin(9600);
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
    Serial.print(estado);
  }
}

