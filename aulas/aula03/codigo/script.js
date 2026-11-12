const PREFIXO="lab/grupo01";
const cliente=mqtt.connect("ws://192.168.1.100:9001",{
  clientId:"web-grupo01-"+Math.random().toString(16).slice(2),
  reconnectPeriod:2000
});
let brokerOnline=false;
function registrar(texto){
  const lista=document.getElementById("mensagens");
  const li=document.createElement("li"); li.textContent=texto;
  lista.prepend(li);
  while(lista.children.length>10) lista.lastElementChild.remove();
}
function render(){
  document.getElementById("conexao").textContent=brokerOnline?"Broker conectado":"Broker desconectado";
  document.querySelectorAll("button").forEach(b=>b.disabled=!brokerOnline);
}
cliente.on("connect",()=>{
  brokerOnline=true;render();
  cliente.subscribe([`${PREFIXO}/led1/state`,`${PREFIXO}/led2/state`]);
});
cliente.on("close",()=>{brokerOnline=false;render();});
cliente.on("message",(topico,payload)=>{
  const valor=payload.toString();
  if(topico===`${PREFIXO}/led1/state`) document.getElementById("estado1").textContent=valor;
  if(topico===`${PREFIXO}/led2/state`) document.getElementById("estado2").textContent=valor;
  registrar(`${topico}: ${valor}`);
});
function controlar(led,comando){
  if(!cliente.connected)return;
  cliente.publish(`${PREFIXO}/led${led}/set`,comando,{retain:false});
}
render();