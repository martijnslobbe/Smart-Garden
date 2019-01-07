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
 - 10 K ohm resistor
 - Push button 
 - Jumper wires (male-male)
 

Functions of the components. 
  The Real Time Clock breakout board is a low-cost, extremely accurate I2C real-time clock (RTC).
    The device incorporates a battery input, and maintains accurate timekeeping when main power to the device is interrupted.
  The push button is a mechanism to sent inputs to the arduino. It can be pressed to let current pass trough.
    In thie project, it's job is to activate the data request function
  The 10 K ohm resistor has a constant resistance of 10 kilo ohm.

Connections:
  Arduino:        RTC DS3231:
  5V              VCC                             (positive power)
  GND             GND                             (negative power)
  SDA             SDA                             (I2C serial data transfer)
  SCL             SCL                             (I2C serial clock signal transfer)

  Arduino:        Extra:          Push button:
  digital 2       --              left terminal   (positive power & data transfer)
  GND             10K resistor    right terminal  (negative power)

*/

//============================================================================================================================================// 


// include the DS3231 libary from Rinky Dink Electronics (http://www.rinkydinkelectronics.com/library.php?id=73)
#include <DS3231.h>

// start an instance of the DS3231 class using the hardware I2C interface
DS3231  rtc(SDA, SCL); 

// I/O pin definitions
const int interruptPin = 2;

// constant values
const int data_request_on = 1;
const int data_request_off = 0;

// global variables
volatile byte data_request = 0;


//============================================================================================================================================// 


void setup() {
  // initialize serial communicatoin
  Serial.begin(9600);

  // initialize communiciation with the rtc breakout
  rtc.begin();
  
  // set the I/O pins as in- or output  
  pinMode(interruptPin, INPUT_PULLUP);

  // the external interrupt pin, triggers when a change in signal is detected
  // the interrupt function (in this case data_request_switch) will act above all current running programms
  // in this case if a change current flow on pin 2 occurs, the function data_request_switch is activated
  attachInterrupt(digitalPinToInterrupt(interruptPin), data_request_switch, CHANGE);
}


//============================================================================================================================================// 


void loop() {
  Serial.println("The program is running");
  // run your main programm here
  
  // if the data_request value is HIGH, than do the following:
  if (data_request == data_request_on) {
    data_gathering();                               // call the function data_gathering
    data_request = data_request_off;                // set the data_request value to LOW
  }
  
  delay(1000);                                      // delay in between readouts
}

// this function is activated when the interrupt pin detects a current change, the function changes the data_request to HIGH
void data_request_switch() {
  data_request = data_request_on;                   // set the data_request value to HIGH
}

// this function is activated when the data_request is on, gathers the data from sensors and prints out the data
void data_gathering() {
  Serial.print("The current time is: ");
  Serial.println(rtc.getTimeStr());                 // print out the current time
  Serial.print("The current temperature is: ");
  Serial.println(rtc.getTemp());                    // print out the current temperature
  // add more data request functions here
}

