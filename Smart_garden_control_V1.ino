/*

    * * * * * * * * * * * * * * * * * *
    *                                 *
    *    AUTOMATIC SMART GARDEN       *
    *      using an Arduino Uno       *
    *                                 *
    * * * * * * * * * * * * * * * * * *

This code was written by Wessel M.
Last updated on 22-1-2019.

Feel free to use this code in your projects.
If you use this code in a tutorial, a reference my Github page.
For information about this code visit my GitHub.
 https://github.com/WesselM


Parts used:
 - Arduino Uno (REV 3)
 - Real Time Clock (Adafruit DS3231)
 - 10 K ohm resistor
 - Push button 
 - Photoresistor (Light Dependent Resistor)
 - 4 Channel relay module (2PH63083A)
 - GROW Led Strip (5050 LED Strip DC12V 3:1)
 - Jumper wires (male-male & male-female)
 - Soil moisture sensor (FC-28)
 - Water pump (Makeblock 50200)
 - Micro switch

Functions of the components. 
  The Real Time Clock breakout board is a low-cost, extremely accurate I2C real-time clock (RTC).
    The device incorporates a battery input, and maintains accurate timekeeping when main power to the device is interrupted.
  The push button is a mechanism to send inputs to the Arduino. It can be pressed to let current pass through.
    In the project, its job is to activate the data request function
  The 10 K ohm resistor has a constant resistance of 10 kilo ohm.
  The Photoresistor is an electrical component whose resistance is influenced by the amount of light.
    The resistance value of a light_value decreases as the light_value is illuminated more strongly.
  The 2PH63083A 4 channel relay module is used to control equipment with high currents. 
    In this project, the relay switches the power to a water pump on or off.
  The GROW Led Strip is a led strip made specific for growing plants. 
    It's 3 red, 1 blue colour setup gives off a light that stimulates a plant grow and blooming.
  The FC-28 soil moisture sensor measures the moisture in soil and similar materials. 
    It acts as a variable resistor.
  The Makeblock 50200 water pump is used to transport the water from a reservoir to its destination.
    Since this water pump uses 12 VDC and an Arduino cannot handle this much power, a relay is used as an on/off switch.
  The micro switch is a switch that let's trough current when it is pressed.
    In the project its job is to make sure that nothing happens when the water reservoir is empty.
    A floating weight will hang on the switch when the water tank is empty, thus pressing in the switch.

Connections data request:
  Arduino:        RTC DS3231:
  5V              VCC                                 (positive power)
  GND             GND                                 (negative power)
  SDA             SDA                                 (I2C serial data transfer)
  SCL             SCL                                 (I2C serial clock signal transfer)

  Arduino:        Extra:          Push button:
  digital 2       --              left terminal       (positive power & data transfer)
  GND             10K resistor    right terminal      (negative power)

Connections lighting system:
  Arduino:        RTC DS3231:
  5V              VCC                                 (positive power)
  GND             GND                                 (negative power)
  SDA             SDA                                 (I2C serial data transfer)
  SCL             SCL                                 (I2C serial clock signal transfer)

  Arduino:        Extra:          Photoresistor:
  5V              --              left terminal       (positive power)
  GND             10K resistor    right terminal      (negative power)
  analog 0        --              right terminal      (data transfer)

  Arduino:        Relay:
  5V              VCC                                 (positive power)
  GND             GND                                 (negative power)
  digital 4       IN3                                 (control signal)

  Power supply:   Relay:          GROW Led Strip:
  12 VDC          K1 NC           Positive wire       (positive power)
  GND             --              Negative wire       (negative power)

Connections water system:
  Arduino:        FC-28:
  digital 5       VCC                                 (positive power)
  GND             GND                                 (negative power)
  analog 1        AO                                  (data transfer)
  
  Arduino:        Relay:
  5V              VCC                                 (positive power)
  GND             GND                                 (negative power)
  digital 3       IN1                                 (control signal)
  
  Arduino:        Extra:          Micro switch:
  digital 6       --              NO terminal         (positive power)
  digital 7       --              Com terminal        (data transfer)
  GND             10K resistor    Com terminal        (negative power)
  
  Power supply:   Relay:          Water pump:
  12 VDC          K1 NO           Positive terminal   (positive power)
  GND             --              Negative terminal   (negative power)

*/

// change the following values to suit your equipment
const int minimum_light_level = 500;                                            // the minimum output that the LDR can give, before the lights turn on
const int start_time = 1000;                                                    // when the light program has to start 10:00 
const int end_time = 21048;                                                     // when the light program has to end 20:00 
const int minimum_water_percentage = 72;                                        // if there is less than this percentage of water, the pump will activate
const int water_amount_time = 5000;                                             // amount of milliseconds water is deposited
const int watering_delay = 10;                                                  // delay in minutes before the water is checked

