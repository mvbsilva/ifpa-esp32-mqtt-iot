import fs from 'node:fs';
import path from 'node:path';
const root=path.resolve(import.meta.dirname,'..');
const main=path.join(root,'android/app/src/main');
const manifest=path.join(main,'AndroidManifest.xml');
let xml=fs.readFileSync(manifest,'utf8');
xml=xml.replace(/<application(\s|>)/, '<application android:usesCleartextTraffic="true" android:networkSecurityConfig="@xml/network_security_config"$1');
fs.writeFileSync(manifest,xml);
const dir=path.join(main,'res/xml');fs.mkdirSync(dir,{recursive:true});
fs.writeFileSync(path.join(dir,'network_security_config.xml'),`<?xml version="1.0" encoding="utf-8"?>
<network-security-config>
  <!-- Permite ws:// em brokers da rede local; use apenas redes confiáveis. -->
  <base-config cleartextTrafficPermitted="true" />
</network-security-config>
`);
console.log('Android configurado para LAN HTTP/WS; WSS continua disponível.');
