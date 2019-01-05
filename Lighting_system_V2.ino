/*

    AUTOMATIC LIGHTING SYSTEM
      using an arduino uno


This code was written by Wessel M.
Last updated on 5-1-2019.

Feel free to use this code in you projects.
If you use this code in a tutorial, a reference to my github would be appreciated.
For information about this code visit my github.
 https://github.com/WesselM


Parts used:
 - Arduino Uno (REV 3)
 - Real Time Clock (Adafruit DS3231)
 - Photoresistor (Light Dependent Resistor)
 - 4 Channel relay module (2PH63083A)
 - GROW Led Strip (5050 LED Strip DC12V 3:1)
 - Jumper wires (male-male & male-female)
 
Functions of the components. 
  The Real Time Clock breakout board is a low-cost, extremely accurate I2C real-time clock (RTC).
    The device incorporates a battery input, and maintains accurate timekeeping when main power to the device is interrupted.
  The Photoresistor is an electrical component whose resistance is influenced by the amount of light.
    The resistance value of an LDR decreases as the LDR is illuminated more strongly.
  The 2PH63083A 4 channel relay module is used to control equipment with high currents. 
    In this project, the relay switches the power to a water pump on or off.
  The GROW Led Strip is a led strip made specific for growing plants. 
    It's 3 red, 1 blue color setup gives off a light that stimulates aplant grow and blooming.
 
 
Connections:
  Arduino:        RTC DS3231:
  5V              VCC                             (positive power)
  GND             GND                             (negative power)
  SDA             SDA                             (I2C serial data transfer)
  SCL             SCL                             (I2C serial clock signal transfer)

  Arduino:        Extra:          Photoresistor:
  5V              --              left terminal   (positive power)
  GND             10K resistor    right terminal  (negative power)
  analog 0        --              right terminal  (data transfer)

  Arduino:        Relay:
  5V              VCC                             (positive power)
  GND             GND                             (negative power)
  digital 2       IN1                             (control signal)

  Power supply:   Relay:          GROW Led Strip:
  12 VDC          K1 NC           Positive wire   (positive power)
  GND             --              Negative wire   (negative power)

*/

//============================================================================================================================================// 


// include the DS3231 libary from Rinky Dink Electronics (http://www.rinkydinkelectronics.com/library.php?id=73)
#include <DS3231.h>

// start an instance of the DS3231 class using the hardware I2C interface
DS3231  rtc(SDA, SCL); 

// change the following values to suit your equipment
const int minimum_light_level = 500;
const int start_time = 1000;  // 10:00 
const int end_time = 1630;    // 18:00 

// I/O pin defenitions
const int LDRpin = A0; 
const int relay_1 = 2;

// constant values
const int relay_on = 0;
const int relay_off = 1;

// variables that change
int LDR_value = 0;    
int lamp_status = 0;
int current_time = 0;
String time_string = "";


//============================================================================================================================================// 


void setup() {
  // initialize serial communicatoin
  Serial.begin(9600);

  // initialize communiciation with the rtc breakout
  rtc.begin();

  // turn off the relay
  digitalWrite(relay_1, relay_off);

  // set the I/O pins as in- or output
  pinMode(relay_1, OUTPUT);
  pinMode(LDR_value, INPUT);
}


//============================================================================================================================================// 


void loop(){
  // request the current time and convert it to an integer  
  time_string = rtc.getTimeStr(FORMAT_SHORT);                                     // request the time using the I2C protocol on the RTC 
  Serial.print("The current time is: ");
  Serial.println(time_string);                                                    // print out the current time "HH:MM". ("15:32")
  time_string.remove(2,1);                                                        // remove the third character ":". ("HH:MM" -> "HHMM")                                             
  current_time = time_string.toInt();                                             // change the time string to an integer

  // if the relay is not already off, turn it off  
  if (bitRead(PORTD, relay_1)!= 1);{                                              // bitRead(PORTD, relay_1) reads out the value off the relay pin
    digitalWrite(relay_1, relay_off);                                             // turn off the relay
  }

  while (start_time <= current_time and current_time < end_time) {
    // request the current time and convert it to an integer
    time_string = rtc.getTimeStr(FORMAT_SHORT);                                   // request the time using the I2C protocol on the RTC 
    Serial.print("The current time is: ");
    Serial.println(time_string);                                                  // print out the current time "HH:MM". ("15:32")
    time_string.remove(2,1);                                                      // remove the third character ":". ("HH:MM" -> "HHMM")                                             
    current_time = time_string.toInt();                                           // change the time string to an integer

    // read out the value from the photoresistor
    LDR_value = analogRead(LDRpin);  

    // if there is less light than necessary and the relay is not already on, then turn it on
    if (LDR_value <= minimum_light_level and bitRead(PORTD, relay_1) != 0) {      // bitRead(PORTD, relay_1) reads out the value off the relay pin
      Serial.print(" \n The light value has changed and is below necessary. Light level: ");
      Serial.println(LDR_value);                                                  // print out the light value
      digitalWrite(relay_1, relay_on);                                            // turn on the relay
      Serial.println("Relay is now ON \n");
    }

    // if there is more light than necessary and the relay is not already off, then turn it off
    else if (LDR_value > minimum_light_level and bitRead(PORTD, relay_1)!= 1) {   // bitRead(PORTD, relay_1) reads out the value off the relay pi
      Serial.print(" \n The light value has changed and is above necessary. Light level: ");
      Serial.println(LDR_value);                                                  // print out the light value
      digitalWrite(relay_1, relay_off);                                           // turn off the relay
      Serial.println("Relay is now OFF \n");
    }
    
    delay(1000); // delay in between readouts
  }

  delay(5000);  // delay in between readouts
}
