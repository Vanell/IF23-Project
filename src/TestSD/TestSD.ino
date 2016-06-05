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
String line;
char c;
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
		Serial.print("done");
	}
	else
		Serial.print("error while opening SD for reading");
}
void write2File(String name, String data){
	char filename[name.length()+1];
  	name.toCharArray(filename, sizeof(filename));
  	char filedata[data.length()+1];
  	data.toCharArray(filedata, sizeof(filedata));
	myFile = SD.open(filename,FILE_WRITE);
    if (myFile) {
    	myFile.println(filedata);
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
			else
				write2File(fileName,command);
		}
	}
	delay(10);
}