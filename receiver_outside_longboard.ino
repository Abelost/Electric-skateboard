/*
  (c) Linus Johansson 
  2022-01-15

  mod. latest: 220204
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
int battery = 100;

RF24 radio(CE_PIN, CSN_PIN);

const uint64_t readingAddress = 0x9090909001;
const uint64_t writingAddress = 0x9090909002;
//const byte addresses[][6] = {"Mauri", "Lovaa"};
//const uint64_t rfAddr = 0x9090909001;

void setup() {
  Serial.begin(9600);
  printf_begin();
  Serial.print("Catz");
  esc.attach(5);
  radio.begin();
  radio.openWritingPipe(writingAddress);
  radio.openReadingPipe(1, readingAddress);
  //radio.openReadingPipe(1, rfAddr);
  radio.setPALevel(RF24_PA_MIN);
  

  //radio.setDataRate( DATARATE ) ;
  radio.setChannel(0x54);
  radio.enableDynamicPayloads() ;
  radio.enableAckPayload();               // not used here
  radio.setRetries(0, 15);                // Smallest time between retries, max no. of retries
  radio.setAutoAck( true ) ;

  radio.printDetails();                   //Print for debugging
  radio.isChipConnected();

  radio.startListening();
}

void loop() {
  delay(5);
  
  radio.stopListening();
  if (battery == 0){
    battery = 100;
  }
  battery -= 1;
  //Serial.println(battery);
  //const char text[] = "Lova!";
  //radio.write(&x, sizeof(x));
  radio.write(&battery, sizeof(battery));

  delay(5);
  
  radio.startListening();
  if ( radio.available()) {
    int servoSpeed = "";
    radio.read(&servoSpeed, sizeof(servoSpeed));
    
    Serial.println(servoSpeed);
    //esc.writeMicroseconds(servoSpeed);
    //esc.write(servoSpeed);
  }

  
}
