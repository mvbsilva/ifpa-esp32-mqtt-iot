# Publicar o ESP32 Control no GitHub Pages

## Professor — primeira publicação

1. No GitHub, crie um repositório **público** chamado `ifpa-esp32-mqtt-iot`, sem inicializar com README (o projeto já inclui um).
2. Descompacte o ZIP e envie **o conteúdo da pasta `ifpa-esp32-mqtt-iot/`**, mantendo `.github/workflows/publicar-dashboard.yml`.
3. Pelo terminal, dentro dessa pasta:

```bash
git init
git add .
git commit -m "Publicar projeto didático ESP32 MQTT"
git branch -M main
git remote add origin https://github.com/SEU-USUARIO/ifpa-esp32-mqtt-iot.git
git push -u origin main
```

4. No GitHub: **Settings → Pages → Build and deployment → Source → GitHub Actions**.
5. Abra **Actions → Publicar Dashboard no GitHub Pages → Run workflow**, se não tiver sido executado automaticamente.
6. Após a conclusão com sucesso, acesse `https://SEU-USUARIO.github.io/ifpa-esp32-mqtt-iot/`.
7. Copie esse endereço e crie um QR Code com a ferramenta institucional de sua preferência. Teste o QR Code no celular antes da aula.

O workflow publica apenas os arquivos de `projeto-final/web/`; não publica o firmware nem as apostilas como páginas do site. O repositório público, porém, continua permitindo baixar os materiais.

## Alunos — celular

1. Escaneie o QR Code ou abra o link HTTPS no Chrome (Android) ou Safari (iPhone).
2. Para criar um ícone: Chrome → Instalar aplicativo/Adicionar à tela inicial; Safari → Compartilhar → Adicionar à Tela de Início.
3. Abra a engrenagem e escolha **HiveMQ Cloud (WSS)** ou **Mosquitto via HTTPS/Caddy (WSS)**.
4. Configure host, porta, caminho, usuário, senha e o grupo. Para HiveMQ Cloud, confirme porta e caminho WSS no console da instância.
5. Salve, conecte e observe as mensagens. A telemetria permanece no tópico `lab/grupo01/telemetria` como JSON com temperatura e umidade.

## Atenção

- O GitHub Pages hospeda somente o frontend: **não executa o broker Mosquitto** nem o firmware ESP32.
- Uma página HTTPS exige um broker com **WebSocket seguro (WSS)** acessível pelo celular. O ESP32 usa MQTT/TLS ou TCP conforme o broker.
- Nunca inclua credenciais do Wi-Fi, HiveMQ ou Mosquitto em commits ou exemplos publicados. Credenciais digitadas no Dashboard não são salvas pela aplicação, mas são usadas na sessão do navegador.
- O Dashboard usa uma biblioteca MQTT.js externa; a primeira abertura exige acesso à internet. Para operação offline real, é preciso empacotar as dependências e adaptar o comportamento de conexão.
- Se houver mais de uma turma, use grupos e credenciais distintas e restrinja permissões MQTT por tópico no broker.
- GitHub Pages é uma hospedagem pública de arquivos estáticos, não um serviço de autenticação dos alunos.

## Atualizações

Ao modificar `projeto-final/web/` e enviar para a branch `main`, o workflow republica o site. Para executar manualmente, use a opção **Run workflow**.
