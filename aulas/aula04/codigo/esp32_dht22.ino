#include <WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>
#include <math.h>
const char* SSID="WIFI_LAB", *SENHA="SENHA_WIFI";
const char* BROKER="192.168.1.100";
const int LED1=23,LED2=22,DHT_PIN=4;
DHT dht(DHT_PIN,DHT22);
WiFiClient rede; PubSubClient mqtt(rede);
bool e1=false,e2=false;
unsigned long ultima=0;
void estados(){
 mqtt.publish("lab/grupo01/led1/state",e1?"ON":"OFF",true);
 mqtt.publish("lab/grupo01/led2/state",e2?"ON":"OFF",true);
}
void receber(char* topico,byte* payload,unsigned int n){
 String v="";for(unsigned int i=0;i<n;i++)v+=(char)payload[i];
 if(v!="ON"&&v!="OFF")return;
 String t=String(topico);
 if(t=="lab/grupo01/led1/set"){
   e1=(v=="ON");digitalWrite(LED1,e1);estados();
 }else if(t=="lab/grupo01/led2/set"){
   e2=(v=="ON");digitalWrite(LED2,e2);estados();
 }
}
void conectar(){
 while(!mqtt.connected()){
  if(mqtt.connect("esp32-grupo01","lab/grupo01/status",1,true,"offline")){
   mqtt.subscribe("lab/grupo01/led1/set");
   mqtt.subscribe("lab/grupo01/led2/set");
   mqtt.publish("lab/grupo01/status","online",true);estados();
  }else delay(2000);
 }
}
void setup(){
 Serial.begin(115200);pinMode(LED1,OUTPUT);pinMode(LED2,OUTPUT);
 digitalWrite(LED1,LOW);digitalWrite(LED2,LOW);
 dht.begin();WiFi.begin(SSID,SENHA);
 while(WiFi.status()!=WL_CONNECTED)delay(500);
 mqtt.setServer(BROKER,1883);mqtt.setCallback(receber);
 mqtt.setBufferSize(256);
}
void loop(){
 if(!mqtt.connected())conectar();mqtt.loop();
 if(millis()-ultima>=5000){
  ultima=millis();
  float t=dht.readTemperature(),u=dht.readHumidity();
  if(isfinite(t)&&isfinite(u)){
   char json[128];
   snprintf(json,sizeof(json),"{\"temperatura\":%.1f,\"umidade\":%.1f,\"uptime\":%lu}",t,u,millis()/1000UL);
   mqtt.publish("lab/grupo01/telemetria",json,false);
   Serial.println(json);
  }
 }
}