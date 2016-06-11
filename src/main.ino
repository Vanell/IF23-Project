//Main File

//Inculde file



////Standard
#include "Bounce2.h"
#include "SoftwareSerial.h"
#include "Arduino.h"

////HomeMade



#include <pin.h>
#include <GPS.cpp>
#include <SDGPS.cpp>
#include <Affichage.cpp>




unsigned long currentMillis;
unsigned long previousMillis_Point =0;
int const nb_data_GPS = 13;
float data_GPS_Loop[13];
String command;



////MAIN////

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
	lcd.clear();
	lcd.setCursor(0,0);

	////Create Char for LCD
	lcd.createChar(0, char_arrow_up);
	lcd.createChar(1, char_arrow_down);
	lcd.createChar(2, char_select);
	lcd.createChar(3, char_back);
	lcd.createChar(4, char_arrow_left );

	//Begin serial computer
	Serial.begin(9600);

	//Begin serial GPS
	ss.begin(4800);

	//Init menu 
	for (int i(0) ; i < nb_level_menu ; i++)
	{
		pos_menu[i] = 0;

	}
	pos_menu[0] = 1;
	max_screen_lvl[0] = 5;//Root menu
	max_screen_lvl[1] = 2;// Menu
	//max_screen_lvl[2] = 3;//Option

	//data GPS initialisation
	for(int i=0;i<nb_data_GPS;i++){
		data_GPS_Loop[i]=0;
		//Serial.println(data_GPS_Loop[i]);
	}
	//SD initialisation
	pinMode(SD_SS,OUTPUT);

	//--> pour check l'init de la SD vaut mieux pas faire un while ?????
	if (!SD.begin(SD_SS)){
		lcd.setCursor(0,0);
		lcd.print("SD NO OK");
   		Serial.println("initialization failed!");
		delay(5000);	
    	return;
	}
	Serial.println("initialization done.");
	lcd.setCursor(2,0);
	lcd.print("SD OK");
	delay(500);

	//LCD message when start
	lcd.setCursor(2,0);
	lcd.print("Hello");
	lcd.setCursor(3,1);
	lcd.print("GPS");

	delay(750);

	lcd.clear();
	lcd.setCursor(0,1);
	lcd.write((uint8_t)0);
	lcd.setCursor(2,1);
	lcd.write((uint8_t)1);
	lcd.setCursor(5,1);
	lcd.write((uint8_t)2);
	lcd.setCursor(7,1);
	lcd.write((uint8_t)3);

	delay(750);
	Serial.println("initialization done");

}

void loop()
{
	currentMillis = millis();//Get time at the begin

	btn_push = ReadKeypad();
	MainMenuBtn();

	if (changeData_LCD || ( pos_menu[1]<= 0 && newData) ) 
	{
		changeData_LCD = false;
		MainMenuDisplay(data_GPS_Loop);
	}

	//Serial
	

	command="";
	while(Serial.available()>0){
		Serial.println("someting in buffer");
		c = Serial.read();
		command=command+c;
	}
	if(command == "send")
		readFile(fileName);
	if(command == "remove"){
		char name[fileName.length()+1];
		fileName.toCharArray(name, sizeof(name));
		SD.remove(name);		
	}
		

	//GPS
	
	get_data_GPS(data_GPS_Loop);
	updateGPSVar(data_GPS_Loop);

	//Quand je decommentte cette partie il ne veut plus compliler pour moi 
	if( millis()-previousMillis_Point > 10000 && nb_satGPS>3 ){
		lcd.setCursor(0,0);
		lcd.print("Saving");
		lcd.setCursor(2,1);
		lcd.print("Point");
		Serial.println("Before writing...");
		//writeWP2File("test.txt","Another cat beef");
		Serial.println("After writing!");
		delay(750);
		previousMillis_Point=millis();
	}




	while(millis()-currentMillis<10){}
}


