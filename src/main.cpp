#include "bluetoothHelper.h"
#include "api&mqtt_client.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
// Global variables and objects
String message = "";
String macAddress = "";
BluetoothHelper bluetoothHelper;
MqttHelper mqtthelper;

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


void setup() {
  Serial.begin(115200);
   Serial.println("Blutut Setup");
  bluetoothHelper.setup(mqtthelper.preferences);
  delay(1000);
   Serial.println("wifi stup");
  setupWiFi();
   Serial.println("mqtt setup");
  mqtthelper.initMqttAndApi(callback);
  delay(1000);
   Serial.println("mac adrres");
  macAddress = WiFi.macAddress();
  Serial.println("start");
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    // bluetoothHelper.loop();
  } else {
  // bluetoothHelper.handleConnectionStatus();
    
  }
  mqtthelper.mqttLoop();
  vTaskDelay(1000 / portTICK_RATE_MS);
}
