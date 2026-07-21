#include "DHT.h"

#define DHTPIN 4
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("Iniciando teste do DHT22...");
  dht.begin();
  Serial.println("dht.begin() concluído");
  Serial.println("---------------------");
}

void loop() {
  delay(2000);

  Serial.println("Lendo sensor...");
  float umidade = dht.readHumidity();
  float temperatura = dht.readTemperature();
  Serial.println("Leitura concluída");

  if (isnan(umidade) || isnan(temperatura)) {
    Serial.println("Falha ao ler o sensor DHT22!");
    return;
  }

  Serial.print("Temperatura: ");
  Serial.println(temperatura + String("°C"));
  Serial.print("Umidade: ");
  Serial.println(umidade + String("%"));
  Serial.println("---------------------");
}
