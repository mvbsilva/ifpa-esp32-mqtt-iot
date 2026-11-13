import fs from 'node:fs';
import path from 'node:path';
const gradle = path.resolve(import.meta.dirname, '../android/app/build.gradle');
if (!fs.existsSync(gradle)) throw new Error('Projeto Android ausente: execute npx cap add android');
let content = fs.readFileSync(gradle, 'utf8');
if (content.includes('// IFPA_RELEASE_SIGNING')) process.exit(0);
content += `
// IFPA_RELEASE_SIGNING - credenciais apenas via variáveis de ambiente do runner
android {
  signingConfigs {
    ifpaRelease {
      storeFile file(System.getenv('IFPA_KEYSTORE_PATH'))
      storePassword System.getenv('IFPA_STORE_PASSWORD')
      keyAlias System.getenv('IFPA_KEY_ALIAS')
      keyPassword System.getenv('IFPA_KEY_PASSWORD')
    }
  }
  buildTypes {
    release {
      signingConfig signingConfigs.ifpaRelease
    }
  }
}
`;
fs.writeFileSync(gradle, content);
console.log('Assinatura release configurada sem gravar segredos no repositório.');
