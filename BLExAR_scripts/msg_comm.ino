/***************************************************************************
* Arduino MakerBLE Message Transfer with BLExAR 
*  -- sending text back and forth between BLExAR app
*
* 
*  by Josh Hrisko | Maker Portal LLC (c) 2022
* 
* 
***************************************************************************/

#include <ArduinoBLE.h>

// Full list of Arduino BLE commands: https://github.com/arduino-libraries/ArduinoBLE
BLEService myService("0000ffe0-0000-1000-8000-00805f9b34fb"); // service characteristic
BLECharacteristic myCharacteristic("EFE5", BLERead | BLEWrite | BLENotify ,0x20);

int ledArray[] = {11,12,13}; // onboard LED array
int ledCount = sizeof (ledArray) / sizeof (ledArray[0]); // for looping through LEDs

void setup() {
  Serial.begin(115200); // start serial monitor
  
  if (!BLE.begin()) { // wait for BLE to start
    Serial.println("failed to initialize BLE!"); 
    while (1);
  }

  myService.addCharacteristic(myCharacteristic); // add characteristic to service
  BLE.addService(myService); // add service to BLE dev

  // Build scan response data packet
  BLEAdvertisingData scanData;
  scanData.setLocalName("MakerBLE"); // set BLE name
  BLE.setDeviceName("MakerBLE"); // set BLE name
  BLE.setScanResponseData(scanData); // set name for scanners
  myCharacteristic.setEventHandler(BLEWritten, WriteCharacteristicHandler); // add written handler
  BLE.advertise(); // advertise BLE from MakerBLE board
}

void loop() {
  BLE.poll(); // startup BLE listening
}

void WriteCharacteristicHandler(BLEDevice central, BLECharacteristic characteristic) {
  String inputText = (char*)myCharacteristic.value(); // read incoming data
  inputText.remove(inputText.length()-1,2); // remove endline characters

  Serial.println("Input Text: "+inputText); // print message sent from BLExAR App
  
  if (inputText.equals("red")) { // if RX red — turn on red LED
    ledFunc(0); // red LED on
  } else if (inputText.equals("green")) {
    ledFunc(1); // green LED on
  } else if (inputText.equals("blue")) {
    ledFunc(2); // blue LED on
  }
  writeBLE("Hi, BLExAR. This is MakerBLE!"); // send response to BLExAR
}

void writeBLE(String message){
  byte plain[message.length()]; // message buffer
  message.getBytes(plain, message.length()); // convert to bytes
  myCharacteristic.writeValue(plain,message.length()); // write to BLE
}

void ledFunc(int pinNum){ // function for stepping through LEDs 
  for (int ii=0;ii<ledCount;ii++){
    if (pinNum==ii){
      digitalWrite(ledArray[ii],LOW); // LOW turns LED on
    } else {
      digitalWrite(ledArray[ii],HIGH); // HIGH turns LED off
    }
  }
}
