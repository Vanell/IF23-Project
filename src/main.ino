//Main File

//Inculde file

////HomeMade
//#include "pins.h"


////Standard
#include "Bounce2.h"
#include "SoftwareSerial.h"
#include "TinyGPS.h"
#include "LiquidCrystal.h"
#include "math.h"

#define RX_GPS     3  
#define TX_GPS     2

#define BPEN       17
#define BP0        16
#define BP1        15

#define pinBat     0

LiquidCrystal lcd(4,5,6,7,8,9);
const long delay_LCD = 750; //Time refresh LCD
unsigned long previousMillis_LCD = 0;

float Vbat; //Value of batterie

int SW = 4; // Value of last button press
//Configuration Button bounce
Bounce debouncerBPEN = Bounce(BPEN,5);
Bounce debouncerBP0 = Bounce(BP0,5);
Bounce debouncerBP1 = Bounce(BP1,5);

//Configuration GPS
TinyGPS gps;
SoftwareSerial ss(RX_GPS, TX_GPS);
const long delay_GPS = 750; //Time refresh GPS
unsigned long previousMillis_GPS = 0;

int nb_satGPS = 0;
float latGPS = 0;
float lonGPS = 0;
int hdopGPS = 0;
float altGPS = 0;
float speedGPS = 0;

float mapfloat(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

float truncateNumber(float x, int a){
	long y;
	y = pow(10,a)* x;
	return (float)y/pow(10,a);
}

void setup()
{
	//Input button 
	pinMode(BPEN,INPUT_PULLUP);
	pinMode(BP0,INPUT_PULLUP);
	pinMode(BP1,INPUT_PULLUP);

	//Input batterie
	pinMode(pinBat,INPUT);
	
	//LCD begin
	lcd.begin(8,2);	

	//Begin serial computer
	Serial.begin(115200);

	//Begin serial GPS
	ss.begin(4800);

	//LCD message when start
	lcd.setCursor(2,0);
	lcd.print("Hello");
	lcd.setCursor(3,1);
	lcd.print("GPS");

}

void loop()
{
	//Update bouton
	debouncerBPEN.update();
	debouncerBP0.update();
	debouncerBP1.update();

	unsigned long currentMillis = millis();

	//LCD 
	if (currentMillis - previousMillis_LCD >= delay_LCD)
	{
		lcd.clear();		
		lcd.setCursor(0,0);

		switch(SW)
		{
			case 1:
				//SW1
				// lcd.print("sw1");
				lcd.print("Nb Sat ");
				lcd.print(nb_satGPS);
				lcd.setCursor(0,1);
				lcd.print("HDOP ");
				lcd.print(hdopGPS);
				break;

			case 2:
				//SW2
				// lcd.print("sw2");

				lcd.print(altGPS);
				lcd.print(" m");
				lcd.setCursor(0,1);
				lcd.print(speedGPS);
				lcd.print(" km/h");
				break;

			case 3:
				//SW3
				lcd.print("sw3");
				break;

			case 4:
				//SW4
				Vbat = mapfloat(analogRead(pinBat),0,1023,0.0,6.5);
				lcd.setCursor(0,1);
		    	lcd.print("Bat:");
	    		lcd.print(Vbat);
	    		break;
		}
		previousMillis_LCD = millis();
	}

	//Button
	if (debouncerBPEN.rose()){
		if(debouncerBP1.read()){
	  		if(debouncerBP0.read()){//SW4
	  			SW = 4;
	  		}else{//SW3
	    		SW = 3;}
		}else{
		  	if(debouncerBP0.read()){//SW2
		    	SW = 2;
		  	}else{//SW1
		    	SW = 1;}
		} 
	}

	//GPS
	bool newData = false;
	unsigned long chars;
	unsigned short sentences, failed;

	// For one second we parse GPS data and report some key values
	while(ss.available()){
  		char c = ss.read();
		if (gps.encode(c))
			newData = true;
	}

	if (currentMillis - previousMillis_GPS >= delay_GPS and newData){
		float flat, flon;
		unsigned long age;
		gps.f_get_position(&flat, &flon, &age);

		//Lattidue
		if (flat != TinyGPS::GPS_INVALID_F_ANGLE)
			latGPS = truncateNumber(flat,6);
		//Longitude
		if (flon != TinyGPS::GPS_INVALID_F_ANGLE)
			lonGPS = truncateNumber(flon,6);

		//Number statellites
		if(gps.satellites() != TinyGPS::GPS_INVALID_SATELLITES)
			nb_satGPS = gps.satellites();
		//Precision HDOP
		if(gps.hdop() != TinyGPS::GPS_INVALID_HDOP)
			hdopGPS = gps.hdop();
		//GPS altitude
		if(gps.f_altitude() != TinyGPS::GPS_INVALID_F_ALTITUDE)
			altGPS = truncateNumber(gps.f_altitude(),2);
		//GPS Speed km/h
		if(gps.f_speed_kmph() != TinyGPS::GPS_INVALID_F_SPEED)
			speedGPS = truncateNumber(gps.f_speed_kmph(),2);

		gps.stats(&chars, &sentences, &failed);
		// Serial.print(" CHARS=");
		// Serial.print(chars);
		// Serial.print(" SENTENCES=");
		// Serial.print(sentences);
		// Serial.print(" CSUM ERR=");
		// Serial.println(failed);
		if (chars == 0)
			Serial.println("** No characters received from GPS: check wiring **");

		previousMillis_GPS = millis();
	}

}