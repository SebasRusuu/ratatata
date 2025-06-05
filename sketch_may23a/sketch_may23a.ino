#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// — Dimensões e endereço do OLED —
#define SCREEN_WIDTH   128
#define SCREEN_HEIGHT   64
#define OLED_RESET     -1
#define SCREEN_ADDR    0x3C

// — Instância de I2C dedicada ao OLED —
TwoWire I2C_OLED = TwoWire(1);
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &I2C_OLED, OLED_RESET);

// — Configurações Wi-Fi / MQTT —
const char* ssid        = "";
const char* password    = "";
const char* mqtt_server = "";

// — Tópicos MQTT —
const char* topic_cmd    = "carro/comando";
const char* topic_sensor = "carro/sensor";

// — Pinos do driver de motor —
const int IN1 = 26, IN2 = 25, IN3 = 33, IN4 = 32;
const int ENA = 14, ENB = 27;

// — Velocidade padrão (0–255) —
int velocidade = 200;

// — Ultrassónico e buzzer —
const int trigPin   = 18;
const int echoPin   = 19;
const int buzzerPin = 12;

// — Parâmetros sonoros —
const float limiteAlerta  = 20.0;
const float limiteCritico =  9.0;
const int   tomHz         = 2000;
const int   durBipMs      =   50;
const int pauseMaxNorm    =  500;
const int pauseMinNorm    =  200;
const int pauseMaxCrit    =   10;
const int pauseMinCrit    =   10;

WiFiClient      espClient;
PubSubClient    client(espClient);
Adafruit_MPU6050 mpu;

// — Funções de movimento —
void parar() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

void frente() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void tras() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void esquerda() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void direita() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

// — Callback MQTT para comandos manuais —
void mqttCallback(char* topic, byte* payload, unsigned int length) {
  String cmd;
  for (unsigned int i = 0; i < length; i++) cmd += (char)payload[i];
  if      (cmd == "frente")   frente();
  else if (cmd == "tras")     tras();
  else if (cmd == "esquerda") esquerda();
  else if (cmd == "direita")  direita();
  else                        parar();
}

// — Wi-Fi / MQTT setup e reconnect —
void setup_wifi() {
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWi-Fi OK, IP: " + WiFi.localIP().toString());
  client.setServer(mqtt_server, 1883);
  client.setCallback(mqttCallback);
}
void reconnect_mqtt() {
  while (!client.connected()) {
    if (client.connect("ESP32Client")) {
      client.subscribe(topic_cmd);
    } else {
      delay(2000);
    }
  }
}

// — Ultrassónico + buzzer setup —
void setup_ultra() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(buzzerPin, OUTPUT);
}
void playTone(int freq, int dur) {
  unsigned long t0 = millis();
  int meio = 1000000 / (2 * freq);
  while (millis() - t0 < dur) {
    digitalWrite(buzzerPin, HIGH);
    delayMicroseconds(meio);
    digitalWrite(buzzerPin, LOW);
    delayMicroseconds(meio);
  }
}

void setup() {
  Serial.begin(115200);
  delay(100);

  // configura pinos de velocidade e direção
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  parar();

  // ultrassónico e buzzer
  setup_ultra();

  // MPU6050 (sem alterações!)
  Wire.begin();
  Serial.println("Iniciando MPU6050…");
  if (!mpu.begin()) {
    Serial.println("Erro: MPU6050 não encontrado!");
    while (1) delay(10);
  }
  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  mpu.setGyroRange(MPU6050_RANGE_250_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
  Serial.println("MPU6050 OK");

  // Wi-Fi e MQTT
  setup_wifi();

  // OLED: inicializa I2C próprio e display
  I2C_OLED.begin(17, 16);  // SDA = GPIO17, SCL = GPIO16
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDR)) {
    Serial.println("Erro ao iniciar SSD1306!");
    for (;;);
  }
  display.clearDisplay();
  display.display();
}

void loop() {
  if (!client.connected()) reconnect_mqtt();
  client.loop();

  // publicações MPU (sem alterações!)
  sensors_event_t accel, gyro, temp;
  mpu.getEvent(&accel, &gyro, &temp);
  char buf[128];
  snprintf(buf, sizeof(buf),
    "{\"ax\":%.3f,\"ay\":%.3f,\"az\":%.3f,"
    "\"gx\":%.3f,\"gy\":%.3f,\"gz\":%.3f}",
    accel.acceleration.x, accel.acceleration.y, accel.acceleration.z,
    gyro.gyro.x, gyro.gyro.y, gyro.gyro.z
  );
  client.publish(topic_sensor, buf);

  // leitura ultrassónica
  digitalWrite(trigPin, LOW);  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH); delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  unsigned long dur = pulseIn(echoPin, HIGH, 30000);
  float dist = (dur > 0) ? (dur * 0.0343f) / 2.0f : -1;

  // — MOSTRA DISTÂNCIA NO OLED (sem alterar MPU) —
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 16);
  if (dist > 0) {
    char dstr[16];
    snprintf(dstr, sizeof(dstr), "Dist: %.1fcm", dist);
    display.println(dstr);
  } else {
    display.println("Sem eco");
  }
  display.display();

  // aviso sonoro e paragem
  if (dist > 0 && dist <= limiteAlerta) {
    int pausa;
    float frac = (dist <= limiteCritico)
      ? constrain(dist / limiteCritico, 0.0, 1.0)
      : constrain((dist - limiteCritico) / (limiteAlerta - limiteCritico), 0.0, 1.0);
    if (dist <= limiteCritico)
      pausa = pauseMinCrit + frac * (pauseMaxCrit - pauseMinCrit);
    else
      pausa = pauseMinNorm + frac * (pauseMaxNorm - pauseMinNorm);
    playTone(tomHz, durBipMs);
    delay(pausa);
  }
  if (dist > 0 && dist <= limiteCritico) {
    parar();
  }

  delay(50);
}