#include "bluetoothHelper.h"
MqttHelper helper;

BluetoothHelper::BluetoothHelper() {}

BluetoothHelper::~BluetoothHelper() {}

void BluetoothHelper::handleConnectionStatus() {
  if (SerialBT.available()) {
      SerialBT.write('WIFI:CONNECTED&');

    bool isRegistered = helper.preferences.getBool(REGISTERED_ESP, false);
    if (!isRegistered) {
      Register_request request;
      String userId =helper.preferences.getString(USER_UID, "null");
      String device_type = helper.preferences.getString(DEVICE_TYPE, "null");

      request.user_id = userId.substring(0, userId.length() - 1);
      request.device_id = WiFi.macAddress();
      request.device_type = device_type.substring(0, device_type.length() - 1);

      if (helper.registerDevice(request)) {
        SerialBT.write('DEVICE:REGISTERED:TRUE&');
        helper.preferences.putBool(REGISTERED_ESP, true);
        SerialBT.end();
      } else {
        SerialBT.write('DEVICE:REGISTERED:FALSE&');
      }
    }
  }
}



void BluetoothHelper::setup(Preferences& preferences) {
  //SerialBT.begin("JET-32-0");
  helper.preferences.begin("JET", false);
  helper.preferences.getString(WIFI_SSID, "null").toCharArray(mySSID, 64);
  helper.preferences.getString(WIFI_PASSWORD, "null").toCharArray(myPassword, 64);
  byte lastChar = strlen(mySSID) - 1;
  mySSID[lastChar] = '\0';
  lastChar = strlen(myPassword) - 1;
  myPassword[lastChar] = '\0';
}

void BluetoothHelper::loop() {
  if (SerialBT.available()) {
    char inputChar = SerialBT.read();
    if (inputChar != '\n') {
      message += String(inputChar);
    }
    else {
      if (message.startsWith("SSID:")) {
        // Extract the SSID from the message
        String ssid = message.substring(5);
        // Do something with the received SSID
        Serial.println("Received SSID: " + ssid);
        helper.preferences.putString(WIFI_SSID, ssid);
      }
      else if (message.startsWith("PASSWORD:")) {
        // Extract the password from the message
        String password = message.substring(9);
        // Do something with the received password
        Serial.println("Received password: " + password);
        helper.preferences.putString(WIFI_PASSWORD, password);
      }
      else if (message == "conn") {
        SerialBT.end();
        byte lastChar = strlen(mySSID) - 1;
        mySSID[lastChar] = '\0';
        lastChar = strlen(myPassword) - 1;
        myPassword[lastChar] = '\0';
        WiFi.begin(mySSID, myPassword);
      }
      else if (message.startsWith("USER_ID:")) {
        String userId = message.substring(8);
        // Do something with the received user ID
        Serial.println("Received UID: " + userId);
        helper.preferences.putString(USER_UID, userId);
      }
      else if (message.startsWith("DEVICE_TYPE:")) {
        String deviceType = message.substring(12);
        // Do something with the received device type
        Serial.println("Received Device: " + deviceType);
        helper.preferences.putString(DEVICE_TYPE, deviceType);
      }
      message = "";
    }
  }
}


