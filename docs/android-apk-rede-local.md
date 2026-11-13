# ESP32 Control IFPA — APK Android e rede local

Esta atualização **não inclui a interface Multi-ESP32**. Usa o Dashboard anterior em `projeto-final/web`.

## Como gerar o APK

1. Envie a pasta `android-app/`, o workflow `.github/workflows/android-apk-release.yml` e este guia ao repositório, preservando `projeto-final/web`.
2. Em GitHub → **Actions** → **Gerar APK Android e publicar Release** → **Run workflow**, execute para validar. O APK aparece como artefato do workflow (não como Release).
3. Para publicar um Release, crie e envie uma tag, por exemplo:

   ```bash
   git tag android-v1.0.0
   git push origin android-v1.0.0
   ```

4. Após a conclusão, acesse **Releases** e baixe `ESP32-Control-IFPA-debug.apk`.
5. No Android, abra o APK e autorize a instalação pelo navegador/gerenciador de arquivos quando solicitado.

## Configuração do broker local

- Instale Mosquitto no computador ou Raspberry Pi da rede Wi-Fi, com listener **WebSocket** na porta `9001`, por exemplo:

  ```conf
  listener 1883 0.0.0.0
  protocol mqtt
  listener 9001 0.0.0.0
  protocol websockets
  allow_anonymous true
  ```

  **Somente para laboratório isolado**. Em rede compartilhada, use autenticação e restrinja acesso via firewall. Se usar Mosquitto 2.x, configure autenticação de forma explícita para ambos os listeners.

- No APK, configure: operação `MQTT real`, perfil `Mosquitto local`, host `192.168.x.x` (IP LAN do computador broker), porta `9001`, protocolo `ws`, caminho `/`.
- ESP32: mesmo IP, porta TCP `1883`, mesmo grupo e tópicos. Celular e broker devem estar na mesma rede ou em redes roteadas entre si. Redes Wi-Fi com isolamento de clientes, firewalls ou VLANs podem bloquear a conexão.
- **Não use `localhost`** como broker no celular: ele aponta para o próprio telefone.
- A aplicação Web hospedada no GitHub Pages (HTTPS) **continua exigindo WSS**. O APK Android, ao contrário, permite `ws://` na LAN através de configuração explícita de WebView/Android.
- O APK embute o Dashboard e MQTT.js, portanto não precisa da internet para abrir a interface; a comunicação exige conectividade com o broker.

## Segurança e limitações

- `ws://` e MQTT TCP `1883` não criptografam dados: use em rede local confiável e sem segredos sensíveis.
- O APK gerado é **debug**, assinado automaticamente pelo runner; serve para testes e instalação direta. Compilações de execuções diferentes podem ter chaves diferentes, impedindo atualização por cima do app anterior. Para releases regulares, é necessário criar uma keystore de produção persistente e armazenar os segredos no GitHub Actions.
- Este pacote prepara a compilação na infraestrutura do GitHub. Não contém APK pré-compilado; o build ainda precisa ser executado no Actions.
- Para HiveMQ Cloud use WSS com hostname e credenciais próprios.
