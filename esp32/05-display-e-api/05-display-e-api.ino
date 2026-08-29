#include "WiFi.h"
#include "WiFiClientSecure.h"
#include "HTTPClient.h"
#include "DHT.h"
#include "configIP.h"
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define DHTPIN 4
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

#define HTTP_TIMEOUT_MS 65000

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1      
#define SCREEN_ADDRESS 0x3C 

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup() {
  
  Serial.begin(115200);
  dht.begin();

  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println("Falha ao iniciar o display OLED");
  } else {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.println("AutoGrow iniciando...");
    display.display();
  }

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  Serial.print("Conectando-se ao WiFi..");
  while(WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("Conexão realizada!");

}

void loop() {
  
  float temp = dht.readTemperature();
  float umid = dht.readHumidity();

  if (isnan(temp) || isnan(umid)) {
    Serial.println("Falha ao ler o sensor DHT22!");
    mostrarErroNoDisplay();
  } else {
    mostrarLeituraNoDisplay(temp, umid);
    enviarDados(temp, umid);
  }

  delay(10000);

}

void mostrarLeituraNoDisplay(float temp, float umid) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("AutoGrow - Leitura");
  display.drawLine(0, 10, 128, 10, SSD1306_WHITE);

  display.setTextSize(2);
  display.setCursor(0, 20);
  display.print("T: ");
  display.print(temp, 1);
  display.println(" C");

  display.setCursor(0, 44);
  display.print("U: ");
  display.print(umid, 1);
  display.println(" %");

  display.display();
}

void mostrarErroNoDisplay() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("AutoGrow");
  display.setCursor(0, 20);
  display.println("Erro ao ler DHT22!");
  display.display();
}

void enviarDados(float temp, float umid) {

  if(WiFi.status() == WL_CONNECTED) {
    WiFiClientSecure client;
    client.setInsecure();

    HTTPClient http;
    http.begin(client, SERVER_URL);
    http.addHeader("Content-Type", "application/json");
    http.setTimeout(HTTP_TIMEOUT_MS);
    http.setConnectTimeout(HTTP_TIMEOUT_MS);

    String json = "{\"nome\":\"DHT22\",\"temp\":" + String(temp, 1) +
                  ",\"umid\":" + String(umid, 1) + "}";
    Serial.println("Enviando " + json);

    int httpCode = http.POST(json);

    if (httpCode > 0) {
      String resposta = http.getString();
      Serial.println("Resposta (" + String(httpCode) + "): " + resposta);
    } else {
      Serial.println("Erro no POST: " + http.errorToString(httpCode));
    }

    http.end();
  } else {
    Serial.print("WiFi desconectado");
  }

}