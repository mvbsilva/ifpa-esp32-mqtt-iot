// IFPA — ESP32 MQTT | Firmware para dashboard web com topicos configuraveis
// Dependencias Arduino IDE: PubSubClient, DHT sensor library (Adafruit), ArduinoJson 7
// ESP32 core: WiFi.h e Preferences.h; configure secrets.h com base no exemplo.
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <time.h>
#include <PubSubClient.h>
#include <DHT.h>
#include <Preferences.h>
#include <ArduinoJson.h>
#include <math.h>
#include "secrets.h"

constexpr uint8_t LED1_PIN=23,LED2_PIN=22,DHT_PIN=4;
constexpr unsigned long READ_INTERVAL=5000;
DHT dht(DHT_PIN,DHT22);
#if MQTT_USE_TLS
WiFiClientSecure net;
#else
WiFiClient net;
#endif
PubSubClient mqtt(net); Preferences prefs;
bool led1=false,led2=false,automatico=false;
bool pendingTopicReconnect=false; unsigned long topicReconnectAt=0;
float limite=30.0f; unsigned long lastRead=0,lastMqttTry=0,lastWiFiTry=0;
const char* keys[]={"led1_set","led1_state","led2_set","led2_state","telemetry","status","mode_set","mode_state","limit_set","limit_state","events"};
const char* suffix[]={"led1/set","led1/state","led2/set","led2/state","telemetria","status","automacao/mode/set","automacao/mode/state","automacao/limite/set","automacao/limite/state","eventos"};
String topics[11];
String configSet=String("ifpa/devices/")+DEVICE_ID+"/config/set";
String configAck=String("ifpa/devices/")+DEVICE_ID+"/config/ack";

void defaults(){for(int i=0;i<11;i++)topics[i]=String("lab/grupo01/")+suffix[i];}
void saveTopics(){JsonDocument doc;for(int i=0;i<11;i++)doc[keys[i]]=topics[i];String s;serializeJson(doc,s);prefs.putString("topics",s);}
void loadTopics(){defaults();String s=prefs.getString("topics","");if(!s.length())return;JsonDocument d;if(deserializeJson(d,s))return;for(int i=0;i<11;i++){const char* v=d[keys[i]];if(v&&strlen(v)>0&&strlen(v)<120)topics[i]=v;}}
void publishAt(int idx,const String& value,bool retained=false){if(mqtt.connected())mqtt.publish(topics[idx].c_str(),value.c_str(),retained);}
void publishStates(){publishAt(1,led1?"ON":"OFF",true);publishAt(3,led2?"ON":"OFF",true);publishAt(7,automatico?"AUTO":"MANUAL",true);publishAt(9,String(limite,1),true);}
void event(const char* name){JsonDocument d;d["evento"]=name;d["uptime"]=millis()/1000;String s;serializeJson(d,s);publishAt(10,s);}
void setLed2(bool on){if(led2==on)return;led2=on;digitalWrite(LED2_PIN,on?HIGH:LOW);publishAt(3,led2?"ON":"OFF",true);}
void subscribeCommands(){const int indices[]={0,2,6,8};for(int i:indices)mqtt.subscribe(topics[i].c_str(),1);mqtt.subscribe(configSet.c_str(),1);}
void reconnectWithNewTopics(){
 // O dispositivo troca as assinaturas sem reiniciar e persiste em NVS.
 pendingTopicReconnect=true;topicReconnectAt=millis()+800;
}
void applyConfig(const String& body){
 JsonDocument d;DeserializationError err=deserializeJson(d,body);
 if(err||!d["topics"].is<JsonObject>()){mqtt.publish(configAck.c_str(),"{\"ok\":false,\"erro\":\"JSON invalido\"}");return;}
 const char* key=d["key"]|"";
 if(strlen(CONFIG_KEY)<8||strcmp(key,CONFIG_KEY)!=0){mqtt.publish(configAck.c_str(),"{\"ok\":false,\"erro\":\"chave invalida\"}");return;}
 String next[11];JsonObject map=d["topics"].as<JsonObject>();
 for(int i=0;i<11;i++){
  const char* v=map[keys[i]]|"";String s(v);
  if(s.length()<1||s.length()>119||s.indexOf('#')>=0||s.indexOf('+')>=0||s.indexOf(' ')>=0){mqtt.publish(configAck.c_str(),"{\"ok\":false,\"erro\":\"topico invalido\"}");return;}
  for(int j=0;j<i;j++)if(s==next[j]){mqtt.publish(configAck.c_str(),"{\"ok\":false,\"erro\":\"topicos duplicados\"}");return;}
  next[i]=s;
 }
 mqtt.publish(configAck.c_str(),"{\"ok\":true,\"mensagem\":\"topicos aplicados; reconectando\"}",false);
 for(int i=0;i<11;i++)topics[i]=next[i];saveTopics();reconnectWithNewTopics();
}
void receive(char* topic,byte* payload,unsigned int length){
 if(length>1500)return;String value;value.reserve(length+1);for(unsigned int i=0;i<length;i++)value+=(char)payload[i];
 String t(topic);if(t==configSet){applyConfig(value);return;}
 if(t==topics[0]&&(value=="ON"||value=="OFF")){led1=value=="ON";digitalWrite(LED1_PIN,led1?HIGH:LOW);publishAt(1,value,true);}
 else if(t==topics[2]&&!automatico&&(value=="ON"||value=="OFF"))setLed2(value=="ON");
 else if(t==topics[6]&&(value=="AUTO"||value=="MANUAL")){automatico=value=="AUTO";prefs.putBool("auto",automatico);setLed2(false);publishStates();event("MODO_ALTERADO");}
 else if(t==topics[8]){char* end=nullptr;float x=strtof(value.c_str(),&end);if(end!=value.c_str()&&*end=='\0'&&isfinite(x)&&x>=15&&x<=45){limite=x;prefs.putFloat("limit",limite);publishAt(9,String(limite,1),true);event("LIMITE_ALTERADO");}}
}
void connectMqtt(){
#if MQTT_USE_TLS
 // TLS exige relógio correto para validar a validade do certificado.
 if(time(nullptr)<1700000000)return;
#endif
 if(mqtt.connected()||millis()-lastMqttTry<3000)return;lastMqttTry=millis();String clientId=String(DEVICE_ID);bool ok;
 if(strlen(MQTT_USER))ok=mqtt.connect(clientId.c_str(),MQTT_USER,MQTT_PASSWORD,topics[5].c_str(),1,true,"offline");
 else ok=mqtt.connect(clientId.c_str(),topics[5].c_str(),1,true,"offline");
 if(ok){subscribeCommands();publishAt(5,"online",true);publishStates();event("CONECTADO");}}
