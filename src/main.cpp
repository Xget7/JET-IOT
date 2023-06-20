#include "bluetoothHelper.h"
#include "api&mqtt_client.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

// Global variables and objects
String message = "";
String macAddress = "";
BluetoothHelper bluetoothHelper;
MqttHelper mqtthelper;


int relay_state = 0;

BluetoothSerial SerialBT;

// Callback function to handle received MQTT messages
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Received message [");
  Serial.print(topic);
  Serial.print("] ");
  char payload_string[length + 1];
  int resultI;
  memcpy(payload_string, payload, length);
  payload_string[length] = '\0';
  resultI = atoi(payload_string);
  relay_state = resultI;
}

void setupWiFi() {
  WiFi.mode(WIFI_STA); // Optional
  WiFi.begin(bluetoothHelper.mySSID, bluetoothHelper.myPassword);
}


void setup() {
  Serial.begin(115200);
  bluetoothHelper.setup(mqtthelper.preferences, false);
  delay(500);
  setupWiFi();
  delay(4000);
    
  if (WiFi.status() != WL_CONNECTED){
       Serial.print("NO HAY WIfi");
    WiFi.disconnect();
    bluetoothHelper.setup(mqtthelper.preferences, true);
  }else{
    Serial.print("Hay WIfi");
    mqtthelper.initMqttAndApi(callback);
  }
  macAddress = WiFi.macAddress();
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
      bluetoothHelper.loop();
  } else {
    // Serial.print("MQTT");
    // delay(1999);
  }


//MQTT FUNCIONANDO
//  if (!mqtthelper.mqttClient.connected()) {
//       Serial.println("B");
//       mqtthelper.mqttClient.setKeepAlive( 10 ); // setting keep alive to 90 seconds
//       mqtthelper.reconnect();
//   }

//   mqtthelper.mqttClient.loop();
  vTaskDelay(10 / portTICK_RATE_MS);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// #include <WiFi.h>
// #include <PubSubClient.h>
// #include <ArduinoJson.h>

// WiFiClient esp32Client;
// PubSubClient mqttClient(esp32Client);

// const char* ssid     = "MovistarFibra-41E600";
// const char* password = "GAwhWkRE6HGciUgpuYEx";

// char *server = "broker.emqx.io";
// int port = 1883;

// int ledpin= 2;

// int var = 0;
// int ledval = 0;

// String userId = "58749835934";
// String deviceMac = WiFi.macAddress();
  

// String resultS = "";

// void wifiInit() {
//     Serial.print("Conectándose a ");
//     Serial.println(ssid);

//     WiFi.begin(ssid, password);

//     while (WiFi.status() != WL_CONNECTED) {
//       Serial.print(".");
//         delay(500);  
//     }
//     Serial.println("");
//     Serial.println("Conectado a WiFi");
//     Serial.println("Dirección IP: ");
//     Serial.println(WiFi.localIP());
//   }

// void callback(char* topic, byte* payload, unsigned int length) {
//   Serial.print("Mensaje recibido [");
//   Serial.print(topic);
//   Serial.print("] ");

//   char payload_string[length + 1];
  
//   int resultI;

//   memcpy(payload_string, payload, length);
//   payload_string[length] = '\0';
//   resultI = atoi(payload_string);
  
//   var = resultI;

// }



// void reconnect() {
//   while (!mqttClient.connected()) {
//     Serial.print("Intentando conectarse MQTT...");

//     StaticJsonDocument<300> jsonDoc;
//     String payload = "";
//     jsonDoc["online"] = 0;
//     jsonDoc["state"] = var;
//     serializeJson(jsonDoc, payload);

//     String topic = userId + "/" + deviceMac + "/state";
//     String willTopic = userId + "/" + deviceMac + "/status";
//     if (mqttClient.connect(&deviceMac[0], "xget", "xget", (char*)willTopic.c_str(), 0, true, (char*)payload.c_str())) {
//       Serial.println("Conectado");
  
//       mqttClient.subscribe(topic.c_str());
      
//     } else {
//       Serial.print("Fallo, rc=");
//       Serial.print(mqttClient.state());
//       Serial.println(" intentar de nuevo en 5 segundos");
//       // Wait 5 seconds before retrying
//       delay(5000);
//     }
//   }
// }

// void setup()
// {
//   pinMode(ledpin,OUTPUT);
//   Serial.begin(115200);
//   delay(10);
//   wifiInit();
//   mqttClient.setServer(server, port);
//   mqttClient.setCallback(callback);
// }

// void loop()
// {
//   if (!mqttClient.connected()) {
//     reconnect();
//   }
//   mqttClient.loop();

//   if(var == 0){
//     Serial.print("LOW");
//     digitalWrite(ledpin,LOW);
//   }
//   else if (var == 1){
//     Serial.print("HIGH");
//     digitalWrite(ledpin,HIGH);
//   }


  
//   StaticJsonDocument<300> jsonDoc;
//   String payload = "";
//   jsonDoc["online"] = 1;
//   jsonDoc["state"] = var;

//   serializeJson(jsonDoc, payload);

//   String topic = userId + "/" + deviceMac + "/status";
  
//   mqttClient.publish(topic.c_str(),payload.c_str());
//   Serial.println(topic);
//   delay(1500);
// }

