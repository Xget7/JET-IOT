#include "api&mqtt_client.h"

// Global variables and objects


MqttHelper::MqttHelper() : mqttClient(esp32Client) {
  server ="mqtt-dashboard.com";
  port = 8884;
}


MqttHelper::~MqttHelper() {}


// Function to reconnect MQTT client
void MqttHelper::reconnect() {
    Serial.println("Attempting MQTT connection...");
     String macAddress = WiFi.macAddress();
    if (mqttClient.connect(&macAddress[0], "xget", "xget")) { 
        Serial.println("Connected");

        String userId = preferences.getString(USER_UID, "null");
        userId = userId.substring(0, userId.length() - 1);
       

        // Concatenate userId, "/", and macAddress
        String topic = userId + "/" + macAddress;

        // Subscribe to the MQTT topic
        mqttClient.subscribe(topic.c_str());
        Serial.print(topic.c_str());

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
      mqttClient.setKeepAlive( 90 ); // setting keep alive to 90 seconds
      reconnect();
      vTaskDelay( 250 );

  }
  mqttClient.loop();
}

// Function to initialize MQTT client and API
void MqttHelper::initMqttAndApi(std::function<void(char*, uint8_t*, unsigned int)> callback) {
  mqttClient.setServer(server, 8883);


  // client.begin("https://jsonplaceholder.typicode.com/todos/1");
  // client.addHeader("Content-Type", "application/json");
  // Serial.print("SERVER");
  // Serial.println(server);
  // Serial.print("PORT");
  // Serial.println(port);
 // mqttClient.setCallback(callback);
}

// Function to register the device
bool MqttHelper::registerDevice(Register_request request) {
  // Convert the register_request object to JSON format
  String payload = "{\"user_id\":\"" + request.user_id + "\",\"device_id\":\"" + request.device_id + "\",\"device_type\":\"" + request.device_type + "\"}";

  // Set headers and body of the HTTP request
  int httpCode = client.POST(payload);

  // Check the HTTP response code
  if (httpCode == HTTP_CODE_OK) {
    String response = client.getString();
    Serial.println("Registration successful: " + response);
    return true;
  } else {
    Serial.println("Registration error. HTTP response code: " + String(httpCode));
    return false;
  }

  // Release the resources of the HTTP request
  client.end();
}
