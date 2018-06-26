/*  
  Developed by Prashant Kumar - https://www.linkedin.com/in/prashant-kumar-21b012154/
  Project: IBMM | NodeMCU ESP32 Tutorials
  Embedded System and Robotics Research Group - https://electronicscu.wixsite.com/research

  GitHub - https://github.com/cubleresearch/NodeMCU_ESP32_Tutorials/tree/master/Projects
  YouTube - https://youtu.be/yf_25O3xtvs

  Upload date: 26 June 2018

  Presence_Scanner
  
  Board: MH-ET LIVE ESP32 DEVKIT
  MCU: NodeMCU ESP32-WROOM-32 - https://www.espressif.com/en/esp-wroom-32/resources
  HTTP Tutorial: https://www.tutorialspoint.com/http/index.htm
  UBIDOTS: https://app.ubidots.com
  UBIDOTS API: https://ubidots.com/docs/api/#rest-api-reference
  BLE Library: ESP32_BLE_Arduino - https://github.com/nkolban/ESP32_BLE_Arduino

  Scans for BLE devices and filter from the list to find nodes from nodes list
  and updates its status online or offline accordingly.

  1) set up BLE
  2) set up WiFi
  3) start scanning
  4) filter through the scanned devices and find nodes
  5) set online status to nodes found
  6) set offline status to nodes not found during scan

*/
#include <BLEDevice.h>
#include <WiFi.h>

#define WIFI_SSID "Joker" // your wifi sssid
#define WIFI_PASSWORD "password" // your wifi password

#define UBIDOT_TOKEN "A1E-f8B5jSdUG3YkSPCrzrpsjV9ewbWFnW" // your Ubidot token
#define HOST "things.ubidots.com"
#define HTTP_PORT 80

#define NODE_COUNT 4 // number of nodes
#define ACTIVE_TIMEOUT 4000 // in millisecond
// timeout for deciding whether node is offline or not

using namespace std;

const string node_list[NODE_COUNT] = {"NODE_0",    // list of nodes unique display name
                                      "NODE_1",
                                      "NODE_2",
                                      "NODE_3",};

BLEScan *scanner;
BLEScanResults found_devices;
unsigned long last_active[NODE_COUNT]; // stores last active cpu time
bool node_active[NODE_COUNT];// store each node status online or offline
WiFiClient client;

// inherit BLEAdvertisedDeviceCallbacks to create your own call back handler
class AttendanceScanner: public BLEAdvertisedDeviceCallbacks {
  //gets called when during scan a new device is found
  void onResult(BLEAdvertisedDevice device){
    unsigned long current_time = millis();
    string node = device.getName().c_str();
    Serial.printf("Node Found: %s Time: %d\n", node.c_str(), current_time);
    int index = -1;
   
    for(int i = 0; i < NODE_COUNT; i++){
      if(node.compare(node_list[i]) == 0)index = i;
    }

    if(index == -1){
      Serial.println("No match was found!");
      return;
    }
    // updates online status of found node
    node_active[index] = true;
    Serial.printf("Update Status: Node_%d - ONLINE\n", index);
    last_active[index] = current_time;
      
  }
};

// uploads node status to ubidots
void upload_status(){
   
  Serial.print("");
  Serial.println(HOST);

  // connect with things.ubidots.com
  if (!client.connect(HOST, HTTP_PORT)) {
    Serial.println("connection failed");
    return;
  }
  
  //upload data
  // create a URI for the request
  // for how to create URI for HTTP request visit https://www.tutorialspoint.com/http/index.htm
  // for how to add data to it for uploading to ubodots.com visit https://ubidots.com/docs/api/#rest-api-reference
  for(int i = 0; i < NODE_COUNT; i++){
    String upload_request = "POST /api/v1.6/devices/esp32_presence_detector/node_";
    upload_request += String(i); // this adds index to variable like
    //node_0, node_1  here 0 and 1 is added by above line to match variable label on ubidots
    upload_request += "/values/?token=";
    upload_request += UBIDOT_TOKEN; // this is the token
    upload_request += " HTTP/1.1\r\n";
    upload_request += "Host: things.ubidots.com\r\n";
    upload_request += "Content-Type: application/json\r\n";
    upload_request += "Content-Length: 12\r\n"; // content length of message string
    upload_request += "\r\n";

    // add data to HTTP request this is a json object
    upload_request += "{\"value\": "; //this message string
    upload_request += String(node_active[i]); // adds node status
    upload_request += "}\r\n"; // till here
    
    Serial.println(upload_request);

    // This will send the HTTP request to the server
    client.print(upload_request);
  }

  // disconnect the server
  client.stop();
  Serial.println();
  Serial.println("Closing connection to server.");
}

void setup() {
  // begin serial communication
  Serial.begin(9600);
  Serial.println("\nInitializing Presence_Detector...");

  for(int i = 0; i < NODE_COUNT; i++){
    last_active[i] = 0;
    node_active[i] = false;
  }
  
  // set up ble enviornment
  Serial.print("  Initializing BLE...");
  BLEDevice::init("Presence_Detector");
  scanner = BLEDevice::getScan(); // get scan object
  // set your own advertised device call backs
  scanner->setAdvertisedDeviceCallbacks(new AttendanceScanner(), false); 
  scanner->setActiveScan(true); // active scan gets the job done faster
  Serial.println("Done");

  // set up wifi environment
  Serial.print("  Initializing WiFi.");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  // wait till esp32 establishes wifi connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.print("\n  Connected to ");
  Serial.println(WiFi.localIP());
  Serial.println("  Done");

  Serial.println("Done");  
  
}
// change the partition scheme to minimal spiffs otherwise sketch wont uploas as it 
// uses a lot of space on flash memory
void loop() {
  // start scanning
  Serial.print("Scanning for devices....\n");
  found_devices = scanner->start(2);// this scans for 2 seconds
  Serial.println("Done");
  Serial.print("Devices found: ");
  Serial.println(found_devices.getCount());

  // update node offline status
  unsigned long current_time = millis(); // returns current cpu time
  for(int i = 0; i < NODE_COUNT; i++){
    // sets offline to those nodes which are inactive active_timeout
    if(current_time - last_active[i] > ACTIVE_TIMEOUT ){ 
        Serial.printf("Update Status: Node_%d - OFFLINE\n", i);
        node_active[i] = false;
      }
  }
  upload_status(); // uploads status to ubidots
}
