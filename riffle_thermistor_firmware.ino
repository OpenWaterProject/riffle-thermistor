#include <SPI.h>
#include <SD.h>
#include <Wire.h>


#define debug 1 // 0: don't print anything out; 1: print out debugging statements

const int num_samples=1000;

// logger

// Set the pins used
#define cardSelect 4

File logfile;

// blink out an error code
void error(uint8_t errno) {
  while(1) {
    uint8_t i;
    for (i=0; i<errno; i++) {
      digitalWrite(13, HIGH);
      delay(100);
      digitalWrite(13, LOW);
      delay(100);
    }
    for (i=errno; i<10; i++) {
      delay(200);
    }
  }
}

// This line is not needed if you have Adafruit SAMD board package 1.6.2+
//   #define Serial SerialUSB

void setup() {
  // connect at 115200 so we can read the GPS fast enough and echo without dropping chars
  // also spit it out
  Serial.begin(115200);
  
  if (debug) Serial.println("\r\nAnalog logger test");
  pinMode(13, OUTPUT);


  // see if the card is present and can be initialized:
  if (!SD.begin(cardSelect)) {
  if (debug)  Serial.println("Card init. failed!");
    error(2);
  }
  char filename[15];
  strcpy(filename, "ANALOG00.TXT");
  for (uint8_t i = 0; i < 100; i++) {
    filename[6] = '0' + i/10;
    filename[7] = '0' + i%10;
    // create if does not exist, do not open existing, write, sync after write
    if (! SD.exists(filename)) {
      break;
    }
  }

  logfile = SD.open(filename, FILE_WRITE);
  if( ! logfile ) {
   if(debug) Serial.print("Couldnt create "); 
   if (debug) Serial.println(filename);
    error(3);
  }
  if (debug) {
    //Serial.print("Writing to "); 
  
  Serial.println(filename);
  }

  pinMode(13, OUTPUT);
  pinMode(8, OUTPUT);
 if (debug)  Serial.println("Ready!");


  
}

uint8_t i=0;
void loop() {

analogReadResolution(12);


// write all the values


float temp = 22.0;

int a = analogRead(0);

        String dataString = "";
  
    dataString += millis();
     dataString += " ";
    dataString += analogRead(0);
   // dataString += " ";
   // dataString += analogRead(3);
  
    
      logfile.println(dataString);


    if(debug) Serial.println(dataString);

      
      
      logfile.flush();

 digitalWrite(8, HIGH);
    delay(10);
    digitalWrite(8, LOW);



// otherwise we've pulled up the stoppin 
//error(5); // flash 5 times to indicate stop condition

  //delay(1000);

}


String padInt(int x, int pad) {
  String strInt = String(x);
  
  String str = "";
  
  if (strInt.length() >= pad) {
    return strInt;
  }
  
  for (int i=0; i < (pad-strInt.length()); i++) {
    str += "0";
  }
  
  str += strInt;
  
  return str;
}

String int2string(int x) {
  // formats an integer as a string assuming x is in 1/100ths
  String str = String(x);
  int strLen = str.length();
  if (strLen <= 2) {
    str = "0." + str;
  } else if (strLen <= 3) {
    str = str.substring(0, 1) + "." + str.substring(1);
  } else if (strLen <= 4) {
    str = str.substring(0, 2) + "." + str.substring(2);
  } else {
    str = "-9999";
  }
  
  return str;
}

