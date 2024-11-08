#define LED1 14
#define LED2 27
#define LED3 26
#define LED4 25
#define BOTON 32
#define BOTON2 35

int val;
int estadoBotonAnterior = LOW;
int animacion = 0;

void setup() {
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);
  pinMode(BOTON, INPUT);
  pinMode(BOTON2, INPUT);
  Serial.begin(9600);
  attachInterrupt(digitalPinToInterrupt(BOTON), FuncionInterrupcion, RISING);
  attachInterrupt(digitalPinToInterrupt(BOTON2), FuncionInterrupcion2, RISING);
}

void FuncionInterrupcion() {
  animacion = (animacion + 1);
  
  if (animacion >= 4) {
    animacion = 1;
    
    
  }
  
}

void FuncionInterrupcion2() {
  animacion = 4;
  
  
}

void loop() {
  estadoBotonAnterior = val;
  switch (animacion) {
    case 0:
      animacion0();
      break;
    case 1:
      animacion1();
      break;
    case 2:
      animacion2();
      break;
    case 3:
      animacion3();
      break;
  }
}

void animacion0() {
  digitalWrite(LED1, HIGH);
  digitalWrite(LED2, LOW);
  digitalWrite(LED3, LOW);
  digitalWrite(LED4, LOW);
  delay(400); 
  digitalWrite(LED1, LOW);
  digitalWrite(LED2, LOW);
  digitalWrite(LED3, LOW);
  digitalWrite(LED4, LOW);
  delay(400);
}

void animacion1() {
  digitalWrite(LED1, HIGH);
  digitalWrite(LED2, HIGH);
  digitalWrite(LED3, LOW);
  digitalWrite(LED4, LOW);
  delay(400); 
  digitalWrite(LED1, LOW);
  digitalWrite(LED2, LOW);
  digitalWrite(LED3, LOW);
  digitalWrite(LED4, LOW);
  delay(400);
}

void animacion2() {
  digitalWrite(LED1, HIGH);
  digitalWrite(LED2, HIGH);
  digitalWrite(LED3, HIGH);
  digitalWrite(LED4, LOW);
  delay(400); 
  digitalWrite(LED1, LOW);
  digitalWrite(LED2, LOW);
  digitalWrite(LED3, LOW);
  digitalWrite(LED4, LOW);
  delay(400);
}

void animacion3() {
  digitalWrite(LED1, HIGH);
  digitalWrite(LED2, HIGH);
  digitalWrite(LED3, HIGH);
  digitalWrite(LED4, HIGH);
  delay(400); 
  digitalWrite(LED1, LOW);
  digitalWrite(LED2, LOW);
  digitalWrite(LED3, LOW);
  digitalWrite(LED4, LOW);
  delay(400);
}

void animacion4() {
  digitalWrite(LED1, LOW);
  digitalWrite(LED2, LOW);
  digitalWrite(LED3, LOW);
  digitalWrite(LED4, LOW);
  delay(4000);

}