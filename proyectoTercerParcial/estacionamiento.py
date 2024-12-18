# Codigo principal del estacionamiento
import urllib.request
import cv2
import numpy as np
import paho.mqtt.client as mqtt  # Instalar biblioteca para poder comunicar con el broker MQTT
import urllib.request   # Para poder leer la imagen desde un link url
import numpy as np
# Configurar MQTT 
mqtt_broker = "4d58dc4d4da64daca6f0003289196edb.s1.eu.hivemq.cloud" # Dirección IP del broker MQTT
mqtt_port = 8883  
mqtt_topics = ["C0", "C1", "C2", "C3", "C4", "C5"]
mqtt_username = "MAMALU" 
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

# URL de la ESP32-CAM y ruta de guardado
url = "http://192.168.10.96/capture"
ruta_imagen = "./estacionamiento/imagen_capturada.jpg"

# Dimensiones y márgenes para ROIs
espacios = [
    (160, 135, 50, 85),    # Espacio 1 
    (110, 135, 50, 85),   # Espacio 2
    (50, 135, 60, 85),   # Espacio 3
    (50, 5, 55, 100),   # Espacio 4
    (105, 5, 55, 100),  # Espacio 5
    (160, 5, 55, 100)   # Espacio 6
]
umbral_diferencia = 800

while True:
    # Capturar imagen desde la ESP32-CAM
    try:
        response = urllib.request.urlopen(url)
        image_np = np.array(bytearray(response.read()), dtype=np.uint8)
        image = cv2.imdecode(image_np, -1)
        cv2.imwrite(ruta_imagen, image)
        print("Imagen guardada en:", ruta_imagen)
    except Exception as e:
        print("Error al capturar imagen:", e)
        break

    # Cargar imágenes de referencia y actual
    foto_referencia = "./estacionamiento/estacionamiento_vacio.jpg"  # Imagen de referencia
    imagen_referencia = cv2.imread(foto_referencia, cv2.IMREAD_GRAYSCALE)
    imagen_actual = cv2.imread(ruta_imagen, cv2.IMREAD_GRAYSCALE)

    if imagen_referencia is None or imagen_actual is None:
        print("Error: No se cargaron las imágenes")
        break
    listaCajones = []
    # Analizar cada espacio
    for i, (x, y, w, h) in enumerate(espacios, 1):
        roi_referencia = imagen_referencia[y:y+h, x:x+w]
        roi_actual = imagen_actual[y:y+h, x:x+w]

        # Calcular diferencias
        diferencia = cv2.absdiff(roi_referencia, roi_actual)
        _, diferencia_binaria = cv2.threshold(diferencia, 30, 255, cv2.THRESH_BINARY)
        diferencia_total = cv2.countNonZero(diferencia_binaria)
        if diferencia_total > umbral_diferencia:
            #print(f"Espacio {i} - Ocupado")
            listaCajones.append(i)
        else:
            #print(f"Espacio {i} - Libre")
            pass
        
    # Publicar en MQTT
    if 1 in listaCajones:
        client.publish("C0", 1)
        print("Cajón 1 ocupado")
    else:
        client.publish("C0", 0)
        print("Cajón 1 libre")
        pass
    if 2 in listaCajones:
        client.publish("C1", 1)
        print("Cajón 2 ocupado")
    else:
        client.publish("C1", 0)
        print("Cajón 2 libre")
        pass
    if 3 in listaCajones:
        client.publish("C2", 1)
        print("Cajón 3 ocupado")
    else:
        client.publish("C2", 0)
        print("Cajón 3 libre")
        pass
    if 4 in listaCajones:
        client.publish("C3", 1)
        print("Cajón 4 ocupado")
    else:
        client.publish("C3", 0)
        print("Cajón 4 libre")
        pass
    if 5 in listaCajones:
        client.publish("C4", 1)
        print("Cajón 5 ocupado")
    else:
        client.publish("C4", 0)
        print("Cajón 5 libre")
        pass
    if 6 in listaCajones:
        client.publish("C5", 1)
        print("Cajón 6 ocupado")
    else:
        client.publish("C5", 0)
        print("Cajón 6 libre")
        pass
    print(listaCajones)

    # Mostrar imagen procesada con ROIs
    imagen_con_areas = cv2.imread(ruta_imagen)
    for x, y, w, h in espacios:
        cv2.rectangle(imagen_con_areas, (x, y), (x+w, y+h), (0, 255, 0), 2)
    cv2.imshow("Estacionamiento", imagen_con_areas)

    # Salir con 'ESC'
    if cv2.waitKey(5000) & 0xFF == 27:
        break

cv2.destroyAllWindows()
