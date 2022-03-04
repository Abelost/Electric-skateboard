(c) Linus Johansson 
  2022-01-23
  mod. latest: 220218
  mod. latest: 220302
  Nrf_nano built in CE pin 9, CSN pin 10
  Oled 1.3" IIC 0x78, 0x7A SH1106
*/
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Wire.h>
#include <printf.h>
#include <Arduino.h>
#include <U8g2lib.h>
#include <U8x8lib.h>
#define CE_PIN 9
#define CSN_PIN 10

int decceleration = 1;
int delayTime = 7;
int maxSpeed = 179; //Får se va maxspeed blir

U8X8_SH1106_128X64_NONAME_HW_I2C u8x8(/* reset=*/ U8X8_PIN_NONE);
RF24 radio(CE_PIN, CSN_PIN);

const uint64_t writingAddress = 0x9090909001;
const uint64_t readingAddress = 0x9090909002;
void setup() {
  Serial.begin(9600);
  radio.begin();
  u8x8.begin();
  u8x8.setFont(u8x8_font_chroma48medium8_r);
  printf_begin();
  pinMode (yPin, INPUT);

void setup() {
  radio.setPALevel(RF24_PA_MIN);
  radio.setChannel(0x54);
  //radio.printDetails();                   //Print for debugging

}


void loop() {
  delay(5);
  sendEscSpeed();
  batteryLevelBoard = receiveBatteryLevelBoard(); 
  //batteryLevelBoard = receiveBatteryLevelBoard(); 
  u8x8.clearLine(0);
  u8x8.drawString(0,0,"Speed: ");
  u8x8.setCursor(7, 0);
  u8x8.print(ySpeedOutput);
//  u8x8.draw(2, 2, ySpeedOutput);
//  Serial.print(ySpeedOutput);
//  delay(5);
  //u8x8.clearDisplay();
}



float receiveBatteryLevelBoard(){
  delay(5);
  radio.startListening();