// include the DS3231 library from Rinky Dink Electronics (http://www.rinkydinkelectronics.com/library.php?id=73)
#include <DS3231.h>

// start an instance of the DS3231 class using the hardware I2C interface
DS3231  rtc(SDA, SCL); 

// I/O pin definitions
const int interrupt_pin = 2;
const int LDR_pin = A0; 
const int relay_pump = 3;
const int relay_lights = 4;
const int FC_28_power = 5;
const int FC_28_readout = A1;
const int water_meter_power = 6;
const int water_meter_readout = 7;


// constant values
const int data_request_on = 1;
const int data_request_off = 0;
const int relay_on = 0;
const int relay_off = 1;
const int FC_28_on = 1;
const int FC_28_off = 0;
const int water_meter_on = 1;
const int water_meter_off = 0;
const int sufficient_water = 0;
const int deficient_water = 1;

// global variables
volatile byte data_request = 0;
String time_string = "";
String current_time_string = "";
int light_value = 0;    
int current_time = 0;
int current_temp = 0;
int soil_sensor_value = 0;
int soil_percentage = 0;
int water_level = 1;



//============================================================================================================================================// 


void setup() { // put your setup code here, to run once:
  // initialize serial communication
  Serial.begin(9600);

  // initialize communication with the rtc breakout
  rtc.begin();
  
  // turn off the relay's
  digitalWrite(relay_pump, relay_off);
  digitalWrite(relay_lights, relay_off);
  
  // turn of the FC_28
  digitalWrite(FC_28_power, FC_28_off);

  // turn of the water meter
  digitalWrite(water_meter_power, water_meter_off);
  
  // set the I/O pins as in- or output
  pinMode(relay_pump, OUTPUT);
  pinMode(relay_lights, OUTPUT);
  pinMode(light_value, INPUT);
  pinMode(FC_28_power, OUTPUT);
  pinMode(FC_28_readout, INPUT);
  pinMode(water_meter_power, OUTPUT);
  pinMode(water_meter_readout, INPUT);
  pinMode(interrupt_pin, INPUT_PULLUP);

  // the external interrupt pin, triggers when a change in signal is detected
  // the interrupt function (in this case data_request_switch) will act above all current running programms
  // if a change current flow on pin 2 occurs, the function data_request_switch is activated
  attachInterrupt(digitalPinToInterrupt(interrupt_pin), data_request_switch, CHANGE);
}


//============================================================================================================================================// 


void loop() { // put your main code here, to run repeatedly:
  main_programm();                                                              // call the function main

  // while it is time to turn on the lights, activate both the main and lighitng functions
  while (start_time < current_time and current_time < end_time) { 
    Serial.println("Light program running");
    get_RTC_data();
    main_programm();                                                            // call the function main
    lighting();                                                                 // call the function lighting
    delay(5000);
  }

  delay(5000);                                                                  // delay in between execution
}

void main_programm() { 
  Serial.println("Main program running");
  // if the data_request value is HIGH, than do the following: 
  if (data_request == data_request_on) { 
    data_gathering();                                                           // call the function data_gathering
    data_request = data_request_off;                                            // set data request to be False
  }

  // check if the relay is not already off and it is not time for light, turn off the relay
  if (bitRead(PORTD, relay_lights)!= relay_off and start_time > current_time or current_time > end_time) { digitalWrite(relay_lights, relay_off); }                                              

  // check if the waterpump relay is on, if it is, then turn it off
  if (bitRead(PORTD, relay_pump)!= relay_off) { digitalWrite(relay_pump, relay_off); }                                                                                     

  // check if there is enough water, if there is not (current passes through the micro switch), print out a message
  get_water_level();                                                            // call the function get_water_level
  if (water_level == deficient_water) { Serial.println("The water reservoir is empty, please refill ASAP! "); } 

  // if the time is devisible by the watering delay and leaves no reminder, then call the watering function
  get_RTC_data();                                                               // call the function get_RTC_data
  if (current_time % watering_delay == 0) { watering(); }                       // call the watering function 
  Serial.println("\n");
}


//============================================================================================================================================// 


void get_RTC_data() { 
  // request the current time and convert it to an integer
  current_temp = rtc.getTemp();
  current_time_string = rtc.getTimeStr();
  time_string = rtc.getTimeStr(FORMAT_SHORT);                                   // request the time using the I2C protocol on the RTC  "HH:MM". ("15:32")
  time_string.remove(2,1);                                                      // remove the third character ":". ("HH:MM" -> "HHMM")                                             
  current_time = time_string.toInt();                                           // change the time string to an integer
}

// this function is activated when the interrupt pin detects a current change, the function changes the data_request to HIGH
void data_request_switch() { 
  data_request = data_request_on;                                               // set the data_request value to HIGH
}

