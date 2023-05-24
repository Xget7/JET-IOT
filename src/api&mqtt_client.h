#ifndef API_MQTT_CLIENT_H
#define API_MQTT_CLIENT_H

// Constants for Wi-Fi and MQTT configurations
#define WIFI_SSID "WIFI_SSID"
#define WIFI_PASSWORD "WIFI_PASSWORD"
#define USER_UID "USER_UID"
#define DEVICE_TYPE "DEVICE_TYPE"
#define REGISTERED_ESP "REGISTERED_ESP"

// Include necessary libraries
#include "Arduino.h"
#include "HTTPClient.h"
#include "WiFi.h"
#include "PubSubClient.h"
#include "Preferences.h"

// Declare global variables and objects
extern HTTPClient client;
extern PubSubClient mqttClient;
extern Preferences preferences;

extern char *server;
extern int port;

// Define the structure for register request
struct register_request
{
   String user_id;
   String device_id;
   String device_type;
};

// Function declarations
void reconnect();
void mqttLoop();
void initMqttAndApi(std::function<void(char*, uint8_t*, unsigned int)> callback);
bool registerDevice(register_request request);

#endif
