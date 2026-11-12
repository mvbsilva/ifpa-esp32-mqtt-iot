#include <WiFi.h>
#include <PubSubClient.h>
const char* SSID="WIFI_LAB", *SENHA="SENHA_WIFI";
const char* BROKER="192.168.1.100";
const char* BASE="lab/grupo01";
const int LED1=23, LED2=22;
WiFiClient rede; PubSubClient mqtt(rede);
bool e1=false, e2=false;
void publicarEstados(){
  mqtt.publish("lab/grupo01/led1/state",e1?"ON":"OFF",true);
  mqtt.publish("lab/grupo01/led2/state",e2?"ON":"OFF",true);
}
void mensagem(char* topico, byte* dados, unsigned int n){
  String valor="";
  for(unsigned int i=0;i<n;i++) valor+=(char)dados[i];
  if(valor!="ON" && valor!="OFF") return;
  String t=String(topico);
  if(t=="lab/grupo01/led1/set"){
    e1=(valor=="ON"); digitalWrite(LED1,e1); publicarEstados();
  } else if(t=="lab/grupo01/led2/set"){
    e2=(valor=="ON"); digitalWrite(LED2,e2); publicarEstados();
  }
}
void conectar(){
  while(!mqtt.connected()){
    if(mqtt.connect("esp32-grupo01")){
      mqtt.subscribe("lab/grupo01/led1/set");
      mqtt.subscribe("lab/grupo01/led2/set");
      publicarEstados();
    } else delay(2000);
  }
}
void setup(){
  Serial.begin(115200);
  pinMode(LED1,OUTPUT); pinMode(LED2,OUTPUT);
  digitalWrite(LED1,LOW); digitalWrite(LED2,LOW);
  WiFi.begin(SSID,SENHA);
  while(WiFi.status()!=WL_CONNECTED) delay(500);
  mqtt.setServer(BROKER,1883); mqtt.setCallback(mensagem);
}
void loop(){if(!mqtt.connected()) conectar(); mqtt.loop();}