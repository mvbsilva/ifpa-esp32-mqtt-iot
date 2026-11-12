# Testes de aceitação em bancada

Preencha **OK / Falhou / Não testado** e anexe evidência de monitor serial ou navegador.

| ID | Ação | Resultado esperado | Situação |
|---|---|---|---|
| T01 | Conectar ESP32 ao Wi-Fi | IP atribuído | Não testado |
| T02 | Iniciar broker | 1883 e 9001 acessíveis na LAN | Não testado |
| T03 | Assinar `lab/grupo01/#` | Mensagens recebidas | Não testado |
| T04 | Abrir dashboard | Broker conectado | Não testado |
| T05 | Acionar LED1 | LED físico e estado confirmados | Não testado |
| T06 | Acionar LED2 em MANUAL | LED físico e estado confirmados | Não testado |
| T07 | Trocar para AUTO | LED2 passa a obedecer regra | Não testado |
| T08 | Alterar limite | Valor confirmado pelo ESP32 | Não testado |
| T09 | Aquecer sensor suavemente | Temperatura sobe e alerta ativa acima do limite | Não testado |
| T10 | Reduzir abaixo de limite − 1 °C | Alerta desativa | Não testado |
| T11 | Desconectar broker | Dashboard indica falha; regra local continua | Não testado |
| T12 | Reconectar broker | Estados retidos sincronizados | Não testado |
| T13 | Interromper telemetria por mais de 15 s | Indicador mostra dados desatualizados | Não testado |
| T14 | Reiniciar ESP32 | Configurações NVS restauradas | Não testado |

**Nota:** o LWT pode demorar alguns segundos para indicar `offline`, dependendo do keepalive MQTT.
