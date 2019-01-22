# Smart-Garden
    * * * * * * * * * * * * * * * * * *
    *                                 *
    *    AUTOMATIC SMART GARDEN       *
    *      using an Arduino Uno       *
    *                                 *
    * * * * * * * * * * * * * * * * * *

Going to include:
- ventilation system
- build in safety system (temperature etc.)
- LED and sound feedback 

All equipment will be used efficiently, saving its lifespan.
A RTC breakout board will be used to time control the system.

Feel free to use the code in your projects.
If you use this code in a tutorial, a reference to my GitHub would be appreciated.
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
  -The Real Time Clock breakout board is a low-cost, extremely accurate I2C real-time clock (RTC).
    The device incorporates a battery input, and maintains accurate timekeeping when main power to the device is interrupted.
  -The push button is a mechanism to send inputs to the Arduino. It can be pressed to let current pass through.
    In the project, its job is to activate the data request function
  -The 10 K ohm resistor has a constant resistance of 10 kilo ohm.
  -The Photoresistor is an electrical component whose resistance is influenced by the amount of light.
    The resistance value of a light_value decreases as the light_value is illuminated more strongly.
  -The 2PH63083A 4 channel relay module is used to control equipment with high currents. 
    In this project, the relay switches the power to a water pump on or off.
  -The GROW Led Strip is a led strip made specific for growing plants. 
    It's 3 red, 1 blue colour setup gives off a light that stimulates a plant grow and blooming.
  -The FC-28 soil moisture sensor measures the moisture in soil and similar materials. 
    It acts as a variable resistor.
  -The Makeblock 50200 water pump is used to transport the water from a reservoir to its destination.
    Since this water pump uses 12 VDC and an Arduino cannot handle this much power, a relay is used as an on/off switch.
  -The micro switch is a switch that let's trough current when it is pressed.
    In the project its job is to make sure that nothing happens when the water reservoir is empty.
    A floating weight will hang on the switch when the water tank is empty, thus pressing in the switch.
