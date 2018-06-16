/*
  Developed by Prashant Kumar - https://www.linkedin.com/in/prashant-kumar-21b012154/
  Project: IBMM | NodeMCU ESP32 Tutorials
  Embedded System and Robotics Research Group - https://electronicscu.wixsite.com/research

  GitHub - https://github.com/cubleresearch/NodeMCU_ESP32_Tutorials/tree/master/Projects
  YouTube - https://youtu.be/qoBqcQsEqZo

  Upload date: 16 June 2018

  BLE_Remote_Light

  Board: EH-MT LIVE ESP32 DEVKIT
  MCU: NodeMCU ESP32-WROOM-32 - https://www.espressif.com/en/esp-wroom-32/resources
  BLE Library: ESP32_BLE_Arduino - https://github.com/nkolban/ESP32_BLE_Arduino
  Custom UUID Generator - https://www.uuidgenerator.net/
  Android App Designer: MIT App Inventor - ai2.appinventor.mit.edu
  Android App Source Code - https://roboindia.com/tutorials/ble-4.0-arduino-led-control-mit-app-inventor
  Android App: LED Control with BLE Module HM10 - https://github.com/cubleresearch/NodeMCU_ESP32_Tutorials/tree/master/Projects
  
  BLE Remote Light turns a light on and off based on signal recieved from smartphone.
  This device uses Bluetooth Low Energy for remote connection.
  

  SETUP - algorithm
  1) set a pin connected to light to OUTPUT and write LIGHT_OFF to it

  2) set device public display name
  3) create server
  4) add custom Remote_Light service to server
  5) set custom server callbacks for client

  6) create "Digital" characteristic with read and write properties for Remote_Light service
  7) set characteristic value
  8) sets callback for "Digital" characteristic

  9) start the service
  10) get advertising object and start advertising the server
  


  LOOP - algorithm
  ---none---
  
*/

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

#define SERVICE_UUID        "a39bc5f4-b111-43d5-870b-03fcf20a88f3" // Remote_Light custom service uuid
#define CHARACTERISTIC_UUID "00002A56-0000-1000-8000-00805f9b34fb" //predefine "Digital" characteristic
using namespace std;
const int LIGHT_ON = HIGH; 
const int LIGHT_OFF = LOW;
const int LIGHT = 18; // pin to which LED(+ve led) is connected)

int light_state = LIGHT_OFF;

BLEServer *remote_light_server;
BLEService *remote_light_service;
BLECharacteristic *digital_characteristic;
BLEAdvertising *remote_light_advertising;

void display_light_state(){
  // for displaying light state on serial monitor
  if(light_state)Serial.println("Light status update: ON");
  else Serial.println("Light status update: OFF");
}

class RemoteLightServerCallbacks: public BLEServerCallbacks{
  // server callbacks for client
public:
  virtual void onConnect(BLEServer *server){
    // gets called when client connects with server
    Serial.println("Client Connected.");
    remote_light_advertising->stop();
    Serial.println("Stopped Advertising");

    display_light_state();
    
  }
  virtual void onDisconnect(BLEServer* server){
    // gets called when client disconnects with server
    Serial.println("Client Disconnected");
    remote_light_advertising->start();
    Serial.println("Started Advertising");
    
    display_light_state();
  }
};

class DigitalCharacteristicCallbacks: public BLECharacteristicCallbacks {
  // "Digital" characteristic callback 
public:
  virtual void onRead(BLECharacteristic* characteristic){
    // no need to define characteristic read operations
  }
  
  virtual void onWrite(BLECharacteristic* characteristic){
    // on write of characteristic value, reads the value and perform operation accordingly
    string value;
    value = characteristic->getValue();
    boolean instructed_light_state = value[0] == '1';
    if(light_state != instructed_light_state){
      light_state = instructed_light_state;
      digitalWrite(LIGHT, light_state);
      display_light_state();
      characteristic->setValue(value);
    }
  }
};

void setup() {
  Serial.begin(9600);
  Serial.println("Starting BLE_Remote_Light_502");

  pinMode(LIGHT, OUTPUT); // set pin to OUTPUT
  digitalWrite(LIGHT, LIGHT_OFF);// writes LIGHT_OFF(HIGH)

  BLEDevice::init("Remote_Light_502"); // sets device public display name
  remote_light_server = BLEDevice::createServer(); // creates server
  // adds custom Remote_Light service to server
  remote_light_service = remote_light_server->createService(SERVICE_UUID);
  // sets server callbacks for client
  remote_light_server->setCallbacks(new RemoteLightServerCallbacks());

  // creates "Digital" characteristic with read and write properties for Remote_Light service
  digital_characteristic = remote_light_service->createCharacteristic(CHARACTERISTIC_UUID,
                                                                      BLECharacteristic::PROPERTY_WRITE | 
                                                                      BLECharacteristic::PROPERTY_READ);
  digital_characteristic->setValue("0");// sets initial characteristic value to 0(LIGHT_OFF)
  // sets callback for "Digital" characteristic
  digital_characteristic->setCallbacks(new DigitalCharacteristicCallbacks());// class is defined above
  
  remote_light_service->start();// starts the service
  remote_light_advertising = remote_light_server->getAdvertising();// gets advertising object
  remote_light_advertising->start();// starts advertising the server
  Serial.println("Started Advertising...");
  // now this can be scanned with app on the smartphone and can get connected to start recieving signals
}

void loop() {
  
}
