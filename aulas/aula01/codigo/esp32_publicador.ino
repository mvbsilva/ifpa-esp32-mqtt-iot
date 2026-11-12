#include <WiFi.h>
#include <PubSubClient.h>
const char* SSID = "WIFI_LAB";
const char* SENHA = "SENHA_WIFI";
const char* BROKER = "192.168.1.100";
WiFiClient rede;
PubSubClient mqtt(rede);
unsigned long anterior = 0;
void conectarMQTT() {
  while (!mqtt.connected()) {
    if (mqtt.connect("esp32-grupo01")) {
      Serial.println("Conectado ao MQTT");
    } else { delay(2000); }
  }
}
void setup() {
  Serial.begin(115200);
  WiFi.begin(SSID, SENHA);
  while (WiFi.status() != WL_CONNECTED) delay(500);
  mqtt.setServer(BROKER, 1883);
}
void loop() {
  if (!mqtt.connected()) conectarMQTT();
  mqtt.loop();
  if (millis() - anterior >= 5000) {
    anterior = millis();
    mqtt.publish("lab/grupo01/teste", "Ola, MQTT!");
    Serial.println("Mensagem publicada");
  }
}