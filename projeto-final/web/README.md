# ESP32 Control — Aplicação Web/PWA

Aplicação responsiva do IFPA Campus Bragança. Abre diretamente no navegador com `index.html` para **demonstração**. Para comunicação MQTT real, a biblioteca MQTT.js é carregada por CDN e o broker deve expor **WebSocket**. Para instalação como PWA e cache offline, sirva a pasta em `localhost` ou HTTPS.

## Execução rápida

1. Abra `index.html` no navegador (modo demonstração). Necessário internet para baixar MQTT.js apenas no modo real.
2. Toque na engrenagem e selecione **MQTT real**; informe host, porta WS/WSS, protocolo e os tópicos.
3. No laboratório, sirva os arquivos com `python3 -m http.server 8080` e abra `http://localhost:8080/projeto-final/web/` **se o servidor foi iniciado na raiz do repositório**, ou `http://localhost:8080/` se iniciado nesta pasta.
4. No celular, acesse o endereço IP do computador na mesma rede, por exemplo `http://192.168.1.10:8080/`. Em HTTP por IP, a PWA não pode ser instalada e o service worker não funciona; para instalar use HTTPS válido. Em iPhone, use Safari > Compartilhar > Adicionar à Tela de Início.

## Segurança e limitações

- Credenciais MQTT são mantidas apenas em memória da página, **não são salvas** em `localStorage`. Os demais campos são persistidos localmente.
- Páginas HTTPS exigem WSS (certificado confiável). Acesso `file://` não instala PWA e pode variar entre navegadores; prefira HTTP local para testes.
- A biblioteca MQTT.js vem de CDN; o dashboard demonstrativo funciona mesmo se ela estiver indisponível.
- Estado dos LEDs em modo real só muda após a confirmação do ESP32 nos tópicos `/state`.
- A alteração de tópicos no navegador não altera o firmware automaticamente: utilize **Enviar tópicos ao ESP32**, com chave definida em `secrets.h`. O dispositivo precisa estar no broker atual; alterar o broker exige reconfigurar o firmware.
- O canal de configuração é um recurso **didático para rede isolada**. Não exponha o broker na internet sem autenticação, TLS e controle de acesso aos tópicos.

Consulte [`../../docs/configuracao-dinamica.md`](../../docs/configuracao-dinamica.md).


### HTTPS
No deploy com Caddy, o padrão de conexão MQTT real é `wss://DOMINIO:443/mqtt`. Veja `../../infra/https-mosquitto/README.md`.

### HiveMQ Cloud
O Dashboard oferece um perfil WSS para HiveMQ Cloud; o firmware suporta MQTT/TLS com CA validada. Consulte `docs/hivemq-cloud.md` (na raiz do repositório).
