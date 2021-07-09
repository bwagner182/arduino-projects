int ledg = 3;          // the PWM pin the green pin LED is attached to
int ledb = 5;          // the PWM pin the blue pin LED is attached to
int ledr = 6;          // the PWM pin the red pin LED is attached to
int brightness = 0;    // how bright the LED is
int fadeAmount = 5;    // how many points to fade the LED by


void setup() 
{
  pinMode(ledg, OUTPUT);
  pinMode(ledb, OUTPUT);
  pinMode(ledr, OUTPUT);
}


void loop() 
{
  
  analogWrite(ledg, brightness);
  analogWrite(ledb, brightness);
  analogWrite(ledr, brightness);

  // change the brightness for next time through the loop:
  brightness = brightness + fadeAmount;

  // reverse the direction of the fading at the ends of the fade:
  if (brightness <= 0 || brightness >= 255) {
    fadeAmount = -fadeAmount;
  }
  // wait for 30 milliseconds to see the dimming effect
  delay(30);

  
}
