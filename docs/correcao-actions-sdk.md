# Correção do workflow Android SDK

A falha `Failed to find package tools` ocorre durante a inicialização do Android SDK, antes da compilação.

Substitua `.github/workflows/android-apk-release.yml` pelo arquivo deste pacote e faça commit na branch principal.

A correção remove a action `android-actions/setup-android@v3`, utiliza o SDK pré-instalado no runner Ubuntu e instala somente pacotes modernos: `platforms;android-35`, `build-tools;35.0.0` e `platform-tools`.

Em **Actions → Gerar APK Android e publicar Release → Run workflow**, execute o teste. Se compilar, o APK aparecerá em **Artifacts**. Para publicar no Release, crie uma tag `android-v1.0.1` e envie-a ao GitHub.

**Observação:** a assinatura atual é debug e não garante atualização sobre APKs de execuções anteriores. Para distribuição estável, configure assinatura release persistente.
