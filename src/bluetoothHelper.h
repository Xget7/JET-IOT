#ifndef BLUETOOTH_HELPER_H
#define BLUETOOTH_HELPER_H

#include <BluetoothSerial.h>
#include "api&mqtt_client.h"


class BluetoothHelper {
public:
  BluetoothHelper();
  ~BluetoothHelper();

  void setup(Preferences& preferences);
  void handleConnectionStatus();
  void loop();
  char mySSID[64];
  char myPassword[64];
  char userUid[64];
  BluetoothSerial SerialBT;
private:
  String message;
  
};

#endif

