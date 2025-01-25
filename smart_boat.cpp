#include <Servo.h>
#include <LiquidCrystal_I2C.h>

// Definición de pines para Arduino UNO R3
const int TRIG_FRENTE = 7;
const int ECHO_FRENTE = 8;
const int TRIG_IZQUIERDA = 2;
const int ECHO_IZQUIERDA = 3;
const int TRIG_DERECHA = 4;
const int ECHO_DERECHA = 5;
const int SERVO_PIN = 9;
const int MOTOR_IN1 = 10;
const int MOTOR_IN2 = 11;
const int LED_VERDE = 12;
const int LED_ROJO = 13;
const int VELOCIDAD_PIN = A0;

// Objetos
Servo timon;
int posicionTimon = 90;  // Posición inicial en el centro
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Variables globales
int distanciaFrente, distanciaIzquierda, distanciaDerecha;
int velocidad;
bool modoAutomatico = false;
bool modoTest = false;
unsigned long ultimoTiempo = 0;
const long INTERVALO_TEST = 1000; // Intervalo para pruebas (1 segundo)

void setup() {
  // Inicialización de pines
  pinMode(TRIG_FRENTE, OUTPUT);
  pinMode(ECHO_FRENTE, INPUT);
  pinMode(TRIG_IZQUIERDA, OUTPUT);
  pinMode(ECHO_IZQUIERDA, INPUT);
  pinMode(TRIG_DERECHA, OUTPUT);
  pinMode(ECHO_DERECHA, INPUT);
  pinMode(MOTOR_IN1, OUTPUT);
  pinMode(MOTOR_IN2, OUTPUT);
  pinMode(LED_VERDE, OUTPUT);
  pinMode(LED_ROJO, OUTPUT);
  
  // Inicialización de componentes
  timon.attach(SERVO_PIN);
  lcd.init();
  lcd.backlight();
  Serial.begin(9600);
  
  // Posición inicial
  timon.write(posicionTimon); // Posicionar servo inicialmente en el centro

  // Mensaje inicial
  Serial.println("Sistema de Bote Inteligente");
  Serial.println("Comandos disponibles:");
  Serial.println("T: Modo Test");
  Serial.println("A: Modo Automático");
  Serial.println("M: Modo Manual");
  Serial.println("En Modo Manual:");
  Serial.println("F: Adelante");
  Serial.println("B: Atrás");
  Serial.println("L: Izquierda");
  Serial.println("R: Derecha");
  Serial.println("S: Detener");
  
  // Test inicial de componentes
  testInicial();
}

void testInicial() {
  Serial.println("\nIniciando test de componentes...");
  
  // Test LEDs
  Serial.println("Probando LEDs...");
  digitalWrite(LED_VERDE, HIGH);
  delay(500);
  digitalWrite(LED_VERDE, LOW);
  digitalWrite(LED_ROJO, HIGH);
  delay(500);
  digitalWrite(LED_ROJO, LOW);
  
  // Test Servo
  Serial.println("Probando servo...");
  timon.write(45);
  delay(500);
  timon.write(135);
  delay(500);
  timon.write(90);
  
  // Test Motor
  Serial.println("Probando motor...");
  avanzar();
  delay(500);
  detener();
  
  // Test Sensores
  Serial.println("Probando sensores...");
  testSensores();
  
  Serial.println("Test inicial completado!\n");
}

void testSensores() {
  // Lectura de sensores
  distanciaFrente = medirDistancia(TRIG_FRENTE, ECHO_FRENTE);
  delay(50);
  distanciaIzquierda = medirDistancia(TRIG_IZQUIERDA, ECHO_IZQUIERDA);
  delay(50);
  distanciaDerecha = medirDistancia(TRIG_DERECHA, ECHO_DERECHA);
  
  // Mostrar lecturas
  Serial.println("Lecturas de sensores:");
  Serial.print("Frente: ");
  Serial.print(distanciaFrente);
  Serial.println(" cm");
  Serial.print("Izquierda: ");
  Serial.print(distanciaIzquierda);
  Serial.println(" cm");
  Serial.print("Derecha: ");
  Serial.print(distanciaDerecha);
  Serial.println(" cm");
}

void loop() {
  // Lectura de comandos seriales
  if (Serial.available()) {
    char comando = Serial.read();
    procesarComando(comando);
  }
  
  // Lectura de sensores y actualización de pantalla
  leerSensores();
  actualizarLCD();
  controlarLEDs();
  
  // Modo de operación
  if (modoTest) {
    ejecutarTest();
  } else if (modoAutomatico) {
    navegacionAutomatica();
  }
  
  delay(100);
}

