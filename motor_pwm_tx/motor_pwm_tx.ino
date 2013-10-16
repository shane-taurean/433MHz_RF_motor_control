/*
Control a motor speed using a potentiometer
*/
#include <SoftwareSerial.h>

#define rxPin 7
#define txPin 8

int ledPin = 2;   //LED 
int potPin_x = 0; //potentiometer to control the speed of motor 1
int potPin_y = 1; //potentiometer to control the speed of motor 2

int speed_x;
int speed_y;

byte txdata[9] = "UUU/XY*$";

SoftwareSerial mySerial(rxPin, txPin); // RX, TX

void setup() {
  Serial.begin (9600);
  pinMode(ledPin, OUTPUT);
  pinMode(potPin_x, INPUT); 
  pinMode(potPin_y, INPUT);   
  pinMode(rxPin, INPUT);
  pinMode(txPin, OUTPUT);  
  
  Serial.println("Commencing Transmission");
  blink(ledPin, 10, 200);  //indicate start/restart
  
  mySerial.begin(1200);
  mySerial.setTimeout(10000); // Set time out 10 sec
  mySerial.flush();           // Flush MySerial buffer
}
 
void loop() {
  motor();
}
 
void motor() {  
  speed_x = analogRead(potPin_x)/4;
  speed_y = analogRead(potPin_y)/4;
  if ( speed_x < 50 ) speed_x = 50;
  if ( speed_y < 50 ) speed_y = 50;
  txdata[4] = speed_x; 
  txdata[5] = speed_y; 
  
  mySerial.write(txdata, sizeof(txdata));

  Serial.print("Speed: "); 
  Serial.print(speed_x);
  Serial.print(", "); 
  Serial.print(speed_y);  
  Serial.print("  "); 
  Serial.write(txdata, sizeof(txdata));
  Serial.println(" |");

  mySerial.flush();          // Flush MySerial buffer  
}

void blink(int whatPin, int howManyTimes, int milliSecs) {
  for ( int i = 0; i < howManyTimes; i++) {
  digitalWrite(whatPin, LOW);
  delay(milliSecs/2);
  digitalWrite(whatPin, HIGH);
  delay(milliSecs/2);
  }
}
