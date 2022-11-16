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
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LEDR, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(LEDR, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);                       // wait for a second
  digitalWrite(LEDR, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);                       // wait for a second
}
