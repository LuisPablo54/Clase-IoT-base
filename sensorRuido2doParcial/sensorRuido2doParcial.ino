// Autores: Luis Pablo López, Diego Mares Rodríguez, María José Gamba Santibáñez
#define pinSensorRuido 14 
#define pinLED 12       

// Umbral
const int umbralRuido = 3000;    // El sensor nos daba un tope de 4095

void setup() {
  pinMode(pinSensorRuido, INPUT);  
  pinMode(pinLED, OUTPUT);          
  Serial.begin(9600);             
}

void loop() {
  
  int valorRuido = analogRead(pinSensorRuido);
  
 
  Serial.print("Nivel de ruido: ");
  Serial.println(valorRuido);
  
  
  if (valorRuido < umbralRuido) {
    digitalWrite(pinLED, HIGH);  
    delay(500);
  } else {
    digitalWrite(pinLED, LOW);   
  }
  
  delay(500);  
}
