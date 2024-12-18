// Autores
// Luis Pablo López Iracheta
// María José Gamba Santibáñez
// Diego Mares Rodríguez
#include <WiFi.h>
#include <PubSubClient.h>
#include <WiFiClientSecure.h>
#include <DHT.h>

//---- WiFi settings
const char* ssid = "LuisEsGenial";
const char* password = "yalose!!";
//---- MQTT Broker settings
const char* mqtt_server = "4d58dc4d4da64daca6f0003289196edb.s1.eu.hivemq.cloud";   // Cluster url
const char* mqtt_username = "MAMALU";  // Broker usuario
const char* mqtt_password = "Cont54%jl03#";      // Broker contraseña
const int mqtt_port = 8883;
const char* mqtt_topic = "led/control"; 

//---------------------------SENSORES PUERTOS ------------------------------
// Definir el pin y modelo del sensor DHT
const byte DHTPIN = 13;  
const byte LLUVIA_PIN = 34; // Sensor de lluvia
const byte SONIDO_PIN = 35; // Sensor de sonido
const byte ledPin1 = 26;    // LED
const byte AIRE_PIN = 32;    // LED
// Pines de los LEDs (Cajones)
const byte C_0 = 22;    // LED Cajon 1
const byte C_1 = 21;    // LED Cajon 2
const byte C_2 = 19;    // LED Cajon 3
const byte C_3 = 18;    // LED Cajon 4
const byte C_4 = 5;    // LED Cajon 5
const byte C_5 = 4;    // LED Cajon 6
unsigned long lastMillis = 0;

#define DHTTYPE DHT11  
DHT dht(DHTPIN, DHTTYPE);

WiFiClientSecure espClient;
PubSubClient client(espClient);

const char* Valor_Temperatura_Topic = "Temperatura";
const char* Valor_Humedad_Topic = "Humedad";
const char* Valor_Lluvia_Topic = "Lluvia";
const char* Valor_Sonido_Topic = "Sonido";
const char* Valor_AIRE_Topic = "Aire";
const char* Valor_CAIRE_Topic = "CAire";

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

// Función de callback MQTT (para recibir mensajes)
void callback(char* topic, byte* payload, unsigned int length) {
  String incommingMessage = "";
    for (int i = 0; i < length; i++) {
    incommingMessage += (char)payload[i];
  }
  // Control del LED en función del mensaje recibido
  if (incommingMessage == "ON") {
    digitalWrite(ledPin1, HIGH);  // Encender LED en pin 26
    Serial.println("LED encendido");
  } else if (incommingMessage == "OFF") {
    digitalWrite(ledPin1, LOW);   // Apagar LED en pin 26
    Serial.println("LED apagado");
  }

  
  // Comparar el tópico recibido con los tópicos de los LEDs
  if (String(topic) == "C0") {
    if (incommingMessage.toInt() == 1) {
      digitalWrite(C_0, HIGH);
    } else {
      digitalWrite(C_0, LOW);
    }
  } else if (String(topic) == "C1") {
    if (incommingMessage.toInt() == 1) {
      digitalWrite(C_1, HIGH);
    } else {
      digitalWrite(C_1, LOW);
    }
  } else if (String(topic) == "C2") {
    if (incommingMessage.toInt() == 1) {
      digitalWrite(C_2, HIGH);
    } else {
      digitalWrite(C_2, LOW);
    }
  } else if (String(topic) == "C3") {
    if (incommingMessage.toInt() == 1) {
      digitalWrite(C_3, HIGH);
    } else {
      digitalWrite(C_3, LOW);
    }
  } else if (String(topic) == "C4") {
    if (incommingMessage.toInt() == 1) {
      digitalWrite(C_4, HIGH);
    } else {
      digitalWrite(C_4, LOW);
    }
  } else if (String(topic) == "C5") {
    if (incommingMessage.toInt() == 1) {
      digitalWrite(C_5, HIGH);
    } else {
      digitalWrite(C_5, LOW);
    }
  }


}

// Función de reconexión a MQTT
void reconnect() {
  while (!client.connected()) {
    Serial.print("Intentando conexión MQTT...");
    String clientId = "ESP32Client-";
    clientId += String(random(0xffff), HEX);
    if (client.connect(clientId.c_str(), mqtt_username, mqtt_password)) {
      Serial.println("Conectado al broker MQTT");
      // Suscribir a todos los tópicos de los LEDs
      client.subscribe("C0");
      client.subscribe("C1");
      client.subscribe("C2");
      client.subscribe("C3");
      client.subscribe("C4");
      client.subscribe("C5");
      
    } else {
      Serial.print("Error, rc=");
      Serial.print(client.state());
      Serial.println(" Intentando de nuevo en 3 segundos...");
      delay(3000);
    }
  }
}

void setup() {
  dht.begin();
  delay(1000);  

  Serial.begin(9600);
    // Configurar pines de LEDs como salida
  pinMode(C_0, OUTPUT);
  pinMode(C_1, OUTPUT);
  pinMode(C_2, OUTPUT);
  pinMode(C_3, OUTPUT);
  pinMode(C_4, OUTPUT);
  pinMode(C_5, OUTPUT);
  // Apagar todos los LEDs
  digitalWrite(C_0, LOW);
  digitalWrite(C_1, LOW);
  digitalWrite(C_2, LOW);
  digitalWrite(C_3, LOW);
  digitalWrite(C_4, LOW);
  digitalWrite(C_5, LOW);

  pinMode(SONIDO_PIN, INPUT); 
  pinMode(ledPin1, OUTPUT);
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
  espClient.setCACert(root_ca);
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);  
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  if (millis() - lastMillis >= 1000) {
    lastMillis = millis();
  
  float temperatura = dht.readTemperature();  
  int humedad = dht.readHumidity();  
  int valorLluvia = analogRead(LLUVIA_PIN);
  int valorSonido = analogRead(SONIDO_PIN);
  int valorAire = analogRead(AIRE_PIN);
  //Serial.println(valorAire);
  float valorCalidadAire = (valorAire / 4095.0) * 3.3;  

  String estadoLluvia;
  if (valorLluvia <= 1023) {
    estadoLluvia = "Aguacero";
  } else if (valorLluvia <= 2047) {
    estadoLluvia = "Lluvia moderada";
  } else if (valorLluvia <= 3071) {
    estadoLluvia = "Lluvia ligera";
  } else {
    estadoLluvia = "Sin Lluvia";
  }

  //Serial.print("Estado de la lluvia: ");
  //Serial.println(estadoLluvia);
  publishMessage(Valor_Temperatura_Topic, String(temperatura), true);    
  publishMessage(Valor_Humedad_Topic, String(humedad), true);
  client.publish(Valor_Lluvia_Topic, estadoLluvia.c_str());
  client.publish(Valor_Sonido_Topic, String(valorSonido).c_str());
  client.publish(Valor_AIRE_Topic, String(valorAire).c_str());
  client.publish(Valor_CAIRE_Topic, String(valorCalidadAire).c_str());
  delay(500);
}
}
void publishMessage(const char* topic, String payload, boolean retained){
  if (client.publish(topic, payload.c_str(), retained))
    Serial.println("Message published [" + String(topic) + "]: " + payload);
}
