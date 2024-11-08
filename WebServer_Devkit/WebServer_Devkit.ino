// Ejemplo de uso del ESP32 como un web server empleando la libreria simplificada ESP32WebServer
#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "LuisEsGenial";  // Parámetros de acceso a red WiFi 
const char* password = "yalose!!";
#define led 21

String html;
WebServer server(80);   // El servidor se conecta por el puerto 80

void setup() {
  Serial.begin (9600);
  pinMode(led, OUTPUT);
	WiFi.begin(ssid, password);
   while (WiFi.status() != WL_CONNECTED) {  //Espera hasta conectarse a la red WiFi
    delay(1000);
    Serial.println("Conectando al WiFi...");
  }
	datosWiFi();  // Función que configura los datos de red
	server.on("/", respuesta);    // Si el servidor recibe datos del cliente:
	server.on("/ON", activaLED);  // va a la rutina cuando recibe el texto Led 
  server.on("/OFF",apagaLED);   // va a la rutina cuando recibe el texto Led2  
  server.begin();   // Inicializa el servidor 
  Serial.println("Servidor iniciado");
}

void loop() {
	server.handleClient(); 	// Mantiene la conexión entre cliente y servidor
}

// Cuando se cumple la condición server.on, se ejecuta esta rutina
void respuesta() {
  code_html();
  server.send(200, "text/html", html);
}

void activaLED() {
  code_html();
  server.send(200, "text/html", html);
  digitalWrite(led, HIGH);  //Enciende led
}

void apagaLED() {
  code_html();
  server.send(200, "text/html", html);
  digitalWrite(led, LOW);   //Apaga led
}


void datosWiFi() { // Función que configura los datos de red del AP
  IPAddress local_ip(192, 168, 57, 139);                          
  IPAddress gateway(192, 168, 57, 1);   
  IPAddress subnet(255, 255, 255, 0);
  WiFi.config(local_ip, gateway, subnet);
}

void code_html() {
  // Función con el código HTML a enviar como respuesta
  //------------------------------------------------------------------------------------------------------
  // inicia variables como string con todo el contenido HTML
  // coloca titulos y encabezados
  //String html = "<html><head><title>Modulo DOIT ESP32</title></head><body align=left>";
  html = "<align=left><font color=\"red\">Estacion 01</font><br>";  
  html += "<hr/>"; // coloca línea horizontal
  // crea los botones de encendido y apagado 
  html += "<font color='green'>Control: </font> <button onclick=\"location.href='/ON'\">ON</button>";
  html += " <button onclick=\"location.href='/OFF'\">OFF</button><br>";
  html += "<hr/>"; // coloca linea horizontal
  //s += "<align=left><font color=\"red\">---------------------------</font><br>";
  html += "<p>&nbsp;</p>"; // cierra línea 
  html += "<p>&nbsp;</p>"; // cierra linea
  html += "</body></html>";
  //------------------------------------------------------------------------------------------------------
  // Fin código HTML
}
