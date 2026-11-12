# Solução de problemas

**O ESP32 não conecta ao Wi-Fi.** Verifique SSID, senha, banda de 2,4 GHz, cobertura e monitor serial.

**O navegador não conecta ao broker.** Confira a porta WebSockets 9001, IP correto, firewall, configuração do listener e a página servida por HTTP.

**O MQTT funciona no terminal, mas não no navegador.** TCP 1883 não é WebSocket; confira `protocol websockets` e `ws://IP:9001`.

**DHT22 retorna NaN.** Confira VCC 3V3, GND, GPIO4, pull-up quando necessário, biblioteca correta e intervalo de leitura.

**O LED não acende.** Confira polaridade, resistor em série, GPIO correto e terra comum.

**O LED2 não responde ao botão.** Em `AUTO`, ele é controlado pelo ESP32; altere para `MANUAL`.

**Os gráficos não aparecem.** Abra o console do navegador e confira carregamento do Chart.js e MQTT.js (CDN), além dos IDs HTML.

**Um grupo interfere no outro.** Personalize prefixo MQTT e client ID do ESP32.
