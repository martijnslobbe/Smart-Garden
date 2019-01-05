/*

    AUTOMATIC WATERING SYSTEM FOR PLANTS
      using an arduino uno


This code was written by Wessel M.
Last updated on 5-1-2019.

Feel free to use this code in you projects.
If you use this code in a tutorial, a reference to my github would be appreciated.
For information about this code visit my github.
 https://github.com/WesselM/Smart-Garden


Parts used:
 - Arduino Uno (REV 3)
 - Soil moisture sensor (FC-28)
 - 4 Channel relay module (2PH63083A)
 - Water pump (Makeblock 50200)
 - Micro switch
 - 10 K ohm resistor
 - Jumper wires

Functions of the components. 
  The FC-28 soil moisture sensor measures the moisture in soil and similar materials. 
    It acts as an variable resistor.
  The 2PH63083A 4 channel relay module is used to control equipment with high currents. 
    In this project, the relay switches the power to a water pump on or off.
  The Makeblock 50200 water pump is used to transport the water from a resevoir to it's destination.
    Since this water pump uses 12 VDC and an arduino cannot handle this much power, a relay is used as an on/of switch.
  The micro switch is a switch that let's trough current when it is pressed.
    In the project it's job is to make sure that nothing happens when the water resevoir is emptry.
    A floating weight will hang on the switch when the water tank is empty, thus pressing in the switch.
  The 10 K ohm resistor has a constant resistance of 10 kilo ohm.

Connections:
  Arduino:        FC-28:
  digital 3       VCC                                 (positive power)
  GND             GND                                 (negative power)
  analog 0        AO                                  (data transfer)

  Arduino:        Relay:
  5V              VCC                                 (positive power)
  GND             GND                                 (negative power)
  digital 2       IN1                                 (control signal)

  Arduino:        Extra:          Micro switch:
  5V              --              NO terminal         (positive power)
  digital 4       --              Com terminal        (data transfer)
  GND             10K resistor    Com terminal        (negative power)

  Power supply:   Relay:          Water pump:
  12 VDC          K1 NO           Positive terminal   (positive power)
  GND             --              Negative terminal   (negative power)

*/

//============================================================================================================================================// 


// change the following values to suit your equipment
const int minimum_water_percentage = 60;                // if there is less than this percentage of water, the pump will activate
const int water_amount_time = 3000;                     // ammount of milliseconds water is deposited

// I/O pin defenitions
const int relay_1 = 2;
const int FC_28_power = 3;
const int FC_28_readout = A0;
const int water_meter_power = 4;
const int water_meter_readout = 5;

// constant values
const int relay_on = 0;
const int relay_off = 1;
const int FC_28_on = 1;
const int FC_28_off = 0;
const int water_meter_on = 1;
const int water_meter_off = 0;
const int sufficient_water = 0;
const int deficient_water = 1;

// variables that change
int sensor_value = 0;
int soil = 0;
int water_level = 0;


// ============================================================================================================================================// 


void setup() {
  // initialize serial communication
  Serial.begin(9600);

  // turn off the relay
  digitalWrite(relay_1, relay_off);

  // turn of the FC_28
  digitalWrite(FC_28_power, FC_28_off);

  digitalWrite(water_meter_power, water_meter_off);

  // set the I/O pins as in- or output
  pinMode(relay_1, OUTPUT);
  pinMode(FC_28_power, OUTPUT);
  pinMode(FC_28_readout, INPUT);
  pinMode(water_meter_power, OUTPUT);
  pinMode(water_meter_readout, INPUT);
}


// ============================================================================================================================================// 


void loop() {
  // give current to the switch, if the switch is pressed in and current passes trough, there is not enough water in the resevoir
  digitalWrite(water_meter_power, water_meter_on);      // set the pin it's power output to HIGH, therefore giving the micro switch power
  water_level = digitalRead(water_meter_readout);       // readout the pin it's voltage input if there is more than 2.2 volt it will show up as HIGH,
                                                        // this means that the switch is on and thus that the water level is too low
  digitalWrite(water_meter_power, water_meter_off);     // set the pin it's power output to LOW, therefore cutting off the micro switch it's power

  // while there is water (no current is able to pass trough the switch) , do everything in the loop over and over again untill the water runs out
  while (water_level == sufficient_water) {
    digitalWrite(water_meter_power, water_meter_on);    // set the pin it's power output to HIGH, therefore giving the micro switch power
    water_level = digitalRead(water_meter_readout);     // readout the pin it's voltage input, if there is more than 2.2 volt it will show up as HIGH,
                                                        // this means that the switch is on and thus that the water level is too low
    digitalWrite(water_meter_power, water_meter_off);   // set the pin it's power output to LOW, therefore cutting off the micro switch it's power

    // give power to the FC_28 and read out its values, then cut off the power (only providing power when gathering data improves its lifespan)
    digitalWrite(FC_28_power, FC_28_on);                // set the pin's power output to HIGH, therefore giving the FC_28 power
    delay(100);                                         // wait 0.1 seconds
    sensor_value = analogRead(FC_28_readout);           // read the analog output from the FC_28
    soil = map(sensor_value, 200, 1024, 100, 0);        // set the value to a scale from 0 to 100 with 200 being 0% and 1024 being 100%
    delay(100);                                         // wait 0.1 seconds
    digitalWrite(FC_28_power, FC_28_off);               // set the power output to value LOW, therefore cutting off the FC_28 it's power

    // print out the values in the serial monitor (for testing and modification)
    Serial.print("Raw input is: ");
    Serial.println(sensor_value);                       // print out the soil moisture value (200-1024)
    Serial.print("Soil moisture percentage: ");
    Serial.print(soil);                                 // print out the soil moisutre percentage (0-100)
    Serial.println("%");

    // if the soil contains less water than necessary, turn on the relay, which in turn provides power to the water pump
    if (soil <= minimum_water_percentage) {
      Serial.println("Water ammount in the soil appears to be below necessary. Adding water. \n");
      digitalWrite(relay_1, relay_on);                  // turn the relay on
      delay(water_amount_time);                         // wait for a given time period
      digitalWrite(relay_1, relay_off);                 // turn the relay off
    }

    delay(1000);                                        // delay in between readouts
  }

  if (water_level == deficient_water) {
    Serial.println("The water resevoir is empty, please refill! \n");
  }

  delay(5000);                                          // delay in between readouts
}
