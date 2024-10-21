#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <WebSocketsServer.h>
#include "index.h"

// Define los comandos
#define CMD_STOP            0
#define CMD_CABLE_UP        1
#define CMD_CABLE_DOWN      2
#define CMD_ARM_UP          4
#define CMD_ARM_DOWN        8
#define CMD_GEAR_LEFT       16
#define CMD_GEAR_RIGHT      32
#define CMD_BRIDGE_UP       64
#define CMD_BRIDGE_DOWN     128
#define CMD_FORWARD         256 
#define CMD_BACKWARD        512 
#define CMD_TURN_LEFT       1024 
#define CMD_TURN_RIGHT      2048 

// Define los pines para controlar el hardware
#define ENB_PIN  14  // Pin ENB para el brazo
#define IN4_PIN  26  // Pin IN4 para el brazo
#define IN3_PIN  27  // Pin IN3 para el brazo

#define IN2_PIN  25  // Pin IN2 para el cable
#define IN1_PIN  33  // Pin IN1 para el cable
#define ENA_PIN  32  // Pin ENA para el cable


//#define ENC_PIN  13  // Pin ENC para la base (JUMPER)
#define IN5_PIN  12   // Pin IN5 para la base 
#define IN6_PIN  13   // Pin IN6 para la base 
                 

#define IN7_PIN  23  // Pin IN7 para el puente
#define IN8_PIN  22  // Pin IN8 para el puente
#define END_PIN  21  // Pin END  el puente

#define IN9_PIN   19  // Pin IN8 ruedas Izquierdas
#define IN10_PIN  18  // Pin IN9 ruedas Izquierdas
#define IN11_PIN  5   // Pin IN10 ruedas Derechas 
#define IN12_PIN  4   // Pin IN11 ruedas Derechas 

// Credenciales de la red WiFi
const char* ssid = "prueba";          // Cambia esto por tu SSID de WiFi
const char* password = "queseyo2024"; // Cambia esto por tu contraseña de WiFi

AsyncWebServer server(80);
WebSocketsServer webSocket = WebSocketsServer(81);

void setup() {
  Serial.begin(9600);

  // Configura los pines como salida
  // CABLE
  pinMode(ENA_PIN, OUTPUT);
  pinMode(IN1_PIN, OUTPUT);
  pinMode(IN2_PIN, OUTPUT);
  
  //BRAZO
  pinMode(ENB_PIN, OUTPUT);
  pinMode(IN3_PIN, OUTPUT);
  pinMode(IN4_PIN, OUTPUT);

//Base
  pinMode(IN5_PIN, OUTPUT);
  pinMode(IN6_PIN, OUTPUT);

//Puente
  pinMode(END_PIN, OUTPUT);
  pinMode(IN7_PIN, OUTPUT);
  pinMode(IN8_PIN, OUTPUT);

  //Ruedas
  pinMode(IN9_PIN, OUTPUT);
  pinMode(IN10_PIN, OUTPUT);
  pinMode(IN11_PIN, OUTPUT);
  pinMode(IN12_PIN, OUTPUT);


  // Inicializa los pines en estado bajo
  digitalWrite(ENA_PIN, LOW);
  digitalWrite(ENB_PIN, LOW);
  digitalWrite(END_PIN, LOW);
  digitalWrite(IN1_PIN, LOW);
  digitalWrite(IN2_PIN, LOW);
  digitalWrite(IN3_PIN, LOW);
  digitalWrite(IN4_PIN, LOW);
  digitalWrite(IN5_PIN, LOW);
  digitalWrite(IN6_PIN, LOW);
  digitalWrite(IN7_PIN, LOW);
  digitalWrite(IN8_PIN, LOW);
  digitalWrite(IN9_PIN, LOW);
  digitalWrite(IN10_PIN, LOW);
  digitalWrite(IN11_PIN, LOW);
  digitalWrite(IN12_PIN, LOW);


  // Conéctate a la red WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando a WiFi...");
  }
  Serial.println("Conectado a WiFi");

  // Inicializa el servidor WebSocket
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);

  // Configura el servidor web
  server.on("/", HTTP_GET, [](AsyncWebServerRequest* request) {
    Serial.println("Solicitud de página web recibida");
    String html = HTML_CONTENT;  // Utiliza el contenido HTML del archivo index.h
    request->send(200, "text/html", html);
  });

  server.begin();
  Serial.print("Dirección IP del servidor web ESP32: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  webSocket.loop();
}

// Función para manejar los eventos de WebSocket
void webSocketEvent(uint8_t num, WStype_t type, uint8_t* payload, size_t length) {
  switch (type) {
    case WStype_DISCONNECTED:
      Serial.printf("[%u] Desconectado!\n", num);
      break;
    case WStype_CONNECTED:
      {
        IPAddress ip = webSocket.remoteIP(num);
        Serial.printf("[%u] Conectado desde %d.%d.%d.%d\n", num, ip[0], ip[1], ip[2], ip[3]);
      }
      break;
    case WStype_TEXT:
      String angle = String((char*)payload);
      int command = angle.toInt();
      Serial.print("Comando: ");
      Serial.println(command);

      switch (command) {
        case CMD_STOP:
          Serial.println("Detener");
          ARM_stop();
          CABLE_stop();
          GEAR_stop();
          BRIDGE_stop();
          WHEELS_stop();
          break;
        case CMD_CABLE_UP:
          Serial.println("Cable hacia arriba");
          CABLE_moveUp();
          break;
        case CMD_CABLE_DOWN:
          Serial.println("Cable hacia abajo");
          CABLE_moveDown();
          break;
        case CMD_ARM_UP:
          Serial.println("Brazo hacia arriba");
          ARM_moveUp();
          break;
        case CMD_ARM_DOWN:
          Serial.println("Brazo hacia abajo");
          ARM_moveDown();
          break;
        case CMD_GEAR_LEFT:
          Serial.println("Giro de grúa a la izquierda");
          GEAR_moveLeft();
          break;
        case CMD_GEAR_RIGHT:
          Serial.println("Giro de grúa a la derecha");
          GEAR_moveRight();
          break;
        case CMD_BRIDGE_UP:
          Serial.println("Puente hacia arriba");
          BRIDGE_moveUp();
          break;
        case CMD_BRIDGE_DOWN:
          Serial.println("Puente hacia la abajo");
          BRIDGE_moveDown();
          break;
        case CMD_FORWARD:
          Serial.println("Avanzar");
          WHEELS_forward();
          break;
        case CMD_BACKWARD:
          Serial.println("Retroceder");
          WHEELS_backward();
          break;
        case CMD_TURN_LEFT:
          Serial.println("Girar a la izquierda");
          WHEELS_turnLeft();
          break;
       case CMD_TURN_RIGHT:
        Serial.println("Girar a la derecha");
        WHEELS_turnRight();
       break;
      default:
          Serial.println("Comando desconocido");
      }
      break;
  }
}

