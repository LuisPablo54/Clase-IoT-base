#Detección de manos con la cámara del ESP32-CAM, es necesario programar el módulo y tomar nota de la IP asignada antes de ejecutar#

import cv2
import mediapipe as mp
import urllib.request
import numpy as np

# Inicializa MediaPipe Hands para detectar las posiciones de las manos en las imagenes que captura
mp_hands = mp.solutions.hands
hands = mp_hands.Hands()

# Inicializa MediaPipe Drawing Utilities para dibujar las conexiones de las manos detectadas
mp_drawing = mp.solutions.drawing_utils

# URL asignada al módulo ESP32-CAM
url = "http://172.20.10.13/capture"

while True:
    img_resp = urllib.request.urlopen(url)
    img_np = np.array(bytearray(img_resp.read()), dtype=np.uint8)
    image = cv2.imdecode(img_np, -1)

    if image is None:
        print("Ignorando frame vacío")
        continue

    # Procesa la imagen, la voltea en espejo, convierte de BGR a RGB y dibuja las marcas y conexiones
    image = cv2.cvtColor(cv2.flip(image, 1), cv2.COLOR_BGR2RGB)

    results = hands.process(image)

    image = cv2.cvtColor(image, cv2.COLOR_RGB2BGR)

    if results.multi_hand_landmarks:
        for hand_landmarks in results.multi_hand_landmarks:
            mp_drawing.draw_landmarks(image, hand_landmarks, mp_hands.HAND_CONNECTIONS)

     # Muestra la imagen en una ventana que llama Detector de manos en el ESP32-CAM y se detiene cuando se presiona la tecla ESC (ASCII 27)
    cv2.imshow('Detector de manos en el ESP32-CAM', image)
    if cv2.waitKey(5) & 0xFF == 27:  # Press 'ESC' to exit
        break

cv2.destroyAllWindows()