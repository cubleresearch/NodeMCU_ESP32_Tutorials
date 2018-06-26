/*  
  Developed by Prashant Kumar - https://www.linkedin.com/in/prashant-kumar-21b012154/
  Project: IBMM | NodeMCU ESP32 Tutorials
  Embedded System and Robotics Research Group - https://electronicscu.wixsite.com/research

  GitHub - https://github.com/cubleresearch/NodeMCU_ESP32_Tutorials/tree/master/Projects
  YouTube - ___________________

  Upload date: 26 June 2018

  Node
  
  Board: MH-ET LIVE ESP32 DEVKIT
  MCU: NodeMCU ESP32-WROOM-32 - https://www.espressif.com/en/esp-wroom-32/resources
  BLE Library: ESP32_BLE_Arduino - https://github.com/nkolban/ESP32_BLE_Arduino
  
  Node advertises a unique display name with BLE signal.

*/
#include<BLEDevice.h>

void setup() {
  // Begin serial communication
  Serial.begin(9600);
  Serial.println("\nInitializing Node: NODE_0");

  //initializes bluetooth low energy environment
  BLEDevice::init("NODE_0");// your own unique display name
  // assign different name to each node and add it in the list of nodes in Presence_detector sketch
  BLEServer *server = BLEDevice::createServer();// creates a server
  BLEAdvertising *advertising = server->getAdvertising(); // get advertising object
  advertising->start(); // starts advertising
  Serial.println("Started Advertising");

  //upload this sketch with different name to each node(esp32)
  // I created 4 nodes
}

void loop() {
  // empty
}
