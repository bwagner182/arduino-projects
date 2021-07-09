#include <RCSwitch.h>
#include <CuteBuzzerSounds.h>
//Install the RCSwitch and CuteledSounds libraries from the Arduino Library Manager
int led1Red = 46;
int led1Green = 45;
int led1Blue = 44;

int led2Red = 3;
int led2Green = 4;
int led2Blue = 5;

int led3Red = 6;
int led3Green = 7;
int led3Blue = 8;

int led4Red = 9;
int led4Green = 10;
int led4Blue = 11;

#define BUZZER_PIN 12

int pinkAvail = 0;
int yellowAvail = 0;
int ltBlueAvail = 0;
int blueAvail = 0;

int state = -1;
//states
// -1 = booted
//  0 = Asking the question, if they buzz in, they're locked out
//  1 = Available people can buzz in


RCSwitch mySwitch = RCSwitch();
unsigned long rec;

void setup() {
  Serial.begin(9600);
  cute.init(BUZZER_PIN);
  Serial.println("booted");
  cute.play(S_CONNECTION);
  mySwitch.enableTransmit(10);
  mySwitch.enableReceive(0);  // Receiver on interrupt 4 => that is pin #2?    
  
  pinMode(led1Red, OUTPUT);
  pinMode(led1Green, OUTPUT);
  pinMode(led1Blue, OUTPUT);
  
  pinMode(led2Red, OUTPUT);
  pinMode(led2Green, OUTPUT);
  pinMode(led2Blue, OUTPUT);
  
  pinMode(led3Red, OUTPUT);
  pinMode(led3Green, OUTPUT);
  pinMode(led3Blue, OUTPUT);
  
  pinMode(led4Red, OUTPUT);
  pinMode(led4Green, OUTPUT);
  pinMode(led4Blue, OUTPUT);

  // pink - Player 1
  analogWrite(led1Red, 255);
  analogWrite(led1Green, 75);
  analogWrite(led1Blue, 225);

  // light blue - Player 2
  analogWrite(led2Red, 50);
  analogWrite(led2Green, 85);
  analogWrite(led2Blue, 255);

  // yellow - Player 3
  analogWrite(led3Red, 255);
  analogWrite(led3Green, 255);
  analogWrite(led3Blue, 25);

  // blue - Player 4
  analogWrite(led4Red, 0);
  analogWrite(led4Green, 0);
  analogWrite(led4Blue, 255);

  // blinkToGo();
  // resetLeds();
}


