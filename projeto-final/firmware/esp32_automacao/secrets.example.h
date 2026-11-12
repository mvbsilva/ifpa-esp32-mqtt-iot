#pragma once
// Copie este arquivo para secrets.h e personalize. Nunca envie secrets.h ao GitHub.
#define WIFI_SSID "SUA_REDE"
#define WIFI_PASSWORD "SUA_SENHA"
#define MQTT_HOST "192.168.1.100"
#define MQTT_PORT 1883
#define MQTT_USER ""       // deixe vazio no laboratorio isolado
#define MQTT_PASSWORD ""
#define DEVICE_ID "esp32-grupo01"  // mesmo ID configurado na aplicacao
#define CONFIG_KEY "troque-esta-chave-laboratorio" // minimo 8 caracteres

// HiveMQ Cloud: configure MQTT_HOST com o hostname da instância e MQTT_PORT 8883.
// Mosquitto LAN sem TLS: MQTT_USE_TLS 0 e porta 1883.
// HiveMQ Cloud com TLS: MQTT_USE_TLS 1, usuário/senha e CA PEM confiável.
#define MQTT_USE_TLS 0
// Substitua pelo certificado da autoridade certificadora que valida seu cluster.
// Consulte a cadeia TLS do serviço e confirme a CA vigente antes de usar.
// Nunca substitua por setInsecure().
#define MQTT_ROOT_CA_PEM R"PEM(-----BEGIN CERTIFICATE-----
COLE_AQUI_O_CERTIFICADO_CA_PEM_VALIDO
-----END CERTIFICATE-----)PEM"
