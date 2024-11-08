#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "GalaxyMyriam";  // Parámetros de acceso a red WiFi 
const char* password = "IberoIoT";

// Pines de los LEDs
const int LED1 = 14;
const int LED2 = 27;
const int LED3 = 26;
const int LED4 = 25;

int animacion = 0;  // Variable para controlar la animación actual

String html;
WebServer server(80);   // El servidor se conecta por el puerto 80

void setup() {
  Serial.begin(9600);
  
  // Configuración de pines de salida
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);
  
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {  // Espera hasta conectarse a la red WiFi
    delay(1000);
    Serial.println("Conectando al WiFi...");
  }
  datosWiFi();  // Configuración de la red
  
  server.on("/", respuesta);   // Ruta raíz, muestra la página web
  server.on("/CAMBIAR", cambiarAnimacion);  // Cambia de animación cuando se presiona el botón
  
  server.begin();   // Inicia el servidor
  Serial.println("Servidor iniciado");
}

void loop() {
  server.handleClient(); // Mantiene la conexión entre cliente y servidor
  
  // Lógica de animaciones de los LEDs
  switch (animacion) {
    case 0:
      animacion1();
      break;
    case 1:
      animacion2();
      break;
    case 2:
      animacion3();
      break;
    case 3:
      animacion4();
      break;
  }
}

// Función que cambia la animación
void cambiarAnimacion() {
  animacion = (animacion + 1) % 4;  // Cambia de animación
  respuesta();  // Muestra nuevamente la página con el estado actualizado
}

// Página HTML que se envía al cliente
void respuesta() {
  code_html();  // Genera el código HTML
  server.send(200, "text/html", html);
}

void datosWiFi() { // Configuración de la red
  IPAddress local_ip(192, 168, 229, 27);
  IPAddress gateway(192, 168, 229, 1);
  IPAddress subnet(255, 255, 255, 0);
  WiFi.config(local_ip, gateway, subnet);
}

// Funciones de animaciones
void animacion1() {
  digitalWrite(LED1, HIGH);
  digitalWrite(LED2, LOW);
  digitalWrite(LED3, LOW);
  digitalWrite(LED4, LOW);
}

void animacion2() {
  digitalWrite(LED1, HIGH);
  digitalWrite(LED2, HIGH);
  digitalWrite(LED3, LOW);
  digitalWrite(LED4, LOW);
}

void animacion3() {
  digitalWrite(LED1, HIGH);
  digitalWrite(LED2, HIGH);
  digitalWrite(LED3, HIGH);
  digitalWrite(LED4, LOW);
}

void animacion4() {
  digitalWrite(LED1, HIGH);
  digitalWrite(LED2, HIGH);
  digitalWrite(LED3, HIGH);
  digitalWrite(LED4, HIGH);
}

// Función que genera el código HTML de la página web
void code_html() {
  html = "<html><head><title>Control de LEDs</title></head><body align=left>";
  html += "<h1>Control de LEDs</h1>";
  html += "<p>Presiona el boton para cambiar la animacino de los LEDs.</p>";
  html += "<button onclick=\"location.href='/CAMBIAR'\">Cambiar Animación</button>";
  html += "</body></html>";
}

