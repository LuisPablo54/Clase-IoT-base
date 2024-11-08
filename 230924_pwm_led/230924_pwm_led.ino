// Autores:
// - Luis Pablo López Iracheta
// - María José Gamba Santibáñez
//  - Diego Mares Rodríguez

// Definir los pines del LED RGB
const int redPin = 25;
const int greenPin = 26;
const int bluePin = 27;
const int buttonPin = 32;

int buttonState = 0;
int currentColor = 0;  // Variable para rastrear el color actual

void setup() {
  // Configurar los pines del LED como salidas
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);

  // Configurar el pin del botón como entrada
  pinMode(buttonPin, INPUT_PULLUP);
}

void loop() {
  // Leer el estado del botón
  buttonState = digitalRead(buttonPin);

  // Si el botón se presiona (estado LOW)
  if (buttonState == LOW) {
    delay(300); // Pequeño retraso para evitar rebotes
    currentColor++;
    if (currentColor > 8) {  // Hay 8 combinaciones posibles (0 a 7)
      currentColor = 0;
    }

    // Cambiar el color del LED RGB dependiendo de la variable currentColor
    switch (currentColor) {
      case 0: setColor(0, 0, 0); break;    // Apagado
      case 1: setColor(255, 0, 0); break;  // Rojo
      case 2: setColor(0, 255, 0); break;  // Verde
      case 3: setColor(0, 0, 255); break;  // Azul
      case 4: setColor(255, 255, 0); break;  // Amarillo
      case 5: setColor(255, 0, 255); break;  // Morado
      case 6: setColor(0, 255, 255); break;  // Turquesa
      case 7: setColor(255, 255, 255); break;  // Blanco
      case 8: setColor(228, 0, 124); break;  // Rosa
    }
  }
}

// Función para configurar el color del LED RGB
void setColor(int redValue, int greenValue, int blueValue) {
  analogWrite(redPin, redValue);
  analogWrite(greenPin, greenValue);
  analogWrite(bluePin, blueValue);
}
