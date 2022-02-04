/*
  (c) Linus Johansson 
  2022-01-23

  mod. latest: 220204
  Nrf_nano built in CE pin 9, CSN pin 10

*/
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Wire.h>
#include <printf.h>
#define CE_PIN 9
#define CSN_PIN 10

const int yPin = A0;
int ySpeed = 90; //Kommer bli 1500 vid ESC
int ySpeedOutput = 90;  //Kommer bli 1500 vid ESC
int acceleration = 1;
int decceleration = 1;
int delayTime = 7;
int maxSpeed = 179;

RF24 radio(CE_PIN, CSN_PIN);

const uint64_t writingAddress = 0x9090909001;
const uint64_t readingAddress = 0x9090909002;
//const byte addresses[][6] = {"Mauri", "Lovaa"};
//const uint64_t rfAddr = 0x9090909001;

void setup() {
  Serial.begin(9600);
  radio.begin();
  printf_begin();
  radio.openWritingPipe(writingAddress);
  radio.openReadingPipe(1, readingAddress);
  //radio.openWritingPipe(rfAddr);
  radio.setPALevel(RF24_PA_MIN);

  //radio.setDataRate( DATARATE ) ;
  radio.setChannel(0x54);
  radio.enableDynamicPayloads() ;
  radio.enableAckPayload();               // not used here
  radio.setRetries(0, 15);                // Smallest time between retries, max no. of retries
  radio.setAutoAck( true ) ;

  radio.printDetails();                   //Print for debugging
  radio.powerUp();
  pinMode (yPin, INPUT);
}

void loop() {
  delay(5);

  radio.stopListening();
//  int yPos = analogRead(yPin);
//  ySpeed = map(yPos, 0, 1023, 0, 180); // kommer bli 0, 1023, 1000, 2000 vid ESC
//
//  if (ySpeed > 88){
//
//    if (ySpeed > ySpeedOutput) {
//      ySpeedOutput += acceleration;
//      delay(delayTime); 
//    }
//    if (ySpeed < ySpeedOutput){
//      ySpeedOutput -= decceleration;
//      delay(delayTime);
//    }
//    ySpeedOutput = constrain(ySpeedOutput, 90, maxSpeed);
//  }
//
//  if (ySpeed < 87){
//    ySpeedOutput = ySpeed;
//  }  

//  Serial.print("Y-Position:   ");
//  Serial.print(yPos);
//  Serial.print("\t \t");
//  Serial.print("Y-Speed: ");
//  Serial.println(ySpeed);
//  Serial.print("\t \t");
  Serial.print("Y-SpeedOutput: ");
  Serial.println(ySpeedOutput);
  Serial.print("\t \t");

  
  radio.write(&ySpeedOutput, sizeof(ySpeedOutput));

  delay(5);
  
  radio.startListening();
  if ( radio.available()) {
    int text = "";
    radio.read(&text, sizeof(text));
    Serial.print("Battery is: ");
    Serial.print(text);
  }



  
  
}
