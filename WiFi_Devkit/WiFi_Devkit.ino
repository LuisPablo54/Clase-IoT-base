#include "WiFi.h"
const char ssid[] = "LuisEsGenial";   // Nombre de la red
const char pass[] = "yalose!!";       // Contraseña de la red
const char hostname[] = "Devkit de Luis/Maria/Mares";  //Nombre personalizado del dispositivo

void setup() {
  //Inicializar la comunicación serial
  Serial.begin(9600);
  //Configurar el nombre del dispositivo antes de conectarlo a la red WiFi
  if (!WiFi.setHostname(hostname)){
    Serial.println("¡Error al configurar el nombre del host!");
  }
  //Inicializar conexión a la red WiFi
  WiFi.begin(ssid, pass);  

  while (WiFi.status() != WL_CONNECTED) {
     Serial.print("Conectando a la red GalaxyMyriam...");
     delay (1000);
  }
}

void loop() {
  // Cuando se conecta a la red solicita nombre de la red, dirección IP, intensidad de la red y nombre del dispositivo 
  Serial.println("Conectado a la red");
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  IPAddress ip = WiFi.localIP();
  Serial.print("Dirección IP: ");
  Serial.println(ip);

  long rssi = WiFi.RSSI();
  Serial.print("Intensidad de la señal (RSSI): ");
  Serial.print(rssi);
  Serial.println(" dBm");

  Serial.print("Nombre del host: ");
  Serial.println(WiFi.getHostname());

  delay(5000);
}