// this function is activated when the data_request is on, gathers the data from sensors and prints out the data
void data_gathering() { 
  // print the time and temperature
  get_RTC_data();                                                               // call the get_RTC_data function
  Serial.print("The current time is: ");
  Serial.println(current_time_string);                                          // print out the current time
  Serial.print("The current temperature is: ");
  Serial.println(current_temp);                                                 // print out the current temperature
  Serial.println("°C");

  // print the soil moisture percentage
  get_soil_moisture();                                                          // call the get_soil_moisture function
  Serial.print("Raw soil moisture sensor output is: ");
  Serial.println(soil_sensor_value);                                            // print out the soil moisture value (200-1024)
  Serial.print("Soil moisture percentage: ");
  Serial.print(soil_percentage);                                                // print out the soil moisture percentage (0-100)
  Serial.println("%");

  // print the water level
  get_water_level();                                                            // call the get_water_level function
  if (water_level == sufficient_water) { Serial.println("The water reservoir contains enough water"); }
  if (water_level == deficient_water) { Serial.println("The water reservoir is empty, please refill ASAP! "); } 

  // print whether the light program is running or not
  if (start_time < current_time and current_time < end_time) { Serial.println("Light programm is running"); }
  if (start_time > current_time or current_time > end_time) { Serial.println("Light programm is not running"); }

  // print the light value
  light_value = analogRead(LDR_pin);                                            // get the new light value from the LDR
  Serial.print("The current light level is: ");
  Serial.print(light_value);                                                    // print out the light level 
  if (light_value <= minimum_light_level) { Serial.println(", and is below necessary"); }
  if (light_value >= minimum_light_level) { Serial.println(", and is above necessary"); }
  Serial.println("\n");
    // add more data requests  here
}


//============================================================================================================================================// 


void get_water_level() { 
  // give power to the switch, if the switch is pressed in and current passes through, there is not enough water in the reservoir
  digitalWrite(water_meter_power, water_meter_on);                               // set the pin it's power output to HIGH, therefore giving the micro switch power
  water_level = digitalRead(water_meter_readout);                                // readout the pin it's voltage input if there is more than 2.2 volt it will show up as HIGH,
                                                                                 // this means that the switch is on and thus that the water level is too low
  digitalWrite(water_meter_power, water_meter_off);                              // set the pin it's power output to LOW, therefore cutting off the micro switch it's power
}

void get_soil_moisture() { 
  // give power to the FC_28 and read out its values, then cut off the power (only providing power when gathering data improves its lifespan)
  digitalWrite(FC_28_power, FC_28_on);                                          // set the pin's power output to HIGH, therefore giving the FC_28 power
  delay(10);                                                                    // wait 0.01 seconds
  soil_sensor_value = analogRead(FC_28_readout);                                // read the analog output from the FC_28
  soil_percentage = map(soil_sensor_value, 200, 1024, 100, 0);                  // set the value to a scale from 0 to 100 with 200 being 0% and 1024 being 100%
  delay(10);                                                                    // wait 0.01 seconds
  digitalWrite(FC_28_power, FC_28_off);                                         // set the power output to value LOW, therefore cutting off the FC_28 it's power
}

void watering() { 
  get_water_level();                                                            // call the get_water_level function
  get_soil_moisture();                                                          // call the get_soil_moisture function

  if (water_level == sufficient_water and soil_percentage <= minimum_water_percentage) { 
    digitalWrite(relay_pump, relay_on);                                         // turn the relay on
    delay(water_amount_time);                                                   // wait for a given time period
    digitalWrite(relay_pump, relay_off);                                        // turn the relay off
    delay(50001);                                                               // delay for 50 sec so that the loop does not run twice a minute 
  }
}


//============================================================================================================================================// 


void lighting() { 
  light_value = analogRead(LDR_pin);                                            // get the new light value from the LDR 

   // if there is less light than necessary and the relay is not already on, then turn it on
        // bitRead(PORTD, relay_lights) reads out the value off the relay pin
  if (light_value <= minimum_light_level and bitRead(PORTD, relay_lights) != 0) { 
    Serial.print("Lamp on: ");
    Serial.println(light_value);                                                // print out the light value
    digitalWrite(relay_lights, relay_on);                                       // turn on the relay
  }

  // if there is more light than necessary and the relay is not already off, then turn it off
        // bitRead(PORTD, relay_lights) reads out the value off the relay pin
  else if (light_value > minimum_light_level and bitRead(PORTD, relay_lights)!= 1) { 
    Serial.print("Lamp off: ");
    Serial.println(light_value);                                                // print out the light value
    digitalWrite(relay_lights, relay_off);                                      // turn off the relay
  }      
}
