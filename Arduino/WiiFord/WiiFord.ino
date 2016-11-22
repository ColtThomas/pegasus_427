/*
 Example sketch for the Wiimote Bluetooth library - developed by Kristian Lauszus
 For more information visit my blog: http://blog.tkjelectronics.dk/ or
 send me an e-mail:  kristianl@tkjelectronics.com
 */

#include <Wii.h>
#include <usbhub.h>

// Satisfy the IDE, which needs to see the include statment in the ino too.
#ifdef dobogusinclude
#include <spi4teensy3.h>
#include <SPI.h>
#endif

USB Usb;
//USBHub Hub1(&Usb); // Some dongles have a hub inside

#define TRIGGER_PIN 3
#define LEFT_PIN 2
#define RIGHT_PIN 4
#define VOL_DOWN_PIN 5
#define VOL_UP_PIN 6
//#define POWER_PIN 9

BTD Btd(&Usb); // You have to create the Bluetooth Dongle instance like so
/* You can create the instance of the class in two ways */
//WII Wii(&Btd,PAIR); // This will start an inquiry and then pair with your Wiimote - you only have to do this once
WII Wii(&Btd); // After that you can simply create the instance like so and then press any button on the Wiimote

bool printAngle;

void setup() {
  pinMode(TRIGGER_PIN, OUTPUT);
  pinMode(LEFT_PIN, OUTPUT);
  pinMode(RIGHT_PIN, OUTPUT);
  pinMode(VOL_UP_PIN, OUTPUT);
  pinMode(VOL_DOWN_PIN, OUTPUT);
//  pinMode(POWER_PIN, OUTPUT);
  
  Serial.begin(115200);
#if !defined(__MIPSEL__)
  while (!Serial); // Wait for serial port to connect - used on Leonardo, Teensy and other boards with built-in USB CDC serial connection
#endif
delay(100);
  if (Usb.Init() == -1) {
    Serial.print(F("\r\nOSC did not start"));
    while (1); //halt
  }
  Serial.print(F("\r\nWiimote Bluetooth Library Started"));
}
void loop() {
//  static unsigned long count = 0;
//  Serial.print("\r\nCount ");
//  Serial.print(count++);
//  Serial.print(" to ");
  Usb.Task();
 // digitalWrite(POWER_PIN, HIGH);
//  Serial.print(count);
  
  if (Wii.wiimoteConnected) {
//    if (Wii.getButtonClick(HOME)) { // You can use getButtonPress to see if the button is held down
//      Serial.print(F("\r\nHOME"));
//      Wii.disconnect();
//    }
//    else {
      if (Wii.getButtonClick(LEFT)) {
        Wii.setLedOff();
        Wii.setLedOn(LED1);
        Serial.print(F("\r\nLeft"));
      }
       if(Wii.getButtonPress(LEFT)) {
        digitalWrite(LEFT_PIN, HIGH);
      }
      else {
        digitalWrite(LEFT_PIN, LOW);
      }
      if (Wii.getButtonClick(RIGHT)) {
        Wii.setLedOff();
        Wii.setLedOn(LED3);
        Serial.print(F("\r\nRight"));
      }
      if(Wii.getButtonPress(RIGHT)) {
        digitalWrite(RIGHT_PIN, HIGH);
      }
      else {
        digitalWrite(RIGHT_PIN, LOW);
      }
      if (Wii.getButtonClick(DOWN)) {
        Wii.setLedOff();
        Wii.setLedOn(LED4);
        Serial.print(F("\r\nDown"));
      }
      if (Wii.getButtonClick(UP)) {
        Wii.setLedOff();
        Wii.setLedOn(LED2);
        Serial.print(F("\r\nUp"));
      }

      if (Wii.getButtonPress(PLUS)) {
        Serial.print(F("\r\nPlus"));
        digitalWrite(VOL_UP_PIN, HIGH);
      }
      else {
        digitalWrite(VOL_UP_PIN, LOW);
      }
      if (Wii.getButtonPress(MINUS)) {
        Serial.print(F("\r\nMinus"));
        digitalWrite(VOL_DOWN_PIN, HIGH);
      }
      else {
        digitalWrite(VOL_DOWN_PIN, LOW);
      }
      if (Wii.getButtonClick(ONE))
        Serial.print(F("\r\nOne"));
      if (Wii.getButtonClick(TWO))
        Serial.print(F("\r\nTwo"));

      if (Wii.getButtonClick(A)) {
        printAngle = !printAngle;
        Serial.print(F("\r\nA"));
      }
      if (Wii.getButtonClick(B)) {
        //Wii.setRumbleToggle();
        digitalWrite(TRIGGER_PIN, HIGH);
        Serial.print(F("\r\nB"));
      }
       if(Wii.getButtonPress(B)) {
        digitalWrite(TRIGGER_PIN, HIGH);
      }
      else {
        digitalWrite(TRIGGER_PIN, LOW);
      }
//    }
  }

}
