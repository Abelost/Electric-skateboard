#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

const int yPin = A0;
int ySpeed = 90; //Kommer bli 1500 vid ESC
int ySpeedOutput = 90;  //Kommer bli 1500 vid ESC
int acceleration = 1;
int decceleration = 1;
int delayTime = 7;
int maxSpeed = 179;

RF24 radio(9, 10); // CE, CSN

const byte addresses[][6] = {"Mauri", "Lova!"};

void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.openWritingPipe(addresses[1]); // 00002
  radio.openReadingPipe(1, addresses[0]); // 00001
  radio.setPALevel(RF24_PA_MIN);
  pinMode (yPin, INPUT);
}

void loop() {
  delay(5);

  radio.stopListening();
  int yPos = analogRead(yPin);
  Serial.println(yPos);
  ySpeed = map(yPos, 0, 1023, 0, 180); // kommer bli 0, 1023, 1000, 2000 vid ESC

  if (ySpeed > 88){

    if (ySpeed > ySpeedOutput) {
      ySpeedOutput += acceleration;
      delay(delayTime); 
    }
    if (ySpeed < ySpeedOutput){
      ySpeedOutput -= decceleration;
      delay(delayTime);
    }
    ySpeedOutput = constrain(ySpeedOutput, 90, maxSpeed);
  }

  if (ySpeed < 87){
    ySpeedOutput = ySpeed;
  }  

  Serial.print("Y-Position:   ");
  Serial.print(yPos);
  Serial.print("\t \t");
  Serial.print("Y-Speed: ");
  Serial.print(ySpeed);
  Serial.print("\t \t");
  Serial.print("Y-SpeedOutput: ");
  Serial.print(ySpeedOutput);
  Serial.print("\t \t");

  
  radio.write(&ySpeedOutput, sizeof(ySpeedOutput));

  delay(5);
  
  radio.startListening();
  if ( radio.available()) {
    int text = "";
    radio.read(&text, sizeof(text));
    Serial.print("Battery is: ");
    Serial.println(text);
  }



  
  
}
