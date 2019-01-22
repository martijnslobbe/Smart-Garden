/*
 
Connections data request:
  Arduino:        RTC DS3231:
  5V              VCC                             (positive power)
  GND             GND                             (negative power)
  SDA             SDA                             (I2C serial data transfer)
  SCL             SCL                             (I2C serial clock signal transfer)

  Arduino:        Extra:          Push button:
  digital 2       --              left terminal   (positive power & data transfer)
  GND             10K resistor    right terminal  (negative power)

Connections lighting system:
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
  digital 4       IN3                             (control signal)

  Power supply:   Relay:          GROW Led Strip:
  12 VDC          K1 NC           Positive wire   (positive power)
  GND             --              Negative wire   (negative power)

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
const int minimum_light_level = 500;
const int start_time = 1000;                // 10:00 
const int end_time = 2000;    // 20:00 
const int minimum_water_percentage = 74;                // if there is less than this percentage of water, the pump will activate
const int water_amount_time = 5000;                     // ammount of milliseconds water is deposited
const int watering_delay = 10;                           // om de 8 minuten wordt er gececkt op watertekort

// include the DS3231 libary from Rinky Dink Electronics (http://www.rinkydinkelectronics.com/library.php?id=73)
#include <DS3231.h>

// start an instance of the DS3231 class using the hardware I2C interface
DS3231  rtc(SDA, SCL); 

// I/O pin definitions
const int interruptPin = 2;
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


void setup() {    // put your setup code here, to run once:
  // initialize serial communicatoin
  Serial.begin(9600);

  // initialize communiciation with the rtc breakout
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
  pinMode(interruptPin, INPUT_PULLUP);

  // the external interrupt pin, triggers when a change in signal is detected
  // the interrupt function (in this case data_request_switch) will act above all current running programms
  // in this case if a change current flow on pin 2 occurs, the function data_request_switch is activated
  attachInterrupt(digitalPinToInterrupt(interruptPin), data_request_switch, CHANGE);
}


//============================================================================================================================================// 


void loop() {     // put your main code here, to run repeatedly:
  main_programm();                                                              // call the function main

  // while it is time to turn on the lights, activate both the main and lighitng functions
  while (start_time < current_time and current_time < end_time) {
    Serial.println("light programm running");
    main_programm();                                                            // call the function main
    lighting();                                                                 // call the function lighting
  }

  delay(5000);                                                                  // delay in between execution
}

void main_programm() {
  Serial.println("\n");
  Serial.println("main programm running");
  // if the data_request value is HIGH, than do the following: 
  if (data_request == data_request_on) { 
    data_gathering();                                                           // call the function data_gathering
    data_request = data_request_off;                                            // set data request to be False
  }

  // bitRead(PORTD, relay_lights) reads out the value off the relay pin, if the relay is not already off, then turn off the relay
  if (bitRead(PORTD, relay_lights)!= relay_off and start_time > current_time and current_time > end_time) { digitalWrite(relay_lights, relay_off); }                                              

  // bitRead(PORTD, relay_pump) reads out the value off the relay pin, if the relay is not already off, then turn off the relay
  if (bitRead(PORTD, relay_pump)!= relay_off) { digitalWrite(relay_pump, relay_off); }                                              

  get_water_level();                                                           // call the function get_water_level
  if (water_level == deficient_water) { Serial.println("The water resevoir is empty, please refill ASAP! "); } 

  get_RTC_data();                                                               // call the function get_RTC_data
  if (current_time % watering_delay == 0) { watering(); }
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
  get_RTC_data();
  Serial.print("The current time is: ");
  Serial.println(current_time_string);                                          // print out the current time
  Serial.print("The current temperature is: ");
  Serial.println(current_temp);                                                 // print out the current temperature

  get_soil_moisture();
  Serial.print("Raw soil moisture sensor output is: ");
  Serial.println(soil_sensor_value);                                            // print out the soil moisture value (200-1024)
  Serial.print("Soil moisture percentage: ");
  Serial.print(soil_percentage);                                                // print out the soil moisutre percentage (0-100)
  Serial.println("%");

  get_water_level();
  if (water_level == sufficient_water) {Serial.println("The water resevoir contains enough water");}
  if (water_level == deficient_water) {Serial.println("The water resevoir is empty, please refill ASAP! ");} 
 
  light_value = analogRead(LDR_pin);
  Serial.print("The current light level is: ");
  Serial.print(light_value);
  if (light_value <= minimum_light_level) {Serial.println(", and is below necessary");}
  if (light_value >= minimum_light_level) {Serial.println(", and is above necessary");}
  Serial.println("\n");
    // add more data requests  here
}


//============================================================================================================================================// 


void get_water_level() {
  // give power to the switch, if the switch is pressed in and current passes trough, there is not enough water in the resevoir
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
  get_water_level();
  get_soil_moisture();

  if (water_level == sufficient_water and soil_percentage <= minimum_water_percentage) {
    digitalWrite(relay_pump, relay_on);                                         // turn the relay on
    delay(water_amount_time);                                                   // wait for a given time period
    digitalWrite(relay_pump, relay_off);                                        // turn the relay off
  }
}


//============================================================================================================================================// 


void lighting() {
  light_value = analogRead(LDR_pin);  

   // if there is less light than necessary and the relay is not already on, then turn it on
        // bitRead(PORTD, relay_lights) reads out the value off the relay pin
  if (light_value <= minimum_light_level and bitRead(PORTD, relay_lights) != 0) {
    Serial.print("Lamp aan  ");
    Serial.println(light_value);                                                // print out the light value
    digitalWrite(relay_lights, relay_on);                                       // turn on the relay
  }

  // if there is more light than necessary and the relay is not already off, then turn it off
        // bitRead(PORTD, relay_lights) reads out the value off the relay pin
  else if (light_value > minimum_light_level and bitRead(PORTD, relay_lights)!= 1) {
    Serial.print("Lamp uit  ");
    Serial.println(light_value);                                                // print out the light value
    digitalWrite(relay_lights, relay_off);                                      // turn off the relay
  }      
}
