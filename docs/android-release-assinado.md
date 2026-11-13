# APK Android de produção — ESP32 Control IFPA

Este fluxo mantém **o Dashboard original**, sem a atualização Multi-ESP32. O APK inclui os arquivos web para abrir sem internet e permite conexão ao Mosquitto da LAN por `ws://IP:9001`, desde que celular e broker possam se alcançar. A opção de tráfego HTTP/WS sem TLS é intencional para laboratório e deve ser usada somente em redes confiáveis.

## 1. Criar a chave UMA VEZ, em computador confiável

Instale JDK e execute (Linux/macOS ou PowerShell, ajustando a sintaxe do terminal):

```bash
keytool -genkeypair -v -keystore ifpa-esp32-control-release.jks -alias ifpa-esp32-control -keyalg RSA -keysize 3072 -validity 10000
```

Escolha senhas fortes. Guarde **arquivo JKS, alias e senhas** em cofre seguro e backup separado. **Não envie a chave ao GitHub, nem ao chat.** Não recrie a chave a cada release: sem a mesma chave, não é possível atualizar instalações existentes normalmente.

Para gerar o conteúdo base64 em Linux: `base64 -w 0 ifpa-esp32-control-release.jks`; no macOS: `base64 < ifpa-esp32-control-release.jks | tr -d '\n'`; no PowerShell: `[Convert]::ToBase64String([IO.File]::ReadAllBytes('ifpa-esp32-control-release.jks'))`.

## 2. Configurar GitHub Secrets

No repositório, **Settings → Secrets and variables → Actions → New repository secret**:

| Nome | Conteúdo |
| --- | --- |
| `ANDROID_KEYSTORE_BASE64` | Base64 completo do JKS |
| `ANDROID_KEYSTORE_PASSWORD` | Senha do keystore |
| `ANDROID_KEY_ALIAS` | `ifpa-esp32-control` (ou alias escolhido) |
| `ANDROID_KEY_PASSWORD` | Senha da chave |

Não registre as senhas em README, commits, logs ou screenshots.

## 3. Executar e publicar

Faça commit do workflow e do script. Em **Actions → Android APK Release assinado → Run workflow**, execute primeiro manualmente. Após sucesso, baixe o artefato `ESP32-Control-IFPA-Release` e teste num aparelho Android.

Para criar Release público: crie uma tag como `android-v1.0.0` e envie-a (`git tag android-v1.0.0 && git push origin android-v1.0.0`). O workflow anexará APK e SHA-256 à página de Releases. A compilação deve passar antes de compartilhar com os alunos.

## 4. Considerações de segurança e Play Protect

Assinatura de produção e checksum comprovam origem e integridade, **não garantem aprovação automática pelo Google Play Protect**. APKs instalados fora da Play Store podem gerar avisos; nunca peça aos alunos para desativar Play Protect. Verifique os requisitos atuais de distribuição e verificação de desenvolvedor Android no Brasil.

O identificador do pacote permanece `br.edu.ifpa.esp32control` conforme o projeto existente; confirme autorização institucional para uso do namespace. A configuração Android permite tráfego claro para WS local; **não use credenciais sensíveis** em broker público ou rede não confiável.

## 5. Limitações e testes

A compilação no GitHub Actions e a instalação real ainda devem ser testadas. Se houver falha, compartilhe o log **sem segredos**. O código fonte não contém a chave privada.
