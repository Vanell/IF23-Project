//Main File

//Inculde file

////HomeMade
#include <pin.h>
#include <GPS.cpp>

////Standard
#include "Bounce2.h"
#include "SoftwareSerial.h"
#include "LiquidCrystal.h"
//#include "math.h"
// #include <SD.h>

//LCD
LiquidCrystal lcd(LCD_RS,LCD_Enable,LCD_D4,LCD_D5,LCD_D6,LCD_D7);
const long delay_LCD = 750; //Time refresh LCD
unsigned long previousMillis_LCD = 0;
bool state_LCD = true; //true : on ; false : off
bool changeData_LCD = true;

unsigned long currentMillis;
unsigned long previousMillis_Point =0;

unsigned long lastMillis_BP = 0 ;

//Battery
float Vbat; //Value of batterie
int percentBat;
float autonomy;
float autom;
float vmin = 4.5;
float v0;
unsigned long t0;

//SD variables
// File myFile;
// String fileName = "test.txt";
// String command="";
// char c;

//Buttons
int SW = 4; // Value of last button press
//Configuration Button bounce
Bounce debouncerBPEN = Bounce(BPEN,5);
Bounce debouncerBP0 = Bounce(BP0,5);
Bounce debouncerBP1 = Bounce(BP1,5);

//Menu
char btn_push;

////Def structure menu
int const nb_level_menu(4);
int pos_menu[nb_level_menu];
int max_screen_lvl[nb_level_menu];

bool mode_itinerary = false;

//GPS
SoftwareSerial ss(RX_GPS, TX_GPS);
int const nb_data_GPS(13);
float data_GPS[nb_data_GPS]; //sat;hdop;lat;long;alt;speed
const long delay_GPS = 750; //Time refresh GPS
unsigned long previousMillis_GPS = 0;

// Define char for screen
byte char_arrow_left[8] = {0b00000,0b00000,0b11000,0b11100,0b11110,0b11100,0b11000,0b00000};
byte char_arrow_up[8]   = {0b00000,0b00000,0b00000,0b00100,0b01110,0b11111,0b11111,0b00000};
byte char_arrow_down[8] = {0b00000,0b00000,0b00000,0b11111,0b11111,0b01110,0b00100,0b00000};
byte char_select[8]     = {0b00000,0b00000,0b00001,0b00011,0b10110,0b11100,0b01000,0b00000};
byte char_back[8]       = {0b00000,0b00000,0b00001,0b00101,0b01101,0b11111,0b01100,0b00100};

////FUNCTION////

float mapfloat(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}


//SD function
// void readFile(String name){
// 	char filename[name.length()+1];
//   	name.toCharArray(filename, sizeof(filename));
// 	myFile = SD.open(filename);
// 	if(myFile){
// 		while(myFile.available()){
// 			c = ((char)myFile.read());
// 			Serial.print(c);
// 		}
// 		myFile.close();
// 		Serial.print("Done");
// 	}
// 	else
// 		Serial.print("Error while opening SD for reading");
// }

// void writeWP2File(String name, String wpName,float lat,float lon, int nbSat,int dop){
// 	char filename[name.length()+1];
//   	name.toCharArray(filename, sizeof(filename));
//   	myFile = SD.open(filename,FILE_WRITE);
//   	char data[10];
//   	dtostrf(lat,1,6,data);
//     if (myFile) {
//     	myFile.print(wpName);
//     	myFile.print(" ");
//     	dtostrf(lat,1,6,data);
//     	myFile.print(data);
//     	myFile.print(" ");
//     	dtostrf(lon,1,6,data);
//     	myFile.print(data);
//     	sprintf(data," %i %i",nbSat,dop);
//     	myFile.println(data);
// 	    myFile.close();
//   	} 
//   	else {
//     	Serial.print("Error while opening SD for writing");
//   	}
// }

//Menu function
char ReadKeypad()
{
	char charBPN;
	//Update bouton
	debouncerBPEN.update();
	debouncerBP0.update();
	debouncerBP1.update();
	if (debouncerBPEN.rose()){

		// lastMillis_BP = millis();
		
		// if (!state_LCD)
		// {
		// 	lcd.display();
		// 	state_LCD = true;
		// }
		// else
		// {
			changeData_LCD = true;
			if(debouncerBP1.read()){
				if(debouncerBP0.read()){//SW4
					charBPN = 'S';
				}else{//SW3
					charBPN = 'B';
				}
			}else{
				if(debouncerBP0.read()){//SW2
					charBPN = 'D';
				}else{//SW1
					charBPN = 'U';
				}
			} 
		// }
	}
	return charBPN;
}

