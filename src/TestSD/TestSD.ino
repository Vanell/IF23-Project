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
int SD_SS=10;
#define SD_MOSI    11
#define SD_MISO    12
#define SD_SCK     13

//Buttons Pin
#define BP0        16
#define BP1        15
#define BPEN       17

//Others
#define pinBat       14

File myFile;
String fileName = "test.txt";
String command="";

LiquidCrystal lcd(4,5,6,7,8,9);
const long delay_LCD = 750; //Time refresh LCD
unsigned long previousMillis_LCD = 0;


void setup(){
	//Input button 
	pinMode(BPEN,INPUT_PULLUP);
	pinMode(BP0,INPUT_PULLUP);
	pinMode(BP1,INPUT_PULLUP);

	//Input batterie
	pinMode(pinBat,INPUT);

	//Output SD
	pinMode(SD_SS,OUTPUT);
	

	
	//LCD begin
	lcd.begin(8,2);	

	//Begin serial computer
	Serial.begin(9600);

	//Begin serial GPS
	//ss.begin(4800);

	//LCD message when start
	lcd.setCursor(0,0);
	//lcd.print("Hello");
	//lcd.setCursor(3,1);
	//cd.print("GPS");

	//SD.begin(SD_SS);
	if (!SD.begin(SD_SS)){

    	lcd.print("SD fail");
    	Serial.println("initialization failed!");
    	return;
	}
    lcd.print("SD done");
    Serial.println("initialization done.");
    
    /*if(SD.exists("test.txt"))
    	SD.remove("test.txt");


    myFile = SD.open("test.txt",FILE_WRITE);
    if (myFile) {
    	lcd.clear();
    	lcd.setCursor(0,0);
	    lcd.print("Writing");
	    Serial.print("Writing");
	    delay(500);
	    myFile.print("testing");
		// close the file:
	    myFile.close();
	    lcd.clear();
	    lcd.setCursor(0,0);
	    lcd.print("done.");
  	} 
  	else {
    // if the file didn't open, print an error:
    	lcd.println("error");
    	Serial.print("Error while opening SD");
  	}
  	myFile = SD.open("test.txt");
	int i =0;
	if(myFile){
    	Serial.println("File opened");
    	Serial.println("Reading file...");
    	char c;
    	while(myFile.available()){
    		lcd.setCursor(i,1);
    		c = myFile.read();
    		lcd.print(c);
    		//if(Serial.available())
    		Serial.print(c);
    		i++;
    	}
    	delay(5000);
    	myFile.close();
    }
    else {
    // if the file didn't open, print an error:
    	lcd.println("error");
    	Serial.print("Error while opening SD");
  	}*/
    


}

void loop(){
	
	if(Serial.available()){
		command = Serial.readString();
	
		if(command == "senddata"){
			myFile = SD.open("test.txt");
			if(myFile){
		    	char c;
		    	while(myFile.available()){
		    		c = myFile.read();
		    		Serial.print(c);
		    	}
		    	myFile.close();
		    	Serial.println("\n");

		    }
		    else {
		    	lcd.println("error");
		    	Serial.print("Error while opening SD");
		  	}
		}
		else{
			myFile = SD.open("test.txt",FILE_WRITE);
		    if (myFile) {
		    	myFile.println(command);
			    myFile.close();
		  	} 
		  	else {
		    // if the file didn't open, print an error:
		    	lcd.println("error");
		    	Serial.print("Error while opening SD");
		  	}

		}
	}
}