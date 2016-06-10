//#include "Bounce2.h"
#include "SoftwareSerial.h"
#include "TinyGPS.h"
#include "LiquidCrystal.h"
#include "math.h"
#include <SD.h>
#define RX_GPS     3
#define TX_GPS     2

#define SD_SS	   10


File myFile;
String fileName = "test.txt";
String command="";
char c;
float lat=43.653243;
float lon=3.333333;

void setup(){
	
	//Serial init
	Serial.begin(57600);
	//SD init
	pinMode(SD_SS,OUTPUT);
	if (!SD.begin(SD_SS)){
    	Serial.println("initialization failed!");
    	return;
	}
	//GPS init
	TinyGPS gps;
	SoftwareSerial ss(RX_GPS,TX_GPS);
    Serial.println("initialization done.");
}

void readFile(String name){
	char filename[name.length()+1];
  	name.toCharArray(filename, sizeof(filename));
	myFile = SD.open(filename);
	if(myFile){
		while(myFile.available()){

			c = ((char)myFile.read());
			Serial.print(c);
		}
		myFile.close();
		Serial.println();
		Serial.print("done");
	}
	else
		Serial.print("error while opening SD for reading");
}
void write2File(String name, char data[]){
	char filename[name.length()+1];
  	name.toCharArray(filename, sizeof(filename));
	myFile = SD.open(filename,FILE_WRITE);
    if (myFile) {
    	myFile.println(data);
	    myFile.close();
  	} 
  	else {
    	Serial.print("error while opening SD for writing");
  	}
}
void writeWP2File(String name, String wpName,float lat,float lon){
	char filename[name.length()+1];
  	name.toCharArray(filename, sizeof(filename));
  	myFile = SD.open(filename,FILE_WRITE);
  	char data[10];
  	dtostrf(lat,1,6,data);
    if (myFile) {
    	myFile.print(wpName);
    	myFile.print(" ");
    	dtostrf(lat,1,6,data);
    	myFile.print(data);
    	myFile.print(" ");
    	dtostrf(lon,1,6,data);
    	myFile.println(data);
	    myFile.close();
  	} 
  	else {
    	Serial.print("error while opening SD for writing");
  	}
}

void loop(){
	char name[fileName.length()+1];
  	fileName.toCharArray(name, sizeof(name));
	if(Serial.available()){
		command = Serial.readString();
		if(command == "send"){
			readFile(fileName);
		}
		else{
			if(command == "remove")
				SD.remove(name);
			else{
				writeWP2File(fileName,"test",lat,lon);
			}
		}
	}
	delay(10);
}