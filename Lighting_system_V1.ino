/*

    AUTOMATIC LIGHTING SYSTEM
      using an arduino uno


This code was written by Wessel M.
Last updated on 2-1-2019.

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
  5V                              left_leg        (positive power)
  GND             10K resistor    right_leg       (negative power)
  analog 0        --              right_leg       (data transfer)

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
String start_time = "09:00";
String end_time = "17:00";

// I/O pin defenitions
const int LDRpin = A0; 
const int relay_1 = 2;

// constant values
const int relay_on = 0;
const int relay_off = 1;

// variables that change
int LDR_value = 0;    
int lamp_status = 0;
String current_time = " ";


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


void loop() { 
  //  read out the RTC and print the values
  current_time = rtc.getTimeStr(FORMAT_SHORT);                                 // read out the current time from the RTC
  Serial.print("The current time is: ");
  Serial.println(current_time);                                                // print out the current time
  
  // print out the current lamp status (1 is on, 0 is off)
  Serial.print("Lamp status: ");
  Serial.println(lamp_status);                                                 // print out the lamp status
  Serial.println("");

  // if the current time is the same as the start time, change the lamp status to 1 (on)
  if (current_time == start_time) { 
    lamp_status = 1;                                                           // change the lamp status to 1 (on)
    Serial.println("It's time for light \n");
  }

  // if the relay is not already off, turn it off  
  if (bitRead(PORTD, relay_1)!= 1);{    // bitRead(PORTD, relay_1) reads out the value off the relay pin
    digitalWrite(relay_1, relay_off);   // turn off the relay
  }

  // run the loop while the lamp status is 1 (on)
  while (lamp_status == 1) {
    // if the current time is the same as the end time, change the lamp status to 0 (off), this will stop the loop 
    current_time = rtc.getTimeStr(FORMAT_SHORT);                               // read out the current time from the RTC
    if (current_time == end_time) {
      lamp_status = 0;                                                         // change the lamp status to 0 (off)
      Serial.println("It's time for dark \n");
    }

    // read out the value from the photoresistor
    LDR_value = analogRead(LDRpin); 

    // if there is less light than necessary and the relay is not already on, then turn it on
    if (LDR_value <= minimum_light_level  and bitRead(PORTD, relay_1) != 0) {        // bitRead(PORTD, relay_1) reads out the value off the relay pin
      Serial.print("The light value has changed and is below necessary. Light level: ");
      Serial.println(LDR_value);                                               // print out the light value
      digitalWrite(relay_1, relay_on);                                         // turn on the relay
      Serial.println("Relay is now ON \n");
    }

    // if there is more light than necessary and the relay is not already off, then turn it off
    else if (LDR_value > minimum_light_level and bitRead(PORTD, relay_1)!= 1) {      // bitRead(PORTD, relay_1) reads out the value off the relay pi
      Serial.print("The light value has changed and is above necessary. Light level: ");
      Serial.println(LDR_value);                                               // print out the light value
      digitalWrite(relay_1, relay_off);                                        // turn off the relay
      Serial.println("Relay is now OFF \n");
    }
    
    delay(100); // delay in between readouts
  }

  delay(5000); // delay in between readouts
}