void MainMenuBtn()
{
	if(btn_push == 'U')
	{
		pos_menu[0] ++;
	}
	else if(btn_push == 'D')
	{
		pos_menu[0] --;   
	}
	else if(btn_push == 'S')
	{
		//faire une putain de boucle for ...
		pos_menu[3] = pos_menu[2];
		pos_menu[2] = pos_menu[1];
		pos_menu[1] = pos_menu[0];
		pos_menu[0] = 1 ;
	}
	else if(btn_push == 'B')
	{
		if (pos_menu[1] > 0)
		{
			//faire une putain de boucle for ...
			pos_menu[0] = pos_menu[1]; 
			pos_menu[1] = pos_menu[2]; 
			pos_menu[2] = pos_menu[3]; 
			pos_menu[3] = 0;
		}else{
			pos_menu[0] = 5 ; // Menu batterie 
		}
	}

	//Gestion des boucles de menu
	if (pos_menu[1] <= 0 && pos_menu[0] > max_screen_lvl[0])
	{
		pos_menu[0] = 1;
	}
	else if (pos_menu[1] <= 0 && pos_menu[0] <= 0 )
	{
		pos_menu[0] = max_screen_lvl[0];
	}
	
	else if (pos_menu[1] > 0 && pos_menu[0] > max_screen_lvl[1])
	{
		pos_menu[0] = 1;
	}else if (pos_menu[1] >0 && pos_menu[0] <= 0 )
	{
		pos_menu[0] = max_screen_lvl[1];
	}

	else if (pos_menu[2] > 0 && pos_menu[0] > max_screen_lvl[2])
	{
		pos_menu[0] = 1;
	}else if (pos_menu[2] >0 && pos_menu[0] <= 0 )
	{
		pos_menu[0] = max_screen_lvl[2];
	}
}

