#include <WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>
#include <Preferences.h>
#include <math.h>
#include <stdlib.h>
const char* SSID="WIFI_LAB", *SENHA="SENHA_WIFI";
const char* BROKER="192.168.1.100";
const char* BASE="lab/grupo01/";
const int LED1=23,LED2=22,DHT_PIN=4;
DHT dht(DHT_PIN,DHT22);
WiFiClient rede; PubSubClient mqtt(rede); Preferences prefs;
bool led1=false,led2=false,automatico=false;
float limite=30.0; const float HISTER=1.0;
unsigned long ultimaLeitura=0,ultimaConexao=0;
String tp(const char* s){return String(BASE)+s;}
void pub(const char* s,const char* v,bool retain=false){
 String t=tp(s);mqtt.publish(t.c_str(),v,retain);
}
void estados(){
 pub("led1/state",led1?"ON":"OFF",true);
 pub("led2/state",led2?"ON":"OFF",true);
 pub("automacao/alerta",automatico&&led2?"ON":"OFF",true);
}
void configuracao(){
 pub("automacao/mode/state",automatico?"AUTO":"MANUAL",true);
 char s[20];snprintf(s,sizeof(s),"%.1f",limite);
 pub("automacao/limite/state",s,true);
}
void evento(const char* nome){
 char json[128];snprintf(json,sizeof(json),
 "{\"evento\":\"%s\",\"uptime\":%lu}",nome,millis()/1000UL);
 pub("eventos",json);
}
void setLed2(bool on){
 if(led2==on)return;
 led2=on;digitalWrite(LED2,on?HIGH:LOW);estados();
}
void receber(char* topico,byte* dados,unsigned int n){
 String v="";for(unsigned int i=0;i<n;i++)v+=(char)dados[i];
 String t=String(topico);
 if(t==tp("led1/set")&&(v=="ON"||v=="OFF")){
  led1=(v=="ON");digitalWrite(LED1,led1);estados();
 }else if(t==tp("led2/set")&&!automatico&&(v=="ON"||v=="OFF")){
  setLed2(v=="ON");
 }else if(t==tp("automacao/mode/set")&&(v=="AUTO"||v=="MANUAL")){
  automatico=(v=="AUTO");prefs.putBool("auto",automatico);
  if(!automatico)setLed2(false);
  configuracao();estados();evento("MODO_ALTERADO");
 }else if(t==tp("automacao/limite/set")){
  char* fim=nullptr;float novo=strtof(v.c_str(),&fim);
  if(fim==v.c_str()||*fim!='\0'||!isfinite(novo)||novo<15||novo>45)return;
  limite=novo;prefs.putFloat("limite",limite);
  configuracao();evento("LIMITE_ALTERADO");
 }
}
void conectarMQTT(){
 if(mqtt.connected()||millis()-ultimaConexao<2000)return;
 ultimaConexao=millis();String status=tp("status");
 if(mqtt.connect("esp32-grupo01",status.c_str(),1,true,"offline")){
  const char* topicos[]={"led1/set","led2/set","automacao/mode/set","automacao/limite/set"};
  for(const char* s:topicos){String t=tp(s);mqtt.subscribe(t.c_str());}
  pub("status","online",true);estados();configuracao();
 }
}
void ler(){
 float t=dht.readTemperature(),u=dht.readHumidity();
 if(!isfinite(t)||!isfinite(u)){Serial.println("Falha DHT22");return;}
 if(automatico){
  if(!led2&&t>limite){setLed2(true);evento("TEMPERATURA_ALTA");}
  else if(led2&&t<limite-HISTER){setLed2(false);evento("TEMPERATURA_NORMAL");}
 }
 char json[128];snprintf(json,sizeof(json),
 "{\"temperatura\":%.1f,\"umidade\":%.1f,\"uptime\":%lu}",
 t,u,millis()/1000UL);
 if(mqtt.connected())pub("telemetria",json);
 Serial.println(json);
}
void setup(){
 Serial.begin(115200);pinMode(LED1,OUTPUT);pinMode(LED2,OUTPUT);
 digitalWrite(LED1,LOW);digitalWrite(LED2,LOW);dht.begin();
 prefs.begin("iot",false);
 automatico=prefs.getBool("auto",false);
 limite=prefs.getFloat("limite",30.0);
 if(!isfinite(limite)||limite<15||limite>45)limite=30;
 WiFi.mode(WIFI_STA);WiFi.begin(SSID,SENHA);
 mqtt.setServer(BROKER,1883);mqtt.setCallback(receber);mqtt.setBufferSize(256);
}
void loop(){
 if(WiFi.status()!=WL_CONNECTED){
  static unsigned long ultimaWifi=0;
  if(millis()-ultimaWifi>=5000){ultimaWifi=millis();WiFi.reconnect();}
 }else{conectarMQTT();if(mqtt.connected())mqtt.loop();}
 if(millis()-ultimaLeitura>=5000){ultimaLeitura=millis();ler();}
}