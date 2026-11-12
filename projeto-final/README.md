# Projeto final — monitoramento e automação

## 1. Preparação

1. Instale Arduino IDE e suporte à placa ESP32.
2. Instale as bibliotecas `PubSubClient`, `DHT sensor library` e `Adafruit Unified Sensor` pelo gerenciador da IDE. `WiFi.h` e `Preferences.h` vêm do core ESP32.
3. Configure o broker conforme [`infra/mosquitto`](../infra/mosquitto/README.md).
4. Em `firmware/esp32_automacao/`, copie `secrets.example.h` para `secrets.h` e edite SSID, senha e IP do broker.
5. Abra `esp32_automacao.ino` no Arduino IDE e envie à placa.
6. Em `web/`, copie `config.example.js` para `config.js` e edite `brokerWs` e `prefixo`.
7. Sirva a pasta `web/` por HTTP local (por exemplo `python3 -m http.server 8000`) e abra `http://localhost:8000`.

**Importante:** os scripts MQTT.js e Chart.js vêm de CDNs e precisam de acesso à Internet no primeiro carregamento; para uso totalmente offline, baixe e hospede localmente versões compatíveis.

## 2. Pinagem

| Componente | ESP32 | Observação |
|---|---|---|
| LED 1 | GPIO23 → resistor 330 Ω → ânodo LED | cátodo ao GND |
| LED 2 | GPIO22 → resistor 330 Ω → ânodo LED | cátodo ao GND |
| DHT22 DATA | GPIO4 | pull-up 4,7–10 kΩ a 3V3 se necessário |
| DHT22 VCC | 3V3 | nunca 5 V no sinal GPIO |
| GND | GND | terra comum |

A sequência resistor/LED em série pode ser invertida eletricamente, desde que a polaridade do LED seja mantida. Verifique a pinagem física do módulo DHT22.

## 3. Operação

- LED1 sempre aceita controle manual.
- LED2 aceita controle manual apenas em `MANUAL`.
- Em `AUTO`, LED2 liga quando `T > limite` e desliga quando `T < limite − 1 °C`.
- As configurações de modo e limite são guardadas em NVS (`Preferences`).
- Telemetria é publicada a cada 5 segundos, e o painel indica ausência de dados recentes após 15 segundos.

## 4. Observações

- A automação local roda mesmo sem broker, desde que o sensor esteja funcional.
- A interface usa mensagens de estado confirmadas pelo ESP32.
- O código de referência tem finalidade educacional; valide o comportamento em bancada antes de ligar cargas reais. LEDs não devem ser substituídos diretamente por motores, bombas ou relés sem interface elétrica apropriada.
- Com mais de um grupo, personalize `BASE`, `CLIENT_ID` e `prefixo` em cada conjunto.

Veja [tópicos MQTT](../docs/topicos-mqtt.md) e [testes](../docs/testes.md).
