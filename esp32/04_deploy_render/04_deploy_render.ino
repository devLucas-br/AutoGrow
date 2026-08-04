#include "WiFi.h"
#include "WiFiClientSecure.h"
#include "HTTPClient.h"
#include "DHT.h"
#include "configIP.h"

#define DHTPIN 4
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

#define HTTP_TIMEOUT_MS 65000

void setup() {
  
  Serial.begin(115200);
  dht.begin();
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
  } else {
    enviarDados(temp, umid);
  }

  delay(10000);

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
