'''
Luis Pablo
Maria Jose
Diego Mares
'''


import serial 
import csv 
import time

PuertoSer = serial.Serial('COM7', 9600)
time.sleep(2)

with open('DatosArduino.csv', mode='w', newline='') as file:
    writer = csv.writer(file)
    writer.writerow(["Lectura", "Valor ADC", "Voltaje"])

    try:
        while True:
            line = PuertoSer.readline().decode('utf-8').strip()
            print(line)
            data = line.split(",")
            writer.writerow(data)
    except KeyboardInterrupt:
        print("Finalizando...")

PuertoSer.close()
