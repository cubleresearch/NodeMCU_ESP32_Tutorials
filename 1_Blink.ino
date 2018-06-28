/*
  This example code is in the public domain.
  Developed by Prashant Kumar - https://www.linkedin.com/in/prashant-kumar-21b012154/
  Project: IBMM | NodeMCU ESP32 Tutorials
  Embedded System and Robotics Research Group - https://electronicscu.wixsite.com/research
  GitHub - https://github.com/cubleresearch/NodeMCU_ESP32_Tutorials
  YouTube - https://youtu.be/ZMk7B28F05A

  Upload date: 13 June 2018


  Blink

  MCU: NodeMCU ESP32-WROOM-32
  Turns on board LED on for one second, then off for one second, repeatedly.

  ESP32 have a on board LED. It is connected to pin 5 on ESP32-WROOM-32 with VCC.
  This sketch turns on and off on board LED for one second each repeatedly.

  SETUP
  1) Set pin mode of pin LED_BUILTIN(Pin 2) to OUTPUT

  LOOP
  1) Write HIGH to pin LED_BUILTIN
  2) Delay execution for 1000ms
  3) Write LOW to pin LED_BUILTIN
  4) Delay execution for 1000ms

  NOTE: If you connect an external LED to pin 2 external LED will glow in alternate to on board LED because on board LED is connected to VCC
*/

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  
  pinMode(LED_BUILTIN, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level(3.3V))
  delay(1000);                       // wait for a second
  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW(0 V)
  delay(1000);                       // wait for a second

  // Increase or decrease argument passed to delay() to change the frequency of LED turning on and off
}
