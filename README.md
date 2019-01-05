# Smart-Garden
Arduino controlled indoor smart-garden / greenhouse. 


Going to include:
- automatic watering system
- lighting system
- ventilation system


All equipment will be used efficiently, saving its lifespan.
A RTC breakout board will be used to time control the system.


Components used up until now:
 - Arduino Uno (REV 3)
 - Real Time Clock (Adafruit DS3231
 - Photoresistor (Light Dependent Resistor)
 - GROW Led Strip (5050 LED Strip DC12V 3:1)
 - Soil moisture sensor (FC-28)
 - Micro switch
 - 10 K ohm resistor
 - 4 Channel relay module (2PH63083A)
 - Water pump (Makeblock 50200)
 - Jumper wires



Functiosn of the components:
 - The Arduino Uno is a microcontroller board The Arduino Uno is a microcontroller board based on the ATmega328.
 It has 14 digital - and 6 analog I/O pins. It will act as the brains of this project. The board will controll equipment based on analog and digital inputs from sensors. 
 - The Real Time Clock breakout board is a low-cost, extremely accurate I2C real-time clock (RTC).
 The device incorporates a battery input, and maintains accurate timekeeping when main power to the device is interrupted.
 - The Photoresistor is an electrical component whose resistance is influenced by the amount of light.
 The resistance value of an LDR decreases as the LDR is illuminated more strongly.
 - The GROW Led Strip is a led strip made specific for growing plants.
 It's 3 red, 1 blue color setup gives off a light that stimulates aplant grow and blooming.
 - The 2PH63083A 4 channel relay module is used to control equipment with high currents. 
 In this project, the relay switches the power to a water pump on or off.
 - The FC-28 soil moisture sensor measures the moisture in soil and similar materials. 
 It acts as an variable resistor.
 - The micro switch is a switch that let's trough current when it is pressed.
 In the project it's job is to make sure that nothing happens when the water resevoir is emptry.
 A floating weight will hang on the switch when the water tank is empty, thus pressing in the switch.
 - The 10 K ohm resistor has a constant resistance of 10 kilo ohm.
 - The Makeblock 50200 water pump is used to transport the water from a resevoir to it's destination.
 Since this water pump uses 12 VDC and an arduino cannot handle this much power, a relay is used as an on/of switch.
