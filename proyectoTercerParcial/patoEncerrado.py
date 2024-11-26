# -*- coding: utf-8 -*-
""" Compara una imagen con una referencia @author: Myriam """

import cv2   # Librería OpenCV para procesar imágenes
import sys   # Manejo de errores

# Ruta de las imágenes que se van a comparar
foto_referencia = "./Patos/CajaVacia.jpg"
foto_actual = "./Patos/pato1.jpg"

# Definir las tres áreas a analizar (x, y, ancho, alto)
areas = [
    (0, 0, 106, 120),   # Espacio 1
    (106, 0, 106, 120), # Espacio 2
    (212, 0, 106, 120), # Espacio 3
    (0, 120, 106, 120), # Espacio 4
    (106, 120, 106, 120), # Espacio 5
    (212, 120, 106, 120)  # Espacio 6
]

# Cargar imágenes en escala de grises
imagen_referencia = cv2.imread(foto_referencia, cv2.IMREAD_GRAYSCALE)
imagen_actual = cv2.imread(foto_actual, cv2.IMREAD_GRAYSCALE)
cv2.imshow("Referencia", imagen_actual)
# Validar que las imágenes se cargaron correctamente
if imagen_referencia is None or imagen_actual is None:
    print("Error: No se cargaron las imágenes")
    sys.exit()

# Leer la imagen original para dibujar las áreas (opcional, en color)
imagen_con_areas = cv2.imread(foto_referencia)

# Definir el umbral de diferencias
umbral_diferencia = 300  # Máximo de píxeles diferentes permitido

# Analizar cada área definida
for i, (x, y, w, h) in enumerate(areas, 1):
    # Extraer las ROI de ambas imágenes
    roi_referencia = imagen_referencia[y:y+h, x:x+w]
    roi_actual = imagen_actual[y:y+h, x:x+w]
    
    # Calcular diferencias
    diferencia = cv2.absdiff(roi_referencia, roi_actual)
    _, diferencia_binaria = cv2.threshold(diferencia, 100, 255, cv2.THRESH_BINARY)
    
    # Contar píxeles diferentes
    diferencia_total = cv2.countNonZero(diferencia_binaria)
    print(f"Área {i} - Diferencia: {diferencia_total}")
    
    # Determinar el estado de cada área
    estado = "Pato en la caja" if diferencia_total > umbral_diferencia else "Área vacía"
    print(f"Resultado para el área {i}: {estado}")
    
    # Dibujar la región analizada en la imagen original
    cv2.rectangle(imagen_con_areas, (x, y), (x+w, y+h), (0, 255, 0), 2)
    
    # Mostrar las imágenes analizadas por área
    #cv2.imshow(f"Referencia Área {i}", roi_referencia)
    #cv2.imshow(f"Actual Área {i}", roi_actual)
    #cv2.imshow(f"Diferencia Área {i}", diferencia_binaria)

# Mostrar la imagen con todas las áreas marcadas

cv2.imshow("Áreas seleccionadas", imagen_con_areas)

cv2.waitKey(0)
cv2.destroyAllWindows()
