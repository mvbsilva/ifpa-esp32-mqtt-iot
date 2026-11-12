# ESP32 Control — HTTPS + WSS + Mosquitto

## Arquitetura

- Navegador: `https://SEU_DOMINIO` (Caddy, porta 443)
- WebSocket seguro: `wss://SEU_DOMINIO/mqtt` (Caddy → Mosquitto porta interna 9001)
- ESP32: `mqtt://IP_DO_SERVIDOR:1883` (TCP, restrito à rede local)
- Telemetria DHT22 **inalterada**: `lab/grupo01/telemetria`, JSON `{"temperatura":26.4,"umidade":71}`.

## Requisitos

Servidor com Docker e Docker Compose; domínio DNS apontando para o IP público do servidor; portas 80 e 443 acessíveis para emissão automática do certificado TLS. Para ESP32 no laboratório, permita a porta 1883 **somente** para a rede local/VPN. Não exponha 1883 publicamente.

## Implantação

```bash
cd infra/https-mosquitto
cp .env.example .env
# Edite .env e defina DOMAIN=iot.seudominio.edu.br
mkdir -p auth
# Crie o primeiro usuário/senha, sem inserir senhas no Git:
docker run --rm -it -v "$(pwd)/auth:/mosquitto/auth" eclipse-mosquitto:2 mosquitto_passwd -c /mosquitto/auth/passwords aluno01
# Para outros usuários, NÃO use -c (ele sobrescreve o arquivo):
docker run --rm -it -v "$(pwd)/auth:/mosquitto/auth" eclipse-mosquitto:2 mosquitto_passwd /mosquitto/auth/passwords aluno02
docker compose up -d
```

Acesse `https://SEU_DOMINIO`. Em Configurações, escolha **MQTT real**, protocolo `wss`, host `SEU_DOMINIO`, porta `443`, caminho `/mqtt` e as credenciais. Não inclua `https://` no campo host. O modo demonstração permanece disponível.

No firmware `secrets.h`, use `MQTT_HOST` com o IP acessível pelo ESP32, `MQTT_PORT 1883`, e `MQTT_USER` / `MQTT_PASSWORD` com as credenciais criadas. O ESP32 usa TCP local (sem TLS); use somente rede confiável ou VPN.

**Importante:** um site HTTPS aberto no celular precisa que o navegador alcance o domínio e confie no certificado. Um domínio interno sem certificado público exigirá configuração adicional de DNS/certificados. `localhost` é apenas para testes no próprio computador.

## Teste de comunicação

```bash
# Dentro do servidor (credenciais de exemplo):
docker compose exec mosquitto mosquitto_sub -h localhost -p 1883 -u aluno01 -P 'SENHA' -t 'lab/grupo01/#' -v
```

Se a interface carregar mas MQTT não conectar, confira DNS, certificados, usuário/senha, WSS `/mqtt`, logs `docker compose logs -f caddy mosquitto`, e se a rede permite acesso ao domínio.

## Segurança

A autenticação do broker é obrigatória. O mesmo broker é compartilhado entre grupos; para isolamento forte entre grupos, configure ACLs do Mosquitto (não incluídas neste exemplo) e credenciais por grupo. Não use credenciais reais em capturas, repositório ou URLs. Os comandos de reconfiguração remota do ESP32 exigem chave; use uma chave única por dispositivo. Para acesso remoto ao ESP32, prefira VPN ou MQTT/TLS no firmware em vez de expor 1883.

## Testes pendentes

Os arquivos foram revisados estaticamente; o conjunto Docker, a emissão real do certificado e a conexão com hardware precisam ser validados no servidor e laboratório.
