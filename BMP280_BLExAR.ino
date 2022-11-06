/***************************************************************************
* Arduino BMP280 BLExAR iOS Serial Plotter
*  -- gathering pressure, temperature, and altitude simply and efficiently
*
* 
*  by Josh Hrisko | Maker Portal LLC (c) d2022
* 
* 
***************************************************************************/

#include <Adafruit_BMP280.h>
#include <ArduinoBLE.h>

BLEService myService("0000ffe0-0000-1000-8000-00805f9b34fb"); // BLExAR BLE service
BLECharacteristic myCharacteristic("FFE1", 
                        BLERead | BLEBroadcast | BLEWrite | BLENotify,0x10); 

Adafruit_BMP280 bmp; // BMP280 I2C device
long previousMillis = 0;  // last time the BMP280 was read, in [millisec]

void setup() {
  if (!BLE.begin()) {
    while (1);
  }

  if (!bmp.begin(BMP280_ADDRESS_ALT, BMP280_CHIPID)) {
    while (1) delay(50);
  }

  // Build scan response data packet
  myService.addCharacteristic(myCharacteristic);
  BLE.addService(myService);
  BLEAdvertisingData scanData;
  scanData.setLocalName("makerBLE");
  BLE.setDeviceName("makerBLE");
  // Copy set parameters in the actual scan response packet
  BLE.setScanResponseData(scanData);
  myCharacteristic.setEventHandler(BLERead, switchCharacteristicWritten);
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
      if (currentMillis - previousMillis >= 1000) {
        previousMillis = currentMillis;
        writeBMP280Str();
      }
    }
  }
}

void switchCharacteristicWritten(BLEDevice central, BLECharacteristic characteristic) {
  writeBMP280Str();
}

void writeBMP280Str(){
    if (bmp.takeForcedMeasurement()) { // check for valid data measurement

    float temp, pres, alti; // pre-alloc for measurements 
    String strToPrint = ""; // string to print 

    temp = bmp.readTemperature();
    pres = bmp.readPressure()/1000.0;
    alti = bmp.readAltitude(1017.5);

//    strToPrint+= String(temp,2); strToPrint+=","; 
//    strToPrint+= String(pres,2); strToPrint+=",";
    strToPrint+= String(alti,2); strToPrint+="\n";

    writeBLE(strToPrint);
  } else {
    writeBLE("Invalid BMP280 Reading");
  }
}


void writeBLE(String message){
  byte plain[message.length()];
  message.getBytes(plain, message.length());
  myCharacteristic.writeValue(plain,message.length());
}
