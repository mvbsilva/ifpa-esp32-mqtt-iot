# Broker Mosquitto no laboratório

1. Instale Mosquitto no computador servidor da rede local.
2. Copie `mosquitto.lab.conf` e inicie: `mosquitto -c mosquitto.lab.conf -v`.
3. Descubra o IP do computador servidor e configure `secrets.h` e `web/config.js`.
4. Libere as portas **1883/TCP** e **9001/TCP** somente na rede isolada.
5. Teste com `mosquitto_sub -h 192.168.1.100 -t 'lab/grupo01/#' -v` (substitua o IP).

**Segurança:** `allow_anonymous true` serve exclusivamente para laboratório isolado. Para redes compartilhadas ou produção, configure autenticação, ACL e TLS/WSS. Não encaminhe essas portas para a Internet.
