//Luis Pablo
//Maria
//Diego Mares

int lectura = 0;                 // Contador de lecturas
const int botonPin = 35;          // Pin donde está conectado el botón
const int Potenciometro = 34;          // Pin donde está conectado el botón
int estadoBoton = 0;             // Estado del botón (presionado o no)

void setup() {
  Serial.begin(9600);            // Inicializa la comunicación serial a 9600 bps
  pinMode(botonPin, INPUT_PULLUP); // Configura el pin del botón con pull-up interno
  pinMode(Potenciometro, INPUT_PULLUP);
}

void loop() {
  estadoBoton = digitalRead(botonPin); // Lee el estado del botón

  if (estadoBoton == LOW) {      // Si el botón está presionado (LOW debido al pull-up)
    int valorADC = analogRead(Potenciometro);      // Lee el valor ADC del pin A0
    float voltaje = (3.3 * valorADC) / 4095.0; // Calcula el voltaje
    lectura = lectura + 1;               // Incrementa el contador de lecturas

    // Envía los datos por serial
    Serial.print(lectura);
    Serial.print(",");
    Serial.print(valorADC);
    Serial.print(",");
    Serial.println(voltaje);

    delay(500);  // Pequeño retraso para evitar lecturas múltiples por una sola pulsación
  }
}

