# HiveMQ Cloud — ESP32 Control (HTTPS/WSS + MQTT/TLS)

## Dashboard Web

1. Hospede o painel em HTTPS. Na engrenagem, escolha **HiveMQ Cloud (WSS)**.
2. Substitua o hostname ilustrativo pelo hostname **exato** da instância. O perfil sugere **WSS 8884** e caminho **/mqtt**; confirme os valores disponibilizados pelo seu plano/cluster.
3. Informe usuário e senha MQTT, selecione **MQTT real** e salve. O navegador não persiste a senha.
4. Se o navegador exibir falha de WebSocket, confira DNS, TLS, firewall, credenciais e a URL WebSocket indicada pelo HiveMQ. Uma página HTTPS não pode usar `ws://`.

## Firmware ESP32

1. Copie `projeto-final/firmware/esp32_automacao/secrets.example.h` para `secrets.h`.
2. Defina `MQTT_HOST`, `MQTT_PORT 8883`, `MQTT_USER`, `MQTT_PASSWORD`, `MQTT_USE_TLS 1`.
3. Substitua `MQTT_ROOT_CA_PEM` pelo PEM da CA que autentica o certificado TLS da sua instância. **Não use `setInsecure()`**.
4. O ESP32 precisa de acesso à internet e sincronização NTP para validar a cadeia de certificados.
5. Grave o firmware e monitore a porta serial. Confirme estados, comandos e telemetria no Dashboard.

**Importante:** mudar o broker na página web não migra o ESP32. Ambos devem se conectar à mesma instância. A sincronização dinâmica altera **tópicos**, não credenciais, rede ou broker do firmware.

## Tópicos e segurança

Mantemos `lab/grupo01/telemetria` com `{"temperatura":26.4,"umidade":71}` e os demais tópicos do curso. Para vários grupos, use prefixos exclusivos e credenciais/ACLs com privilégios mínimos. O canal de configuração remota `ifpa/devices/ID/config/set` também precisa de ACL.

## Verificações

- No navegador, a conexão deve ser `wss://HOST:PORTA/CAMINHO` com certificado válido.
- No ESP32, a conexão é MQTT/TLS na porta definida pelo cluster (tipicamente 8883).
- Observe mensagens de `status`, estados dos LEDs e JSON conjunto do DHT22.
- Se houver erro TLS, confira relógio NTP e cadeia de certificação antes de trocar a CA.

Não há testes físicos nem teste de conexão à conta HiveMQ neste pacote.
