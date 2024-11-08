/* Ejemplo base tomado de https://community.hivemq.com/t/hivemq-using-esp32-and-nodered/1291 
Modificado para que lea los valores de humedad y temperatura que mide el sensor DHT11
Reemplazar XXXX por los valores correspondientes*/

#include <WiFi.h>
#include <PubSubClient.h>
#include <WiFiClientSecure.h>
#include <DHT.h>

//---- WiFi settings
const char* ssid = "INFINITUM38A4_2.4";
const char* password = "sY4mX2MZkG";
//---- MQTT Broker settings
const char* mqtt_server = "4d58dc4d4da64daca6f0003289196edb.s1.eu.hivemq.cloud";   // Cluster url
const char* mqtt_username = "MAMALU";  // Broker usuario
const char* mqtt_password = "Cont54%jl03#";      // Broker contraseña
const int mqtt_port =8883;
// Definir el pin y modelo del sensor DHT
const byte DHTPIN = 13;  
// Definir el pin para el sensor de lluvia (asumiendo que es un sensor analógico)
const byte LLUVIA_PIN = 34;
#define DHTTYPE DHT11  
// Configuración del sensor DHT
DHT dht(DHTPIN, DHTTYPE);

WiFiClientSecure espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;

#define MSG_BUFFER_SIZE (50)
char msg[MSG_BUFFER_SIZE];

const char* Valor_Temperatura_Topic= "Temperatura";
const char* Valor_Humedad_Topic="Humedad";
const char* Valor_Lluvia_Topic="Lluvia";

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
// Iniciar el sensor DHT
dht.begin();
delay(2000);  //Esperar a que se estabilice el sensor

Serial.begin(9600);
Serial.print("\nConectando a ");
Serial.println(ssid);

WiFi.mode(WIFI_STA);
WiFi.begin(ssid, password);

while (WiFi.status() != WL_CONNECTED) {
delay(500);
Serial.print(".");
}
randomSeed(micros());
Serial.println("\nWiFi connected\nIP address: ");
Serial.println(WiFi.localIP());

while (!Serial) delay(1);

espClient.setCACert(root_ca);
client.setServer(mqtt_server, mqtt_port);
client.setCallback(callback);
}

void loop() {

  if (!client.connected()) reconnect();
  client.loop();
  // Leer la temperatura en grados Celsius y la humedad en porcentaje
  float temperatura = dht.readTemperature();
  int humedad = dht.readHumidity();
// Leer el valor del sensor de lluvia (asumiendo que es analógico)
  int lluvia = analogRead(LLUVIA_PIN);

publishMessage(Valor_Temperatura_Topic,String(temperatura),true);    
publishMessage(Valor_Humedad_Topic,String(humedad),true);
publishMessage(Valor_Lluvia_Topic,String(lluvia),true);
delay(2000);
}

//=======================================================================Function=================================================================================

void reconnect() {
// Loop until we’re reconnected
while (!client.connected()) {
Serial.print("Attempting MQTT connection…");
String clientId = "ESP32Client-"; // Create a random client ID
clientId += String(random(0xffff), HEX);
// Attempt to connect
if (client.connect(clientId.c_str(), mqtt_username, mqtt_password)) {
Serial.println("connected");

  client.subscribe(Valor_Temperatura_Topic);   // subscribe the topics here
  client.subscribe(Valor_Humedad_Topic);   // subscribe the topics here
  client.subscribe(Valor_Lluvia_Topic);   // subscribe the topics here
} else {
  Serial.print("failed, rc=");
  Serial.print(client.state());
  Serial.println(" try again in 5 seconds");   // Wait 5 seconds before retrying
  delay(5000);
}
}
}

//=======================================
// This void is called every time we have a message from the broker

void callback(char* topic, byte* payload, unsigned int length) {
String incommingMessage = "";
if( strcmp(topic,Valor_Temperatura_Topic) == 0){
 for (int i = 0; i < length; i++) {
 incommingMessage+=(char)payload[i];
 }
 Serial.println("Mensaje recibido: ["+String(topic)+"]"+incommingMessage);
 delay(2000);
}
else if( strcmp(topic,Valor_Humedad_Topic) == 0){
 for (int i = 0; i < length; i++) {
  incommingMessage+=(char)payload[i];
 }
  Serial.println("Mensaje recibido: ["+String(topic)+"]"+incommingMessage);
  delay(2000);
 }
 else if (strcmp(topic, Valor_Lluvia_Topic) == 0) {  // Manejar el tópico de lluvia
    for (int i = 0; i < length; i++) {
      incommingMessage += (char)payload[i];
    }
    Serial.println("Mensaje recibido: [" + String(topic) + "] " + incommingMessage);
    delay(2000);
  }
}


//======================================= publising as string
void publishMessage(const char* topic, String payload , boolean retained){
if (client.publish(topic, payload.c_str(), true))
Serial.println("Message publised ["+String(topic)+"]: "+payload);
}