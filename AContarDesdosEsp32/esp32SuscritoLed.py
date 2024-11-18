# -*- coding: utf-8 -*-
import paho.mqtt.client as mqtt  # Instalar biblioteca para poder comunicar con el broker MQTT
import urllib.request   # Para poder leer la imagen desde un link url


# Configurar MQTT 
mqtt_broker = "4d58dc4d4da64daca6f0003289196edb.s1.eu.hivemq.cloud" # Dirección IP del broker MQTT
mqtt_port = 8883  
mqtt_topic = "Cuenta_Dedos_Topic" # Tema al que se publicará el número de dedos arriba
mqtt_username = "MAMALU" #
mqtt_password = "Cont54%jl03#"

# Inicializar cliente MQTT, puerto del broker y TLS para conexiones seguras, esto es necesario cuando 
# el broker usa conexiones seguras (puerto 8883)
client = mqtt.Client()
client.username_pw_set(mqtt_username, mqtt_password)
client.tls_set()

try:
    client.connect(mqtt_broker, mqtt_port, 60)
    client.loop_start()
    print("Connectado al broker MQTT")
except Exception as e:
    print(f"La conexión al broker falló, error: {e}")

#suscribir al topic Inice, Medio, Anular y Menique

client.subscribe("Indice")
client.subscribe("Medio")
client.subscribe("Anular")
client.subscribe("Menique")

# Encender los leds en puerto 12, 14, 35, 34


