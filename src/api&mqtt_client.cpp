#include "api&mqtt_client.h"

// Global variables and objects


MqttHelper::MqttHelper() : mqttClient(esp32Client) {
  server ="z002205d.ala.us-east-1.emqxsl.com";
  port = 8883;
}


MqttHelper::~MqttHelper() {}

String filterMac(String mac) {
  mac.replace(":", "");
  return mac;
}

// Function to reconnect MQTT client
void MqttHelper::reconnect() {
     Serial.println("Attempting MQTT connection...");
      String macAddress = WiFi.macAddress();
      macAddress = filterMac(macAddress);
      StaticJsonDocument<300> jsonDoc;
      String offlinePayLoad = "";
      jsonDoc["online"] = 0;
      jsonDoc["state"] = 0;
      serializeJson(jsonDoc, offlinePayLoad);
      String userId = preferences.getString(USER_UID, "null");
      userId = userId.substring(0, userId.length() - 1);

      //real willtopic
      // String willTopic = userId + "/" + macAddress + "/status";
      // mock
      String willTopic = "e134b077-d0db-48a5-ab5a-0512c460559a/7821849C1782/data";


    if (mqttClient.connect(&macAddress[0], "xget", "eltupa2005", (char*)willTopic.c_str(), 0, true, (char*)offlinePayLoad.c_str())) { 
        Serial.println("Connected");

       //real topic
        // String topic = userId + "/" + macAddress + "/state";
         //mock topic
       String topic ="e134b077-d0db-48a5-ab5a-0512c460559a/7821849C1782/data";
        // Subscribe to the MQTT topic
        mqttClient.subscribe(topic.c_str());
        Serial.println(topic.c_str());
    } else {
        Serial.print("Failed, rc=");
        Serial.print(mqttClient.state());
        Serial.println(" retrying in 5 seconds");
        // Wait for 5 seconds before retrying
        delay(5000);
    }
}

// Function to handle MQTT client's loop
void MqttHelper::mqttLoop() {
  Serial.println("A");
  if (!mqttClient.connected()) {
      Serial.println("B");
      reconnect();
      vTaskDelay( 250 );
  }
  mqttClient.loop();


}

// Function to initialize MQTT client and API
void MqttHelper::initMqttAndApi(std::function<void(char*, uint8_t*, unsigned int)> callback) {
  mqttClient.setServer(server, port);
  client.begin("https://jsonplaceholder.typicode.com/todos/1");
  client.addHeader("Content-Type", "application/json");
   mqttClient.setCallback(callback);
}

