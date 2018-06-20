/*
  Developed by Prashant Kumar - https://www.linkedin.com/in/prashant-kumar-21b012154/
  Project: IBMM | NodeMCU ESP32 Tutorials
  Embedded System and Robotics Research Group - https://electronicscu.wixsite.com/research

  GitHub - https://github.com/cubleresearch/NodeMCU_ESP32_Tutorials/tree/master/Projects
  YouTube - https://youtu.be/JSAzADO7MIg

  Upload date: 20 June 2018

  CPU Temperature Record

  MCU: NodeMCU ESP32-WROOM-32 - https://www.espressif.com/en/esp-wroom-32/resources
  Board: MH-ET LIVE ESP32 DEVKIT
  HTTP Tutorial: https://www.tutorialspoint.com/http/index.htm
  UBIDOTS: https://app.ubidots.com
  UBIDOTS API: https://ubidots.com/docs/api/#rest-api-reference
  
  This Sketch records and uploads temperature of CPU of NodeMCU ESP32-WROOM-32 to ubidots.com every second.
  For communicationg with ubidots.com it uses HTTP/1.1.

  SETUP - algorithm
  1) connect to a wifi
  2) whit for wifi connection to establish
  
  LOOP - algorithm
  1) connect to things.ubidots.com
  2) create URI for HTTP request
  3) send request
  4) if connection to sever is not available for 5 seconds then disconnect the server
  5) read responese
  6) disconnect server

 */

#include <WiFi.h>

const char* WiFi_SSID = "Joker"; // your wifi ssid
const char* WiFi_PASSWORD = "password"; // your wifi password

const char* HOST = "things.ubidots.com"; //host as things.ubidots.com
const int HTTP_PORT = 80; // default HTTP port

const char* UBIDOT_TOKEN = "A1E-Rk01FURp383bDzweloG9RTHussz314"; // UBIDOT token for uploading data
//to find your token go to ubidots and click on api crediantials

void setup()
{
    Serial.begin(9600);
    delay(10);

    //connecting to a WiFi network
    Serial.print("Connecting to ");
    Serial.println(WiFi_SSID);
    WiFi.begin(WiFi_SSID, WiFi_PASSWORD);

    //wait for connection to establish
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
}

void loop()
{
    delay(1000);

    Serial.print("connecting to ");
    Serial.println(HOST);

    // Use WiFiClient class to create TCP connections
    WiFiClient client;

    // connect with things.ubidots.com
    if (!client.connect(HOST, HTTP_PORT)) {
        Serial.println("connection failed");
        return;
    }

    // read temperature
    float cpu_temp = temperatureRead();// this function returns value of cpu temperature

    // create a URI for the request
    // for how to create URI for HTTP request visit https://www.tutorialspoint.com/http/index.htm
    // for how to add data to it for uploading to ubodots.com visit https://ubidots.com/docs/api/#rest-api-reference
    String post_cpu_temperature_request = "POST /api/v1.6/devices/esp32_1/cpu-temperature/values/?token=";
    post_cpu_temperature_request += UBIDOT_TOKEN; // this the token
    post_cpu_temperature_request += " HTTP/1.1\r\n";
    post_cpu_temperature_request += "Host: things.ubidots.com\r\n";
    post_cpu_temperature_request += "Content-Type: application/json\r\n";
    post_cpu_temperature_request += "Content-Length: 18\r\n"; // content length of message string
    post_cpu_temperature_request += "\r\n";

    // add temperature value to HTTP request this si a json script
    post_cpu_temperature_request += "{\"value\": "; //this message string
    post_cpu_temperature_request += String(cpu_temp);
    post_cpu_temperature_request += "}\r\n"; // till here

    Serial.print("Posting temperature value to ubidots.com: ");
    Serial.println(post_cpu_temperature_request);

    // This will send the HTTP request to the server
    client.print(post_cpu_temperature_request);

    // if connection to sever is not available for 5 seconds then disconnect the server then
    unsigned long timeout = millis();
    while (client.available() == 0) {
        if (millis() - timeout > 5000) {
            Serial.println(">> Client Timeout !");
            client.stop();
            return;
        }
    }

    // Read all the lines of the reply from server and print them to Serial
    while(client.available()) {
        String line = client.readStringUntil('\r');
        Serial.print(line);
    }

    // disconnect the server
    client.stop();
    Serial.println();
    Serial.println("closing connection");
}

