// Definir el pin donde está conectada la salida digital del sensor de lluvia
const int sensorPin = 35;  // Pin GPIO 25 en el ESP32
int sensorValue = 0;       // Variable para almacenar el valor leído

void setup() {
  // Configurar el pin como entrada
  pinMode(sensorPin, INPUT);
  
  // Iniciar la comunicación serie para monitorear los valores en el Monitor Serial
  Serial.begin(9600);
}

void loop() {
  // Leer el valor del sensor (0 o 1)
  sensorValue = digitalRead(sensorPin);

  // Mostrar si hay lluvia o no en el Monitor Serial
  if (sensorValue == 1) {
    Serial.println("No se detecta lluvia.");
  } else {
    Serial.println("¡Se detecta lluvia!");
  }

  // Pausa de medio segundo antes de la siguiente lectura
  delay(2000);
}