void loop() {

  // //Comment this in if you want to scroll through all the available sounds
  // //And see their names printed on the serial monitor
  // soundSamples();

  // Serial.print(state);
  if (mySwitch.available()) {
    rec = mySwitch.getReceivedValue();

    // //Comment in this section to see ALL 433mhz stuff coming in
    Serial.print("Received ");
    Serial.print( mySwitch.getReceivedValue() );
    Serial.print(" / ");
    Serial.print( mySwitch.getReceivedBitlength() );
    Serial.print("bit ");
    Serial.print("Protocol: ");
    Serial.println( mySwitch.getReceivedProtocol() );
    //End Debug Section


//This section of stuff deals with the leader's remote and will work regardless of the "state" of the game
    if(rec == 209761){ //button a resets everything
      pinkAvail = 1;
      yellowAvail = 1;
      ltBlueAvail = 1;
      blueAvail = 1;
      analogWrite(led1Red,230);
      analogWrite(led2Red,230);
      analogWrite(led3Red,230);
      analogWrite(led4Red,230);
      
      analogWrite(led1Green, 115);
      analogWrite(led2Green, 115);
      analogWrite(led3Green, 115);
      analogWrite(led4Green, 115);
      
      analogWrite(led1Blue, 0);
      analogWrite(led2Blue, 0);
      analogWrite(led3Blue, 0);
      analogWrite(led4Blue, 0);
      
      state = 0;
      cute.play(S_MODE3);
      Serial.println("State set to 0. Everything reset.");
    }

    if(rec == 209762){ //button b - Buttons unlocked!
      state = 1;

      //This function resets the LEDs. Take a look at it at the bottom of the code.
      resetLeds();
      
      //if you comment in the line below, the lights will blink 3 times showing the contestants that it
      //is ok to buzz in. This is 2 places in the code.
      blinkToGo();

      
      cute.play(S_HAPPY_SHORT);
        Serial.println("State set to 1. Locked and Loaded!");
    }

    if(rec == 209764){ //button c - Let the remaining people try!!

      //if you comment in the line below, the lights will blink 3 times showing the contestants that it
      //is ok to buzz in. This is 2 places in the code.
      blinkToGo();

      //This function resets the LEDs. Take a look at it at the bottom of the code.
      resetLeds();
      state = 1;
      cute.play(S_HAPPY_SHORT);
        Serial.println("State set to 1. Available Players can buzz!!!");
    }


// End Leader Remote Section

    if(state == 0){

      //In this state we want to lock out anyone who pushes the button early
      if(rec == 1){ // pink - Player 1
        cute.play(S_FART1);
        pinkAvail = 0; //make the button unavailable
        mySwitch.send(21, 24);
        analogWrite(led1Red, 0); //turn the led off
        analogWrite(led1Green, 0);
        analogWrite(led1Blue, 0);
        
        Serial.println("pink disabled");
      }

      if(rec == 2){ // lt Blue - Player 2
        cute.play(S_FART1);
        ltBlueAvail = 0;
        mySwitch.send(22, 24);
        analogWrite(led2Red, 0);
        analogWrite(led2Green, 0);
        analogWrite(led2Blue, 0);
        
        Serial.println("light blue disabled");
      }

      if(rec == 3){ // yellow - Player 3
        cute.play(S_FART1);
        yellowAvail = 0;
        mySwitch.send(23, 24);
        analogWrite(led3Red, 0);
        analogWrite(led3Green, 0);
        analogWrite(led3Blue, 0);
        
        Serial.println("green disabled");
      }

      if(rec == 4){ // blue - Player 4
        cute.play(S_FART1);
        blueAvail = 0;
        mySwitch.send(24, 24);
        analogWrite(led4Red, 0);
        analogWrite(led4Green, 0);
        analogWrite(led4Blue, 0);
        
        Serial.println("blue disabled");
      }
    }

    if(state == 1){
      //In this state we want to allow users to buzz in 
      if(rec == 1 && pinkAvail == 1){ //pink - Player 1
        pinkAvail = 0; //make the button unavailable
        mySwitch.send(11, 24);
        cute.play(S_BUTTON_PUSHED);
        // pink
        analogWrite(led1Red, 0);
        analogWrite(led1Green, 0);
        analogWrite(led1Blue, 0);
        delay(100);
        // pink
        analogWrite(led1Red, 255);
        analogWrite(led1Green, 75);
        analogWrite(led1Blue, 225); //turn the led on
        
        analogWrite(led2Red, 0);
        analogWrite(led2Green, 0);
        analogWrite(led2Blue, 0);

        analogWrite(led3Red, 0);
        analogWrite(led3Green, 0);
        analogWrite(led3Blue, 0);

        analogWrite(led4Red, 0);
        analogWrite(led4Green, 0);
        analogWrite(led4Blue, 0);
        Serial.println("red buzzed in");

        state = 2;
      }

      if(rec == 2  && ltBlueAvail == 1){ //light blue - Player 2
        ltBlueAvail = 0;
        mySwitch.send(12, 24);
        cute.play(S_BUTTON_PUSHED);
        analogWrite(led2Red, 0);
        analogWrite(led2Green, 0);
        analogWrite(led2Blue, 0);
        delay(100);
        // light blue
        analogWrite(led2Red, 50);
        analogWrite(led2Green, 85);
        analogWrite(led2Blue, 255);
        
        analogWrite(led1Red, 0);
        analogWrite(led1Green, 0);
        analogWrite(led1Blue, 0);

        analogWrite(led3Red, 0);
        analogWrite(led3Green, 0);
        analogWrite(led3Blue, 0);
    
        analogWrite(led4Red, 0);
        analogWrite(led4Green, 0);
        analogWrite(led4Blue, 0);
        Serial.println("light blue buzzed in");
        state = 2;
      }

      if(rec == 3  && yellowAvail == 1){ //yellow - Player 3
        yellowAvail = 0;
        mySwitch.send(13, 24);
        cute.play(S_BUTTON_PUSHED);
        analogWrite(led3Red, 0);
        analogWrite(led3Green, 0);
        analogWrite(led3Blue, 0);
        
        delay(100);

        // yellow
        analogWrite(led3Red, 255);
        analogWrite(led3Green, 255);
        analogWrite(led3Blue, 25);

        analogWrite(led1Red, 0);
        analogWrite(led1Green, 0);
        analogWrite(led1Blue, 0);
    
        analogWrite(led2Red, 0);
        analogWrite(led2Green, 0);
        analogWrite(led2Blue, 0);
    
        analogWrite(led4Red, 0);
        analogWrite(led4Green, 0);
        analogWrite(led4Blue, 0);

        Serial.println("yellow buzzed in");
        state = 2;
      }

      if(rec == 4 && blueAvail == 1){ //blue - Player 4
        blueAvail = 0;
        mySwitch.send(14, 24);
        cute.play(S_BUTTON_PUSHED);
        analogWrite(led4Red, 0);
        analogWrite(led4Green, 0);
        analogWrite(led4Blue, 0);
        delay(100);
        analogWrite(led4Red, 0);
        analogWrite(led4Green, 0);
        analogWrite(led4Blue, 255);
        
        analogWrite(led1Red, 0);
        analogWrite(led1Green, 0);
        analogWrite(led1Blue, 0);
    
        analogWrite(led2Red, 0);
        analogWrite(led2Green, 0);
        analogWrite(led2Blue, 0);
    
        analogWrite(led3Red, 0);
        analogWrite(led3Green, 0);
        analogWrite(led3Blue, 0);
    
        Serial.println("blue buzzed in");
        state = 2;
      }
    }


    }
    mySwitch.resetAvailable();

}

