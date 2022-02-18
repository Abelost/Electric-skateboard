/*
  (c) Linus Johansson 
  2022-01-23

  mod. latest: 220218
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
float batteryLevelBoard = 0;
int ySpeed = 90; //Kommer bli 1500 vid ESC
int ySpeedOutput = 90;  //Kommer bli 1500 vid ESC
int acceleration = 1;
int decceleration = 1;
int delayTime = 7;
int maxSpeed = 179; //Får se va maxspeed blir

RF24 radio(CE_PIN, CSN_PIN);

const uint64_t writingAddress = 0x9090909001;
const uint64_t readingAddress = 0x9090909002;


void setup() {
  Serial.begin(9600);
  radio.begin();
  printf_begin();
  pinMode (yPin, INPUT);
  
  radio.openWritingPipe(writingAddress);
  radio.openReadingPipe(1, readingAddress);
  radio.setPALevel(RF24_PA_MIN);
  radio.setChannel(0x54);
  //radio.printDetails();                   //Print for debugging
}


void loop() {
  delay(5);
  sendEscSpeed();
  batteryLevelBoard = receiveBatteryLevelBoard(); 
}


float receiveBatteryLevelBoard(){
  delay(5);
  radio.startListening();
  if ( radio.available()) {
    float battery_board = 0;
    radio.read(&battery_board, sizeof(battery_board));
    Serial.print("Battery is: "); 
    Serial.print(battery_board);  
    return battery_board;
  }
}


void sendEscSpeed(){
  radio.stopListening();
  int yPos = analogRead(yPin);
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
  Serial.print("Y-SpeedOutput: ");
  Serial.println(ySpeedOutput);
  radio.write(&ySpeedOutput, sizeof(ySpeedOutput));
}
