# Configuração dinâmica de tópicos MQTT

A aplicação tem uma engrenagem discreta no cabeçalho. Nela, cada aluno pode editar servidor WebSocket, grupo e **11 tópicos** (comando/estado dos LEDs, telemetria, status, automação e eventos). Os campos são salvos no armazenamento local do navegador, exceto senhas.

## Sincronização com ESP32

O firmware atualizado escuta permanentemente `ifpa/devices/<DEVICE_ID>/config/set` e responde em `ifpa/devices/<DEVICE_ID>/config/ack`. Para configurar o dispositivo, o aplicativo envia JSON com `key` (chave configurada no firmware) e `topics` (mapa completo dos 11 nomes). O ESP32 valida a chave, os nomes e a ausência de duplicações, grava os tópicos em NVS (`Preferences`) e reconecta ao broker para assinar os novos comandos. O servidor e as credenciais Wi-Fi/MQTT do ESP32 continuam definidos em `secrets.h`.

### Exemplo de mensagem

```json
{"key":"CHAVE_DE_LABORATORIO","topics":{"led1_set":"lab/grupo02/led1/set","led1_state":"lab/grupo02/led1/state","led2_set":"lab/grupo02/led2/set","led2_state":"lab/grupo02/led2/state","telemetry":"lab/grupo02/telemetria","status":"lab/grupo02/status","mode_set":"lab/grupo02/automacao/mode/set","mode_state":"lab/grupo02/automacao/mode/state","limit_set":"lab/grupo02/automacao/limite/set","limit_state":"lab/grupo02/automacao/limite/state","events":"lab/grupo02/eventos"}}
```

**Ordem recomendada:** conectar navegador ao broker → editar tópicos → enviar configuração ao ESP32 → aguardar confirmação em Atividade recente → salvar e aplicar no navegador. Caso o ESP32 esteja offline ou a chave seja incorreta, os tópicos do dispositivo não mudarão.

## Compatibilidade

O firmware exige ArduinoJson 7, PubSubClient e DHT sensor library (Adafruit). O ESP32 usa MQTT TCP na porta 1883; o navegador usa WebSocket na porta 9001 (ou WSS). O broker Mosquitto precisa ter listeners adequados.

## Segurança

Chave no JSON não é criptografia: em `ws://` ou MQTT sem TLS, ela trafega em claro. Utilize somente rede isolada de laboratório ou TLS e ACLs adequadas. Evite divulgar `secrets.h`.
