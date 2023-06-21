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
  //testing with mock wifi 
  WiFi.begin("lauchita", "lauchita");

  // Real wifi
  // WiFi.begin(bluetoothHelper.mySSID, bluetoothHelper.myPassword);
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
    if (!mqtthelper.mqttClient.connected()) {
          Serial.println("B");
          mqtthelper.mqttClient.setKeepAlive( 10 ); // setting keep alive to 90 seconds
          mqtthelper.reconnect();
    }
    mqtthelper.mqttClient.loop();
  }


  vTaskDelay(10 / portTICK_RATE_MS);
}
