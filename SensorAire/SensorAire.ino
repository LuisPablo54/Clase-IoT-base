// Autores:
// Luis Pablo López Iracheta - María José Gamba Santibáñez - Diego Mares Rodríguez
#define MQ135_PIN 35  // Pin ADC en la ESP32 conectado al MQ135

void setup() {
  Serial.begin(115200);
  analogReadResolution(12);  
}

void loop() {
  // Pimer valor = Concentracion de gaces 0 - 4095
  // Segundo valor = Caliadad del aire 0 - 3.3
  int valorADC = analogRead(MQ135_PIN);  
  float voltaje = (valorADC / 4095.0) * 3.3;  
  
 
  Serial.print("Concentracion de gaces: ");
  Serial.print(valorADC);
  Serial.print(" - Caliadad del aire: ");
  Serial.println(voltaje);

  delay(2000);
}

