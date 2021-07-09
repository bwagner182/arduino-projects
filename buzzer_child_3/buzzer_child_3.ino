#include <RCSwitch.h>
#include <CuteBuzzerSounds.h>

int ledg = 3;          // the PWM pin the green pin of the LED is attached to
int ledb = 5;          // the PWM pin the blue pin of the LED is attached to
int ledr = 6;          // the PWM pin the red pin of the LED is attached to

int brightnessR = 20;    // how bright the LED is
int brightnessG = 20;    // how bright the LED is
int brightnessB = 20;    // how bright the LED is

// Remote buttons
long buttonA = 209761;
long buttonB = 209762;
long buttonC = 209764;
long buttonD = 209768;

int buttonPin = 9;

#define BUZZER_PIN 7

int playerAvailable = 1;
int buttonState = 0;

RCSwitch mySwitch = RCSwitch();

void setup() 
{
  pinMode(ledg, OUTPUT);
  pinMode(ledb, OUTPUT);
  pinMode(ledr, OUTPUT);

  pinMode(buttonPin, INPUT);

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
    long rec = mySwitch.getReceivedValue();
  
    if (rec == buttonA) {
      playerAvailable = 1;
    } else if (rec == buttonC) {
      playerAvailable = 1;
    } else if ( rec == 13 ) {
      brightnessR = 255;
      brightnessG = 255;
      brightnessB = 25;
    } else if (rec == 11 || rec == 12 || rec == 14) {
      // turn lights off
      brightnessR = 0;
      brightnessG = 0;
      brightnessB = 0;     
    } else if (rec == 23) {
      playerAvailable = 0;
      brightnessR = 255;
      brightnessG = 255;
      brightnessB = 25;
    } else if (rec == 21 || rec == 22 || rec == 24 || rec == buttonB) {
      // do nothing
      
    } else {
      Serial.println("Received unknown signal");

      Serial.print("Received ");
      Serial.print( mySwitch.getReceivedValue() );
      Serial.print(" / ");
      Serial.print( mySwitch.getReceivedBitlength() );
      Serial.print("bit ");
      Serial.print("Protocol: ");
      Serial.println( mySwitch.getReceivedProtocol() );
    }
  
    mySwitch.resetAvailable();
  }

  buttonState = digitalRead(buttonPin);

  if (buttonState != 0 && playerAvailable == 1) {
    mySwitch.send(3, 24);
  } else if (buttonState != 0 && playerAvailable == 0) {
    cute.play(S_DISCONNECTION);
  }
  
  delay(30);
}
