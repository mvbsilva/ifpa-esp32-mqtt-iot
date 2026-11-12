const PREFIXO="lab/grupo01";
const cliente=mqtt.connect("ws://192.168.1.100:9001",{
 clientId:"web-grupo01-"+Math.random().toString(16).slice(2),reconnectPeriod:2000
});
let broker=false,dispositivo=false,modo=null,ultima=0,contador=0;
const leds={1:null,2:null};
const $=id=>document.getElementById(id);
function grafico(id,rotulo,cor,min,max){
 return new Chart($(id),{type:"line",data:{labels:[],datasets:[{
 label:rotulo,data:[],borderColor:cor,backgroundColor:cor+"22",fill:true,tension:.25
 }]},options:{responsive:true,maintainAspectRatio:false,animation:false,
 scales:{y:{suggestedMin:min,suggestedMax:max}}}});
}
const gt=grafico("graficoT","Temperatura (°C)","#dc2626",20,40);
const gu=grafico("graficoU","Umidade (%)","#2563eb",0,100);
function ponto(g,h,v){g.data.labels.push(h);g.data.datasets[0].data.push(v);
 if(g.data.labels.length>20){g.data.labels.shift();g.data.datasets[0].data.shift();}
 g.update("none");}
function render(){
 $("broker").textContent=broker?"Conectado":"Desconectado";
 $("dispositivo").textContent=!broker?"Desconhecido":dispositivo?"Online":"Offline / aguardando";
 $("frescor").textContent=broker&&dispositivo&&ultima&&Date.now()-ultima<=15000?"Atualizada":"Sem dados recentes";
 for(const n of [1,2]) $("estado"+n).textContent=broker&&dispositivo?(leds[n]??"Aguardando"):"Desconhecido";
 document.querySelectorAll("[data-led]").forEach(b=>{
  b.disabled=!broker||!dispositivo||(b.dataset.led==="2"&&modo!=="MANUAL");
 });
 $("modo").disabled=!broker||!dispositivo;
 $("limite").disabled=!broker||!dispositivo||modo!=="AUTO";
 $("salvar").disabled=!broker||!dispositivo||modo!=="AUTO";
}
function publicar(sufixo,valor){if(!cliente.connected||!dispositivo)return;
 cliente.publish(`${PREFIXO}/${sufixo}`,String(valor),{qos:0,retain:false});}
cliente.on("connect",()=>{
 broker=true;dispositivo=false;modo=null;ultima=0;render();
 cliente.subscribe(["status","led1/state","led2/state","telemetria",
 "automacao/mode/state","automacao/limite/state","automacao/alerta","eventos"].map(s=>`${PREFIXO}/${s}`),{qos:1});
});
cliente.on("close",()=>{broker=false;dispositivo=false;modo=null;render();});
cliente.on("message",(topico,buffer)=>{
 const s=topico.slice(PREFIXO.length+1),v=buffer.toString();
 if(s==="status"){dispositivo=v==="online";if(!dispositivo)ultima=0;}
 if(s==="led1/state"&&(v==="ON"||v==="OFF"))leds[1]=v;
 if(s==="led2/state"&&(v==="ON"||v==="OFF"))leds[2]=v;
 if(s==="automacao/mode/state"&&(v==="AUTO"||v==="MANUAL")){
  modo=v;$("modo").value=v;$("modoAtual").textContent=v;
 }
 if(s==="automacao/limite/state"){
  const n=Number(v);if(Number.isFinite(n)&&n>=15&&n<=45){
   $("limite").value=n;$("limiteAtual").textContent=n.toFixed(1)+" °C";
  }
 }
 if(s==="automacao/alerta"&&(v==="ON"||v==="OFF"))$("alerta").textContent=v;
 if(s==="telemetria"){
  try{const d=JSON.parse(v),t=d.temperatura,u=d.umidade;
   if(typeof t==="number"&&typeof u==="number"&&Number.isFinite(t)&&Number.isFinite(u)&&u>=0&&u<=100){
    $("temperatura").textContent=t.toFixed(1)+" °C";
    $("umidade").textContent=u.toFixed(1)+" %";
    ultima=Date.now();$("contador").textContent=++contador;
    const h=new Date().toLocaleTimeString("pt-BR");ponto(gt,h,t);ponto(gu,h,u);
   }
  }catch(e){console.warn("JSON inválido",e);}
 }
 if(s==="eventos"){
  try{const d=JSON.parse(v);const li=document.createElement("li");
   li.textContent=`${d.evento} | uptime: ${d.uptime}s`;
   $("eventos").prepend(li);
   while($("eventos").children.length>15)$("eventos").lastElementChild.remove();
  }catch(e){console.warn("Evento inválido",e);}
 }
 render();
});
cliente.on("error",e=>console.error("MQTT:",e.message));
document.querySelectorAll("[data-led]").forEach(b=>b.addEventListener("click",()=>{
 publicar(`led${b.dataset.led}/set`,b.dataset.comando);
}));
$("modo").addEventListener("change",e=>publicar("automacao/mode/set",e.target.value));
$("salvar").addEventListener("click",()=>{
 const n=Number($("limite").value);
 if(!Number.isFinite(n)||n<15||n>45){window.alert("Use 15 a 45 °C");return;}
 publicar("automacao/limite/set",n.toFixed(1));
});
$("limpar").addEventListener("click",()=>{
 for(const g of [gt,gu]){g.data.labels=[];g.data.datasets[0].data=[];g.update("none");}
});
setInterval(render,1000);render();