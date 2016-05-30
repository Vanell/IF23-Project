//#include "Bounce2.h"
#include "SoftwareSerial.h"
//#include "TinyGPS.h"
#include "LiquidCrystal.h"
#include "math.h"
#include <SD.h>
#define RX_GPS     3
#define TX_GPS     2

//LCD Pin
#define LCD_RS     4
#define LCD_Enable 5
#define LCD_D4     6
#define LCD_D5     7
#define LCD_D6     8
#define LCD_D7     9

//SD Pin
#define SD_SS	   10


LiquidCrystal lcd(4,5,6,7,8,9);

File myFile;
String fileName = "test.txt";
String command="";
String line;
char c;

void setup(){
	pinMode(SD_SS,OUTPUT);
	Serial.begin(115200);
	if (!SD.begin(SD_SS)){
    	Serial.println("initialization failed!");
    	return;
	}
    Serial.println("initialization done.");

    

  


}

void loop(){
	if(Serial.available()){
		//Serial.println("sommething in buffer");
		command = Serial.readString();
		//Serial.println(command);
		if(command == "send"){
			line="";
			//Serial.println("commande send");
			myFile = SD.open("test.txt");
			if(myFile){
		    	
		    	while(myFile.available()){
		    		c = ((char)myFile.read());
					Serial.print(c);
		    	}
		    	myFile.close();
		    	Serial.println("done");
		    }
		    else {
		    	Serial.print("Error while opening SD 1");
		  	}
		}
		else{
			if(command == "remove")
				SD.remove("test.txt");
			else{
				myFile = SD.open("test.txt",FILE_WRITE);
			    if (myFile) {
			    	myFile.println(command);
				    myFile.close();
			  	} 
			  	else {
			    	Serial.print("Error while opening SD 2");
			  	}
			  }

		}
	}
	delay(10);
}