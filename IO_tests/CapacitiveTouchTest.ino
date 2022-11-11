/***************************************************************************
* Arduino MakerBLE Capacitive Touch 
*  -- gathering capacitive touch data and analyzing it
*
* 
*  by Josh Hrisko | Maker Portal LLC (c) 2022
* 
* 
***************************************************************************/

int sensorVal = 0;  // variable to store the value coming from the sensor
int sensorPin = 0; // digital pin #0 to read
int touchCount = 0; // for multi-touch analysis
int ledArray[] = {LEDR,LEDG,LEDB}; // onboard LED array
int ledCount = sizeof (ledArray) / sizeof (ledArray[0]);

void setup() {
  Serial.begin(115200); // start serial monitor
  for (int ii=0;ii<ledCount;ii++){
    pinMode(ledArray[ii], OUTPUT); // LED output 
    digitalWrite(ledArray[ii],HIGH); // turn off all LEDs to start
  }
}

void loop() {
  // read value from the sensor:
  sensorVal = digitalRead(sensorPin);
  
  if (sensorVal==1){
    touchCount+=1;
    pinFunc(touchCount-1); // change LED color based on touch count
    Serial.println("Touch Count: "+String(touchCount)); // print touch count
    if (touchCount>ledCount){
      touchCount = 0; // set touches to 0 if over LED count
    }
  }
  delay(100);
}

void pinFunc(int pinNum){ // function for stepping through LED 
  for (int ii=0;ii<ledCount;ii++){
    if (pinNum==ii){
      digitalWrite(ledArray[ii],LOW); // LOW turns LED on
    } else {
      digitalWrite(ledArray[ii],HIGH); // HIGH turns LED off
    }
  }
}
