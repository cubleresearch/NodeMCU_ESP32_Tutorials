/*
  Developed by Prashant Kumar - https://www.linkedin.com/in/prashant-kumar-21b012154/
  Project: IBMM | NodeMCU ESP32 Tutorials
  Embedded System and Robotics Research Group - https://electronicscu.wixsite.com/research
  
  GitHub - https://github.com/cubleresearch/NodeMCU_ESP32_Tutorials/tree/master/Projects
  YouTube - https://youtu.be/wnFf-Rnkfoo
  
  Upload date: 10 July 2018
  
  AWS Iot Core
  
  Board: MH-ET LIVE ESP32 DEVKIT - https://www.rhydolabz.com/wireless-wifi-c-130_134/nodemcu32-esp32-wireless-module-p-2495.html
  MCU: NodeMCU ESP32-WROOM-32 - https://www.espressif.com/en/esp-wroom-32/resources
  DHT11 - http://www.fingerpointengg.com/product/temperature-humidity-sensor-dht11/
  DHT Sensor Library - https://github.com/adafruit/DHT-sensor-library
  Hornbill-Examples - https://github.com/ExploreEmbedded/Hornbill-Examples
  aws-iot-device-sdk-embedded-C - https://github.com/aws/aws-iot-device-sdk-embedded-C
  MQTT - https://www.survivingwithandroid.com/2016/10/mqtt-protocol-tutorial.html
  AWS - https://aws.amazon.com/
  
  
  Reads temperature and humidity from DHT11 and publishes that data to AWS using MQTT Protocol.

  Note: 1)Connect VCC to 3.3V, GND to GND and DATA pin to pin specified in #define DHT_PIN of DHT11
        2)In this sketch Hornbill-Examples library is used for the handling of publishing data via MQTT
          Hornbill-Examples uses aws-iot-device-sdk-embedded-C library provided by AWS
          to interact with AWS.
          Use aws-iot-device-sdk-embedded-C library for creating your own code for handling transfer.
          Link for aws-iot-device-sdk-embedded-C is given above.
        3)DHT-sensor-library is used for reading data from DHT11
          Link is given above.
 */

#include<WiFi.h>
#include<DHT.h>
#include<AWS_IOT.h>

#define DHT_PIN 33 // pin connected to data pin of DHT11
#define DHT_TYPE DHT11

#define WIFI_SSID "Joker" // your wifi ssid
#define WIFI_PASSWD "password" //your wifi password

#define CLIENT_ID "Your_own_unique_id_klsdjiranwfla"// thing unique ID, can be any unique id
#define MQTT_TOPIC "your_update_mqtt_topic" //topic for the MQTT data
#define AWS_HOST "your_aws_host" // your host for uploading data to AWS,

DHT dht(DHT_PIN, DHT_TYPE);
AWS_IOT aws;

void setup(){
  Serial.begin(9600);
  Serial.print("\nInitializing Temp_Humidity_DHT11_0.....\n");

  Serial.print("\n  Initializing WIFI: Connecting to ");
  Serial.println(WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASSWD);
  Serial.print("  ");
  while(WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(500);
  }
  Serial.println("\n  Connected.\n  Done");

  Serial.print("\n  Initializing DHT11...");
  dht.begin();
  Serial.println("  Done.");

  Serial.println("\n  Initializing connetction to AWS....");
  if(aws.connect(AWS_HOST, CLIENT_ID) == 0){ // connects to host and returns 0 upon success
    Serial.println("  Connected to AWS\n  Done.");
  }
  else {
    Serial.println("  Connection failed!");
  }
  Serial.println("  Done.\n\nDone.\n");
}

void loop(){
  // read temperature and humidity
  float temp = dht.readTemperature(); // return temperature in °C
  float humidity = dht.readHumidity();// return humidity in %

  // check whether reading was successful or not
  if(temp == NAN || humidity == NAN){ // NAN means no available data
    Serial.println("Reading failed.");
  }
  else{
    //create string payload for publishing
    String temp_humidity = "Temperature: ";
    temp_humidity += String(temp);
    temp_humidity += " °C Humidity: ";
    temp_humidity += String(humidity);
    temp_humidity += " %";

    char payload[40];
    temp_humidity.toCharArray(payload, 40);

    Serial.println("Publishing:- ");
    Serial.println(payload);
     if(aws.publish(MQTT_TOPIC, payload) == 0){// publishes payload and returns 0 upon success
      Serial.println("Success\n");
    }
    else{
      Serial.println("Failed!\n");
    }
  }

  delay(1000);
}

