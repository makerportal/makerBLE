/***************************************************************************
* Transmitting BMP280 to BLExAR iOS App using the MakerBLE Board
*  -- gathering pressure, temperature, and altitude simply and efficiently
*
* 
*  by Josh Hrisko | Maker Portal LLC (c) 2022
* 
* 
***************************************************************************/

#include <Adafruit_BMP280.h>
#include <ArduinoBLE.h>

// Full list of Arduino BLE commands: https://github.com/arduino-libraries/ArduinoBLE
BLEService myService("0000ffe0-0000-1000-8000-00805f9b34fb"); // BLExAR BLE service
BLECharacteristic myCharacteristic("FFE1", BLEWrite | BLENotify,0x10); 

Adafruit_BMP280 bmp; // BMP280 I2C device
long previousMillis = 0;  // last time the BMP280 was read, in [millisec]
int TXdelay = 500; // delay between sends

void setup() {
  if (!BLE.begin()) {
    while (1); // wait for BLE
  }

  if (!bmp.begin(BMP280_ADDRESS_ALT, BMP280_CHIPID)) {
    while (1) delay(50); // wait for BMP280 sensor to start/settle
  }

  // Build scan response data packet
  myService.addCharacteristic(myCharacteristic); // add BLE characteristic
  BLE.addService(myService); // add BLE service
  BLEAdvertisingData scanData;
  scanData.setLocalName("MakerBLE"); // set name
  BLE.setDeviceName("MakerBLE"); // set name

  BLE.setScanResponseData(scanData);// set data for scanners (BLE apps)
  BLE.advertise(); // advertise BLE device
  
  // raw values for fastest response times
  // full list: https://github.com/adafruit/Adafruit_BMP280_Library/blob/master/keywords.txt
  bmp.setSampling(Adafruit_BMP280::MODE_FORCED,   // Operating Mode
                  Adafruit_BMP280::SAMPLING_X1,   // Temp. oversampling
                  Adafruit_BMP280::SAMPLING_X1,   // Pressure oversampling
                  Adafruit_BMP280::FILTER_OFF,    // Filtering
                  Adafruit_BMP280::STANDBY_MS_1); // Standby time
}

void loop() {
  // wait for a BLE central connection
  BLEDevice central = BLE.central();

  // if a central is connected to the peripheral:
  if (central) {
    while (central.connected()) {
      long currentMillis = millis();
      if (currentMillis - previousMillis >= TXdelay) { // send after delay
        previousMillis = currentMillis;
        writeBMP280Str(); // write data
      }
    }
  }
}

void writeBMP280Str(){
    if (bmp.takeForcedMeasurement()) { // check for valid data measurement

    float temp, pres, alti; // pre-alloc for measurements 
    String strToPrint = ""; // string to print 

    temp = bmp.readTemperature(); // read temperature [C]
    pres = bmp.readPressure()/100.0; // read pressure [hPa]
    alti = bmp.readAltitude(1017.5); // approx altitude [m], with hPa pressure ref. at sea level

//    strToPrint+= String(temp,2); strToPrint+=","; // uncomment to send temp
//    strToPrint+= String(pres,2); strToPrint+=","; // uncomment to send pressure
    strToPrint+= String(alti,2); strToPrint+="\n"; // uncomment to send altitude

    writeBLE(strToPrint); // send string over BLE
  } else {
    writeBLE("Invalid BMP280 Reading");
  }
}


void writeBLE(String message){
  byte plain[message.length()]; // message buffer
  message.getBytes(plain, message.length()); // convert to bytes
  myCharacteristic.writeValue(plain,message.length()); // writing to BLE
}
