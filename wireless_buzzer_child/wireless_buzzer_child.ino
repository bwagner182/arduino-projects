#include <RCSwitch.h>
#include <CuteBuzzerSounds.h>

int ledg = 3;          // the PWM pin the green pin LED is attached to
int ledb = 5;          // the PWM pin the blue pin LED is attached to
int ledr = 6;          // the PWM pin the red pin LED is attached to
int brightnessR = 20;    // how bright the LED is
int brightnessG = 20;    // how bright the LED is
int brightnessB = 20;    // how bright the LED is
int fadeAmount = 5;    // how many points to fade the LED by

long buttonA = 209761;
long buttonB = 209762;
long buttonC = 209764;
long buttonD = 209768;

#define BUZZER_PIN 7

RCSwitch mySwitch = RCSwitch();

void setup() 
{
  pinMode(ledg, OUTPUT);
  pinMode(ledb, OUTPUT);
  pinMode(ledr, OUTPUT);

  Serial.begin(9600);
  mySwitch.enableReceive(0); // receiver on interrupt 0 or Pin D2
  mySwitch.enableTransmit(10);
  cute.init(BUZZER_PIN);
  cute.play(S_CONNECTION);
}


void loop() 
{

  analogWrite(ledg, brightnessG);
  analogWrite(ledb, brightnessB);
  analogWrite(ledr, brightnessR);
  if (mySwitch.available()) {
    long button = mySwitch.getReceivedValue();
  
    if (button == buttonA) {
      brightnessR = 255;;
      brightnessG = 255;
      brightnessB = 255;
      cute.play(S_FART1);
    } else if (button == buttonB && brightnessR > 0) {
      brightnessR = brightnessR - fadeAmount; 
    } else if (button == buttonC && brightnessG > 0) {
      brightnessG = brightnessG - fadeAmount;
    } else if (button ==  buttonD && brightnessB > 0) {
      brightnessB = brightnessB - fadeAmount;
    } else if ( button == 2 ) {
      brightnessR = 0;
      brightnessG = 0;
      brightnessB = 0;
    } else if (button = 1) {
      brightnessR = 100;
      brightnessG = 255;
      brightnessB = 85; 
    } else {
      Serial.println("There was an error");

      Serial.print("Received ");
      Serial.print( mySwitch.getReceivedValue() );
      Serial.print(" / ");
      Serial.print( mySwitch.getReceivedBitlength() );
      Serial.print("bit ");
      Serial.print("Protocol: ");
      Serial.println( mySwitch.getReceivedProtocol() );
    }
  
    Serial.print("Red: ");
    Serial.print( brightnessR );
    Serial.print(" / ");
    Serial.print( "Green: ");
    Serial.print(brightnessG);
    Serial.print(" / ");
    Serial.print("Blue: ");
    Serial.println( brightnessB );
  
    mySwitch.resetAvailable();
  }
  delay(30);
}
