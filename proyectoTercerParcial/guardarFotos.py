# -*- coding: utf-8 -*-
# Guardar Foto
""" Toma una foto, la guarda y la muestra en una ventana @author: Myriam """
import urllib.request
import cv2
import numpy as np
# URL de la ESP32-CAM
url = "http://192.168.10.96/capture"
ruta_referencia = "./estacionamiento/estacionamiento_vacio.jpg"
try:
    response = urllib.request.urlopen(url)
    image_np = np.array(bytearray(response.read()), dtype=np.uint8)
    image = cv2.imdecode(image_np, -1)
    cv2.imwrite(ruta_referencia, image)
    print("Imagen de referencia guardada en:", ruta_referencia)
    
except Exception as e:
    print("Error al capturar imagen:", e)
