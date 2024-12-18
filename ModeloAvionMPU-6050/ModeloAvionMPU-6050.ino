#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

// Definición de pines para los LEDs
const int ledSur = 27;    // Sur - Inclinación negativa en Y
const int ledCentro = 26; // Centro - Posición centrada
const int ledEste = 25;   // Este - Inclinación positiva en X
const int ledOeste = 32;  // Oeste - Inclinación negativa en X
const int ledNorte = 33;  // Norte - Inclinación positiva en Y

// MPU-6050
Adafruit_MPU6050 mpu;

void setup() {
  Serial.begin(115200);

  // Configuración de pines como salida
  pinMode(ledSur, OUTPUT);
  pinMode(ledCentro, OUTPUT);
  pinMode(ledEste, OUTPUT);
  pinMode(ledOeste, OUTPUT);
  pinMode(ledNorte, OUTPUT);

  // Inicializar el MPU-6050
  if (!mpu.begin()) {
    Serial.println("Error al detectar el MPU-6050. Verifica conexiones.");
    while (1);
  }
  Serial.println("MPU-6050 listo.");

  // Configuración del MPU-6050
  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);

  delay(100);
}

void loop() {
  // Obtener datos del acelerómetro
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  // Limpiar todos los LEDs
  apagarTodos();

  // Evaluar inclinación en X (Este-Oeste) y Y (Norte-Sur)
  if (a.acceleration.y > 3) {
    digitalWrite(ledNorte, HIGH); // Inclinación hacia el norte
    Serial.println("Norte");
  } else if (a.acceleration.y < -3) {
    digitalWrite(ledSur, HIGH); // Inclinación hacia el sur
    Serial.println("Sur");
  } else if (a.acceleration.x > 3) {
    digitalWrite(ledEste, HIGH); // Inclinación hacia el este
    Serial.println("Este");
  } else if (a.acceleration.x < -3) {
    digitalWrite(ledOeste, HIGH); // Inclinación hacia el oeste
    Serial.println("Oeste");
  } else {
    digitalWrite(ledCentro, HIGH); // Módulo centrado
    Serial.println("Centrado");
  }

  // Mostrar valores en el monitor serial (para depuración)
  Serial.print("X: ");
  Serial.print(a.acceleration.x);
  Serial.print(" | Y: ");
  Serial.print(a.acceleration.y);
  Serial.print(" | Z: ");
  Serial.println(a.acceleration.z);

  delay(300); // Pequeña pausa para evitar parpadeos rápidos
}

void apagarTodos() {
  digitalWrite(ledSur, LOW);
  digitalWrite(ledCentro, LOW);
  digitalWrite(ledEste, LOW);
  digitalWrite(ledOeste, LOW);
  digitalWrite(ledNorte, LOW);
}
