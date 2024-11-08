""" Detección de manos con la cámara de la laptop, es necesario instalar MediaPipe antes de ejecutar"""
 
import cv2	
# OpenCV es una biblioteca de visión por computadora que permite capturar y procesar tanto imágenes como video
import mediapipe as mp	
# Mediapipe es una biblioteca de google para el procesamiento de imágenes, puede detectar, manos, rostros, iris, poses, seguimiento y detección de objetos, entre otros.
# Se utiliza para la creación rápida de prototipos de canales de percepción con modelos de inteligencia artificial y aprendizaje automático.
# https://mediapipe-readthedocs-io.translate.goog/en/latest/solutions/hands.html?_x_tr_sl=en&_x_tr_tl=es&_x_tr_hl=es&_x_tr_pto=rq

# Inicializa MediaPipe Hands para detectar las posiciones de las manos en las imagenes que captura
mp_hands = mp.solutions.hands
hands = mp_hands.Hands()

# Inicializa MediaPipe Drawing Utilities para dibujar las conexiones de las manos detectadas
mp_drawing = mp.solutions.drawing_utils

# Iniciar captura de video en tiempo real
cap = cv2.VideoCapture(0)  # Inicia la cámara de la laptop, esto se especifica con el (0)

# Mientras la cámara esté abierta se captura cada fotograma y se comprueba si la conexión fue exitosa
# La cámara se voltea (en espejo) y se convierte de BGR (formato de OpenCV) a RGB (formato de Mediapipe)
while cap.isOpened():
    success, image = cap.read()
    
    if not success:
        print("Ignorando frame vacío")
        continue

    image = cv2.cvtColor(cv2.flip(image, 1), cv2.COLOR_BGR2RGB)

    # Procesa la imagen y detecta las manos
    results = hands.process(image)

    image = cv2.cvtColor(image, cv2.COLOR_RGB2BGR)

    # Dibuja las manos detectadas con sus marcas y conexiones
    if results.multi_hand_landmarks:
        for hand_landmarks in results.multi_hand_landmarks:
            mp_drawing.draw_landmarks(
                image, hand_landmarks, mp_hands.HAND_CONNECTIONS)

    # Muestra la imagen en una ventana que llama Detector de manos y se detiene cuando se presiona la tecla ESC (ASCII 27)
    cv2.imshow('Detector de manos', image)
    if cv2.waitKey(5) & 0xFF == 27:
        break
# Se libera la cámara y las ventanas de OpenCV cuando se termina el programa
cap.release()
cv2.destroyAllWindows()