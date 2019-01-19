//unchangeable variables
const int relay_on = 0;
const int relay_off = 1;
const int relay_1 = 2;

const int minimum_water_percentage = 60; // 0-100
const int water_amount_time = 3000; // milliseconds

//variables
int soil = 0;


// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  
  // turn off the relay
  digitalWrite(relay_1, relay_off);

  // set the I/O pin as an output
  pinMode(relay_1, OUTPUT);
}


// the loop routine runs over and over again forever:
void loop() {
  // read the input on analog pin 0:
  int sensorValue = analogRead(A0);
  
  //map the value to a percentage
  soil = map(sensorValue, 200, 700, 100, 0);

////////////////////////////////////////////////////////
 
  // print out the soil water percentage you calculated:
  Serial.print("Raw input is: ");
  Serial.println(sensorValue);
  Serial.print("Soil moisture percentage: ");
  Serial.print(soil);
  Serial.println("% \n");

////////////////////////////////////////////////////////

  if (soil <= minimum_water_percentage) {
    Serial.println("Water percentage too low. Adding water. \n");
    digitalWrite(relay_1, relay_on);
    delay(water_amount_time);
    digitalWrite(relay_1, relay_off);
  }
  
  delay(1500);        // delay in between reads for stability
} 
