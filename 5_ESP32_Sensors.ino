/*
  Developed by Prashant Kumar - https://www.linkedin.com/in/prashant-kumar-21b012154/
  Project: IBMM | NodeMCU ESP32 Tutorials
  Embedded System and Robotics Research Group - https://electronicscu.wixsite.com/research

  GitHub - https://github.com/cubleresearch/NodeMCU_ESP32_Tutorials
  YouTube - https://youtu.be/j43XVp0r6yE

  Upload date: 12 July 2018
  
  ESP32 Sensors

  Board: MH-ET LIVE ESP32 DEVKIT - https://www.rhydolabz.com/wireless-wifi-c-130_134/nodemcu32-esp32-wireless-module-p-2495.html
  MCU: NodeMCU ESP32-WROOM-32 - https://www.espressif.com/en/esp-wroom-32/resources

  References:-
  https://github.com/espressif/arduino-esp32/blob/master/cores/esp32/esp32-hal.h
  https://github.com/espressif/arduino-esp32/blob/master/cores/esp32/esp32-hal-dac.h
  https://github.com/espressif/arduino-esp32/blob/master/cores/esp32/esp32-hal-touch.h
  
  
  Reads value from inbuilt sensors (Hall, Temperature, Touch).
  
  NOTE: Temperature Sensor is internal and reads CPU temperature.
        Connect wire to pin 4 for Touch Sensor.
 */
void setup() {
  // Begin Serial Communication
  Serial.begin(9600);
  Serial.println("Initializing ESP32......Done");
}

void loop() {
  // read CPU Temperature
  Serial.print("\nCPU Temp.: ");
  Serial.print(temperatureRead()); //returns CPU Temperature in ºC
  Serial.print(" ºC");

  //read Hall Sensor value
  Serial.print(" Hall: ");
  Serial.print(hallRead()); // reads Hall sensor

  //read touch sensor
  Serial.print(" Touch: ");
  Serial.print(touchRead(4));// reads touch sensor on pin 4

  delay(1000);
}
