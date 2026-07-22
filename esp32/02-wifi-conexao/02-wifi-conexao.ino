#include "WiFi.h"
#include "config.h"

void setup() {
  
  Serial.begin(115200);
  Serial.println("Iniciando conexão com WiFi");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Conectando..");
  while(WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("Conexão bem sucedida!");

}

void loop() {
  
  delay(5000);
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("Status: Conectado");
  } else {
    Serial.println("Status: Desconectado");
  }

}
