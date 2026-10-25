#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <RadioLib.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>

// --- Pinos Heltec V3 (ESP32-S3) ---
#define OLED_SDA 17
#define OLED_SCL 18
#define OLED_RST 21
#define VEXT_PIN 36 
#define LED_PIN 35 

// --- Pinos LoRa SX1262 (Heltec V3) ---
#define LORA_SCK 9
#define LORA_MISO 11
#define LORA_MOSI 10
#define LORA_SS 8
#define LORA_RST 12
#define LORA_DIO1 14
#define LORA_BUSY 13

const char* ssid = "NOME_WIFI";
const char* password = "DIGITE_A_SENHA_AQUI";
const char* serverName = "https://SEU_DOMINIO/univesp/pi2026s2/api.php";

Adafruit_SSD1306 display(128, 64, &Wire, OLED_RST);
SX1262 radio = new Module(LORA_SS, LORA_DIO1, LORA_RST, LORA_BUSY);

void enviarParaAPI(String node_id, String umidade) {
  if(WiFi.status() == WL_CONNECTED){
    WiFiClientSecure client;
    client.setInsecure(); 
    HTTPClient http;
    http.begin(client, serverName); 
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    String httpRequestData = "node_id=" + node_id + "&umidade=" + umidade;
    http.POST(httpRequestData);
    http.end();
  }
}

void setup() {
  Serial.begin(115200);
  delay(2000); // Dá tempo para o monitor serial abrir
  
  pinMode(LED_PIN, OUTPUT);
  pinMode(VEXT_PIN, OUTPUT);
  digitalWrite(VEXT_PIN, LOW); // Liga a energia do display
  delay(50);

  pinMode(OLED_RST, OUTPUT);
  digitalWrite(OLED_RST, LOW); delay(20);
  digitalWrite(OLED_RST, HIGH); delay(50);
  
  Wire.begin(OLED_SDA, OLED_SCL);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C, false, false);
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(0,0);
  display.println("Conectando WiFi...");
  display.display();

  WiFi.begin(ssid, password);
  int timeout = 0;
  while(WiFi.status() != WL_CONNECTED && timeout < 20) {
    delay(500);
    timeout++;
  }
  
  display.clearDisplay();
  display.setCursor(0,0);
  display.println("WiFi OK! Init LoRa...");
  display.display();

  // Configura SPI para o LoRa V3 e inicializa
  SPI.begin(LORA_SCK, LORA_MISO, LORA_MOSI, LORA_SS);
  int state = radio.begin(915.0, 125.0, 7, 5, 0x12, 10, 8, 1.6, false);
  
  if (state != RADIOLIB_ERR_NONE) {
    display.clearDisplay();
    display.setCursor(0,0);
    display.print("Erro LoRa: ");
    display.println(state);
    display.display();
    while (true) delay(1000); // O delay impede o erro de Watchdog
  }
  
  display.clearDisplay();
  display.setCursor(0,0);
  display.println("Master V3 Ativo");
  display.setCursor(0, 15);
  display.println("Aguardando dados...");
  display.display();
}

void loop() {
  String payload = "";
  // Tenta receber um pacote
  int state = radio.receive(payload);
  
  if (state == RADIOLIB_ERR_NONE) {
    digitalWrite(LED_PIN, HIGH);
    
    int separatorIndex = payload.indexOf(':');
    if(separatorIndex != -1) {
      String node_id = payload.substring(0, separatorIndex);
      String umidade = payload.substring(separatorIndex + 1);
      
      display.clearDisplay();
      display.setCursor(0, 0);
      display.println("<< RECEBIDO OK <<");
      display.setCursor(0, 20);
      display.print("ID: "); display.println(node_id);
      display.setCursor(0, 35);
      display.print("Umidade: "); display.print(umidade); display.println("%");
      display.display();
      
      enviarParaAPI(node_id, umidade);
    }
    digitalWrite(LED_PIN, LOW); 
  }
}