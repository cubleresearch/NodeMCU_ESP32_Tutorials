/*
  Developed by Prashant Kumar - https://www.linkedin.com/in/prashant-kumar-21b012154/
  Project: IBMM | NodeMCU ESP32 Tutorials
  Embedded System and Robotics Research Group - https://electronicscu.wixsite.com/research

  GitHub - https://github.com/cubleresearch/NodeMCU_ESP32_Tutorials
  YouTube - https://youtu.be/eDHIU5-mEkA

  Upload date: 11 July 2018
  
  Analog I/O

  Board: MH-ET LIVE ESP32 DEVKIT
  MCU: NodeMCU ESP32-WROOM-32 - https://www.espressif.com/en/esp-wroom-32/resources
  Reference_0: https://github.com/espressif/arduino-esp32/blob/master/cores/esp32/esp32-hal-ledc.h
  Reference_1: https://esp-idf.readthedocs.io/en/latest/api-reference/peripherals/ledc.html#
  
  Reads analog input value (12 bit precision)using one of the GPIO pins from a potentiometer
  and changes brightness of LED.
  
  NOTE: Connect external LED  +ve to GPIO 33 and -ve to GND
        Connect one of the side pin of potentiometer to 3.3V and another side pin to
        GND, connect middle pin to GPIO 32
 */

#define LED_CHANNEL 0 // channel to use for controlling brightness of LED
#define LED_PRECISION 12 // 12 bit precision for controlling brightness of LED Range:0 - 4095
#define LED_BASE_FREQUENCY 5000 // frequency of the signal
// to learn more about LED PWM control visit https://esp-idf.readthedocs.io/en/latest/api-reference/pdderipherals/ledc.html#

#define LED 33 // pin connected to +ve of LED
#define ANALOG_INPUT 32 // pin that will read anlog input

void setup() {
  Serial.begin(9600);
  Serial.print("\nInitializing LED brightness control...");
  
  // set up ledc configuration
  ledcSetup(LED_CHANNEL, LED_BASE_FREQUENCY, LED_PRECISION);
  ledcAttachPin(LED, LED_CHANNEL);// attach pin of LED to channel to pass PWM signal

  // set analog input pin 
  pinMode(ANALOG_INPUT, ANALOG);
  Serial.println("Done");
}

void loop() {
  // read analog input
  int value = analogRead(ANALOG_INPUT); // return analog input in range 0 - 4095 (default)
  Serial.print(value);
  float f = float(value) / 4095 * 100;
  Serial.print("  ");
  Serial.print(f);
  Serial.println(" %");

  //write duty cycle to LED
  ledcWrite(LED_CHANNEL, value);

  delay(100);
}
