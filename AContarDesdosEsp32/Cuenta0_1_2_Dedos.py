# -*- coding: utf-8 -*-
""" Publica en MQTT el número de dedos arriba (0, 1 o 2)  @author: Myriam 
Programa basado en la guía https://ai.google.dev/edge/mediapipe/solutions/vision/hand_landmarker/python?hl=es-419 """

import cv2
import mediapipe as mp
import paho.mqtt.client as mqtt  # Instalar biblioteca para poder comunicar con el broker MQTT
import urllib.request   # Para poder leer la imagen desde un link url
import numpy as np
import threading
import time

# Configurar MQTT 
mqtt_broker = "4d58dc4d4da64daca6f0003289196edb.s1.eu.hivemq.cloud" # Dirección IP del broker MQTT
mqtt_port = 8883  
mqtt_topic = "Indice"
mqtt_topic = "Medio"
mqtt_topic = "Anular"
mqtt_topic = "Menique"
mqtt_username = "MAMALU" #
mqtt_password = "Cont54%jl03#"


client = mqtt.Client()
client.username_pw_set(mqtt_username, mqtt_password)
client.tls_set()

try:
    client.connect(mqtt_broker, mqtt_port, 60)
    client.loop_start()
    print("Connectado al broker MQTT")
except Exception as e:
    print(f"La conexión al broker falló, error: {e}")


mp_hands = mp.solutions.hands
hands = mp_hands.Hands(max_num_hands=1) 
mp_drawing = mp.solutions.drawing_utils

# Captura el video desde la URL de la ESP32-CAM. 
esp32_cam_url = "http://192.168.1.147/capture" 


def contar_dedos(hand_landmarks):
    dedos_ids = [8, 12, 16, 20]  
    listaDesdos = []

    for id in dedos_ids:
        if hand_landmarks.landmark[id].y < hand_landmarks.landmark[id - 2].y:
                    
            listaDesdos.append(id)
    return listaDesdos

while True:
    # Obtener la imagen desde la ESP32-CAM con urllib.request y luego la convierte a una 
    # imagen OpenCV que si puede procesar
    response = urllib.request.urlopen(esp32_cam_url)
    image_np = np.array(bytearray(response.read()), dtype=np.uint8)
    image = cv2.imdecode(image_np, -1)

     # Procesa la imagen, la voltea en espejo, convierte de BGR a RGB y dibuja las marcas y conexiones
    image = cv2.cvtColor(cv2.flip(image, 1), cv2.COLOR_BGR2RGB)
    results = hands.process(image)
    image = cv2.cvtColor(image, cv2.COLOR_RGB2BGR)

   
    numeroDelDedo = []
    if results.multi_hand_landmarks:        #Si la imagen contiene más de una mano, se ejecuta para cada una
        for hand_landmarks in results.multi_hand_landmarks:
            numeroDelDedo = contar_dedos(hand_landmarks)  
            mp_drawing.draw_landmarks(image, hand_landmarks, mp_hands.HAND_CONNECTIONS)



# Si en la lista tengo el dedo 8, entonces publico "Indice"
    if 8 in numeroDelDedo:
        client.publish("Indice", 1)
        print("Indice")
    # Si en la lista tengo el dedo 12, entonces publico "Medio"
    if 12 in numeroDelDedo:
        client.publish("Medio", 1)
        print("Medio")
    # Si en la lista tengo el dedo 16, entonces publico "Anular"
    if 16 in numeroDelDedo:
        client.publish("Anular", 1)
        print("Anular")
    # Si en la lista tengo el dedo 20, entonces publico "Meñique"
    if 20 in numeroDelDedo:
        client.publish("Menique", 1)
        print("Meñique")

#Dedos abajo
    if not 8 in numeroDelDedo:
        client.publish("Indice", 0)
    if not 12 in numeroDelDedo:
        client.publish("Medio", 0)
    if not 16 in numeroDelDedo:
        client.publish("Anular", 0)
    if not 20 in numeroDelDedo:
        client.publish("Menique", 0)
        
    #Agregamos un delay para que no se publique el mensaje muchas veces
    
    # Mostrar la imagen con las manos detectadas
    
    cv2.imshow('Cuenta dedos ESP32cam', cv2.resize(image, (640, 480)))
    if cv2.waitKey(15) & 0xFF == 27:  # Presiona 'ESC' para salir
        break
    cv2.waitKey(1000)

cv2.destroyAllWindows()