void readSensor(){float t=dht.readTemperature(),h=dht.readHumidity();if(!isfinite(t)||!isfinite(h)){Serial.println("Falha na leitura DHT22");return;}
 if(automatico){if(!led2&&t>limite){setLed2(true);event("TEMPERATURA_ALTA");}else if(led2&&t<limite-1.0f){setLed2(false);event("TEMPERATURA_NORMAL");}}
 JsonDocument d;d["temperatura"]=roundf(t*10)/10;d["umidade"]=roundf(h*10)/10;d["uptime"]=millis()/1000;String s;serializeJson(d,s);publishAt(4,s);Serial.println(s);}
void setup(){Serial.begin(115200);pinMode(LED1_PIN,OUTPUT);pinMode(LED2_PIN,OUTPUT);digitalWrite(LED1_PIN,LOW);digitalWrite(LED2_PIN,LOW);dht.begin();
 prefs.begin("ifpa-iot",false);automatico=prefs.getBool("auto",false);limite=prefs.getFloat("limit",30);if(!isfinite(limite)||limite<15||limite>45)limite=30;loadTopics();
 WiFi.mode(WIFI_STA);WiFi.begin(WIFI_SSID,WIFI_PASSWORD);
#if MQTT_USE_TLS
 // Validação TLS obrigatória: nunca usar setInsecure() em serviços públicos.
 net.setCACert(MQTT_ROOT_CA_PEM);
 configTime(0,0,"pool.ntp.org","time.google.com");
#endif
 mqtt.setServer(MQTT_HOST,MQTT_PORT);mqtt.setCallback(receive);mqtt.setBufferSize(2048);}
void loop(){if(pendingTopicReconnect && (long)(millis()-topicReconnectAt)>=0){pendingTopicReconnect=false;mqtt.disconnect();lastMqttTry=millis()-3000;}if(WiFi.status()!=WL_CONNECTED){if(millis()-lastWiFiTry>5000){lastWiFiTry=millis();WiFi.reconnect();}}
 else{connectMqtt();if(mqtt.connected())mqtt.loop();}if(millis()-lastRead>READ_INTERVAL){lastRead=millis();readSensor();}}
