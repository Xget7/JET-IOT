#include "api&mqtt_client.h"

// Global variables and objects
HTTPClient client;
PubSubClient mqttClient;
Preferences preferences;

char *server = "broker.emqx.io";
int port = 8083;

// Function to reconnect MQTT client
void reconnect() {
  while (!mqttClient.connected()) {
    Serial.print("Attempting MQTT connection...");

    if (mqttClient.connect("arduinoClient")) {
      Serial.println("Connected");

      String userId = preferences.getString(USER_UID, "null").substring(0, userId.length() - 1);
      String macAddress = WiFi.macAddress();

      // Concatenate userId, "/", and macAddress
      String topic = userId + "/" + macAddress;

      // Subscribe to the MQTT topic
      mqttClient.subscribe(topic.c_str());
      
    } else {
      Serial.print("Failed, rc=");
      Serial.print(mqttClient.state());
      Serial.println(" retrying in 5 seconds");
      // Wait for 5 seconds before retrying
      delay(5000);
    }
  }
}

// Function to handle MQTT client's loop
void mqttLoop() {
  if (!mqttClient.connected()) {
      reconnect();
  }
  mqttClient.loop();
}

// Function to initialize MQTT client and API
void initMqttAndApi(std::function<void(char*, uint8_t*, unsigned int)> callback) {
  client.begin("https://jsonplaceholder.typicode.com/todos/1");
  client.addHeader("Content-Type", "application/json");
  mqttClient.setServer(server, port);
  mqttClient.setCallback(callback);
}

// Function to register the device
bool registerDevice(register_request request) {
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
