
// Librerias a utilizar
#include <WiFi.h> // libreria módulo ESP32
 
void setup()
{
  Serial.begin(9600);
  WiFi.mode(WIFI_STA); // modo de operación como estación
  delay(100);
  Serial.flush(); // limpia buffer serial
}
 
void loop()
{
  int n = WiFi.scanNetworks(); // busca si hay redes
  // si no hay (n=0)
  if (n == 0)
      {
        Serial.println("no hay Redes disponibles"); 
      }
  else
      {
        Serial.println("Redes encontradas:  ");
        delay(2000);
        for (int i = 0; i < n; ++i)
      {
      // Muestra SSID y RSSI de cada red encontrada
      Serial.print(i + 1); // enumera las redes 
      Serial.print(": ");
      Serial.print(WiFi.SSID(i)); // muestra el nombre 
      Serial.print(" (");
      Serial.print(WiFi.RSSI(i)); // muestra la señal en dBm
      Serial.println(")");
      delay(1000); // pausa antes de mostrar otra red
    }
  }
  // Espera 5 segundos antes de volver a buscar
  delay(5000);
}
