/*
  Developed by Prashant Kumar - https://www.linkedin.com/in/prashant-kumar-21b012154/
  Project: IBMM | NodeMCU ESP32 Tutorials
  Embedded System and Robotics Research Group - https://electronicscu.wixsite.com/research

  GitHub - https://github.com/cubleresearch/NodeMCU_ESP32_Tutorials
  YouTube - https://youtu.be/nx4ZrdLnW-w

  Upload date: 27 June 2018
  
  Serial Communication

  Board: MH-ET LIVE ESP32 DEVKIT
  MCU: NodeMCU ESP32-WROOM-32 - https://www.espressif.com/en/esp-wroom-32/resources
  Reference: https://www.arduino.cc/reference/en/language/functions/communication/serial/
  Blink: https://github.com/cubleresearch/NodeMCU_ESP32_Tutorials/blob/master/1_Blink.ino
  
  Reading and writing serial data to control inbuilt led.

  Note: in esp32 builtin led is connected to vcc
        so it turns on when it is set to LOW and off when set to HIGH
*/
char data[4];
void setup() {
  // begin serial communication
  Serial.begin(9600);// your own baud value
  Serial.println("\nInitialized serial communication.");// sends data to laptop via serial connection
  // to check your baud value
  //  1) go to device manager
  //  2) find your com port
  //  3) got to its properties to check its baud value

  Serial.println("Enter \"on\" or \"off\" to control on board inbuilt led");

  // set up inbuilt led
  pinMode(LED_BUILTIN, OUTPUT);

}

void loop() {
  // wait for the incoming data
  while(Serial.available() == 0);// available function return 0 when there is no data to read
  // and return integer value greater than 0 when there is data to read

  // read incoming data
  int i = 0;
  while(Serial.available() != 0){
    data[i++] = Serial.read();// reads serial data one byte each time it gets called
  }
  data[i] = '\0'; // specifies end of string
  Serial.print("Recieved: ");
  Serial.println(data);

  
  //turns on or off led. in esp32 builtin led is connected to vcc
  // so it turns on when it is set to LOW and off when set to HIGH
  if(strcmp(data, "on") == 0)digitalWrite(LED_BUILTIN, LOW);
  if(strcmp(data, "off") == 0)digitalWrite(LED_BUILTIN, HIGH);
}