// Funciones para controlar el movimiento del cable
void CABLE_moveUp() {
  analogWrite(ENB_PIN, 150);  // Control de velocidad (PWM)
  digitalWrite(IN3_PIN, HIGH);
  digitalWrite(IN4_PIN, LOW);
}

void CABLE_moveDown() {
  analogWrite(ENB_PIN, 75);  // Control de velocidad (PWM)
  digitalWrite(IN3_PIN, LOW);
  digitalWrite(IN4_PIN, HIGH);
}

void CABLE_stop() {
  digitalWrite(IN3_PIN, LOW);
  digitalWrite(IN4_PIN, LOW);
}

// Funciones para controlar el movimiento del brazo
void ARM_moveUp() {
  analogWrite(ENA_PIN, 175);  // Control de velocidad a la mitad (PWM)
  digitalWrite(IN1_PIN, HIGH);
  digitalWrite(IN2_PIN, LOW);
}

void ARM_moveDown() {
  analogWrite(ENA_PIN, 75);  // Control de velocidad a la mitad (PWM)
  digitalWrite(IN1_PIN, LOW);
  digitalWrite(IN2_PIN, HIGH);
}

void ARM_stop() {
  digitalWrite(IN1_PIN, LOW);
  digitalWrite(IN2_PIN, LOW);
}

// Funciones para controlar el giro de la grúa
void GEAR_moveLeft() {
  //analogWrite(ENC_PIN, 256);  // Control de velocidad a la mitad (PWM)
  digitalWrite(IN5_PIN, HIGH);
  digitalWrite(IN6_PIN, LOW);
}

void GEAR_moveRight() {
 // analogWrite(ENC_PIN, 256);  // Control de velocidad a la mitad (PWM)
  digitalWrite(IN5_PIN, LOW);
  digitalWrite(IN6_PIN, HIGH);
}

void GEAR_stop() {
  digitalWrite(IN5_PIN, LOW);
  digitalWrite(IN6_PIN, LOW);
}

// Funciones para controlar el movimiento del puente
void BRIDGE_moveUp() {
  analogWrite(END_PIN, 150);  // Control de velocidad a la mitad (PWM)
  digitalWrite(IN7_PIN, HIGH);
  digitalWrite(IN8_PIN, LOW);
}

void BRIDGE_moveDown() {
  analogWrite(END_PIN, 150);  // Control de velocidad a la mitad (PWM)
  digitalWrite(IN7_PIN, LOW);
  digitalWrite(IN8_PIN, HIGH);
}

void BRIDGE_stop() {
  digitalWrite(IN7_PIN, LOW);
  digitalWrite(IN8_PIN, LOW);
}

// Funciones para controlar el movimiento de las ruedas
void WHEELS_forward() {
  digitalWrite(IN9_PIN, HIGH);  // Motor izquierdo hacia adelante
  digitalWrite(IN10_PIN, LOW);   // Motor izquierdo hacia adelante
  digitalWrite(IN11_PIN, HIGH);  // Motor derecho hacia adelante
  digitalWrite(IN12_PIN, LOW);   // Motor derecho hacia adelante
}

void WHEELS_backward() {
  digitalWrite(IN9_PIN, LOW);    // Motor izquierdo hacia atrás
  digitalWrite(IN10_PIN, HIGH);   // Motor izquierdo hacia atrás
  digitalWrite(IN11_PIN, LOW);    // Motor derecho hacia atrás
  digitalWrite(IN12_PIN, HIGH);   // Motor derecho hacia atrás
}

void WHEELS_stop() {
  digitalWrite(IN9_PIN, LOW);
  digitalWrite(IN10_PIN, LOW);
  digitalWrite(IN11_PIN, LOW);
  digitalWrite(IN12_PIN, LOW);
}

// Funciones para controlar el giro de las ruedas
void WHEELS_turnLeft() {
  digitalWrite(IN9_PIN, LOW);   // Motor izquierdo hacia atrás
  digitalWrite(IN10_PIN, HIGH);  // Motor izquierdo hacia atrás
  digitalWrite(IN11_PIN, HIGH);  // Motor derecho hacia adelante
  digitalWrite(IN12_PIN, LOW);   // Motor derecho hacia adelante
}

void WHEELS_turnRight() {
  digitalWrite(IN9_PIN, HIGH);   // Motor izquierdo hacia adelante
  digitalWrite(IN10_PIN, LOW);    // Motor izquierdo hacia adelante
  digitalWrite(IN11_PIN, LOW);    // Motor derecho hacia atrás
  digitalWrite(IN12_PIN, HIGH);   // Motor derecho hacia atrás
}
