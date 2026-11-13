import fs from 'node:fs';
import path from 'node:path';
const root=path.resolve(import.meta.dirname,'..');
const src=path.resolve(root,'../projeto-final/web');
const dest=path.join(root,'www');
fs.rmSync(dest,{recursive:true,force:true});
fs.cpSync(src,dest,{recursive:true});
const lib=path.join(root,'node_modules/mqtt/dist/mqtt.min.js');
if(!fs.existsSync(lib)) throw new Error('Biblioteca MQTT.js não instalada: execute npm install');
fs.mkdirSync(path.join(dest,'vendor'),{recursive:true});
fs.copyFileSync(lib,path.join(dest,'vendor/mqtt.min.js'));
const html=path.join(dest,'index.html');
let page=fs.readFileSync(html,'utf8');
page=page.replace(/https:\/\/unpkg\.com\/mqtt@[^"']+\/dist\/mqtt\.min\.js/g,'vendor/mqtt.min.js');
if(!page.includes('vendor/mqtt.min.js')) throw new Error('Referência MQTT.js não encontrada no HTML');
fs.writeFileSync(html,page);
const js=path.join(dest,'app.js');
let app=fs.readFileSync(js,'utf8');
// No APK, WebView roda em https://localhost; WS na LAN é permitido pelo manifesto + mixedContent.
app=app.replace("const secure=location.protocol==='https:';const base=", "const isAndroidApp=!!window.Capacitor?.isNativePlatform?.();const secure=location.protocol==='https:'&&!isAndroidApp;const base=");
app=app.replace("next.operation==='real'&&location.protocol==='https:'&&next.protocol!=='wss'", "next.operation==='real'&&location.protocol==='https:'&&!isAndroidApp&&next.protocol!=='wss'");
app=app.replace("if('serviceWorker'in navigator&&location.protocol!=='file:')", "if(!isAndroidApp&&'serviceWorker'in navigator&&location.protocol!=='file:')");
fs.writeFileSync(js,app);
// Capacitor expõe o bridge via capacitor.js para detectar app nativo.
page=fs.readFileSync(html,'utf8').replace('<script defer src="app.js"></script>', '<script src="capacitor.js"></script><script defer src="app.js"></script>');
fs.writeFileSync(html,page);
console.log('Dashboard anterior empacotado, MQTT.js local, suporte WS/WSS.');
