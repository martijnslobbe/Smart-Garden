# Smart-Garden
    * * * * * * * * * * * * * * * * * *
    *                                 *
    *    AUTOMATIC SMART GARDEN       *
    *      using an Arduino Uno       *
    *                                 *
    * * * * * * * * * * * * * * * * * *

Bevat:
- ventilatie systeem
- ingebouwd beveilegingssysteem (temperatuur etc.)
- LED en geluid feedback 

Alle apparatuur word zo efficient mogelijk gebruikt, zodat het langer meekan.
Voor het time control systeem gebruiken we een RTC breakout board.

Gebruikte onderdelen:
 - Arduino Uno (REV 3)
 - Real Time Clock (Adafruit DS3231)
 - 10 K ohm resistor
 - Push button 
 - Photoresistor (Light Dependent Resistor)
 - 4 Channel relay module (2PH63083A)
 - GROW Led Strip (5050 LED Strip DC12V 3:1)
 - Kabeltjes (male-male & male-female)
 - Soil moisture sensor (FC-28)
 - Water pomp (Makeblock 50200)
 - Micro switch

Functies van de verschillende onderdelen. 
  - De Real Time Clock breakout board is een accurate I2C real-time clock (RTC).
    Het apparaat heeft een batterij input, en houdt de tijd bij wanneer de stroomtoevoer naar het apparaat onderbroken wordt.
  - De push button is een mechanisme om input te verzenden naar de Arduino. Als je erop drukt wordt er stroom doorgelaten.
    De taak van de push button is om de data request functie te activeren.
  - De 10 K ohm resistor heeft een constante weerstand van 10 kilo ohm.
  - De Photoresistor is een onderdeel, waarvan de weerstand bepaald wordt door de hoeveelheid licht.
    De resistance value van een light_value verminderd als de light_value sterker belicht wordt.
  - De 2PH63083A 4 channel relay module wordt gebruikt om de apparatuur met hoge stroomtoevoer te controleren.
    De relay zet in dit project de stroom naar de waterpomp aan of uit.
  - De GROW Led Strip is een led strip speciaal gemaakt voor het gebruik bij groeinde planten.
    Door de speciale 3 rode en 1 blauwe kleur setup in de led strip wordt de groei van de planten gestimuleerd.
  - Om de vochtigheid van de grond te bepalen gebruiken we de FC-28 soil moisture sensor.
    Het werkt als een variable resistor. 
  - Het watertransport van het reservoir naar de desbetreffende bestemming wordt geregeld door de Makeblock50200 water pump.
    Aangezien de waterpomp 12 VDC gebruikt en de Arduino dat niet aankan, gebruiken we een relay als een aan en uit knop.
  - De micro switch is een switch die stroom doorlaat wanneer hij ingedrukt wordt.
    Het is de bedoeling dat de micro switch ervoor zorgt dat er niets gebeurd wanneer het water reservoir leeg is.
    Als de watertank leeg is hangt er een gewichtje aan de switch, waardoor die ingedrukt wordt.