void MainMenuDisplay()
{
	lcd.clear();        
	lcd.setCursor(0,0);

	if (pos_menu[1] <= 0)//Root menu
	{
		switch (pos_menu[0])
		{
			case 1:
				lcd.print("Nb Sat ");
				lcd.print((int)data_GPS[0]);
				lcd.setCursor(0,1);
				lcd.print("HDOP ");
				lcd.print((int)data_GPS[1]);
				break;
			case 2:
				lcd.print(data_GPS[4]);
				lcd.print(" m");
				lcd.setCursor(0,1);
				lcd.print(data_GPS[5]);
				lcd.print(" km/h");
				break;
			case 3:
				lcd.print(data_GPS[2]);//Lattitude
				lcd.setCursor(0,1);
				lcd.print(data_GPS[3]);//Longitude
				break;
			case 4:
				lcd.print("Date");
				lcd.setCursor(0,1);
				lcd.print("Time");
				break;
			case 5:
				Vbat = mapfloat(analogRead(pinBat),0,1023,0,6.2);
				percentBat = mapfloat(analogRead(pinBat),0,1023,0,100);
				autonomy = 18-(6.2 -Vbat)/0.11;
				lcd.setCursor(0,1);
		    	lcd.print("Bat:");
	    		lcd.print(Vbat);
	    		lcd.print((int)autonomy);
	    		lcd.print(percentBat);
	    		lcd.print("% ");
	    		lcd.print((int)autonomy);
	    		lcd.print(" h");
				break;
		}
	}else if (pos_menu[1] > 0 && pos_menu[2] <= 0) // Menu
	{
		switch(pos_menu[0])
		{
			case 1:
				lcd.write((uint8_t)4);
				lcd.print("Pr pts");
				lcd.setCursor(1,1);
				lcd.print("Pr iti");
				break;
			case 2:
				lcd.setCursor(1,0);
				lcd.print("Pr pts");
				lcd.setCursor(0,1);
				lcd.write((uint8_t)4);

				if(mode_itinerary)
				{
					lcd.print("Stop iti");
				}else
				{
					lcd.print("Pr iti");
				}
			break;
			// case 3:
			// 	lcd.write((uint8_t)4);
			// 	lcd.print("Option");
			// 	break;
		}
	}else if ( pos_menu[2] > 0 && pos_menu[3] <= 0) //Menu choose
	{
		switch(pos_menu[1])
		{
			case 1 : 
				//Launch functuion take point
				lcd.print("Point");
				lcd.setCursor(0,1);
				lcd.print("Took !");
				delay(750);
				//appelle fonction prise point
				//Facon sale pour le retour en arrière 
				pos_menu[0] = pos_menu[2];
				pos_menu[1] = 0;
				pos_menu[2] = 0;
				changeData_LCD = true;
				break;

			case 2 : 
				//Launch function take itinerary
				if (mode_itinerary)
				{
					lcd.print("Itinerary");
					lcd.setCursor(0,1);
					lcd.print("Stop");
					mode_itinerary = !mode_itinerary;
					//appelle function itineraire
				}else
				{
					lcd.print("Itinerary");
					lcd.setCursor(0,1);
					lcd.print("Progress");
					mode_itinerary = !mode_itinerary;
					//appelle function itineraire
				}
				delay(1000);
				//Facon sale pour le retour en arrière 
				pos_menu[0] = pos_menu[2];
				pos_menu[1] = 0;
				pos_menu[2] = 0;
				changeData_LCD = true;
				break;

			// case 3 : //Choose option
			// 	switch (pos_menu[0])
			// 	{
			// 		case 1 : 
			// 		//Option 1
			// 		lcd.print("Option 1");
			// 		break;
			// 		case 2 : 
			// 		//Option 2
			// 		lcd.print("Option 2");
			// 		break;
			// 		case 3 : 
			// 		//Option 3
			// 		lcd.print("Option 3");
			// 		break;

			// 	}
			// 	break;
		}
	}
	// else if (pos_menu[3] > 0 && pos_menu[2] == 3)//optioyn 
	// {
	// 	switch(pos_menu[1])
	// 	{
	// 		case 1 : 
	// 			//Option 1
	// 			lcd.print("Opt 1");
	// 			lcd.setCursor(0,1);
	// 			lcd.print("Choose");
	// 			delay(750);
	// 			break;
	// 		case 2 : 
	// 			//Option 2
	// 			lcd.print("Opt 2");
	// 			lcd.setCursor(0,1);
	// 			lcd.print("Choose");
	// 			delay(750);
	// 			break;
	// 		case 3 : 
	// 			//Option 3
	// 			lcd.print("Opt 3");
	// 			lcd.setCursor(0,1);
	// 			lcd.print("Choose");
	// 			delay(750);
	// 			break;
	// 	}
	// 	pos_menu[0] = pos_menu[3];
	// 	pos_menu[1] = 0;
	// 	pos_menu[2] = 0;
	// 	pos_menu[3] = 0;
	// 	changeData_LCD = true;
	// }
	else 
	{
		lcd.print("Error");
		lcd.setCursor(0,1);
		lcd.print("Menu");
		delay(250);
	}
}



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

	//SD initialisation
	// pinMode(SD_SS,OUTPUT);

	// //--> pour check l'init de la SD vaut mieux pas faire un while ?????
	// if (!SD.begin(SD_SS)){
	// 	lcd.setCursor(0,0);
	// 	lcd.print("SD NO OK");
 //   		Serial.println("initialization failed!");
	// 	delay(5000);	
 //    	return;
	// }
	// Serial.println("initialization done.");
	// lcd.setCursor(2,0);
	// lcd.print("SD OK");
	// delay(500);

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

}

void loop()
{
	currentMillis = millis();//Get time at the begin

	btn_push = ReadKeypad();
	MainMenuBtn();

	if (changeData_LCD || ( pos_menu[1]<= 0 && newDataGPS) ) 
	{
		changeData_LCD = false;
		MainMenuDisplay();
	}

	//SD
	// command="";
	// while(Serial.available()>0){
	// 	c = Serial.read();
	// 	command=command+c;
	// }
	// if(command == "send")
	// 	readFile(fileName);
	// if(command == "remove"){
	// 	char name[fileName.length()+1];
	// 	fileName.toCharArray(name, sizeof(name));
	// 	SD.remove(name);		
	// }
		

	//GPS
	data_GPS = get_data_GPS(ss);

	//Quand je decommentte cette partie il ne veut plus compliler pour moi 
	// if( millis()-previousMillis_Point > 60000 && nb_satGPS>3 ){
	// 	lcd.setCursor(0,0);
	// 	lcd.print("Saving");
	// 	lcd.setCursor(2,1);
	// 	lcd.print("Point");
	// 	writeWP2File(fileName,"night test",latGPS,lonGPS,nb_satGPS,hdopGPS);
	// 	delay(750);
	// 	previousMillis_Point=millis();
	// }

	while(millis()-currentMillis<10){}
}
 