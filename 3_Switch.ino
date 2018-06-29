
/*
  Developed by Prashant Kumar - https://www.linkedin.com/in/prashant-kumar-21b012154/
  Project: IBMM | NodeMCU ESP32 Tutorials
  Embedded System and Robotics Research Group - https://electronicscu.wixsite.com/research

  GitHub - https://github.com/cubleresearch/NodeMCU_ESP32_Tutorials
  YouTube - https://youtu.be/T_ONOh13_VM

  Upload date: 29 June 2018
  Switch

  Board: MH-ET LIVE ESP32 DEVKIT
  MCU: NodeMCU ESP32-WROOM-32 - https://www.espressif.com/en/esp-wroom-32/resources
  Reference: https://www.arduino.cc/reference/en/
  PULLUP and PULLDOWN: https://www.electronics-tutorials.ws/logic/pull-up-resistor.html
  Switch Debouncing: http://www.labbookpages.co.uk/electronics/debounce.html
  
  Reads input value using one of the GPIO pins from a switch and turns a led on and off.
  Switch debouncing is handlled as well as input floating value when circuit is open.

  NOTE: Connect external LED  +ve to pin 27 and -ve to GND
        Connect one pin of button to pin 13 and other to 3V3 pin
*/
#include<esp32-hal-gpio.h>

#define SWITCH 13 // pin which will be used as input
#define LED 27

bool led_status = LOW;
bool switch_last_status = LOW;

void setup() {
  // begin serial communication
  Serial.begin(9600);
  Serial.println("\nInitializing switch....");

  //setting up led pin
  pinMode(LED, OUTPUT);
  digitalWrite(LED, led_status);
  Serial.printf("LED: %s\nDone", led_status == HIGH ? "ON" : "OFF");

  // setting pin to INPUT mode
  pinMode(SWITCH, INPUT); // sets pin to INPUT mode
  pinMode(SWITCH, PULLDOWN);// sets pin default input value to LOW
  // when button is not pressed (i.e. circuit is open) then reading input value returns 
  // randomly HIGH or LOW, above statement sets pin to LOW when circuit is open
  // use PULLUP to set HIGH as default 
  // visit https://www.electronics-tutorials.ws/logic/pull-up-resistor.html
  // to learn more about pullup and pulldown resistors
}
bool readSwitch(){
  // handles switch debouncing
  //returns HIGH or LOW
  // when a button is pressed it doesn't just cloes or opens  the circuit
  // it debounces for a very short time (for millisecond)
  // visit http://www.labbookpages.co.uk/electronics/debounce.html to learn about switch debouncing
  // so program needs to stop for a while and read again 
  // the new read value will be the correct value
  bool switch_current_status = digitalRead(SWITCH);// read input
  if(switch_current_status != switch_last_status){
    delay(5);// pausing for 5 millisecons is enough
    switch_current_status = digitalRead(SWITCH);// read again
  }
  return switch_current_status;
}
void loop() {
  // read input
  bool switch_current_status = readSwitch();
  // when input value goes LOW to HIGH means switch was pressed
  if(switch_last_status == LOW && switch_current_status == HIGH){
    // invert the led status 
    if(led_status == LOW) digitalWrite(LED, HIGH);
    else digitalWrite(LED, LOW);
    led_status = !led_status;
    Serial.printf("Switch was pressed:- Time: %d LED: %s\n", millis(), led_status == HIGH ? "ON" : "OFF");
  }
  switch_last_status = switch_current_status;
}