void procesarComando(char comando) {
  Serial.print("Comando recibido: ");
  Serial.println(comando);
  
  switch (comando) {
    case 'T':
      modoTest = !modoTest;
      modoAutomatico = false;
      Serial.println(modoTest ? "Modo Test ACTIVADO" : "Modo Test DESACTIVADO");
      break;
    case 'A':
      modoAutomatico = !modoAutomatico;
      modoTest = false;
      Serial.println(modoAutomatico ? "Modo Automático ACTIVADO" : "Modo Automático DESACTIVADO");
      break;
    case 'M':
      modoAutomatico = false;
      modoTest = false;
      Serial.println("Modo Manual ACTIVADO");
      break;
    case 'F':
      if (!modoAutomatico && !modoTest) {
        avanzar();
        Serial.println("Avanzando");
      }
      break;
    case 'B':
      if (!modoAutomatico && !modoTest) {
        retroceder();
        Serial.println("Retrocediendo");
      }
      break;
    case 'L':
      if (!modoAutomatico && !modoTest) {
        girarIzquierda();
        Serial.println("Girando Izquierda");
      }
      break;
    case 'R':
      if (!modoAutomatico && !modoTest) {
        girarDerecha();
        Serial.println("Girando Derecha");
      }
      break;
    case 'S':
      if (!modoAutomatico && !modoTest) {
        detener();
        Serial.println("Detenido");
      }
      break;
  }
}

void ejecutarTest() {
  unsigned long tiempoActual = millis();

  if (tiempoActual - ultimoTiempo >= INTERVALO_TEST) {
    ultimoTiempo = tiempoActual;
    
    // Test de sensores
    testSensores();
    
    // Test de velocidad
    velocidad = analogRead(VELOCIDAD_PIN);
    Serial.print("Lectura de velocidad: ");
    Serial.print(map(velocidad, 0, 1023, 0, 100));
    Serial.println("%");
    
    // Estado del sistema
    Serial.print("Estado del sistema: ");
    if (distanciaFrente > 50 && distanciaIzquierda > 30 && distanciaDerecha > 30) {
      Serial.println("Camino libre");
    } else {
      Serial.println("Obstáculo detectado");
    }
    Serial.println("--------------------");
  }
}

void leerSensores() {
  distanciaFrente = medirDistancia(TRIG_FRENTE, ECHO_FRENTE);
  distanciaIzquierda = medirDistancia(TRIG_IZQUIERDA, ECHO_IZQUIERDA);
  distanciaDerecha = medirDistancia(TRIG_DERECHA, ECHO_DERECHA);
  velocidad = analogRead(VELOCIDAD_PIN);
}

int medirDistancia(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  long duracion = pulseIn(echoPin, HIGH);
  int distancia = duracion * 0.034 / 2;
  
  return distancia;
}

void actualizarLCD() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("F:");
  lcd.print(distanciaFrente);
  lcd.print(" I:");
  lcd.print(distanciaIzquierda);
  lcd.print(" D:");
  lcd.print(distanciaDerecha);
  
  lcd.setCursor(0, 1);
  if (modoTest) {
    lcd.print("MODO TEST");
  } else if (modoAutomatico) {
    lcd.print("AUTO ");
  } else {
    lcd.print("MANUAL ");
  }
  lcd.print(map(velocidad, 0, 1023, 0, 100));
  lcd.print("%");
}

void controlarLEDs() {
  if (distanciaFrente > 50 && distanciaIzquierda > 30 && distanciaDerecha > 30) {
    digitalWrite(LED_VERDE, HIGH);
    digitalWrite(LED_ROJO, LOW);
  } else {
    digitalWrite(LED_VERDE, LOW);
    digitalWrite(LED_ROJO, HIGH);
  }
}

void navegacionAutomatica() {
  if (distanciaFrente > 50 && distanciaIzquierda > 30 && distanciaDerecha > 30) {
    avanzar();
    timon.write(90);
  } else if (distanciaIzquierda > distanciaDerecha) {
    girarIzquierda();
  } else if (distanciaDerecha > distanciaIzquierda) {
    girarDerecha();
  } else {
    detener();
  }
}

void avanzar() {
  digitalWrite(MOTOR_IN1, HIGH);
  digitalWrite(MOTOR_IN2, LOW);
	
}

void retroceder() {
  digitalWrite(MOTOR_IN1, LOW);
  digitalWrite(MOTOR_IN2, HIGH);
}

void girarIzquierda() {
    // Incrementa el ángulo para girar hacia la izquierda, pero no más allá de 180
    if (posicionTimon < 180) {
        posicionTimon += 10; // Ajusta el valor (10) para la velocidad de giro
        timon.write(posicionTimon);
        Serial.print("Girando Izquierda: ");
        Serial.println(posicionTimon);
    } else {
        Serial.println("Máximo giro a la izquierda alcanzado");
    }
}

void girarDerecha() {
    // Decrementa el ángulo para girar hacia la derecha, pero no más allá de 0
    if (posicionTimon > 0) {
        posicionTimon -= 10; // Ajusta el valor (10) para la velocidad de giro
        timon.write(posicionTimon);
        Serial.print("Girando Derecha: ");
        Serial.println(posicionTimon);
    } else {
        Serial.println("Máximo giro a la derecha alcanzado");
    }
}

void detener() {
  digitalWrite(MOTOR_IN1, LOW);
  digitalWrite(MOTOR_IN2, LOW);
}
