/*
  AnalogReadSerial
  Reads an analog input on pin 0, prints the result to the serial monitor.
  Attach the center pin of a potentiometer to pin A0, and the outside pins to +5V and ground.

 This example code is in the public domain.
 */

// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  
}

// the loop routine runs over and over again forever:
void loop() {
fct_deerivee(0);
}

int fct_derivee(int x){
  int sensorValueD = analogRead(A0);
  int sensorValueG = analogRead(A1);
  int derivee = sensorValueD-sensorValueG;
  resultat=derivee+x
  Serial.println(resultat);
  //delay(10);
  }