void blinkToGo(){
  for (int i = 0; i < 3; i++) {
    // pink
    analogWrite(led1Red, 255);
    analogWrite(led1Green, 75);
    analogWrite(led1Blue, 225);

    // light blue
    analogWrite(led2Red, 50);
    analogWrite(led2Green, 85);
    analogWrite(led2Blue, 255);

    // yellow
    analogWrite(led3Red, 255);
    analogWrite(led3Green, 255);
    analogWrite(led3Blue, 25);

    // blue
    analogWrite(led4Red, 0);
    analogWrite(led4Green, 0);
    analogWrite(led4Blue, 255);
    
    delay(100);

    analogWrite(led1Red, 0);
    analogWrite(led1Green, 0);
    analogWrite(led1Blue, 0);

    analogWrite(led2Red, 0);
    analogWrite(led2Green, 0);
    analogWrite(led2Blue, 0);

    analogWrite(led3Red, 0);
    analogWrite(led3Green, 0);
    analogWrite(led3Blue, 0);

    analogWrite(led4Red, 0);
    analogWrite(led4Green, 0);
    analogWrite(led4Blue, 0);
    
    delay(100);
  }
}
void soundSamples(){
  //Sound examples
  //Comment this section in to hear all the sounds and see the names on the serial monitor
  Serial.println("HAPPY");
    cute.play(S_HAPPY);
  delay(500);
  Serial.println("CONNECTION");
    cute.play(S_CONNECTION);
  delay(500);
  Serial.println("DISCONNECTION");
    cute.play(S_DISCONNECTION);
  delay(500);
  Serial.println("BUTTON_PUSHED");
    cute.play(S_BUTTON_PUSHED);
  delay(500);
  Serial.println("MODE1");
    cute.play(S_MODE1);
  delay(500);
  Serial.println("MODE2");
    cute.play(S_MODE2);
  delay(500);
  Serial.println("MODE3");
    cute.play(S_MODE3);
  delay(500);
  Serial.println("SURPRISE");
    cute.play(S_SURPRISE);
  delay(500);
  Serial.println("OHOOH");
    cute.play(S_OHOOH);
  delay(500);
  Serial.println("OHOOH2");
    cute.play(S_OHOOH2);
  delay(500);
  Serial.println("CUDDLY");
    cute.play(S_CUDDLY);
  delay(500);
  Serial.println("SLEEPING");
    cute.play(S_SLEEPING);
  delay(500);
  Serial.println("HAPPY");
    cute.play(S_HAPPY);
  delay(500);
  Serial.println("SUPER_HAPPY");
    cute.play(S_SUPER_HAPPY);
  delay(500);
  Serial.println("HAPPY_SHORT");
    cute.play(S_HAPPY_SHORT);
  delay(500);
  Serial.println("SAD");
    cute.play(S_SAD);
  delay(500);
  Serial.println("CONFUSED");
    cute.play(S_CONFUSED);
  delay(500);
  Serial.println("FART1");
    cute.play(S_FART1);
  delay(500);
  Serial.println("FART2");
    cute.play(S_FART2);
  delay(500);
  Serial.println("FART3");
    cute.play(S_FART3);
  delay(500);
}

void resetLeds(){
  //Let's reset the LEDs. If someone has already buzzed in, we want their LED to be off
  //If they haven't, we want it to be on.
  if(pinkAvail == 1){
    analogWrite(led1Red, 255);
    analogWrite(led1Green, 75);
    analogWrite(led1Blue, 225);
  }else{
    analogWrite(led1Red, 0);
    analogWrite(led1Green, 0);
    analogWrite(led1Blue, 0);
  }

  if(ltBlueAvail == 1){
    analogWrite(led2Red, 50);
    analogWrite(led2Green, 85);
    analogWrite(led2Blue, 255);
  }else{
    analogWrite(led2Red, 0);
    analogWrite(led2Green, 0);
    analogWrite(led2Blue, 0);
  }

  if(yellowAvail == 1){
    analogWrite(led3Red, 255);
    analogWrite(led3Green, 255);
    analogWrite(led3Blue, 25);
  }else{
    analogWrite(led3Red, 0);
    analogWrite(led3Green, 0);
    analogWrite(led3Blue, 0);
  }

  if(blueAvail == 1){
    analogWrite(led4Red, 0);
    analogWrite(led4Green, 0);
    analogWrite(led4Blue, 255);
  }else{
    analogWrite(led4Red, 0);
    analogWrite(led4Green, 0);
    analogWrite(led4Blue, 0);
  }
}
