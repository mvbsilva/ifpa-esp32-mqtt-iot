# Contrato MQTT — grupo 01

Prefixo padrão: `lab/grupo01`. O ESP32 usa TCP `1883`; a página web usa WebSockets `9001`.

| Tópico | Publicador | Payload | Retido |
|---|---|---|---|
| `led1/set` | Web | `ON` ou `OFF` | Não |
| `led2/set` | Web | `ON` ou `OFF` | Não |
| `led1/state` | ESP32 | `ON` ou `OFF` | Sim |
| `led2/state` | ESP32 | `ON` ou `OFF` | Sim |
| `status` | ESP32 / LWT | `online` / `offline` | Sim |
| `telemetria` | ESP32 | `{"temperatura":27.4,"umidade":72,"uptime":120}` | Não |
| `automacao/mode/set` | Web | `AUTO` ou `MANUAL` | Não |
| `automacao/mode/state` | ESP32 | `AUTO` ou `MANUAL` | Sim |
| `automacao/limite/set` | Web | `30.0` | Não |
| `automacao/limite/state` | ESP32 | `30.0` | Sim |
| `automacao/alerta` | ESP32 | `ON` / `OFF` | Sim |
| `eventos` | ESP32 | `{"evento":"TEMPERATURA_ALTA","uptime":120}` | Não |

Todos os nomes acima são sufixos do prefixo `lab/grupo01/`. `set` representa comando, `state` representa confirmação. Não retenha comandos para evitar acionamentos antigos após reconexão.
