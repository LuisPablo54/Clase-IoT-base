// Autores:
// - Luis Pablo López Iracheta
// - María José Gamba Santibáñez
// - Diego Mares Rodríguez
// - Mateo Serrato Ascencio


#include "DHT.h"

// Definir el pin del sensor DHT y del LED
#define DHTPIN 33  
#define LEDPIN 32  


#define DHTTYPE DHT11  

// Crear una instancia del sensor DHT
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  // Iniciar comunicación serial
  Serial.begin(115200);
  
  // Iniciar el sensor DHT
  dht.begin();

  // Configurar el pin del LED como salida
  pinMode(LEDPIN, OUTPUT);

  // Apagar el LED inicialmente
  digitalWrite(LEDPIN, LOW);
}

void loop() {
  // Esperar 2 segundos entre lecturas
  delay(2000);

  // Leer la temperatura en grados Celsius
  float temperatura = dht.readTemperature();
  
  // Verificar si hay errores en la lectura del sensor
  if (isnan(temperatura)) {
    Serial.println("Error al leer el sensor DHT");
    return;
  }

  // Mostrar la temperatura en el monitor serial
  Serial.print("Temperatura: ");
  Serial.print(temperatura);
  Serial.println(" °C");

  // Encender el LED si la temperatura es mayor a 26°C
  if (temperatura > 26) {
    digitalWrite(LEDPIN, HIGH);  // Encender el LED
    Serial.println("LED encendido");
  }
  // Apagar el LED si la temperatura es menor a 23°C
  else if (temperatura < 23) {
    digitalWrite(LEDPIN, LOW);  // Apagar el LED
    Serial.println("LED apagado");
  }
}
