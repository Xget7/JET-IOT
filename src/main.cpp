#include "bluetoothHelper.h"
#include "api&mqtt_client.h"

// Global variables and objects
String message = "";
String macAddress = "";
BluetoothHelper bluetoothHelper;

// char mySSID[64];
// char userUid[64];
// char myPassword[64];
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

void setupMQTTAndAPI() {
  initMqttAndApi(callback);
}

void setup() {
  Serial.begin(9600);
  bluetoothHelper.setup(preferences);
  delay(1000);
  setupWiFi();
  setupMQTTAndAPI();
  macAddress = WiFi.macAddress();
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
       bluetoothHelper.loop();
  } else {
    bluetoothHelper.handleConnectionStatus();
    mqttLoop();
  }
}
