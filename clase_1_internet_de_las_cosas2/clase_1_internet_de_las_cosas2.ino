// LUIS PABLO LOPEZ 
// MARIA JOSE GAMBA
// DIEGO MARES 

#define LED1 14
#define LED2 27
#define LED3 26
#define LED4 25
#define BOTON1 32  // Botón para cambiar la animación
#define BOTON2 35  // Botón para apagar todos los LEDs

int valBoton1;
int valBoton2;
int estadoBotonAnterior1 = LOW;
int animacion = 0;
unsigned long tiempoAnimacion = 2000;  // Duración de cada animación en milisegundos (2 segundos)
unsigned long tiempoInicioAnimacion = 0;

void setup() {
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);
  pinMode(BOTON1, INPUT);
  pinMode(BOTON2, INPUT);
}

void loop() {
  valBoton1 = digitalRead(BOTON1);
  valBoton2 = digitalRead(BOTON2);

  // Si se presiona el botón 2, apaga todos los LEDs
  if (valBoton2 == HIGH) {
    apagarLeds();
    animacion = 0; // Resetea la animación para que empiece en la primera cuando se presione el BOTON1
  }

  // Si se presiona el botón 1 y el estado anterior era LOW, cambia la animación
  if (valBoton1 == HIGH ) {
    animacion = (animacion + 1); // Cambia de animación cada vez que se presiona el botón
    tiempoInicioAnimacion = millis();  // Reinicia el tiempo cuando cambia la animación
    delay(200); // Pequeño delay para evitar rebotes
  }


  // Cambia de animación automáticamente después de cierto tiempo
  if (millis() - tiempoInicioAnimacion >= tiempoAnimacion && valBoton2 == LOW) {
    animacion = (animacion + 1) % 4;  // Cambia automáticamente a la siguiente animación
    tiempoInicioAnimacion = millis(); // Reinicia el tiempo de la nueva animación
  }

  // Ejecuta la animación correspondiente si no se está presionando el botón 2
  if (valBoton2 == LOW) {
    switch (animacion) {
      case 0:
        animacion1();
        break;
      case 1:
        animacion2();
        break;
      case 2:
        animacion3();
        break;
      case 3:
        animacion4();
        break;
    }
  }
}


void animacion1() {
  digitalWrite(LED1, HIGH);
  digitalWrite(LED2, LOW);
  digitalWrite(LED3, LOW);
  digitalWrite(LED4, LOW);
  delay(250); 
  digitalWrite(LED1, LOW);
  delay(250);
}

void animacion2() {
  digitalWrite(LED1, HIGH);
  digitalWrite(LED2, HIGH);
  digitalWrite(LED3, LOW);
  digitalWrite(LED4, LOW);
  delay(250); 
  digitalWrite(LED1, LOW);
  digitalWrite(LED2, LOW);
  delay(250);
}

void animacion3() {
  digitalWrite(LED1, HIGH);
  digitalWrite(LED2, HIGH);
  digitalWrite(LED3, HIGH);
  digitalWrite(LED4, LOW);
  delay(250); 
  digitalWrite(LED1, LOW);
  digitalWrite(LED2, LOW);
  digitalWrite(LED3, LOW);
  delay(250);
}

void animacion4() {
  digitalWrite(LED1, HIGH);
  digitalWrite(LED2, HIGH);
  digitalWrite(LED3, HIGH);
  digitalWrite(LED4, HIGH);
  delay(250); 
  digitalWrite(LED1, LOW);
  digitalWrite(LED2, LOW);
  digitalWrite(LED3, LOW);
  digitalWrite(LED4, LOW);
  delay(250);
}

void apagarLeds() {
  digitalWrite(LED1, LOW);
  digitalWrite(LED2, LOW);
  digitalWrite(LED3, LOW);
  digitalWrite(LED4, LOW);
}
