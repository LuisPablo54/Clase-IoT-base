/* Ejemplo base tomado de https://community.hivemq.com/t/hivemq-using-esp32-and-nodered/1291 
Modificado para que lea los valores de los sensores y reciba mensajes del broker MQTT
Reemplazar XXXX por los valores correspondientes*/

#include <WiFi.h>
#include <PubSubClient.h>
#include <WiFiClientSecure.h>
#include <DHT.h>

//---- Configuración de la red WiFi y MQTT
const char* ssid = "GalaxyMyriam";  // Parámetros de acceso a red WiFi 
const char* password = "IberoIoT";
//---- MQTT Broker settings
const char* mqtt_server = "4d58dc4d4da64daca6f0003289196edb.s1.eu.hivemq.cloud";   // Cluster url
const char* mqtt_username = "MAMALU";  // Broker usuario
const char* mqtt_password = "Cont54%jl03#";      // Broker contraseña
const int mqtt_port = 8883;

// Definición de Pines y Sensor DHT
#define DHTPIN 23  
#define DHTTYPE DHT22  
#define POWER_PIN 26  // Pin que controla la alimentación del sensor de lluvia (Vcc) 
#define DO_PIN    25  // Pin digital DO
#define LED_PIN   27  // Pin digital LED

// Configuración del sensor DHT y variables
DHT dht(DHTPIN, DHTTYPE);
WiFiClientSecure espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE (50)
char msg[MSG_BUFFER_SIZE];

unsigned long previousMillis = 0;  // Almacena el último tiempo en que se realizó una acción
const long interval = 2000;        // Intervalo de 2 segundos


// Definir tópicos
const char* Valor_Temperatura_Topic= "sensor1";
const char* Valor_Humedad_Topic="sensor2";
const char* Valor_Lluvia_Topic="sensor3";
const char* Estado_LED_Topic="LED";

// Certificado SSL para conectarse al broker MQTT almacenado en root_ca
static const char *root_ca PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
MIIFazCCA1OgAwIBAgIRAIIQz7DSQONZRGPgu2OCiwAwDQYJKoZIhvcNAQELBQAw
TzELMAkGA1UEBhMCVVMxKTAnBgNVBAoTIEludGVybmV0IFNlY3VyaXR5IFJlc2Vh
cmNoIEdyb3VwMRUwEwYDVQQDEwxJU1JHIFJvb3QgWDEwHhcNMTUwNjA0MTEwNDM4
WhcNMzUwNjA0MTEwNDM4WjBPMQswCQYDVQQGEwJVUzEpMCcGA1UEChMgSW50ZXJu
ZXQgU2VjdXJpdHkgUmVzZWFyY2ggR3JvdXAxFTATBgNVBAMTDElTUkcgUm9vdCBY
MTCCAiIwDQYJKoZIhvcNAQEBBQADggIPADCCAgoCggIBAK3oJHP0FDfzm54rVygc
h77ct984kIxuPOZXoHj3dcKi/vVqbvYATyjb3miGbESTtrFj/RQSa78f0uoxmyF+
0TM8ukj13Xnfs7j/EvEhmkvBioZxaUpmZmyPfjxwv60pIgbz5MDmgK7iS4+3mX6U
A5/TR5d8mUgjU+g4rk8Kb4Mu0UlXjIB0ttov0DiNewNwIRt18jA8+o+u3dpjq+sW
T8KOEUt+zwvo/7V3LvSye0rgTBIlDHCNAymg4VMk7BPZ7hm/ELNKjD+Jo2FR3qyH
B5T0Y3HsLuJvW5iB4YlcNHlsdu87kGJ55tukmi8mxdAQ4Q7e2RCOFvu396j3x+UC
B5iPNgiV5+I3lg02dZ77DnKxHZu8A/lJBdiB3QW0KtZB6awBdpUKD9jf1b0SHzUv
KBds0pjBqAlkd25HN7rOrFleaJ1/ctaJxQZBKT5ZPt0m9STJEadao0xAH0ahmbWn
OlFuhjuefXKnEgV4We0+UXgVCwOPjdAvBbI+e0ocS3MFEvzG6uBQE3xDk3SzynTn
jh8BCNAw1FtxNrQHusEwMFxIt4I7mKZ9YIqioymCzLq9gwQbooMDQaHWBfEbwrbw
qHyGO0aoSCqI3Haadr8faqU9GY/rOPNk3sgrDQoo//fb4hVC1CLQJ13hef4Y53CI
rU7m2Ys6xt0nUW7/vGT1M0NPAgMBAAGjQjBAMA4GA1UdDwEB/wQEAwIBBjAPBgNV
HRMBAf8EBTADAQH/MB0GA1UdDgQWBBR5tFnme7bl5AFzgAiIyBpY9umbbjANBgkq
hkiG9w0BAQsFAAOCAgEAVR9YqbyyqFDQDLHYGmkgJykIrGF1XIpu+ILlaS/V9lZL
ubhzEFnTIZd+50xx+7LSYK05qAvqFyFWhfFQDlnrzuBZ6brJFe+GnY+EgPbk6ZGQ
3BebYhtF8GaV0nxvwuo77x/Py9auJ/GpsMiu/X1+mvoiBOv/2X/qkSsisRcOj/KK
NFtY2PwByVS5uCbMiogziUwthDyC3+6WVwW6LLv3xLfHTjuCvjHIInNzktHCgKQ5
ORAzI4JMPJ+GslWYHb4phowim57iaztXOoJwTdwJx4nLCgdNbOhdjsnvzqvHu7Ur
TkXWStAmzOVyyghqpZXjFaH3pO3JLF+l+/+sKAIuvtd7u+Nxe5AW0wdeRlN8NwdC
jNPElpzVmbUq4JUagEiuTDkHzsxHpFKVK7q4+63SM1N95R1NbdWhscdCb+ZAJzVc
oyi3B43njTOQ5yOf+1CceWxG1bQVs5ZufpsMljq4Ui0/1lvh+wjChP4kqKOJ2qxq
4RgqsahDYVvTH9w7jXbyLeiNdd8XM2w9U/t7y0Ff/9yi0GE44Za4rF2LN9d11TPA
mRGunUHBcnWEvgJBQl9nJEiU0Zsnvgc/ubhPgXRR4Xq37Z0j4r7g1SgEEzwxA57d
emyPxgcYxn/eR44/KJ4EBs+lVDR3veyJm+kXQ99b21/+jh5Xos1AnX5iItreGCc=
-----END CERTIFICATE-----
)EOF";

