/*
  (c) Linus Johansson 
  2022-01-15

  mod. latest: 220218
  Will probably change UNO to nano at a later stage.

*/

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Wire.h>
#include <Servo.h>
#include <printf.h>
#define CE_PIN 10
#define CSN_PIN 9

Servo esc;
float battery = 100;

RF24 radio(CE_PIN, CSN_PIN);

const uint64_t readingAddress = 0x9090909001;
const uint64_t writingAddress = 0x9090909002;

void setup() {
  Serial.begin(9600);
  printf_begin();
  esc.attach(5);
  
  radio.begin();
  radio.openWritingPipe(writingAddress);
  radio.openReadingPipe(1, readingAddress);
  radio.setPALevel(RF24_PA_MIN);
  radio.setChannel(0x54);
  //radio.printDetails();                   //Print for debugging
}

void loop() {
  delay(5);
  
  sendBatteryLevel();

  receiveEscSpeed();
   
}


void sendBatteryLevel(){
  radio.stopListening();
  if (battery < 20){
    battery = 100;
  }
  battery -= 0.1;
  Serial.println(battery);
  radio.write(&battery, sizeof(battery));
  delay(5);
  }


void receiveEscSpeed(){
  radio.startListening();
  if ( radio.available()) {
    int servoSpeed = "";
    radio.read(&servoSpeed, sizeof(servoSpeed));
    
    Serial.println(servoSpeed);
    esc.writeMicroseconds(servoSpeed);
    esc.write(servoSpeed);
  }
}
