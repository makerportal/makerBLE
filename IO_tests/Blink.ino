/***************************************************************************
* Arduino MakerBLE RGB LED Blinking
*  -- blinking onboard RGB LED to test the MakerBLE board
*
* 
*  by Josh Hrisko | Maker Portal LLC (c) 2022
* 
* 
***************************************************************************/

void setup() {
  pinMode(LEDR, OUTPUT); // LEDR, LEDG, LEDB work with MakerBLE
}

void loop() {
  digitalWrite(LEDR, HIGH);   // turn the LED on (LOW is 'on' for onboard RGB)
  delay(500);                       // wait for 500 ms
  digitalWrite(LEDR, LOW);    // turn the LED off (HIGH is 'off' for onboard RGB)
  delay(500);                       // wait for 500 ms
}
