// Autores:
// - Luis Pablo López Iracheta
// - María José Gamba Santibáñez
//  - Diego Mares Rodríguez

#include <Wire.h>
int sensorPIR = 2;   // Pin digital donde está conectado el PIR
int estadoPIR = 0;   // Variable para almacenar el estado del PIR

void setup() {
  pinMode(sensorPIR, INPUT);  // Configura el pin del sensor PIR como entrada
  Serial.begin(9600);         // Inicializa la comunicación serial
}

void loop() {
  estadoPIR = digitalRead(sensorPIR);  // Lee el estado del PIR

  if (estadoPIR == HIGH) {  // Si se detecta movimiento
    Serial.println("Movimiento detectado");  // Envía un mensaje al puerto serial
    delay(1000);  // Espera 1 segundo para evitar múltiples detecciones
  }
  delay(1000);
}
