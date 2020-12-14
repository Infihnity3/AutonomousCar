// Test standalone RTC module FYP Afiq
// BLE test

// include reuired library
#include<BLEDevice.h>
#include<ArduinoJson.h>
#include <WiFi.h>

BLEScan* BLEscanner; // name the scanner
static BLEAddress *deviceAddress;

// scanning variable
#define maxOfList 150
int currentList = 0;
String maclist[maxOfList];

int scanTimer = 5;

// callback
class BLEscannercallback : public BLEAdvertisedDeviceCallbacks
{
    void onResult(BLEAdvertisedDevice advertisedDevice)
    {
      deviceAddress = new BLEAddress(advertisedDevice.getAddress());
      //JSON
      char dataJson[60];
      StaticJsonDocument<70> Data;
      Data["MAC"] = deviceAddress->toString().c_str();
      Data["RSSI"] = advertisedDevice.getRSSI();
      serializeJson(Data, dataJson);
      //    Serial.println(dataJson);

      String mac2 = deviceAddress->toString().c_str();

      int flag = 0;
      for (int i = 0; i < maxOfList; i++)
      {
        if (mac2 == maclist[i])
        {
          flag = 1;
        }
      }

      if (flag == 0)
      {
        maclist[currentList] = mac2;
        Serial.print(currentList + 1);
        Serial.printf("\t MAC: ");
        Serial.print(mac2);
        Serial.printf("\t ");
        Serial.print(WiFi.macAddress());
        Serial.printf("\t RSSI: %0d", advertisedDevice.getRSSI());
        Serial.println();
        currentList++;
      }

    }
};

void setup()
{
  // setup the serial connection
  Serial.begin(115200);
  Serial.println(F("[SYSTEM] Program Starting..."));

  // start the BLE and scan function
  BLEDevice::init("");

  // scanner setup and callback
  BLEscanner = BLEDevice::getScan();
  BLEscanner -> setAdvertisedDeviceCallbacks(new BLEscannercallback());
  BLEscanner -> setActiveScan(true);
  BLEscanner -> setInterval(100);
  BLEscanner -> setWindow(99);
}

void loop()
{
  BLEScanResults deviceFound = BLEscanner ->start(scanTimer);
}
