/*
Control a motor speed using a potentiometer
*/
#include <SoftwareSerial.h>

#define rxPin 7
#define txPin 8
#define lim 20

int enablePin_A = 9;   // pwm to motor 1
int enablePin_B = 10;  // pwm to motor 2
int ledPin = 2; //LED 

//byte speed_xbyte = 0;
int speed_x = 0;
int speed_y = 0;

SoftwareSerial mySerial(rxPin, txPin); // RX, TX

void setup() {
  Serial.begin (9600);
  pinMode(enablePin_A, OUTPUT);
  pinMode(enablePin_B, OUTPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(rxPin, INPUT);
  pinMode(txPin, OUTPUT);
  
//  pinMode(potPin, INPUT); 
  blink(ledPin, 10, 200);  //indicate start/restart  
  Serial.println("Commencing Reception");
  mySerial.begin(1200);
  mySerial.setTimeout(10000);      // Set time out 10 sec
  mySerial.flush();                // Software serial buffer flush
}
 
void loop() {
  motor();
}
 
void motor() {  
  byte tmp[lim];
  byte value[lim];
  byte len=0; 
  int i = 0, j = 0, fail = 0, found = 0;
  for( i=0; i < lim; i++ )  tmp[i]=0;
 
  if ( mySerial.available() > 0 ) {

    mySerial.setTimeout(2000);// Set timeout for 20 sec
    mySerial.flush();          // Flush MySerial buffer

    if ( mySerial.findUntil("$","$") ) { //checking the condition
      len = mySerial.readBytesUntil('$',(char *)tmp,lim); // From this instruction we can get length of the string
/*
      Serial.print("Debug:");
      Serial.write(tmp, lim);    
      Serial.print("  :");
      Serial.println(len);  
*/      
      for ( i = 0; i < lim ; i++ )  value[i] = 0;

      i = 0;      j = 0;
      
      while( (tmp[i++] != '/') && !fail ) {
        if ( i > lim ) { fail = 1; break; }
        if ( tmp[i] == '/' )  found = 1;
      }
  
      if ( found == 1 && !fail ) {
        while( (tmp[i] != '*') && !fail ) {
          if ( i > lim ) { fail = 1; break; }
          value[j++] = tmp[i++]; 
          if ( tmp[i] == '*' )  found = 2;
        }
        
        if ( found == 2 && !fail ) {
          speed_x = value[0];
          speed_y = value[1];
          
          if ( speed_x >= 50 && speed_x < 256 )
            analogWrite(enablePin_A, (int)speed_x); 
          
          if ( speed_y >= 50 && speed_y < 256 )
            analogWrite(enablePin_B, (int)speed_y); 
          
          Serial.print("speed: ");  
          Serial.print(speed_x);
          Serial.print(", ");  
          Serial.println(speed_y);   
          
          speed_x = 0; 
          speed_y = 0; 
        } 
      }
 
      if ( fail ) Serial.println("FAIL"); 

    } //else Serial.println("DELIMITER @ NOT FOUND");              
 
  } //else Serial.println("serial empty");
}

void blink(int whatPin, int howManyTimes, int milliSecs) {
  for ( int i = 0; i < howManyTimes; i++) {
    digitalWrite(whatPin, LOW);
    delay(milliSecs/2);
    digitalWrite(whatPin, HIGH);
    delay(milliSecs/2);
  }
}
