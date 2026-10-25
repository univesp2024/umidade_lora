#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <SPI.h>
#include <LoRa.h>

// --- Configurações Heltec V2 (Display e LED) ---
#define OLED_SDA 4
#define OLED_SCL 15
#define OLED_RST 16
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define VEXT_PIN 21
#define LED_PIN 25 // Pino do LED embutido

// --- Configurações Heltec V2 (LoRa) ---
#define LORA_SCK 5
#define LORA_MISO 19
#define LORA_MOSI 27
#define LORA_SS 18
#define LORA_RST 14
#define LORA_DI0 26
#define LORA_BAND 915E6

// --- Configurações do Sensor ---
#define SENSOR_PIN 36
const int valorSeco = 4095;
const int valorMolhado = 1500; 

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RST);

void setup() {
  Serial.begin(115200);
  
  pinMode(LED_PIN, OUTPUT);
  pinMode(VEXT_PIN, OUTPUT);
  digitalWrite(VEXT_PIN, LOW);
  delay(50);

  pinMode(OLED_RST, OUTPUT);
  digitalWrite(OLED_RST, LOW);
  delay(20);
  digitalWrite(OLED_RST, HIGH);
  delay(50);
  
  Wire.begin(OLED_SDA, OLED_SCL);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C, false, false);
  
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(0,0);
  display.println("Iniciando Slave...");
  display.display();

  SPI.begin(LORA_SCK, LORA_MISO, LORA_MOSI, LORA_SS);
  LoRa.setPins(LORA_SS, LORA_RST, LORA_DI0);
  if (!LoRa.begin(LORA_BAND)) {
    display.println("Erro LoRa!");
    display.display();
    while (true);
  }
  
  pinMode(SENSOR_PIN, INPUT);
}

void loop() {
  int leituraAnalogica = analogRead(SENSOR_PIN);
  int umidadePercentual = map(leituraAnalogica, valorSeco, valorMolhado, 0, 100);
  umidadePercentual = constrain(umidadePercentual, 0, 100);
  
  String payload = "NODE_1:" + String(umidadePercentual);
  
  // --- INDICATIVO DE ENVIO: Atualiza Tela e Acende LED ---
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println(">> ENVIANDO DADOS <<");
  display.setCursor(0, 20);
  display.print("Umidade: ");
  display.print(umidadePercentual);
  display.println("%");
  display.display();
  
  digitalWrite(LED_PIN, HIGH); // Liga o LED
  
  LoRa.beginPacket();
  LoRa.print(payload);
  LoRa.endPacket();
  
  digitalWrite(LED_PIN, LOW); // Apaga o LED
  
  // Atualiza tela para modo de espera
  display.setCursor(0, 40);
  display.println("Envio Concluido.");
  display.display();
  
  delay(10000);
}