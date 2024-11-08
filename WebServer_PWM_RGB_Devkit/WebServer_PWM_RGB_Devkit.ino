#include <WiFi.h>
#include <WebServer.h>

// Parámetros de acceso a la red WiFi
const char* ssid = "INFINITUM38A4_2.4";
const char* password = "sY4mX2MZkG";
const char hostname[] = "Devkit de Myriam";  // Nombre personalizado del dispositivo

// Pines para los LEDs del RGB
#define bluePin 27
#define greenPin 26
#define redPin 25 

// Variables para almacenar los valores PWM de los LEDs RGB
int redValue = 0;
int greenValue = 0;
int blueValue = 0;

String html;
WebServer server(80);   // El servidor se conecta por el puerto 80

void setup() {
  // Inicializar la comunicación serial
  Serial.begin(9600);

  // Configurar el nombre del dispositivo antes de conectarlo a la red WiFi
  if (!WiFi.setHostname(hostname)) {
    Serial.println("¡Error al configurar el nombre del host!");
  }

  // Inicializar conexión a la red WiFi
  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print("Conectando a la red Elegida...");
    delay(1000);
  }

  // Una vez conectado, mostrar información sobre la red en el monitor serial
  Serial.println("\nConectado a la red");
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

  // Configuración de los pines del LED RGB
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);

  // Inicializa el servidor y rutas
  server.on("/", respuesta);  // Página principal
  server.on("/setRGB", setRGB);  // Ruta para ajustar los valores RGB
  server.begin();
  Serial.println("Servidor web iniciado.");
}

void loop() {
  server.handleClient();  // Mantiene la conexión entre cliente y servidor
}

// Respuesta HTML cuando se accede a la página principal
void respuesta() {
  code_html();
  server.send(200, "text/html", html);
}

// Función que ajusta los valores RGB
void setRGB() {
  // Leer los parámetros RGB de la URL
  if (server.hasArg("red")) {
    redValue = server.arg("red").toInt();
  }
  if (server.hasArg("green")) {
    greenValue = server.arg("green").toInt();
  }
  if (server.hasArg("blue")) {
    blueValue = server.arg("blue").toInt();
  }

  // Aplicar los valores PWM a los pines del LED RGB
  analogWrite(redPin, redValue);
  analogWrite(greenPin, greenValue);
  analogWrite(bluePin, blueValue);

  // Responder al cliente
  code_html();  // Actualiza la página con los nuevos valores
  server.send(200, "text/html", html);
}

// Función que genera el código HTML de la página web
void code_html() {
  html = "<html><head><title>Control del LED RGB</title></head><body align=left>";
  html += "<h1>Control del LED RGB</h1>";
  html += "<hr/>";
  
  // Formulario para ajustar el valor de cada color (red, green, blue)
  html += "<form action='/setRGB' method='get'>";
  html += "Rojo (0-255): <input type='number' name='red' value='" + String(redValue) + "'><br>";
  html += "Verde (0-255): <input type='number' name='green' value='" + String(greenValue) + "'><br>";
  html += "Azul (0-255): <input type='number' name='blue' value='" + String(blueValue) + "'><br><br>";
  html += "<input type='submit' value='Actualizar LED RGB'>";
  html += "</form>";
  
  html += "<hr/>";
  html += "<p>&nbsp;</p>"; // Espacio
  html += "</body></html>";
}