void setup() {
// Iniciar el sensor DHT y esperar a que se estabilice
dht.begin();  
delay(2000);  

// Configurar pines
pinMode(POWER_PIN, OUTPUT); 
pinMode(DO_PIN, INPUT);
pinMode(LED_PIN, OUTPUT); 

// Conecta el ESP32 a la red WiFi
Serial.begin(9600);
Serial.print("\nConectando a ");
Serial.println(ssid);

WiFi.mode(WIFI_STA);
WiFi.begin(ssid, password);

while (WiFi.status() != WL_CONNECTED) {
delay(1000);
Serial.print(".");
}
randomSeed(micros());
Serial.println("\nWiFi conectado \ndirección IP: ");
Serial.println(WiFi.localIP());

//Si la conexión WiFi es exitosa, el ESP32 obtiene una dirección IP y se conecta al broker MQTT utilizando el certificado SSL
while (!Serial) delay(1);
espClient.setCACert(root_ca);
client.setServer(mqtt_server, mqtt_port);
client.setCallback(callback);
}

void loop() {
// El ESP32 publica datos de los sensores a través de MQTT
// Se usa el método publishMessage para enviar esta información a los tópicos 
// Si la conexión MQTT se pierde, esta función intenta reconectarse y re-suscribirse a los tópicos
unsigned long currentMillis = millis();  // Obtener el tiempo actual
  if (!client.connected()){
  reconnect();
  }
  client.loop();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
  // Leer la temperatura en grados Celsius y la humedad en porcentaje
  float temperatura = dht.readTemperature();
  int humedad = dht.readHumidity();
  // Enciende el sensor de lluvia y lee el estado
  digitalWrite(POWER_PIN, HIGH);  
  delay(10);                     
  int rain_state = digitalRead(DO_PIN); 
  delay(10);
  // Apaga el sensor de lluvia
  digitalWrite(POWER_PIN, LOW);  

  if (rain_state == HIGH){
    publishMessage(Valor_Lluvia_Topic,String("No llueve"),true);
  }
  else {
    publishMessage(Valor_Lluvia_Topic,String("Lloviendo"),true);
  }
publishMessage(Valor_Temperatura_Topic,String(temperatura),true);    
publishMessage(Valor_Humedad_Topic,String(humedad),true);
 }
}

// Si la conexión MQTT se pierde, esta función intenta reconectarse y re-suscribirse a los tópicos
void reconnect() {
while (!client.connected()) {
Serial.print("Intentado conectar al broker MQTT…");
String clientId = "ESP32Client-"; 
clientId += String(random(0xffff), HEX);
if (client.connect(clientId.c_str(), mqtt_username, mqtt_password)) {
Serial.println("conectado");
  client.subscribe(Valor_Temperatura_Topic); 
  client.subscribe(Valor_Humedad_Topic);   
  client.subscribe(Valor_Lluvia_Topic);   
  client.subscribe(Estado_LED_Topic); 
} else {
  Serial.print("falló, rc=");
  Serial.print(client.state());
  Serial.println(" intenta de nuevo en 5 segundos");   // Wait 5 seconds before retrying
  delay(5000);
}
}
}

// Cuando el ESP32 recibe un mensaje en el tópico Estado_LED_Topic, se manda llamar esta rutina
// Evalúa si debe encender (on) o apagar (off) el LED conectado al pin 27.
void callback(char* topic, byte* payload, unsigned int length) {
String incommingMessage = "";
if( strcmp(topic,Estado_LED_Topic) == 0){
 for (int i = 0; i < length; i++) {
 incommingMessage+=(char)payload[i];
 }
 if (incommingMessage == "on"){
  digitalWrite(LED_PIN, HIGH);
  Serial.println("Llegó el mensaje: "+ incommingMessage);
  delay(50);
 }
  if (incommingMessage == "off"){
  digitalWrite(LED_PIN, LOW);
  Serial.println("Llegó el mensaje: " + incommingMessage);
  delay(50);
  }
 }
}

//Publica los valores que envía a los tópicos como cadenas de caracteres (eco en el serial monitor)
void publishMessage(const char* topic, String payload , boolean retained){
if (client.publish(topic, payload.c_str(), true))
Serial.println("Mensaje publicado ["+String(topic)+"]: "+payload);
}
