'''
Autores:
    - Luis Pablo López Iracheta
    - María José Gamba Santibáñez
    - Diego Mares Rodríguez
'''
# Importación de librerías
import serial
import csv
import time
from datetime import datetime

PuertoSer = serial.Serial('COM7', 9600) # Puerto serial al que está conectado el ESP32
time.sleep(2)

with open('DeteccionesProy2.csv', mode='w', newline='') as file:
    writer = csv.writer(file)
    writer.writerow(["Año", "Mes", "Día", "Hora", "Minuto", "Segundo", "Tipo de Dato"]) # Se escribe la cabecera del archivo csv

    try:
        while True:
            line = PuertoSer.readline().decode('utf-8', errors='ignore').strip()
            if "Movimiento detectado" in line:
                now = datetime.now()
                writer.writerow([now.year, now.month, now.day, now.hour, now.minute, now.second, "Movimiento"]) # Se escribe en el archivo csv
                print(f"Movimiento detectado: {now}")
    except KeyboardInterrupt:
        print("Finalizando...")

PuertoSer.close()

